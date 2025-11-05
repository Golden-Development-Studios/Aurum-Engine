#pragma once
#include <memory>
#include <Framework/Logger.hpp>
#include <Framework/Config.hpp>
#include <Framework/Timer.hpp>
#include <Engine/Window.hpp>
#include <Engine/Renderer.hpp>
#include <Engine/EventDispatcher.hpp>


namespace Aurum
{
    class Application
    {
    public:
        Application(HINSTANCE hInstance);
        virtual ~Application();

        void Run();
        EventDispatcher& GetEventDispatcher() { return eventDispatcher_; }  // ✅ add this

    protected:
        virtual void OnInitialize() {}
        virtual void OnUpdate(float deltaTime) {}
        virtual void OnShutdown() {}

    private:
        bool running_ = true;
        HINSTANCE hInstance_;

        std::unique_ptr<Window> window_;
        std::unique_ptr<Renderer> renderer_;

        EventDispatcher eventDispatcher_;
        Aurum::FrameTimer timer_;

        void Initialize();
        void Shutdown();
    };
}

