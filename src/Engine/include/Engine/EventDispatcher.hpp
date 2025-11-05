#pragma once
#include <functional>
#include <unordered_map>
#include <vector>
#include <typeindex>
#include <memory>
#include <Framework/Logger.hpp>

namespace Aurum
{
    class Event;

    class EventDispatcher
    {
    public:
        template<typename T>
        void Subscribe(std::function<void(const T&)> callback)
        {
            auto& subscribers = listeners_[std::type_index(typeid(T))];
            subscribers.push_back([cb = std::move(callback)](const Event& e)
            {
                cb(static_cast<const T&>(e));
            });
        }

        template<typename T>
        void Publish(const T& event)
        {
            auto it = listeners_.find(std::type_index(typeid(T)));
            if (it != listeners_.end())
            {
                for (auto& listener : it->second)
                    listener(event);
            }
        }

    private:
        std::unordered_map<std::type_index, std::vector<std::function<void(const Event&)>>> listeners_;
    };
}
