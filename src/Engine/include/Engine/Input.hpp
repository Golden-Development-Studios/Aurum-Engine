#pragma once
#include <Windows.h>
#include <unordered_map>
#include <Framework/Logger.hpp>
#include <Engine/EventDispatcher.hpp>
#include <Engine/Event.hpp>

namespace Aurum
{
    class InputManager
    {
    public:
        explicit InputManager(EventDispatcher& dispatcher)
            : dispatcher_(dispatcher) {}

        void ProcessMessage(UINT msg, WPARAM wParam, LPARAM lParam)
        {
            switch (msg)
            {
                case WM_KEYDOWN:
                {
                    bool repeat = (lParam & 0x40000000) != 0;
                    dispatcher_.Publish(KeyPressedEvent((int)wParam, repeat));
                    break;
                }
                case WM_KEYUP:
                {
                    dispatcher_.Publish(KeyReleasedEvent((int)wParam));
                    break;
                }
                default:
                    break;
            }
        }

    private:
        EventDispatcher& dispatcher_;
    };
}
