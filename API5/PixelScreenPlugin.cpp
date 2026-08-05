/*---------------------------------------------------------*\
| OpenRGBMatrixTextPlugin.cpp                               |
|                                                           |
|   OpenRGB Matrix Text Plugin                              |
|                                                           |
|   This file is part of the OpenRGB Matrix Text Plugin     |
|   project                                                 |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
| \*---------------------------------------------------------*/

#include "PixelScreenPlugin.h"
#include "PixelScreenTab.h"
#include <QFile>
#include <QDateTime>
#include <QTime>
#include <QDir>
#include <QThread>
#include <QMetaObject>
#include <fstream>
#include <cmath>

#include <algorithm>
OpenRGBPluginAPIInterface* PixelScreenPlugin::api = nullptr;
PixelScreenPlugin* PixelScreenPlugin::plugin_instance = nullptr;


OpenRGBPluginInfo PixelScreenPlugin::GetPluginInfo()
{
    OpenRGBPluginInfo info;

    info.Name           = "Pixel Screen Plugin";
    info.Description    = "Render custom scrolling text, clock, and pixel art on OpenRGB matrices";
    info.Version        = VERSION_STRING;
    info.Commit         = GIT_COMMIT_ID;
    info.URL            = "https://gitlab.com/OpenRGBDevelopers/OpenRGBPixelScreenPlugin";

    info.Label          = "Pixel Screen";
    info.Location       = OPENRGB_PLUGIN_LOCATION_TOP;

    if (!info.Icon.load(":/images/OpenRGBPixelScreenPlugin.png")) {
        LOG_WARNING("[PixelScreenPlugin] Icon not found – using empty pixmap");
    }

    return(info);
}

unsigned int PixelScreenPlugin::GetPluginAPIVersion()
{
    return(OPENRGB_PLUGIN_API_VERSION);
}

/*---------------------------------------------------------*\
| Plugin Functionality                                      |
\*---------------------------------------------------------*/
void PixelScreenPlugin::Load(OpenRGBPluginAPIInterface* api_interface_ptr)
{
    plugin_instance = this;
    api = api_interface_ptr;

    LOG_INFO("[PixelScreenPlugin] Loading version %s (%s), build date %s\n", VERSION_STRING, GIT_COMMIT_ID, BUILDDATE_STRING);

    // Load fonts and configurations
    LoadFonts();
    LoadSettings();

    // Rebuild active device matrix zones list
    UpdateControllers();

    // Initialize hardware sensor manager BEFORE creating UI so
    // DeviceSettingsPage constructors can connect to its signals
    sensor_manager = new HardwareSensorManager(this);
    connect(sensor_manager, &HardwareSensorManager::sensorDataUpdated,
            this, &PixelScreenPlugin::OnSensorDataUpdated);
    sensor_timer = new QTimer(this);
    sensor_timer->setInterval(1000);
    connect(sensor_timer, &QTimer::timeout, this, &PixelScreenPlugin::OnSensorTimerTimeout);
    sensor_timer->start();

    // Create settings tab UI (DeviceSettingsPage constructors will find sensor_manager ready)
    ui = new PixelScreenTab(this);
    ui->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // Start rendering frame timer
    render_timer = new QTimer(this);
    connect(render_timer, &QTimer::timeout, this, &PixelScreenPlugin::RenderFrame);
    render_timer->start(20); // 50 FPS timer tick

    OnSensorTimerTimeout(); // fetch only if enabled && Sensor Data mode
}

QWidget* PixelScreenPlugin::GetWidget()
{
    return ui;
}

QMenu* PixelScreenPlugin::GetTrayMenu()
{
    return nullptr;
}

void PixelScreenPlugin::Unload()
{
    LOG_INFO("[PixelScreenPlugin] Unloading\n");

    if (render_timer)
    {
        render_timer->stop();
    }

    if (sensor_timer)
    {
        sensor_timer->stop();
        sensor_timer->deleteLater();
        sensor_timer = nullptr;
    }
    if (sensor_manager)
    {
        sensor_manager->onAboutToQuit();
        sensor_manager->deleteLater();
        sensor_manager = nullptr;
    }

    SaveSettings();

    if (api)
    {
        // Unregister update callbacks
        for (RGBControllerInterface* controller : api->GetRGBControllers())
        {
            if (controller)
            {
                controller->UnregisterUpdateCallback(this);
                controller->UnregisterUpdateCallback(controller);
            }
        }
    }
    plugin_instance = nullptr;
}


void PixelScreenPlugin::OnProfileAboutToLoad()
{
}

void PixelScreenPlugin::OnProfileLoad(nlohmann::json /*profile_data*/)
{
}

