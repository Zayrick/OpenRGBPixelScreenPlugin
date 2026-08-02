# OpenRGB Matrix Text Plugin

A high-performance C++ plugin for [OpenRGB](https://gitlab.com/CalcProgrammer1/OpenRGB) that renders scrolling text, static custom messages, and local time clocks directly onto RGB device layouts with matrix zones (e.g., matrices, panels, and segmented LED strips).

Inspired by the SignalRGB WLED clock/pixel-art text layout effect, this plugin adapts text rendering and smooth frame animation to OpenRGB's native C++ plugin SDK, offering premium real-time rendering.

## Features

- 🕒 **Time/Clock Mode**: Displays the current local system time, supporting customizable date-time formats (such as `hh:mm tt` or `HH:mm:ss`). It includes a customizable flashing colon separator.
- 💬 **Custom Text Mode**: Displays a user-specified custom string on selected matrices.
- 🔠 **Multiple Font Sizes**: Select between **Small** (3x5), **Medium** (5x7), **Large** (6x8), and **Chinese** (8x8) font weights.
- 🇨🇳 **Full UTF-8 & Chinese Font Support**: Renders multi-byte Chinese characters natively using the embedded 8x8 font grid database.
- 🎨 **Visual Settings**:
  - Dynamic **Text Color** and **Background Color** selection.
  - **Invert Color** checkbox to swap text and background rendering styles.
  - **Padding X & Y** spinboxes to adjust offsets and align text perfectly on different grid layouts.
- 🏃 **Animation Options**:
  - Scroll speeds from 1 to 100.
  - Multiple scroll directions: `Off` (Static), `Left`, `Right`, and `Ping-Pong` (bounce back and forth).
- 🎛️ **Target Zone Selector**: Multi-select target matrix zones dynamically from your detected OpenRGB controllers.

## Build Instructions

### Prerequisites
- [Qt Creator](https://www.qt.io/product/development-tools) and a C++17 compiler (GCC, Clang, or MSVC) configured to build Qt applications.
- Ensure the Qt version matches the Qt version of your OpenRGB binary (usually Qt 5.x or Qt 6.x).

### Steps
1. Clone the repository and initialize submodules:
   ```bash
   git clone --recursive https://github.com/qiangqiang101/OpenRGBMatrixTextPlugin
   cd OpenRGBMatrixTextPlugin
   ```
2. Open `OpenRGBMatrixTextPlugin.pro` in **Qt Creator**.
3. Choose the build kit matching your compiler and Qt version.
4. Build the project.
5. Retrieve the built plugin binary:
   - On Windows: Look in the build output `release/` or `debug/` directory for `OpenRGBMatrixTextPlugin.dll`.
   - On Linux/macOS: Look for `libOpenRGBMatrixTextPlugin.so` or `libOpenRGBMatrixTextPlugin.dylib`.

## Installation

1. Copy the built plugin binary to your OpenRGB plugins directory:
   - **Windows**: `%APPDATA%\OpenRGB\plugins`
   - **Linux**: `~/.config/OpenRGB/plugins`
2. Open OpenRGB, go to **Settings** -> **Plugins**.
3. Click **Install Plugin** and select the plugin file.
4. Go to the new **Matrix Text** tab on the top menu to configure your matrix displays!
