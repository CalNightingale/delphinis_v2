# Delphinis - 2D Game Engine

A simple, elegant 2D game engine built in C++ using pure ECS architecture.

## Play Online

**[Play Pong in your browser!](https://CalNightingale.github.io/delphinis_v2/)**

The game is compiled to WebAssembly and runs directly in your browser with no plugins required.

## Building

### Native Build (Desktop)

**Prerequisites:**
- C++17 compatible compiler
- CMake 3.15+
- OpenGL 3.3+
- GLEW (install via `brew install glew` on macOS)

**Build Instructions:**
```bash
mkdir build
cd build
cmake ..
make
```

**Run Games:**
```bash
./bin/hello   # Hello world demo
./bin/pong    # Full Pong game with AI
```

Press ESC to exit.

### Web Build (WebAssembly)

**Prerequisites:**
- [Emscripten SDK](https://emscripten.org/docs/getting_started/downloads.html)

**Build Instructions:**
```bash
mkdir build-web
cd build-web
emcmake cmake ..
emmake make pong
```

**Run Locally:**
```bash
# Serve the built files
python3 -m http.server 8000

# Open http://localhost:8000/bin/pong.html
```

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

- ✅ Pure ECS architecture implemented
- ✅ Complete 2D rendering system (OpenGL/WebGL)
- ✅ Physics and collision detection (AABB)
- ✅ Fixed timestep game loop (60 FPS)
- ✅ Working Pong game (player vs AI)
- ✅ Native desktop builds (macOS, Linux, Windows)
- ✅ WebAssembly builds via Emscripten
- ✅ GitHub Pages deployment automation

## Features

- **Pure ECS**: Components are data, systems are logic
- **Multi-platform**: Desktop (OpenGL) and Web (WebGL 2.0)
- **Simple API**: Create games in ~200 lines of code
- **Fixed timestep**: Consistent physics simulation
- **Modular**: Easy to add new components and systems

## Acknowledgements
I'd like to thank [Matt LaGrandeur](https://www.mattlag.com/bitfonts/) for the cool pong-style font!
