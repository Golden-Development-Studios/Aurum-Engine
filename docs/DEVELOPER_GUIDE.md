# 🧰 Developer’s Learning Guide — Aurum Framework & Engine

---

## ⚙️ Recommended Skill Areas

To build Aurum effectively, mastery in the following domains is essential:

| Area | Focus Topics |
|-------|---------------|
| **C++ Core** | Modern C++ (C++17/20), memory management, templates, smart pointers |
| **Engine Architecture** | ECS design, data-oriented programming, modular design |
| **Math & Physics** | Linear algebra, kinematics, simulation determinism |
| **Graphics** | OpenGL/Vulkan fundamentals, shaders, rendering pipelines |
| **Concurrency** | Multithreading, job systems, task schedulers |
| **Scripting** | Lua or Python embedding, API binding with sol2/pybind11 |
| **Tooling** | ImGui, serialization, debugging, profiling |
| **Networking** | Deterministic sync, serialization, client-server logic |

---

## 📚 Learning Resources

| Domain | Recommended Learning Resources |
|--------|--------------------------------|
| **C++ & Systems** | “Game Engine Architecture” by Jason Gregory, “Effective C++” by Scott Meyers |
| **ECS** | Overwatch GDC talk: *ECS in Practice*, *EnTT* source study |
| **Rendering** | LearnOpenGL.com, VulkanTutorial.com, *Real-Time Rendering* |
| **Physics** | *Game Physics Cookbook*, Box2D source code |
| **Scripting** | Lua in C++, pybind11 docs |
| **Concurrency** | *C++ Concurrency in Action* |
| **Engine Design** | Handmade Hero (Casey Muratori), Cherno’s “Game Engine” YouTube series |
| **Simulation** | KSP Dev Blogs, Space Engineers Dev Notes, Factorio Friday Facts |

---

## 🧩 Development Workflow

1. Build **Aurum Framework Core**
2. Integrate rendering + ECS
3. Add a sandbox for testing
4. Expand into **Aurum Engine**
5. Implement physics, scripting, tooling
6. Gradually modularize into separate libraries
7. Build the **Aurum Editor**

---

## 🧠 Developer Philosophy

> “Know every layer you depend on. If you didn’t write it, understand it.”

Aurum development encourages *deep understanding* over abstraction.  
Every subsystem should be designed, documented, and justified in its purpose.

---
