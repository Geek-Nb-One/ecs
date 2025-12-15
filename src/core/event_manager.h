#pragma once

#include <unordered_map>
#include <typeindex>
#include <functional>

#include <iostream>
#include <list>

// This is the interface for MemberFunctionHandler that each specialization will use

class HandlerFunctionBase
{
public:
    // Call the member function
    void exec(void *evnt)
    {
        call(evnt);
    }

private:
    // Implemented by MemberFunctionHandler
    virtual void call(void *evnt) = 0;
};

template <class T, class EventType>
class MemberFunctionHandler : public HandlerFunctionBase
{
public:
    typedef void (T::*MemberFunction)(EventType *);

    MemberFunctionHandler(T *instance, MemberFunction memberFunction) : instance{instance}, memberFunction{memberFunction} {};

    void call(void *evnt)
    {
        // Cast event to the correct type and call member function
        (instance->*memberFunction)(static_cast<EventType *>(evnt));
    }

private:
    // Pointer to class instance
    T *instance;

    // Pointer to member function
    MemberFunction memberFunction;
};

typedef std::list<HandlerFunctionBase *> HandlerList;

class EventManager
{
public:
    template <typename T, typename EventType>
    void subscribe(T *instance, void (T::*memberFunction)(EventType *))
    {

        HandlerList *handler = subscribers[typeid(EventType)];

        if (handler == nullptr)
        {
            handler = new HandlerList();
            subscribers[typeid(EventType)] = handler;
        }
        handler->push_back(new MemberFunctionHandler<T, EventType>(instance, memberFunction));
    }

    template <typename EventType, typename... Args>
    void publish(Args &&...args)
    {
        EventType *event = new EventType(std::forward<Args>(args)...);

        auto it = subscribers.find(typeid(EventType));

        if (it != subscribers.end())
        {

            for (auto &subscriber : *(it->second))
            {
                subscriber->exec(static_cast<void *>(event));
            }
        }
    }

private:
    std::unordered_map<std::type_index, HandlerList *> subscribers{};
};