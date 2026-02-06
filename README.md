# MineSweeper

## Installation

### Compiling from source code

#### Configure CMake

```bash
rm -rf build
cmake -B build -S . -DCMAKE_INSTALL_PREFIX=/usr
```

#### Build the project

```bash
cmake --build build
```

#### 
To install the project into the default directory:

```bash
sudo cmake --install build
```

To install the project into a different directory:
```bash
DESTDIR=/path/to/dir cmake --install build
```
