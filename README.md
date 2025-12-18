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

### Run Games

```bash
./bin/hello   # Hello world demo
./bin/pong    # Pong (stub)
```

Press ESC to exit.

## Project Structure

```
delphinis_v2/
├── CLAUDE.md           # Design philosophy and guidelines
├── CMakeLists.txt      # Main build configuration
├── src/                # Engine source files
├── include/            # Engine headers
├── games/              # Game implementations
│   ├── CMakeLists.txt # Game build configuration
│   ├── hello/         # Hello world demo
│   │   └── main.cpp
│   └── pong/          # Pong game
│       └── main.cpp
├── examples/           # Tutorial examples
└── tests/              # Test programs
```

## Adding a New Game

Creating a new game is simple:

1. Create a directory under `games/` with your game name
2. Add a `main.cpp` file in that directory
3. Add one line to `games/CMakeLists.txt`: `add_game(your_game_name)`
4. Rebuild: `make`

Your game will automatically link against the engine library and dependencies.

## Dependencies

- **GLFW**: Window and input handling (fetched automatically)
- **GLEW**: OpenGL extension loading
- **OpenGL**: Graphics rendering

## Current Status

- Project structure set up with engine/game separation
- Build system configured for easy game creation
- Basic OpenGL window rendering working
- Multiple game binaries supported
