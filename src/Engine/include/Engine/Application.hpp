#pragma once
#include <memory>
#include <Framework/Logger.hpp>
#include <Framework/Config.hpp>
#include <Framework/Timer.hpp>
#include <Engine/Window.hpp>
#include <Engine/Renderer.hpp>
#include <Engine/EventDispatcher.hpp>
#include <Engine/Input.hpp>
#include <Engine/TimeSystem.hpp>
#include <Engine/EngineRuntimeConfig.hpp>


namespace Aurum
{
    class Application
    {
    public:
        Application(HINSTANCE hInstance);
        virtual ~Application();

        void Run();

        // Accessors
        EventDispatcher& GetEventDispatcher() { return eventDispatcher_; }
        InputManager& GetInputManager() { return input_; }
        TimeSystem& GetTimeSystem() { return timeSystem_; }

    protected:
        // Lifecycle methods for derived applications
        virtual void OnInitialize() {}
        virtual void OnUpdate(float deltaTime) {}
        virtual void OnShutdown() {}

        TimeSystem timeSystem_;
        EngineRuntimeConfig runtimeConfig_;

    private:
        bool running_ = true;
        HINSTANCE hInstance_;

        std::unique_ptr<Window> window_;
        std::unique_ptr<Renderer> renderer_;

        EventDispatcher eventDispatcher_;
        InputManager input_{ eventDispatcher_ };  // ✅ integrated InputManager tied to EventDispatcher
        FrameTimer timer_;

        void Initialize();
        void Shutdown();
    };
}
