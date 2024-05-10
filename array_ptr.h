#pragma once

#include <cassert>
#include <cstdlib>
#include <algorithm>

template <typename Type>
class ArrayPtr 
{
public:
    
    ArrayPtr() = default;

    explicit ArrayPtr(size_t size)
    {
        if (size == 0)
        {
            raw_ptr = nullptr;
        }
        else
        {
            raw_ptr = new Type[size];
        }
    }
 
    explicit ArrayPtr(Type* raw_ptr_) noexcept : raw_ptr(raw_ptr_){}

    ArrayPtr(const ArrayPtr&) = delete;

    ~ArrayPtr() 
    {
        delete[] raw_ptr;
    }

    ArrayPtr& operator=(const ArrayPtr&) = delete;

    [[nodiscard]] Type* release() noexcept
    {
        Type* tmp = raw_ptr;
        raw_ptr = nullptr;
        return tmp;
    }

    Type& operator[](size_t index) noexcept 
    {
        return raw_ptr[index];
    }

    const Type& operator[](size_t index) const noexcept 
    {
        return raw_ptr[index];
    }

    explicit operator bool() const 
    {
        if (raw_ptr)
        {
            return true;
        }
        return false;
    }

    Type* get() const noexcept 
    {
        return &raw_ptr[0];
    }

    void swap(ArrayPtr& other) noexcept 
    {
        std::swap(other.raw_ptr, raw_ptr);
    }

private:

    Type* raw_ptr = nullptr;
};