nlohmann::json PixelScreenPlugin::OnProfileSave()
{
    nlohmann::json profile_json;
    return profile_json;
}

unsigned char* PixelScreenPlugin::OnSDKCommand(unsigned int /*pkt_id*/, unsigned char* /*data*/, unsigned int* /*data_size*/)
{
    return nullptr;
}

/*---------------------------------------------------------*\
| Update Signals                                            |
\*---------------------------------------------------------*/
void PixelScreenPlugin::ProfileManagerUpdated(unsigned int /*update_reason*/)
{
}

void PixelScreenPlugin::ResourceManagerUpdated(unsigned int /*update_reason*/)
{
    QMetaObject::invokeMethod(this, "UpdateControllers", Qt::QueuedConnection);
}

void PixelScreenPlugin::SettingsManagerUpdated(unsigned int /*update_reason*/)
{
}


void PixelScreenPlugin::OnControllerUpdate(void* callback_arg, unsigned int /*reason*/, void* controller_ptr)
{
    if (!plugin_instance) return;
    if (plugin_instance->in_callback) return;

    PixelScreenPlugin* plugin = static_cast<PixelScreenPlugin*>(callback_arg);
    if (!plugin || plugin != plugin_instance) return;

    RGBControllerInterface* controller = static_cast<RGBControllerInterface*>(controller_ptr);
    if (!controller) return;

    // Guard against recursion when UpdateLEDs is called
    plugin_instance->in_callback = true;
    
    std::shared_lock<std::shared_mutex> lock(plugin->matrix_zones_mutex);
    for (const auto& target : plugin->matrix_zones)
    {
        if (target.controller == controller)
        {
            auto it = plugin->settings.device_settings.find(target.display_name);
            if (it != plugin->settings.device_settings.end() && it->second.enabled)
            {
                plugin->OverlayTextOnController(target, it->second, true);
            }
        }
    }
    
    plugin_instance->in_callback = false;
}

/*---------------------------------------------------------*\
| Controller Zones Filtering                                |
\*---------------------------------------------------------*/
void PixelScreenPlugin::UpdateControllers()
{
    std::unique_lock<std::shared_mutex> lock(matrix_zones_mutex);

    matrix_zones.clear();

    for (RGBControllerInterface* controller : api->GetRGBControllers())
    {
        if (!controller) continue;
        
        controller->UnregisterUpdateCallback(this);
        controller->UnregisterUpdateCallback(controller);

        for (unsigned int zone_idx = 0; zone_idx < controller->GetZoneCount(); zone_idx++)
        {
            if (controller->GetZoneType(zone_idx) == ZONE_TYPE_MATRIX)
            {
                MatrixZoneTarget target;
                target.controller = controller;
                target.zone_idx = zone_idx;
                target.controller_name = controller->GetName();
                target.zone_name = controller->GetZoneName(zone_idx);
                target.display_name = target.controller_name + " - " + target.zone_name;
                
                matrix_zones.push_back(target);
            }
        }
        
        // Register update callback for effect engine overlay support
        controller->RegisterUpdateCallback(OnControllerUpdate, this);
    }

    if (ui)
    {
        QMetaObject::invokeMethod(ui, "UpdateDeviceList", Qt::QueuedConnection);
    }
}

