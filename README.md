# Prank Squad

A 2.5D stick figure fighting game inspired by **Little Fighter 2** (小朋友齐打交), built with **Godot 4** and **C++ GDExtension**.

## Features

- **Data-driven frame system** — All character actions driven by `FrameData`, not hardcoded logic
- **C++ core engine** — Frame interpreter, combat system, AI written in C++ for 60fps performance
- **Text-to-character generation** — Describe a character in natural language, and the system generates its stats, moves, and sprites automatically
- **2.5D perspective** — True 3-axis movement (X/Y/Z) rendered in 2D
- **Local multiplayer** — Up to 4 players on the same screen

## Project Status

Phase 0 — Core engine scaffolding. See [PRD.md](PRD.md) for full roadmap.

## Build

### Prerequisites

- Godot 4.3+
- C++17 compiler (MSVC on Windows, GCC/Clang on Linux/macOS)
- SCons (`pip install scons`)
- godot-cpp submodule

### Setup

```bash
git submodule update --init
cd godot-cpp
scons platform=windows
cd ..
scons platform=windows
```

Open `project.godot` in Godot and run.

## License

MIT
