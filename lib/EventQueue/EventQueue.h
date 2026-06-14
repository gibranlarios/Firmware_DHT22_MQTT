#ifndef EVENT_QUEUE_H
#define EVENT_QUEUE_H

#include <Arduino.h>

struct DeviceEvent
{
    String eventType;
    String message;
    unsigned long uptimeMs;
};

template <size_t Capacity>
class EventQueue
{
public:
    void enqueue(const String &eventType, const String &message, unsigned long uptimeMs)
    {
        if (_count == Capacity)
        {
            _head = (_head + 1) % Capacity;
            --_count;
        }

        _events[_tail] = {eventType, message, uptimeMs};
        _tail = (_tail + 1) % Capacity;
        ++_count;
    }

    bool peek(DeviceEvent &event) const
    {
        if (isEmpty())
        {
            return false;
        }

        event = _events[_head];
        return true;
    }

    bool pop()
    {
        if (isEmpty())
        {
            return false;
        }

        _head = (_head + 1) % Capacity;
        --_count;
        return true;
    }

    bool isEmpty() const
    {
        return _count == 0;
    }

private:
    static_assert(Capacity > 0, "EventQueue capacity must be greater than zero");

    DeviceEvent _events[Capacity];
    size_t _head = 0;
    size_t _tail = 0;
    size_t _count = 0;
};

#endif