/*---------------------------------------------------------*\
| Font Loading and Configuration File Management            |
\*---------------------------------------------------------*/
void PixelScreenPlugin::LoadFonts()
{
    // 1. Read WLED_Text.json
    QFile file(":/resources/WLED_Text.json");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        std::string raw_data = file.readAll().toStdString();
        file.close();

        try
        {
            nlohmann::json json_data = nlohmann::json::parse(raw_data);

            auto parse_font_map = [](const nlohmann::json& font_json, std::map<char, Glyph>& font_map)
            {
                font_map.clear();
                for (auto& el : font_json.items())
                {
                    if (el.key().empty()) continue;
                    char c = el.key()[0];
                    Glyph g;
                    for (auto& row : el.value())
                    {
                        std::vector<unsigned char> row_data;
                        for (auto& cell : row)
                        {
                            row_data.push_back(cell.get<unsigned char>());
                        }
                        g.grid.push_back(row_data);
                    }
                    if (!g.grid.empty())
                    {
                        g.height = g.grid.size();
                        g.width = g.grid[0].size();
                    }
                    font_map[c] = g;
                }
            };

            if (json_data.contains("SMALL_LETTERS")) parse_font_map(json_data["SMALL_LETTERS"], small_letters);
            if (json_data.contains("LETTERS")) parse_font_map(json_data["LETTERS"], letters);
            if (json_data.contains("LARGE_LETTERS")) parse_font_map(json_data["LARGE_LETTERS"], large_letters);
            if (json_data.contains("DIGITS")) parse_font_map(json_data["DIGITS"], digits);
            if (json_data.contains("SMALL_DIGITS")) parse_font_map(json_data["SMALL_DIGITS"], small_digits);
            if (json_data.contains("LARGE_DIGITS")) parse_font_map(json_data["LARGE_DIGITS"], large_digits);

            LOG_INFO("[OpenRGBMatrixTextPlugin] Loaded standard fonts: SMALL_LETTERS(%lu), LETTERS(%lu), LARGE_LETTERS(%lu)\n",
                     small_letters.size(), letters.size(), large_letters.size());
        }
        catch (const std::exception& e)
        {
            LOG_ERROR("[OpenRGBMatrixTextPlugin] Failed to parse standard fonts JSON: %s\n", e.what());
        }
    }
    else
    {
        LOG_ERROR("[OpenRGBMatrixTextPlugin] Failed to open standard fonts resource file\n");
    }

    // 2. Read WLED_Text_ZH.json
    QFile zh_file(":/resources/WLED_Text_ZH.json");
    if (zh_file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        std::string raw_data = zh_file.readAll().toStdString();
        zh_file.close();

        try
        {
            nlohmann::json json_data = nlohmann::json::parse(raw_data);
            zh_font.clear();

            for (auto& el : json_data.items())
            {
                std::string key = el.key();
                Glyph g;
                for (auto& row : el.value())
                {
                    std::vector<unsigned char> row_data;
                    for (auto& cell : row)
                    {
                        row_data.push_back(cell.get<unsigned char>());
                    }
                    g.grid.push_back(row_data);
                }
                if (!g.grid.empty())
                {
                    g.height = g.grid.size();
                    g.width = g.grid[0].size();
                }
                zh_font[key] = g;
            }

            LOG_INFO("[PixelScreenPlugin] Loaded Chinese fonts: ZH_FONT(%lu)\n", zh_font.size());
        }
        catch (const std::exception& e)
        {
            LOG_ERROR("[PixelScreenPlugin] Failed to parse Chinese fonts JSON: %s\n", e.what());
        }
    }
    else
    {
        LOG_ERROR("[PixelScreenPlugin] Failed to open Chinese fonts resource file\n");
    }
}

void PixelScreenPlugin::LoadSettings()
{
    std::string settings_path = (api->GetConfigurationDirectory() / "plugins" / "settings" / "PixelScreenSettings.json").string();
    QFile file(QString::fromStdString(settings_path));
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        std::string raw_data = file.readAll().toStdString();
        file.close();

        try
        {
            nlohmann::json root = nlohmann::json::parse(raw_data);
            if (root.contains("devices") && root["devices"].is_object())
            {
                settings.device_settings.clear();
                for (auto& el : root["devices"].items())
                {
                    std::string dev_name = el.key();
                    nlohmann::json j = el.value();
                    DeviceMatrixSettings dev_s;

                    if (j.contains("enabled")) dev_s.enabled = j["enabled"];
                    if (j.contains("display_mode")) dev_s.display_mode = j["display_mode"];
                    if (j.contains("font_size")) dev_s.font_size = j["font_size"];
                    if (j.contains("custom_text")) dev_s.custom_text = j["custom_text"];
                    if (j.contains("time_format")) dev_s.time_format = j["time_format"];
                    if (j.contains("pixel_art_json")) dev_s.pixel_art_json = j["pixel_art_json"];
                    if (j.contains("scroll_direction")) dev_s.scroll_direction = j["scroll_direction"];
                    if (j.contains("scroll_speed")) dev_s.scroll_speed = j["scroll_speed"];
                    if (j.contains("fps")) dev_s.fps = j["fps"];
                    if (j.contains("text_r")) dev_s.text_r = j["text_r"];
                    if (j.contains("text_g")) dev_s.text_g = j["text_g"];
                    if (j.contains("text_b")) dev_s.text_b = j["text_b"];
                    if (j.contains("invert_color")) dev_s.invert_color = j["invert_color"];
                    if (j.contains("padding_x")) dev_s.padding_x = j["padding_x"];
                    if (j.contains("padding_y")) dev_s.padding_y = j["padding_y"];
                    if (j.contains("text_align")) dev_s.text_align = j["text_align"];
                    if (j.contains("sensor_format")) dev_s.sensor_format = j["sensor_format"].get<std::string>();
                    if (j.contains("sensor_update_interval")) dev_s.sensor_update_interval = j["sensor_update_interval"];

                    settings.device_settings[dev_name] = dev_s;
                }
            }
        }
        catch (const std::exception& e)
        {
            LOG_ERROR("[PixelScreenPlugin] Failed to parse settings JSON: %s\n", e.what());
        }
    }
}

