# ⚙️ Aurum Framework & Aurum Engine
### By Golden Developments

> “Engineered for precision. Built for scale. Forged in gold.”

---

## 🧭 Overview

**Aurum** is a high-performance C++ framework and engine ecosystem created by **Golden Developments**.  
It is designed to power everything from indie prototypes to **KSP-level** simulations and beyond.

The ecosystem consists of:

- **Aurum Framework** – the low-level, modular foundation (math, ECS, IO, platform abstraction).
- **Aurum Engine** – the high-level layer for real-time rendering, simulation, and game logic.

Built upon a *brutal-efficiency* philosophy, Aurum prioritizes **clarity**, **control**, and **scalability** over convenience.  
Every subsystem is engineered to be replaceable, transparent, and deterministic.

---

## 🧱 Core Features

| Category | Key Features |
|-----------|---------------|
| **Framework Core** | C++17/20 base, math & memory libs, platform layer (Windows → Linux soon) |
| **ECS** | Lightweight entity registry & system management |
| **Rendering** | OpenGL/Vulkan abstraction, camera & mesh pipelines |
| **Physics** | Deterministic simulation base (expandable) |
| **Scripting** | Lua/Python bindings for gameplay & tools |
| **Tooling** | ImGui integration, debug overlays, profiler |
| **Modularity** | Plugin system, data-driven configs |
| **Scalability** | Multithreaded job system, async streaming, KSP-scale world design |

---

## 🧭 Project Roadmap

| Phase | Focus | Goal |
|-------|--------|------|
| **Beginner** | Core Framework | Windowing, math, ECS, renderer |
| **Intermediate** | Engine Systems | Physics, scripting, tooling |
| **Advanced** | Full Ecosystem | Multithreading, procedural worlds, editor |

See full breakdown in [`ROADMAP_AND_TASKS.md`](./ROADMAP_AND_TASKS.md)

---

## 🛠️ Getting Started

### Requirements
- **Language:** C++17 or higher  
- **Compiler:** MSVC / Clang / GCC  
- **Build System:** CMake (3.20+)  
- **Dependencies:**  
  - SDL or GLFW (windowing/input)  
  - GLAD / Volk (graphics loader)  
  - ImGui (debug UI)

### Setup
```bash
git clone https://github.com/GoldenDevelopments/Aurum.git
cd Aurum
mkdir build && cd build
cmake ..
cmake --build .
Run the AurumSandbox target to verify the framework boots.
