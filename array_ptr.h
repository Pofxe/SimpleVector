#pragma once

#include <cassert>
#include <cstdlib>
#include <algorithm>

template <typename Type>
class ArrayPtr 
{
public:
    
    // Инициализирует нулевым указателем
    ArrayPtr() = default; 

    // Создает в куче массив
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
 
    // Конструктор хранящий адрус массива в куче
    explicit ArrayPtr(Type* raw_ptr_) noexcept : raw_ptr(raw_ptr_){}

    // Запрет копирования
    ArrayPtr(const ArrayPtr&) = delete;

    // Деструктор
    ~ArrayPtr() 
    {
        delete[] raw_ptr;
    }

    // Запрет присваивания
    ArrayPtr& operator=(const ArrayPtr&) = delete;

    // Обнуляет указатель на массив O(1)
    void release() noexcept
    {
        raw_ptr = nullptr;
    }

    // Получение ссылки по индексу O(1)
    Type& operator[](size_t index) noexcept 
    {
        return raw_ptr[index];
    }

    // Получение константной ссылки по индексу O(1)
    const Type& operator[](size_t index) const noexcept 
    {
        return raw_ptr[index];
    }

    // Проверка на нулевой указатель O(1)
    explicit operator bool() const 
    {
        if (raw_ptr)
        {
            return true;
        }
        return false;
    }

    // Получение одреса массива O(1)
    Type* get() const noexcept 
    {
        return &raw_ptr[0];
    }

    // Обмен значений O(1)
    void swap(ArrayPtr& other) noexcept 
    {
        std::swap(other.raw_ptr, raw_ptr);
    }

private:

    Type* raw_ptr = nullptr;
};