void PixelScreenPlugin::SaveSettings()
{
    // Create folders if they do not exist
    std::string settings_dir = (api->GetConfigurationDirectory() / "plugins" / "settings").string();
    QDir().mkpath(QString::fromStdString(settings_dir));

    std::string settings_path = (api->GetConfigurationDirectory() / "plugins" / "settings" / "PixelScreenSettings.json").string();
    std::ofstream file(settings_path, std::ios::out | std::ios::binary);
    if (file)
    {
        nlohmann::json root;
        nlohmann::json devices_json;

        for (const auto& pair : settings.device_settings)
        {
            nlohmann::json j;
            const auto& dev_s = pair.second;

            j["enabled"] = dev_s.enabled;
            j["display_mode"] = dev_s.display_mode;
            j["font_size"] = dev_s.font_size;
            j["custom_text"] = dev_s.custom_text;
            j["time_format"] = dev_s.time_format;
            j["pixel_art_json"] = dev_s.pixel_art_json;
            j["scroll_direction"] = dev_s.scroll_direction;
            j["scroll_speed"] = dev_s.scroll_speed;
            j["fps"] = dev_s.fps;
            j["text_r"] = dev_s.text_r;
            j["text_g"] = dev_s.text_g;
            j["text_b"] = dev_s.text_b;
            j["invert_color"] = dev_s.invert_color;
            j["padding_x"] = dev_s.padding_x;
            j["padding_y"] = dev_s.padding_y;
            j["text_align"] = dev_s.text_align;
            j["sensor_format"] = dev_s.sensor_format;
            j["sensor_update_interval"] = dev_s.sensor_update_interval;

            devices_json[pair.first] = j;
        }

        root["devices"] = devices_json;
        file << root.dump(4);
        file.close();
    }
}

/*---------------------------------------------------------*\
| UTF-8 Parsing and Render Processing                       |
\*---------------------------------------------------------*/
std::vector<std::string> PixelScreenPlugin::SplitUTF8(const std::string& str)
{
    std::vector<std::string> chars;
    for (size_t i = 0; i < str.length();)
    {
        unsigned char c = str[i];
        size_t len = 1;
        if (c >= 0xf0) len = 4;
        else if (c >= 0xe0) len = 3;
        else if (c >= 0xc0) len = 2;
        
        if (i + len > str.length()) len = str.length() - i;
        chars.push_back(str.substr(i, len));
        i += len;
    }
    return chars;
}

int PixelScreenPlugin::GetSpacing(const std::string& ch, const std::string& font_size, bool time)
{
    bool is_chinese = ch.length() > 1;
    if (font_size == "Chinese")
    {
        if (is_chinese)
        {
            return 9;
        }
        else
        {
            char digit = ch[0];
            switch (digit)
            {
                case '|': return 2;
                case 'i': case 'l': case '`': case '(': case ')': case ';': case ':': case '\'': case ',': case '.': case ' ': return 3;
                case 'I': case '!': case '[': case ']': case '1': return 4;
                case 'f': case 'h': case 'j': case 'k': case 'n': case 't': case 'u': case 'x':
                case 'y': case 'Z': case 'z': case '~': case '$': case '{': case '}': case '<': case '>': return 5;
                default: return 6;
            }
        }
    }
    else if (font_size == "Medium")
    {
        if (time)
        {
            char digit = ch[0];
            switch (digit)
            {
                case ':': case ';': case '.': return 2;
                case ' ': return 1;
                default: return 5;
            }
        }
        else
        {
            char digit = ch[0];
            switch (digit)
            {
                case ' ': return 1;
                case '!': case '|': case ':': case '\'': case '.': return 2;
                case '`': case '(': case ')': case '[': case ']': case ';': case ',': case '1': return 3;
                case 'a': case 'c': case 'I': case 'i': case 'j': case 'L': case 'l': case 'r':
                case 'Y': case '$': case '^': case '*': case '-': case '=': case '+': case '{':
                case '}': case '\\': case '"': case '<': case '>': case '/': case '?': return 4;
                case 'T': case 'W': case '@': case '#': case '%': case '&': return 6;
                default: return 5;
            }
        }
    }
    else if (font_size == "Large")
    {
        if (time)
        {
            char digit = ch[0];
            switch (digit)
            {
                case ':': case ';': case '.': return 3;
                case ' ': return 2;
                default: return 6;
            }
        }
        else
        {
            char digit = ch[0];
            switch (digit)
            {
                case '|': return 2;
                case 'i': case 'l': case '`': case '(': case ')': case ';': case ':': case '\'': case ',': case '.': case ' ': return 3;
                case 'I': case '!': case '[': case ']': case '1': return 4;
                case 'f': case 'h': case 'j': case 'k': case 'n': case 't': case 'u': case 'x':
                case 'y': case 'Z': case 'z': case '~': case '$': case '{': case '}': case '<': case '>': return 5;
                default: return 6;
            }
        }
    }
    else // Small
    {
        if (time)
        {
            char digit = ch[0];
            switch (digit)
            {
                case ':': case ';': case '.': return 2;
                case ' ': return 1;
                default: return 4;
            }
        }
        else
        {
            char digit = ch[0];
            switch (digit)
            {
                case ' ': return 1;
                case 'i': case 'l': case '!': case '|': case ':': case '.': return 2;
                case 'j': case 'r': case '1': case '`': case '(': case ')': case '[': case ']':
                case ';': case '\'': case ',': return 3;
                case '~': return 5;
                default: return 4;
            }
        }
    }
    return 6;
}

