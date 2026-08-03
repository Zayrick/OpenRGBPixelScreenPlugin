/*---------------------------------------------------------*\
| OpenRGBMatrixTextPlugin.h                                 |
|                                                           |
|   OpenRGB Matrix Text Plugin                              |
|                                                           |
|   This file is part of the OpenRGB Matrix Text Plugin     |
|   project                                                 |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
| \*---------------------------------------------------------*/

#pragma once

#include <QMenu>
#include <QObject>
#include <QWidget>
#include <QTimer>
#include <map>
#include <vector>
#include <string>
#include <shared_mutex>
#include "OpenRGBPluginInterface.h"
#include "RGBController.h"
#include "LogManager.h"

// Font structure representing a character's LED matrix layout
struct Glyph
{
    std::vector<std::vector<unsigned char>> grid;
    unsigned int width = 0;
    unsigned int height = 0;
};

// Target matrix zone structure
struct MatrixZoneTarget
{
    RGBController* controller;
    unsigned int zone_idx;
    std::string controller_name;
    std::string zone_name;
    std::string display_name; // "ControllerName: ZoneName"
};

// Per-device matrix settings structure
struct DeviceMatrixSettings
{
    bool enabled = false;
    int display_mode = 1;       // 0: Time, 1: Custom Text, 2: Pixel Art
    std::string font_size = "Medium"; // "Small", "Medium", "Large", "Chinese"
    std::string custom_text = "OpenRGB";
    std::string time_format = "hh:mm tt";
    std::string pixel_art_json = "[ [1, 0, 0, 1], [0, 1, 1, 0], [0, 1, 1, 0], [1, 0, 0, 1] ]"; // 2D Pixel Art Matrix JSON
    std::string scroll_direction = "Left"; // "Off", "Left", "Right", "Ping-Pong"
    int scroll_speed = 50;      // 1 to 100
    int fps = 20;               // 1 to 60 FPS
    unsigned char text_r = 255;
    unsigned char text_g = 255;
    unsigned char text_b = 255;
    bool invert_color = false;
    int padding_x = 0;
    int padding_y = 0;
    int text_align = 0; // 0: Start, 1: Center, 2: End

    // Runtime state
    float scroll_offset = 0.0f;
    int ping_pong_direction = -1;
};

struct MatrixTextSettings
{
    std::map<std::string, DeviceMatrixSettings> device_settings;

    DeviceMatrixSettings& GetForDevice(const std::string& display_name)
    {
        auto it = device_settings.find(display_name);
        if (it == device_settings.end())
        {
            DeviceMatrixSettings default_s;
            device_settings[display_name] = default_s;
        }
        return device_settings[display_name];
    }

    bool HasDevice(const std::string& display_name) const
    {
        return device_settings.find(display_name) != device_settings.end();
    }
};

class PixelScreenTab;

class PixelScreenPlugin : public QObject, public OpenRGBPluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID OpenRGBPluginInterface_IID FILE "PixelScreenPlugin.json")
    Q_INTERFACES(OpenRGBPluginInterface)

public:
    ~PixelScreenPlugin() {};

    /*-----------------------------------------------------*\
    | Plugin Information                                    |
    \*-----------------------------------------------------*/
    virtual OpenRGBPluginInfo   GetPluginInfo()                                                                 override;
    virtual unsigned int        GetPluginAPIVersion()                                                           override;

    /*-----------------------------------------------------*\
    | Plugin Functionality                                  |
    \*-----------------------------------------------------*/
    virtual void                Load(ResourceManagerInterface* resource_manager_ptr)                            override;
    virtual QWidget*            GetWidget()                                                                     override;
    virtual QMenu*              GetTrayMenu()                                                                   override;
    virtual void                Unload()                                                                        override;

    /*-----------------------------------------------------*\
    | Update Signals                                        |
    \*-----------------------------------------------------*/
    void                        ProfileManagerUpdated(unsigned int update_reason);
    void                        ResourceManagerUpdated(unsigned int update_reason);
    void                        SettingsManagerUpdated(unsigned int update_reason);

    /*-----------------------------------------------------*\
    | Font Loading and Text Rendering                       |
    \*-----------------------------------------------------*/
    void                        LoadFonts();
    void                        LoadSettings();
    void                        SaveSettings();
    
    std::vector<MatrixZoneTarget> GetMatrixZones();
    void                        UpdateControllers();
    static void                 OnDeviceListChanged(void* arg);
    static void                 OnControllerUpdate(void* arg);

private slots:
    void                        RenderFrame();

public:
    /*-----------------------------------------------------*\
    | Plugin Global Variables                               |
    \*-----------------------------------------------------*/
    static ResourceManagerInterface*   api;
    static PixelScreenPlugin*          plugin_instance;
    MatrixTextSettings                  settings;
    std::vector<MatrixZoneTarget>       matrix_zones;
    std::shared_mutex                   matrix_zones_mutex;

private:
    /*-----------------------------------------------------*\
    | User interface widget                                 |
    \*-----------------------------------------------------*/
    PixelScreenTab*                    ui = nullptr;
    QTimer*                             render_timer = nullptr;
    bool                                in_callback = false;
    
    // Font databases loaded from JSON
    std::map<char, Glyph>               small_letters;
    std::map<char, Glyph>               letters;
    std::map<char, Glyph>               large_letters;
    std::map<char, Glyph>               digits;
    std::map<char, Glyph>               small_digits;
    std::map<char, Glyph>               large_digits;
    std::map<std::string, Glyph>        zh_font;

    // Scrolling states
    float                               scroll_offset = 0.0f;
    int                                 ping_pong_direction = -1;

    // Helper functions for rendering
    std::vector<std::string>            SplitUTF8(const std::string& str);
    int                                 GetSpacing(const std::string& ch, const std::string& font_size, bool time);
    Glyph                               GetGlyph(const std::string& ch, const std::string& font_size, bool time);
    std::string                         FormatDateTime(const std::string& format);
    void                                OverlayTextOnController(const MatrixZoneTarget& target, DeviceMatrixSettings& dev_settings, bool transparent);
};

/*---------------------------------------------------------*\
| Logging mapping to QDebug for compatibility              |
\*---------------------------------------------------------*/
#include <QDebug>
#undef  LOG_FATAL
#undef  LOG_ERROR
#undef  LOG_WARNING
#undef  LOG_INFO
#define LOG_INFO(...)    qDebug(__VA_ARGS__)
#define LOG_WARN(...)   LOG_WARNING(__VA_ARGS__)
#define LOG_ERROR(...)   qCritical(__VA_ARGS__)
#define LOG_WARNING(...) qWarning(__VA_ARGS__)
#define LOG_WARN(...)   LOG_WARNING(__VA_ARGS__)
