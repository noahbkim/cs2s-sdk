#pragma once

namespace cs2s
{

template<typename T>
struct defer
{
    T callback;

    explicit defer(T&& callback) : callback(std::forward<T>(callback))
    {
    }

    ~defer()
    {
        callback();
    }
};

}