Glyph PixelScreenPlugin::GetGlyph(const std::string& ch, const std::string& font_size, bool /*time*/)
{
    char c = ch.empty() ? ' ' : ch[0];

    auto search_maps = [&](const std::map<char, Glyph>& primary, const std::map<char, Glyph>& secondary) -> Glyph
    {
        auto it = primary.find(c);
        if (it != primary.end()) return it->second;
        it = secondary.find(c);
        if (it != secondary.end()) return it->second;

        // Try uppercase
        if (c >= 'a' && c <= 'z')
        {
            char upper_c = c - 32;
            it = primary.find(upper_c);
            if (it != primary.end()) return it->second;
            it = secondary.find(upper_c);
            if (it != secondary.end()) return it->second;
        }
        // Try lowercase
        else if (c >= 'A' && c <= 'Z')
        {
            char lower_c = c + 32;
            it = primary.find(lower_c);
            if (it != primary.end()) return it->second;
            it = secondary.find(lower_c);
            if (it != secondary.end()) return it->second;
        }

        Glyph blank;
        blank.width = 4;
        blank.height = 8;
        blank.grid.assign(8, std::vector<unsigned char>(4, 0));
        return blank;
    };

    if (font_size == "Chinese")
    {
        if (ch.length() > 1)
        {
            auto it = zh_font.find(ch);
            if (it != zh_font.end()) return it->second;
        }
        return search_maps(large_letters, large_digits);
    }
    else if (font_size == "Large")
    {
        return search_maps(large_letters, large_digits);
    }
    else if (font_size == "Small")
    {
        return search_maps(small_letters, small_digits);
    }
    else // Medium
    {
        return search_maps(letters, digits);
    }
}

std::string PixelScreenPlugin::FormatDateTime(const std::string& format)
{
    QDateTime now = QDateTime::currentDateTime();
    QString qformat = QString::fromStdString(format);
    
    if (qformat.contains("tt"))
    {
        qformat.replace("tt", "ap");
    }
    else if (qformat.contains("t"))
    {
        qformat.replace("t", "a");
    }

    if (qformat.contains("aa"))
    {
        qformat.replace("aa", "ap");
    }
    else if (qformat.contains("AA"))
    {
        qformat.replace("AA", "AP");
    }
    
    QString result = now.toString(qformat);
    std::string str = result.toLower().toStdString();

    // Capitalize the first letter of each word
    bool cap_next = true;
    for (char& c : str)
    {
        if (std::isalpha((unsigned char)c))
        {
            if (cap_next) c = std::toupper((unsigned char)c);
            cap_next = false;
        }
        else
        {
            cap_next = true;
        }
    }

    // Convert "Am" -> "AM" and "Pm" -> "PM" when they stand as AM/PM meridian indicators
    for (size_t i = 0; i < str.size(); i++)
    {
        if ((str[i] == 'A' || str[i] == 'P') && i + 1 < str.size() && str[i + 1] == 'm')
        {
            bool prev_alpha = (i > 0 && std::isalpha((unsigned char)str[i - 1]));
            bool next_alpha = (i + 2 < str.size() && std::isalpha((unsigned char)str[i + 2]));
            if (!prev_alpha && !next_alpha)
            {
                str[i + 1] = 'M';
            }
        }
    }

    return str;
}

void PixelScreenPlugin::OnSensorDataUpdated()
{
    // Sensor cache is updated inside HardwareSensorManager.
    // resolveFormat() is called per-frame in OverlayTextOnController.
}

