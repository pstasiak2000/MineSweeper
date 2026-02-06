# MineSweeper

MineSweeper is a simple Minesweeper clone implemented in C using GTK4.  
It provides a classic Minesweeper experience with a modern GTK4 interface, and is designed to run on Linux systems.

---

## Table of Contents

- [Installation](#installation)
  - [Arch Linux](#arch-linux)
  - [Compiling from Source](#compiling-from-source)
- [Post-install Notes](#post-install-notes)
- [Dependencies](#dependencies)
- [Troubleshooting](#troubleshooting)

---

## Installation

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

```warning
Note: The PKGBUILD assumes the repository layout is unchanged. Moving files or directories may cause build failures.
```