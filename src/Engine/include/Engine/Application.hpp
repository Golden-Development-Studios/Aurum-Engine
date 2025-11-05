#pragma once
#include <memory>
#include <Framework/Logger.hpp>
#include <Framework/Config.hpp>
#include <Framework/Timer.hpp>
#include <Engine/Window.hpp>
#include <Engine/Renderer.hpp>

namespace Aurum
{
    class Application
    {
    public:
        Application(HINSTANCE hInstance);
        virtual ~Application();

        void Run();

    protected:
        virtual void OnInitialize() {}
        virtual void OnUpdate(float deltaTime) {}
        virtual void OnShutdown() {}

    private:
        bool running_ = true;
        HINSTANCE hInstance_;

        std::unique_ptr<Window> window_;
        std::unique_ptr<Renderer> renderer_;

        Aurum::FrameTimer timer_;  // ✅ Correct type and namespace

        void Initialize();
        void Shutdown();
    };
}