void PixelScreenPlugin::OnSensorTimerTimeout()
{
    if (!sensor_manager) return;

    // Only run curl/fetchSensors if at least one enabled device is set to "Sensor Data" mode (display_mode == 4)
    for (const auto& pair : settings.device_settings)
    {
        if (pair.second.enabled && pair.second.display_mode == 4)
        {
            sensor_manager->fetchSensors();
            break;
        }
    }
}

void PixelScreenPlugin::RenderFrame()
{
    std::shared_lock<std::shared_mutex> lock(matrix_zones_mutex);
    if (matrix_zones.empty()) return;

    in_callback = true;
    for (const auto& target : matrix_zones)
    {
        auto it = settings.device_settings.find(target.display_name);
        if (it != settings.device_settings.end() && it->second.enabled)
        {
            DeviceMatrixSettings& dev_s = it->second;

            float speed_step = (dev_s.scroll_speed / 100.0f) * 1.0f;
            if (dev_s.scroll_direction == "Left")
            {
                dev_s.scroll_offset -= speed_step;
            }
            else if (dev_s.scroll_direction == "Right")
            {
                dev_s.scroll_offset += speed_step;
            }
            else if (dev_s.scroll_direction == "Ping-Pong")
            {
                dev_s.scroll_offset += speed_step * dev_s.ping_pong_direction;
            }

            OverlayTextOnController(target, dev_s, true);
        }
    }
    in_callback = false;
}

