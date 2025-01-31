#pragma once
#ifndef SINGLETON_BASE_HPP
#define SINGLETON_BASE_HPP

template<typename T>
class SingletonBase
{
public:
    static T* get()
    {
        static T instance;
        return &instance;
    }
};

#endif // SINGLETON_BASE_HPP