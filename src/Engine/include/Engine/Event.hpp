#pragma once
#include <string>
#include <sstream>

namespace Aurum
{
    // ---------------------------------
    // Event Type Enumeration
    // ---------------------------------
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

    // ---------------------------------
    // Base Event Class
    // ---------------------------------
    class Event
    {
    public:
        virtual ~Event() = default;

        virtual EventType GetType() const = 0;
        virtual std::string ToString() const { return "Event"; }
    };

    // ---------------------------------
    // Window Events
    // ---------------------------------
    class WindowResizeEvent : public Event
    {
    public:
        WindowResizeEvent(int w, int h)
            : width_(w), height_(h) {}

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

    // ---------------------------------
    // Keyboard Events
    // ---------------------------------
    class KeyPressedEvent : public Event
    {
    public:
        KeyPressedEvent(int key, bool repeat)
            : keyCode_(key), repeat_(repeat) {}

        int GetKeyCode() const { return keyCode_; }
        bool IsRepeat() const { return repeat_; }

        EventType GetType() const override { return EventType::KeyPressed; }

        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "KeyPressedEvent: " << keyCode_;
            if (repeat_) ss << " (repeat)";
            return ss.str();
        }

    private:
        int keyCode_;
        bool repeat_;
    };

    class KeyReleasedEvent : public Event
    {
    public:
        explicit KeyReleasedEvent(int key)
            : keyCode_(key) {}

        int GetKeyCode() const { return keyCode_; }

        EventType GetType() const override { return EventType::KeyReleased; }

        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "KeyReleasedEvent: " << keyCode_;
            return ss.str();
        }

    private:
        int keyCode_;
    };
}
