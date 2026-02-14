# MineSweeper
[![Build](https://github.com/pstasiak2000/MineSweeper/actions/workflows/deb-release.yml/badge.svg)](https://github.com/pstasiak2000/MineSweeper/releases/latest)
[![GitHub release](https://img.shields.io/github/v/release/pstasiak2000/MineSweeper)](https://github.com/pstasiak2000/MineSweeper/releases/latest)

MineSweeper is a simple Minesweeper clone implemented in C using GTK4.  
It provides a classic Minesweeper experience with a modern GTK4 interface, and is designed to run on Linux systems.

---

## Table of Contents

- [Installation](#installation)
  - [Arch Linux](#arch-linux)
  - [Ubuntu/Debian](#debian)
  - [Compiling from Source](#compiling-from-source)
- [Post-install Notes](#post-install-notes)
- [Dependencies](#dependencies)
- [Troubleshooting](#troubleshooting)

---

## Installation

### Ubuntu/Debian

#### Ubuntu 23.0 / Debian 13 or newer
You can download the build and release .deb package (follow the Build and Release .deb status badge above) and install it using

```bash
 sudo apt install ./minesweeper*.deb
```

#### Ubuntu 22.0 / Debian 12 or older 
For older versions of Ubuntu, you will need to build the .deb package first. The script is provided in ```/scripts/build-deb.sh```.

Firstly make sure the relevant dependencies are installed:

```bash
  sudo apt update
  sudo apt install libgtk-4-dev librsvg2-dev devscripts debhelper dh-cmake 
```
Now from repo root, run the build script 

```bash
./scripts/build-deb.sh
```

This will create a ```minesweeper_X.Y.Z-1_amd64.deb``` in the parent directory. You can now install the application with

```bash
  sudo apt install ./minesweeper*.deb
```

### Arch Linux

On Arch Linux and derivatives, you can build and install using the provided PKGBUILD:

```bash
# Clone the repository
git clone https://github.com/pstasiak2000/MineSweeper.git
cd MineSweeper

# Copy the Arch PKGBUILD to the root
cp packaging/arch/PKGBUILD .

# Build and install the package
makepkg -si
```

> [!WARNING]
> Note: The PKGBUILD assumes the repository layout is unchanged. Moving files or directories may cause build failures.

After installation, MineSweeper will be available as a standard application and should appear in your desktop environment’s application menu.

## Compiling from source

If you prefer to build directly from source, follow these steps:

### 1. Configure the project with CMake

```bash
# Clean previous builds
rm -rf build

# Configure the build directory
cmake -B build -S . -DCMAKE_INSTALL_PREFIX=/usr
```
If you wish to compile the source code with developer tools included, you can enable them by setting ```-DCOMPILE_WITH_DEV_TOOLS=TRUE``` during the configuration.


### 2. Build the project

```bash
cmake --build build
```

This will compile all source files and generate the MineSweeper executable in the build directory.

### 3. Install the project

* **To install to the default systems directories:**

```bash
sudo cmake --install build
```

* **To install to a custom directory (useful for staging or packaging):**

```bash
DESTDIR=/path/to/install cmake --install build
```


## Post-install notes
* The application installs a ```.desktop``` file, so it should integrate with most Linux desktop environments automatically.
* Icons are installed in standard locations (```/usr/share/icons/hicolor/...```) and should appear in menus and window decorations.
* If you’re running KDE, you may need to refresh the application cache:
```bash
kbuildsycoca5
```
* To uninstall:
    * If installed via PKGBUILD:
    
    ```bash
        sudo pacman -R minesweeper
    ```

    * If installed manually: remove the installed files in ```/usr/bin```, ```/usr/share/applications/```, ```/usr/share/icons/```, and ```/usr/share/metainfo/```.


## Dependencies

* GTK4
* librsvg
* glib2 (for building)
* cmake (for building)
* pkgconf (for building)

## Troubleshooting

* **CMAKE cannot find** ```CMakeLists.txt```**:**
Make sure you run ``` cmake -S .``` from the **repository root**, not from ```src/``` or any other subdirectory.

* **Resources errors (** ```resources.c```, ```.gresource.xml```, ```.png``` **/** ```.svg``` **missing):**
Ensure that ```resources.c``` is generated or included in ```src/``` and that the paths in ```resources.gresource.xml``` are correct relative to the repository root.

* **PKGBUILD build errors with local sources:**
Use ```source=("file://../")``` if the PKGBUILD is in a subdirectory, pointing to the repository root.

This ensures ```$srcdir``` resolves correctly.

* **Application icon not showing in KDE:**
Refresh the application cache:

```bash
kbuildsycoca5
```

* **General build errors:**
Make sure all dependencies (```gtk4```, ```librsvg```,```glib2```,```cmake```,```pkgconf```) are installed.

Clean your build directory if necessary:

```bash
rm -rf build
```
