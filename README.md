# WasIstLos

An unofficial WhatsApp desktop application for Linux.

> [!NOTE]
> We are looking for flatpak and snap maintainers. Please get in touch if you are interested!

<p align="left">
    <a href="https://flathub.org/apps/details/com.github.xeco23.WasIstLos">
        <img align="center" alt="Download on Flathub" src="https://flathub.org/assets/badges/flathub-badge-en.png" width="200">
    </a>
    <a href="https://snapcraft.io/wasistlos">
        <img align="center" alt="Get it from the Snap Store" src="https://snapcraft.io/static/images/badges/en/snap-store-black.svg" width="200">
    </a>
</p>

[![Action Status](https://github.com/xeco23/WasIstLos/workflows/Linter/badge.svg)](https://github.com/xeco23/WasIstLos/actions/workflows/linter.yml)
[![Action Status](https://github.com/xeco23/WasIstLos/workflows/Build/badge.svg)](https://github.com/xeco23/WasIstLos/actions/workflows/build.yml)
[![Action Status](https://github.com/xeco23/WasIstLos/workflows/Install/badge.svg)](https://github.com/xeco23/WasIstLos/actions/workflows/install.yml)
[![Action Status](https://github.com/xeco23/WasIstLos/workflows/Release/badge.svg)](https://github.com/xeco23/WasIstLos/actions/workflows/release.yml)
[![POEditor](https://img.shields.io/badge/Translations-POEditor-brightgreen)](https://poeditor.com/join/project/jMGkxVn3vN)

![App Window](screenshot/app.png)


## About

WasIstLos is an unofficial WhatsApp desktop application written in C++ with the help of gtkmm and WebKitGtk libraries.
Check out [wiki](https://github.com/xeco23/WasIstLos/wiki) for further details.


## Features

* Features come with WhatsApp Web
  * WhatsApp specific keyboard shortcuts work with *Alt* key instead of *Cmd*
* Zoom in/out
* System tray icon
* Notification sounds
* Autostart with system
* Fullscreen mode
* Show/Hide headerbar by pressing *Alt+H*
* Localization support in system language
* Spell checking in system language. You need to install the corresponding dictionary to get this working i.e. `hunspell-en_us` package for US English
* Open chat by phone number


## Using WasIstLos

The application is available from a number of Linux distributions:

[![Packaging status](https://repology.org/badge/vertical-allrepos/wasistlos.svg)](https://repology.org/project/wasistlos/versions)


## Dependencies

* cmake >= 3.12
* intltool
* gtkmm-3.0
* webkit2gtk-4.1
* ayatana-appindicator3-0.1
* libcanberra
* libhunspell (Optional for spell checking)


## Build & Run

### Development

```bash
# Create a debug build directory and go into it
mkdir -p build/debug && cd build/debug

# Build the project
cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=/usr ../..
make -j4

# Optionally, to update the default translation file
make update-translation

# Run
./wasistlos
```

### Local installation

```bash
# Run inside the build directory once the application is built
# You'll probably need administrator privileges for this
make install
```

### Uninstall

```bash
# Run inside the build directory if you want to uninstall all files
# install_manifest.txt file is created when you run make install
xargs rm < install_manifest.txt
```


## Packaging

### Debian

```bash
# Don't forget to update the version number (0) in debian/changelog before this
# Build the package.
dpkg-buildpackage -uc -us -ui
```

### Snap

```bash
# Build the package. Pass --use-lxd option in a virtual environment
snapcraft
```

### AppImage

```bash
# Make sure that the application is installed into the `<Project Root>/AppDir` directory
make install DESTDIR=../../AppDir

# Build the package
appimage-builder --skip-test --recipe ./appimage/AppImageBuilder.yml
```


## Contributing

Please read [contributing](CONTRIBUTING.md).

### Code Contributors

[![Code Contributors](https://opencollective.com/WasIstLos/contributors.svg?width=880&button=false)](https://github.com/xeco23/WasIstLos/graphs/contributors)
