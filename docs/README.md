# ⚙️ Aurum Framework & Aurum Engine
### By Golden Developments Studios
# Golden Developments Studios is operated by GitHub user @goldendev20.


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

---

## 🧩 Repository Structure

/Aurum
│
├── /src                # C++ source code
│   ├── /Framework      # Aurum Framework core
│   ├── /Engine         # Aurum Engine systems
│   └── /Sandbox        # Testing / examples
│
├── /docs               # Project documentation
│   ├── PROJECT_BRIEF.md
│   ├── FEATURE_SPEC.md
│   ├── FUTURE_VISION.md
│   ├── DEVELOPER_GUIDE.md
│   ├── ROADMAP_AND_TASKS.md
│   └── README.md       # This file
│
└── CMakeLists.txt      # Build configuration

---

## 🌌 Vision

Aurum aims to become a universal, data-driven ecosystem capable of powering:

- Simulation sandboxes
- AI & robotics experimentation
- Game worlds with planetary-scale mechanics
- Scientific visualization tools
It’s built not only to create worlds — but to simulate them accurately.

---

## 📚 Documentation

All core documentation is located in the /docs directory:

PROJECT_BRIEF.md
 – Mission, goals, structure
FEATURE_SPEC.md
 – System-level feature definitions
FUTURE_VISION.md
 – Long-term ambitions
DEVELOPER_GUIDE.md
 – Learning resources
ROADMAP_AND_TASKS.md
 – Task breakdown by phase

 ---

 ## 🧠 Development Philosophy

“Every abstraction should justify its existence.”

Aurum is about understanding and ownership.
Each module is deliberately designed for transparency, maintainability, and extendability — even under extreme simulation loads.

---

## 🏷️ License

All rights reserved © Golden Developments
Usage, modification, or redistribution requires explicit permission unless otherwise stated per project.

---

## 🤝 Contact

Golden Developments
📧 goldendev.contact@example.com
 (placeholder)
🌐 https://github.com/GoldenDev20

---

## ✨ Aurum – Forged for those who build worlds.