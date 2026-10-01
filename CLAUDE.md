# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Build System

This is a C++20 CMake project using vcpkg for dependency management. It targets Windows with MSVC.

```bash
# Configure (from repo root, out-of-source build)
cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=<vcpkg-root>/scripts/buildsystems/vcpkg.cmake

# Build
cmake --build build --config Debug
cmake --build build --config Release

# Run (working directory must be repo root so data/ paths resolve)
./build/Debug/medusa.exe
```

The VS debugger working directory is set to `${CMAKE_SOURCE_DIR}` (repo root), so shaders and assets load relative to there. When running from the CLI, `cd` to the repo root first.

There are no automated tests or lint steps yet.

## Project Structure

Three static libraries compose the engine:

- **`medusa_core`** — YAML config loading, spdlog logging, timing utilities
- **`medusa_engine`** — Platform-agnostic engine: geometry, rendering pipeline, asset management, game objects, font system
- **`medusa_opengl`** — OpenGL 4.6 implementation of all graphics interfaces

The main executable (`medusa.cpp`) ties them together and runs the `GLDemo` test harness from `test/`.

Public headers live under `include/medusa/`; implementation under `src/`. Both `include/` and `src/` are on the include path, so internal headers use `#include "..."` paths relative to `src/`.

## Architecture

### Graphics Abstraction

All graphics concepts are defined as pure interfaces in `include/medusa/graphics/`. The OpenGL implementations live in `src/opengl/graphics/`. Adding a new backend means implementing these interfaces and a new `IContext`.

Key interfaces:
- `IContext` (`src/opengl/context_gl.h`) — device; creates shaders, textures, GPU memory, descriptors
- `IShader` / `ShaderGL` — GLSL program
- `ITexture` / `TextureGL` — 2D textures with filtering, wrapping, swizzle, bindless handles
- `IMemory` / `MemoryGL` — typed GPU buffer (Uniform, ShaderStorage, Array, ElementArray, etc.)
- `IDescriptor` / `DescriptorGL` — VAO, describes vertex layout to a shader

Forward declarations for all graphics types are in `include/medusa/graphics_fwd.h`.

### Rendering Pipeline

Rendering is organized as a hierarchy: `IScene → IView → ILayer → IPipeline → IPass`.

- **IPass** — links a shader to a `PipelineState` (blend, depth, stencil, raster) and a descriptor; issues draw calls
- **IPipeline** — owns one or more passes; manages transform/material GPU buffers
- **ILayer** — groups pipelines for a logical render layer (world, UI, HUD, etc.)
- **IView** — a viewport with a camera; contains layers
- **IScene** — top-level; manages views and owns the scene graph

`PipelineState` is the large value-type in `include/medusa/renderer/pipeline_state.h`; it is parsed from YAML shader entries in `data/assets.yaml` by `PipelineStateParser`.

### Asset Management

`AssetManager` (`src/engine/resources/asset_manager.h`) is the single access point. Assets are declared in `data/assets.yaml` and loaded on demand through type-specialized loaders:

| Loader | Library | Asset type |
|---|---|---|
| `ShaderLoader` | — | GLSL + YAML pipeline state |
| `TextureLoader` | SDL2_image | PNG/JPG → `ITexture` |
| `ModelLoader` | assimp | OBJ/MTL → `IModel` |
| `FontLoader` | FreeType + msdfgen | TTF → per-glyph MSDF textures |

Asset locations are registered as `IAssetLocation` (directory or future packaged `.dat` zip). The YAML structure under `data/assets.yaml` groups resources by type (`models`, `shaders`, `textures`, `fonts`).

### Transform & Material Buffers (Instancing)

Transforms are stored in a central GPU-side `GenericArray<glm::mat4>` (ShaderStorage buffer). Each rendered object holds an index into this buffer rather than uploading a matrix per draw call. The `basic` vertex shader reads the matrix by index, enabling efficient batch/instanced rendering. Materials use the same pattern — a 128-byte `Material` struct in a ShaderStorage buffer with bindless diffuse texture handles.

### Font Rendering (MSDF)

Fonts use a 3-stage GLSL pipeline (`data/shaders/msdf.vs/gs/fs`):
1. **Vertex shader** — emits a glyph index and position per character
2. **Geometry shader** — expands each point into a screen-space quad using bearing/dimension from the glyph buffer
3. **Fragment shader** — samples the MSDF texture, applies a median filter for sharp edges at any scale

`FontLoader` calls FreeType to rasterize each glyph, then `msdfgen` to generate the signed-distance-field texture. Glyph metrics (`FontGlyph`: dimension, bearing, advance, bindless texture handle) are stored in a ShaderStorage buffer uploaded once per font.

### Game Objects

`IEntity` → components (`TransformComponent`, `MeshComponent`, `ObjectComponent`). The scene graph (`ISceneGraph`) owns entities; `ISceneManager` tracks active scenes. Transform components push `glm::mat4` into the engine's transform buffer and hold the resulting index.

## Key Files

| Purpose | Path |
|---|---|
| Engine interface | `include/medusa/engine/engine.h` |
| Graphics interfaces | `include/medusa/graphics/` |
| Forward decls | `include/medusa/graphics_fwd.h` |
| Pipeline state (large) | `include/medusa/renderer/pipeline_state.h` |
| Asset manager | `src/engine/resources/asset_manager.h` |
| OpenGL context | `src/opengl/context_gl.h` / `context_gl.cpp` |
| Asset registry | `data/assets.yaml` |
| Shaders | `data/shaders/` (`basic`, `wireframe`, `msdf`) |
| Test harness | `test/gl_demo.cpp` |
| App entry point | `medusa.cpp` |

## Data & Configuration

- `config/medusa.config.yaml` — window dimensions (currently 1600×1200)
- `data/assets.yaml` — all named asset declarations; add new shaders/textures/models/fonts here
- The engine registers `../data/` as the asset location at startup; relative paths in YAML are resolved against that root
