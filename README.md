# 🪐 N-Body Gravity Simulator

A real-time gravitational N-body simulation built with **C++ and SFML 3.0**, featuring three simulation modes with orbital trail rendering.

![N-Body Gravity Sim](Screenshot%202026-06-17%20151804.png)

## ✨ Features

- **Solar System** — Sun with 8 planets in realistic circular orbits
- **Random Orbits** — 25 bodies orbiting a central mass
- **Binary Star** — Two massive stars orbiting each other with surrounding bodies
- Colorful orbital trails with fade effect
- Add new bodies by clicking anywhere
- Adjustable simulation speed

## 🎮 Controls

| Key | Action |
|-----|--------|
| `1` | Solar System mode |
| `2` | Random Orbits mode |
| `3` | Binary Star mode |
| `Space` | Pause / Resume |
| `T` | Toggle trails |
| `LMB` | Add body at cursor |
| `+` / `-` | Speed up / slow down |
| `Esc` | Quit |

## 🛠️ Build from Source

**Requirements:** C++17, CMake 3.16+, SFML 3.0, vcpkg, MinGW

```bash
mkdir build && cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/scripts/buildsystems/vcpkg.cmake \
         -DSFML_DIR=C:/vcpkg/installed/x86-mingw-dynamic/share/sfml \
         -G "MinGW Makefiles"
mingw32-make
```

## ▶️ Run (Windows)

1. Download `NBodyGravity-v1.0-Windows.zip`
2. Extract all files
3. Run `NBodyGravity.exe`

## 🔬 Physics

Uses direct O(n²) gravitational force calculation with softening factor to prevent singularities at close range.
