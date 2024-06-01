#pragma once

#include "detector.h"

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

    // Конструктор перемещения
    ArrayPtr(ArrayPtr&& other) noexcept : raw_ptr(other.raw_ptr)
    {
        other.raw_ptr = nullptr;
    }

    // Деструктор
    ~ArrayPtr() 
    {
        //delete[] raw_ptr;
    }

    // Запрет присваивания
    ArrayPtr& operator=(const ArrayPtr&) = delete;

    // Оператор присваивания перемещением
    ArrayPtr& operator=(ArrayPtr&& other) noexcept
    {
        if (this != &other)
        {
            delete[] raw_ptr;
            raw_ptr = other.raw_ptr;
            other.raw_ptr = nullptr;
        }
        return *this;
    }

    // Прекращает владение массивом и удаляет указатель
    Type* release() noexcept 
    {
        Type* tmp = raw_ptr;
        raw_ptr = nullptr;
        return tmp;
        
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

    // Получение адреса массива O(1)
    Type* get() const noexcept 
    {
        return raw_ptr;
    }

    // Обмен значений O(1)
    void swap(ArrayPtr& other) noexcept 
    {
        std::swap(other.raw_ptr, raw_ptr);
    }

private:

    Type* raw_ptr = nullptr;
};