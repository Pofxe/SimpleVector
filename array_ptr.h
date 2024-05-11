#pragma once

#include <cassert>
#include <cstdlib>
#include <algorithm>

template <typename Type>
class ArrayPtr 
{
public:
    
    // �������������� ������� ����������
    ArrayPtr() = default; 

    // ������� � ���� ������
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
 
    // ����������� �������� ����� ������� � ����
    explicit ArrayPtr(Type* raw_ptr_) noexcept : raw_ptr(raw_ptr_){}

    // ������ �����������
    ArrayPtr(const ArrayPtr&) = delete;

    // ����������
    ~ArrayPtr() 
    {
        delete[] raw_ptr;
    }

    // ������ ������������
    ArrayPtr& operator=(const ArrayPtr&) = delete;

    // �������� ��������� �� ������ O(1)
    void release() noexcept
    {
        raw_ptr = nullptr;
    }

    // ��������� ������ �� ������� O(1)
    Type& operator[](size_t index) noexcept 
    {
        return raw_ptr[index];
    }

    // ��������� ����������� ������ �� ������� O(1)
    const Type& operator[](size_t index) const noexcept 
    {
        return raw_ptr[index];
    }

    // �������� �� ������� ��������� O(1)
    explicit operator bool() const 
    {
        if (raw_ptr)
        {
            return true;
        }
        return false;
    }

    // ��������� ������ ������� O(1)
    Type* get() const noexcept 
    {
        return &raw_ptr[0];
    }

    // ����� �������� O(1)
    void swap(ArrayPtr& other) noexcept 
    {
        std::swap(other.raw_ptr, raw_ptr);
    }

private:

    Type* raw_ptr = nullptr;
};