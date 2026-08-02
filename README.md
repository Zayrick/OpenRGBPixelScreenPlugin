# Pixel Screen Plugin
![Github All Releases](https://img.shields.io/github/downloads/qiangqiang101/OpenRGBPixelScreenPlugin/total.svg)
![GitHub release (latest by date)](https://img.shields.io/github/v/release/qiangqiang101/OpenRGBPixelScreenPlugin)
![GitHub](https://img.shields.io/github/license/qiangqiang101/OpenRGBPixelScreenPlugin)
![GitHub branch checks state](https://img.shields.io/github/checks-status/qiangqiang101/OpenRGBPixelScreenPlugin/master)
![GitHub issues](https://img.shields.io/github/issues/qiangqiang101/OpenRGBPixelScreenPlugin)
![GitHub forks](https://img.shields.io/github/forks/qiangqiang101/OpenRGBPixelScreenPlugin?style=social)
![GitHub Repo stars](https://img.shields.io/github/stars/qiangqiang101/OpenRGBPixelScreenPlugin?style=social)
![YouTube Channel Subscribers](https://img.shields.io/youtube/channel/subscribers/UCAZlasvEy1euunP1M7nwj5Q?style=social)
[![Donate via PayPal](https://img.shields.io/badge/Donate-Paypal-brightgreen)](https://paypal.me/imnotmental)
[![Follow on Patreon](https://img.shields.io/badge/Donate-Patreon-orange)](https://www.patreon.com/imnotmental)

[中文 README](README.CN.md)

<img width="870" height="621" alt="image" src="https://github.com/user-attachments/assets/1f94cb0d-d0f7-4d63-8864-2c253d62358f" />
<img width="870" height="621" alt="image" src="https://github.com/user-attachments/assets/b7f89322-ac52-4860-b4c5-aa84c43c4ebc" />

Render custom scrolling text, clock, hardware sensors and pixel art on OpenRGB matrices.

# 🛠️ Download
## OpenRGB 0.9+ Pipeline/Next (Plugin API Version 5)
- [Windows 64](https://github.com/qiangqiang101/OpenRGBPixelScreenPlugin/releases)
- [Linux amd64](https://github.com/qiangqiang101/OpenRGBPixelScreenPlugin/releases)
- [Linux arm64](https://github.com/qiangqiang101/OpenRGBPixelScreenPlugin/releases)
- [Linux armhf](https://github.com/qiangqiang101/OpenRGBPixelScreenPlugin/releases)
- [Linux i386](https://github.com/qiangqiang101/OpenRGBPixelScreenPlugin/releases)

## OpenRGB 1.0 RC - 1.0 RC3 (Plugin API Version 4)
- [Windows 86](https://github.com/qiangqiang101/OpenRGBPixelScreenPlugin/releases)
- [Windows 64](https://github.com/qiangqiang101/OpenRGBPixelScreenPlugin/releases)
- [Linux amd64](https://github.com/qiangqiang101/OpenRGBPixelScreenPlugin/releases)
- [Linux arm64](https://github.com/qiangqiang101/OpenRGBPixelScreenPlugin/releases)
- [Linux armhf](https://github.com/qiangqiang101/OpenRGBPixelScreenPlugin/releases)
- [Linux i386](https://github.com/qiangqiang101/OpenRGBPixelScreenPlugin/releases)

You can get older releases [here](https://github.com/qiangqiang101/OpenRGBPixelScreenPlugin/releases).

# 🔨 Build yourself
This repository contains two plugin variants, each built against a different OpenRGB plugin API:

- `API5/` — for OpenRGB 0.9+ / Pipeline / Next (Plugin API 5)
- `API4/` — for OpenRGB 1.0 RC / older builds (Plugin API 4)

Use the matching folder for the OpenRGB version you plan to run.

## Prerequisites

Before building, install:

- Git
- A Qt development environment with `qmake`
- A C++17-capable compiler
- The OpenRGB source code included in this repo under each API folder (`API4/OpenRGB` or `API5/OpenRGB`)

Recommended Qt versions:

- OpenRGB 0.9+ / API 5: Qt 6.8.x
- OpenRGB 1.0 RC / API 4: Qt 5.15.x

### Windows

- Install Qt with the MSVC toolchain and `qmake`
- Install the Visual Studio C++ build tools or a compatible compiler
- Open a Qt-enabled shell (for example, Qt 6 x64 Native Tools Command Prompt or Qt Creator)

### Linux

Install the usual Qt build dependencies, for example:

- Debian/Ubuntu:
  `sudo apt install git build-essential qtcreator qtbase5-dev qtchooser qt5-qmake qtbase5-dev-tools libusb-1.0-0-dev libhidapi-dev pkgconf libmbedtls-dev qttools5-dev-tools`

- Fedora:
  `sudo dnf install automake gcc-c++ git hidapi-devel libusbx-devel mbedtls-devel pkgconf qt5-qtbase-devel qt5-linguist`

For Qt 6-based builds, use the matching Qt 6 development packages instead of the Qt 5 ones.

## Build with Qt Creator (recommended)

1. Clone the repo with submodules:
   `git clone --recurse-submodules https://github.com/qiangqiang101/OpenRGBPixelScreenPlugin.git`
2. Open the matching project file in Qt Creator:
   - `API5/PixelScreenPlugin.pro`
   - or `API4/PixelScreenPlugin.pro`
3. Select the correct Qt kit and compiler for your target.
4. Run `qmake` and then build the project.
5. The plugin output will be generated in the build output directory for that kit.

## Build from the command line

### API 5 build example

```bash
cd API5
qmake PixelScreenPlugin.pro
make -j$(nproc)
```

### API 4 build example

```bash
cd API4
qmake PixelScreenPlugin.pro
make -j$(nproc)
```

### Windows (MSVC)

```bat
cd API5
qmake PixelScreenPlugin.pro
nmake
```

or:

```bat
cd API4
qmake PixelScreenPlugin.pro
nmake
```

## Install / use the plugin

After building, copy the generated plugin library into your OpenRGB plugins directory or use the plugin manager in OpenRGB to load it.

For Linux, the project is set up for installation into a standard OpenRGB plugin path when using `make install` with an appropriate `PREFIX` value:

```bash
make install PREFIX=/usr
```

On Windows, the plugin is typically loaded from the build output folder or copied into the OpenRGB plugin directory used by your installed OpenRGB build.

## Notes

- The project files automatically generate metadata such as version info and API version at build time.
- The plugin source uses the bundled OpenRGB SDK in the `OpenRGB/` folder inside each API directory, so you should build the matching API folder for the OpenRGB version you want to target.
- If you are unsure which API to use, choose `API5` for newer OpenRGB builds and `API4` for older/RC builds.
