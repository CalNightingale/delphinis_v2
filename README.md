# Delphinis - 2D Game Engine

A simple, elegant 2D game engine built in C++ using pure ECS architecture.

## Building

### Prerequisites
- C++17 compatible compiler
- CMake 3.15+
- OpenGL 3.3+
- GLEW (install via `brew install glew` on macOS)

### Build Instructions

```bash
mkdir build
cd build
cmake ..
make
```

### Run

```bash
./bin/delphinis
```

Press ESC to exit.

## Project Structure

```
delphinis_v2/
├── CLAUDE.md           # Design philosophy and guidelines
├── CMakeLists.txt      # Build configuration
├── src/                # Source files
│   └── main.cpp       # Entry point
├── include/            # Header files
├── examples/           # Example games
└── tests/             # Test programs
```

## Dependencies

- **GLFW**: Window and input handling (fetched automatically)
- **GLEW**: OpenGL extension loading
- **OpenGL**: Graphics rendering

## Current Status

Project initialized with basic OpenGL window rendering.
