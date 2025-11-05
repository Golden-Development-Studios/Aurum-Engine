#pragma once
#include <string>
#include <sstream>

namespace Aurum
{
    enum class EventType
    {
        None = 0,
        WindowClose,
        WindowResize,
        KeyPressed,
        KeyReleased,
        MouseMoved,
        MouseButtonPressed,
        MouseButtonReleased
    };

    class Event
    {
    public:
        virtual ~Event() = default;
        virtual EventType GetType() const = 0;
        virtual std::string ToString() const { return "Event"; }
    };

    // -------------------------
    // Example event definitions
    // -------------------------
    class WindowResizeEvent : public Event
    {
    public:
        WindowResizeEvent(int w, int h) : width_(w), height_(h) {}

        int GetWidth() const { return width_; }
        int GetHeight() const { return height_; }

        EventType GetType() const override { return EventType::WindowResize; }

        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "WindowResizeEvent: " << width_ << "x" << height_;
            return ss.str();
        }

    private:
        int width_, height_;
    };

    class WindowCloseEvent : public Event
    {
    public:
        EventType GetType() const override { return EventType::WindowClose; }
        std::string ToString() const override { return "WindowCloseEvent"; }
    };
}