void PixelScreenPlugin::OverlayTextOnController(const MatrixZoneTarget& target, DeviceMatrixSettings& dev_s, bool transparent)
{
    if (!target.controller) return;

    struct RenderedGlyph {
        Glyph glyph;
        int offset_x;
        int offset_y;    // line row offset in pixels
        bool blink_off;  // true = glyph occupies space but renders no pixels
    };

    std::string text = "";
    bool is_time_mode = (dev_s.display_mode == 0);
    bool colon_blink_off = false;
    if (is_time_mode)
    {
        text = FormatDateTime(dev_s.time_format);
        QTime now = QTime::currentTime();
        colon_blink_off = (now.second() % 2 != 0);
    }
    else if (dev_s.display_mode == 1)
    {
        text = dev_s.custom_text;
    }
    else if (dev_s.display_mode == 4)
    {
        // Sensor Data: resolve [sensor\path] tokens from HardwareSensorManager
        if (sensor_manager)
            text = sensor_manager->resolveFormat(dev_s.sensor_format);
        else
            text = dev_s.sensor_format;
    }
    else if (dev_s.display_mode == 2 || dev_s.display_mode == 3)
    {
        // Parse 2D Pixel Art Matrix JSON array (e.g. [[1,0,0,1], [0,1,1,0], ...])
        try
        {
            nlohmann::json art_j = nlohmann::json::parse(dev_s.pixel_art_json);
            if (art_j.is_array() && !art_j.empty())
            {
                Glyph custom_glyph;
                custom_glyph.height = art_j.size();
                custom_glyph.width = 0;
                custom_glyph.grid.resize(custom_glyph.height);

                for (size_t r = 0; r < art_j.size(); r++)
                {
                    if (art_j[r].is_array())
                    {
                        if (art_j[r].size() > custom_glyph.width)
                        {
                            custom_glyph.width = art_j[r].size();
                        }
                        custom_glyph.grid[r].resize(art_j[r].size());
                        for (size_t c = 0; c < art_j[r].size(); c++)
                        {
                            custom_glyph.grid[r][c] = art_j[r][c].get<int>();
                        }
                    }
                }

                if (custom_glyph.width > 0 && custom_glyph.height > 0)
                {
                    std::vector<RenderedGlyph> rendered_glyphs;
                    rendered_glyphs.push_back({custom_glyph, 0});
                    int total_width = custom_glyph.width;

                    const unsigned int* map = target.controller->GetZoneMatrixMapData(target.zone_idx);
                    unsigned int matrix_w = target.controller->GetZoneMatrixMapWidth(target.zone_idx);
                    unsigned int matrix_h = target.controller->GetZoneMatrixMapHeight(target.zone_idx);
                    unsigned int start_idx = target.controller->GetZoneStartIndex(target.zone_idx);
                    
                    if (matrix_w == 0 || matrix_h == 0 || !map) return;

                    int buffer_width = total_width;
                    if (dev_s.scroll_direction != "Off" && dev_s.scroll_direction != "Ping-Pong")
                    {
                        buffer_width += matrix_w / 2;
                    }
                    
                    if (dev_s.scroll_direction == "Ping-Pong")
                    {
                        float min_offset = (float)matrix_w - (float)buffer_width;
                        if (min_offset > 0.0f) min_offset = 0.0f;
                        
                        if (dev_s.scroll_offset <= min_offset)
                        {
                            dev_s.scroll_offset = min_offset;
                            dev_s.ping_pong_direction = 1;
                        }
                        else if (dev_s.scroll_offset >= 0.0f)
                        {
                            dev_s.scroll_offset = 0.0f;
                            dev_s.ping_pong_direction = -1;
                        }
                    }
                    else if (dev_s.scroll_direction != "Off")
                    {
                        if (dev_s.scroll_offset <= -buffer_width) dev_s.scroll_offset += buffer_width;
                        if (dev_s.scroll_offset >= buffer_width) dev_s.scroll_offset -= buffer_width;
                    }
                    else
                    {
                        dev_s.scroll_offset = 0.0f;
                    }
                    
                    RGBColor text_color = ToRGBColor(dev_s.text_r, dev_s.text_g, dev_s.text_b);
                    
                    for (unsigned int y = 0; y < matrix_h; y++)
                    {
                        for (unsigned int x = 0; x < matrix_w; x++)
                        {
                            unsigned int led_idx = map[y * matrix_w + x];
                            if (led_idx == 0xFFFFFFFF) continue;
                            
                            int src_x;
                            if (dev_s.scroll_direction == "Off")
                            {
                                src_x = (int)x;
                            }
                            else
                            {
                                src_x = (int)std::floor((float)x - dev_s.scroll_offset) % buffer_width;
                                if (src_x < 0) src_x += buffer_width;
                            }
                            
                            int src_y = (int)y - dev_s.padding_y;
                            src_x = src_x - dev_s.padding_x;
                            
                            bool pixel_on = false;
                            
                            if (src_x >= 0 && src_x < total_width)
                            {
                                for (const auto& rg : rendered_glyphs)
                                {
                                    int local_x = src_x - rg.offset_x;
                                    if (local_x >= 0 && local_x < (int)rg.glyph.width)
                                    {
                                        if (src_y >= 0 && src_y < (int)rg.glyph.height)
                                        {
                                            if (rg.glyph.grid[src_y][local_x] > 0)
                                            {
                                                pixel_on = true;
                                            }
                                        }
                                        break;
                                    }
                                }
                            }
                            
                            RGBColor underlying_color = (start_idx + led_idx < target.controller->GetLEDCount()) ? target.controller->GetColor(start_idx + led_idx) : ToRGBColor(0, 0, 0);
                            
                            RGBColor pixel_color;
                            if (dev_s.invert_color)
                            {
                                pixel_color = pixel_on ? underlying_color : text_color;
                            }
                            else
                            {
                                pixel_color = pixel_on ? text_color : underlying_color;
                            }
                            
                            target.controller->SetColor(start_idx + led_idx, pixel_color);
                        }
                    }
                    return;
                }
            }
        }
        catch (...)
        {
            text = "ERR";
        }
    }
    
    // Split text into lines on \n
    std::vector<std::string> lines;
    {
        std::string remaining = text;
        size_t pos;
        while ((pos = remaining.find('\n')) != std::string::npos)
        {
            lines.push_back(remaining.substr(0, pos));
            remaining = remaining.substr(pos + 1);
        }
        lines.push_back(remaining);
    }

    // Determine glyph height for line spacing (use first glyph of first line)
    int glyph_h = 8; // default
    {
        for (const auto& line : lines)
        {
            std::vector<std::string> first_chars = SplitUTF8(line);
            if (!first_chars.empty())
            {
                Glyph sample = GetGlyph(first_chars[0], dev_s.font_size, is_time_mode);
                if (sample.height > 0) { glyph_h = (int)sample.height; break; }
            }
        }
    }
    int line_spacing = glyph_h + 1; // 1px gap between lines

    // Pass 1: compute width of each line
    std::vector<int> line_widths;
    for (const auto& line : lines)
    {
        std::vector<std::string> chars = SplitUTF8(line);
        int w = 0;
        for (const auto& ch : chars)
            w += GetSpacing(ch, dev_s.font_size, is_time_mode);
        line_widths.push_back(w);
    }

    int total_width = 0;
    for (int w : line_widths)
        if (w > total_width) total_width = w;
    if (total_width <= 0) total_width = 1;

    // Pass 2: build rendered glyphs with per-line alignment offset baked into offset_x
    std::vector<RenderedGlyph> rendered_glyphs;
    for (int line_idx = 0; line_idx < (int)lines.size(); line_idx++)
    {
        std::vector<std::string> chars = SplitUTF8(lines[line_idx]);
        int line_w = line_widths[line_idx];
        int line_y = line_idx * line_spacing;

        // Per-line alignment offset (shifts shorter lines relative to widest)
        int line_align = 0;
        if (dev_s.text_align == 1) // Center: shift shorter lines right
            line_align = (total_width - line_w) / 2;
        else if (dev_s.text_align == 2) // End: right-align each line
            line_align = total_width - line_w;

        int line_x = 0;
        for (const auto& ch : chars)
        {
            Glyph g = GetGlyph(ch, dev_s.font_size, is_time_mode);
            int spacing = GetSpacing(ch, dev_s.font_size, is_time_mode);
            bool blink_off = colon_blink_off && (ch == ":");
            rendered_glyphs.push_back({g, line_x + line_align, line_y, blink_off});
            line_x += spacing;
        }
    }

    const unsigned int* map = target.controller->GetZoneMatrixMapData(target.zone_idx);
    unsigned int matrix_w = target.controller->GetZoneMatrixMapWidth(target.zone_idx);
    unsigned int matrix_h = target.controller->GetZoneMatrixMapHeight(target.zone_idx);
    unsigned int start_idx = target.controller->GetZoneStartIndex(target.zone_idx);
    
    if (matrix_w == 0 || matrix_h == 0 || !map) return;
    
    int buffer_width = total_width;
    if (dev_s.scroll_direction != "Off" && dev_s.scroll_direction != "Ping-Pong")
    {
        buffer_width += matrix_w / 2;
    }
    
    if (dev_s.scroll_direction == "Ping-Pong")
    {
        float min_offset = (float)matrix_w - (float)buffer_width;
        if (min_offset > 0.0f) min_offset = 0.0f;
        
        if (dev_s.scroll_offset <= min_offset)
        {
            dev_s.scroll_offset = min_offset;
            dev_s.ping_pong_direction = 1;
        }
        else if (dev_s.scroll_offset >= 0.0f)
        {
            dev_s.scroll_offset = 0.0f;
            dev_s.ping_pong_direction = -1;
        }
    }
    else if (dev_s.scroll_direction != "Off")
    {
        if (dev_s.scroll_offset <= -buffer_width) dev_s.scroll_offset += buffer_width;
        if (dev_s.scroll_offset >= buffer_width) dev_s.scroll_offset -= buffer_width;
    }
    int screen_align = 0;
    if (dev_s.scroll_direction == "Off")
    {
        if (dev_s.text_align == 1)      // Center
        {
            screen_align = ((int)matrix_w - total_width) / 2;
        }
        else if (dev_s.text_align == 2) // End
        {
            screen_align = (int)matrix_w - total_width;
        }
    }

    RGBColor text_color = ToRGBColor(dev_s.text_r, dev_s.text_g, dev_s.text_b);
    
    for (unsigned int y = 0; y < matrix_h; y++)
    {
        for (unsigned int x = 0; x < matrix_w; x++)
        {
            unsigned int led_idx = map[y * matrix_w + x];
            if (led_idx == 0xFFFFFFFF) continue;
            
            int src_x;
            if (dev_s.scroll_direction == "Off")
            {
                src_x = (int)x - screen_align;
            }
            else
            {
                src_x = (int)std::floor((float)x - dev_s.scroll_offset) % buffer_width;
                if (src_x < 0) src_x += buffer_width;
            }
            
            int src_y = (int)y - dev_s.padding_y;
            src_x = src_x - dev_s.padding_x;
            
            bool pixel_on = false;
            
            for (const auto& rg : rendered_glyphs)
            {
                if (rg.blink_off) continue;
                int local_x = src_x - rg.offset_x;
                if (local_x >= 0 && local_x < (int)rg.glyph.width)
                {
                    int local_y = src_y - rg.offset_y;
                    if (local_y >= 0 && local_y < (int)rg.glyph.height)
                    {
                        if (rg.glyph.grid[local_y][local_x] > 0)
                        {
                            pixel_on = true;
                            break;
                        }
                    }
                }
            }
            
            RGBColor underlying_color = (start_idx + led_idx < target.controller->GetLEDCount()) ? target.controller->GetColor(start_idx + led_idx) : ToRGBColor(0, 0, 0);
            
            RGBColor pixel_color;
            if (dev_s.invert_color)
            {
                pixel_color = pixel_on ? underlying_color : text_color;
            }
            else
            {
                pixel_color = pixel_on ? text_color : underlying_color;
            }
            
            target.controller->SetColor(start_idx + led_idx, pixel_color);
        }
    }
    
    if (!transparent)
    {
        target.controller->UpdateLEDs();
    }
}
