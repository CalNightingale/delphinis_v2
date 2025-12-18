# Delphinis - 2D Game Engine

## Project Vision

Delphinis is a general-purpose 2D game engine built in C++ using a pure Entity Component System (ECS) architecture. The engine prioritizes simplicity, elegance, and consistency while maintaining the performance benefits of data-oriented design.

## Design Philosophy

### Core Principles

1. **Simplicity First**: Every feature should be implemented in the simplest way that works. Avoid premature optimization and over-engineering.

2. **Pure ECS Architecture**: Strict adherence to ECS principles:
   - **Entities**: Simple integer IDs with no behavior
   - **Components**: Pure data structures with no logic
   - **Systems**: All game logic lives here, operating on components

3. **Consistency Throughout**: Code should follow uniform patterns:
   - Naming conventions should be predictable
   - Similar problems should have similar solutions
   - API design should feel cohesive across all modules

4. **Data-Oriented Design**: Optimize for cache-friendly memory layouts and batch processing where appropriate.

5. **Explicit Over Implicit**: Behavior should be obvious from reading the code. Avoid hidden state and magic.

## Technical Requirements

### Platform Support
- Native desktop (Windows, macOS, Linux)
- Web (via Emscripten/WebGL)

### Rendering
- OpenGL 3.3+ for desktop
- WebGL 2.0 for web
- Abstract renderer interface to support both targets

### Language Standards
- C++17 or later
- Minimize external dependencies
- Standard library preferred over third-party solutions when reasonable

## Architecture Overview

### ECS Implementation

```
Entity: uint32_t (just an ID)
Component: Plain data struct (no methods except constructors)
System: Class that processes entities with specific component combinations
```

### Core Modules

1. **ECS Core**
   - Entity manager
   - Component pools (dense, cache-friendly storage)
   - System registry and execution order
   - Query interface for system access to entities

2. **Rendering**
   - OpenGL/WebGL abstraction layer
   - Sprite rendering system
   - Camera system
   - Layer/depth sorting

3. **Physics & Collision**
   - 2D physics simulation
   - Collision detection (AABB, circle, polygon)
   - Collision response
   - Spatial partitioning for performance

4. **Animation**
   - Sprite sheet management
   - Frame-based animation
   - Animation state machine

5. **Audio**
   - Sound effect playback
   - Music streaming
   - Volume and spatial audio control

6. **Asset Management**
   - Resource loading and caching
   - Asset lifetime management
   - Asynchronous loading support

## Code Style Guidelines

### Naming Conventions
- `PascalCase` for types (classes, structs, enums)
- `camelCase` for functions and variables
- `SCREAMING_SNAKE_CASE` for constants and macros
- `m_` prefix for member variables

### File Organization
- One class per header/source file pair
- Group related functionality in directories
- Keep headers minimal and focused

### Comments
- Document why, not what
- Use comments sparingly - prefer self-documenting code
- API documentation for public interfaces

### Error Handling
- Use assertions for programming errors
- Return error codes or optionals for recoverable errors
- Fail fast and clearly

## Development Approach

1. **Incremental Implementation**: Build the engine feature by feature, ensuring each component works before moving to the next.

2. **Test As You Go**: Create simple test programs for each major system to validate functionality.

3. **Refactor Ruthlessly**: If a pattern isn't working, change it early. Don't let technical debt accumulate.

4. **Performance When It Matters**: Profile before optimizing. Focus on algorithmic improvements over micro-optimizations.

## Success Criteria

The engine is successful if:
- A simple game (e.g., Pong, Breakout) can be implemented in under 200 lines
- The ECS architecture feels natural, not burdensome
- The same game code compiles and runs on desktop and web
- The codebase remains readable and maintainable as it grows

## Non-Goals

- We are NOT building a Unity competitor
- We are NOT supporting 3D (stay focused on 2D)
- We are NOT building an editor (code-first approach)
- We are NOT trying to be the fastest engine (but we should be reasonably fast)

## Current Status

Project initialized. Beginning implementation of core ECS architecture.
