#pragma once

#include "array_ptr.h"

#include <iostream>
#include <cassert>
#include <initializer_list>
#include <stdexcept>

// ��������������� ����� ��� ������ � ������� reserve
class ReserveProxyObj 
{
public:

    ReserveProxyObj(size_t capacity) : capacity(capacity){}

    size_t get_capacity() 
    {
        return capacity;
    }

private:

    size_t capacity;
};

template <typename Type>
class SimpleVector 
{
public:

    using Iterator = Type*;
    using ConstIterator = const Type*;

//===================================================================== ������������ � ���������� ==========================================================

    SimpleVector() noexcept = default;

    // ������� ������ � ���������� �� ���������
    explicit SimpleVector(size_t size) : SimpleVector(size, Type()){}

    // ������� ������ � ��������� ����������
    SimpleVector(size_t size, const Type& value) : items(size), size(size), capacity(size)
    {
        std::fill(items.get(), items.get() + size, value);
    }

    // ������� ������ � ������� {}
    SimpleVector(std::initializer_list<Type> init) : items(init.size()), size(init.size()), capacity(init.size())
    {
        std::copy(init.begin(), init.end(), items.get());
    }

    // ����������� � ��������������� �����
    explicit SimpleVector(ReserveProxyObj obj)
    {
        reserve(obj.get_capacity());
    }

    // ����������� ����������� O(N)
    SimpleVector(const SimpleVector& other) : items(other.capacity), size(other.size)
    {
        std::copy(other.begin(), other.end(), items.get());
    }

    // ����������� �����������
    SimpleVector(SimpleVector&& other) : items(other.capacity)
    {
        swap(other);
    }

    ~SimpleVector()
    {
        items.release();
    }

//================================================================ ��������� ===============================================================================
 
    // ��������� ������ �� ������� O(1)
    Type& operator[](size_t index) noexcept 
    {
        return items[index];
    }

    // ��������� ����������� ������ �� ������� O(1)
    const Type& operator[](size_t index) const noexcept 
    {
        return items[index];
    }

    // ������������� �������� ������������ O(N)
    SimpleVector& operator=(const SimpleVector& rhs)
    {
        if (&items != &rhs.items)
        {
            ArrayPtr<Type> temp(rhs.get_capacity());

            std::copy(rhs.begin(), rhs.end(), temp.get());

            items.swap(temp);
            size = rhs.get_size();
            capacity = rhs.get_capacity();
        }
        return *this;
    }

//===================================================================== ��������� ==========================================================================
    // �������� �� ������ O(1)
    Iterator begin() noexcept
    {
        return items.get();
    }

    // �������� �� ����� O(1)
    Iterator end() noexcept
    {
        return items.get() + size;
    }

    // ����������� �������� �� ������ O(1)
    ConstIterator begin() const noexcept
    {
        return items.get();
    }

    // ����������� �������� �� ����� O(1)
    ConstIterator end() const noexcept
    {
        return items.get() + size;
    }

    // O(1)
    ConstIterator cbegin() const noexcept
    {
        return begin();
    }

    // O(1)
    ConstIterator cend() const noexcept
    {
        return end();
    }

//===================================================================== ������ =============================================================================
    
//------------------------------------------------------------- ���������� � ���������� --------------------------------------------------------------------
    // ��������� ������������������ O(N)
    void fill(Iterator first, Iterator last)
    {
        assert(first < last);

        for (; first != last; ++first)
        {
            *first = std::move(Type());
        }
    }

    // ���������� � ����� � ������������ O(N)
    void push_back(const Type& item)
    {
        if (size + 1 > capacity)
        {
            size_t new_capacity = std::max(size + 1, capacity * 2);
            ArrayPtr<Type> temp(new_capacity);

            std::fill(temp.get(), temp.get() + new_capacity, Type());
            std::copy(items.get(), items.get() + size, temp.get());

            items.swap(temp);
            capacity = new_capacity;
        }
        items[size] = item;
        ++size;
    }

    // ���������� � ����� � ������������ O(N)
    void push_back(Type&& item)
    {
        if (size + 1 > capacity)
        {
            size_t new_capacity = std::max(size + 1, capacity * 2);
            ArrayPtr<Type> temp(new_capacity);

            std::move(items.get(), items.get() + size, temp.get());
            items.swap(temp);

            capacity = new_capacity;
        }
        items[size] = std::move(item);
        ++size;
    }

    // ���������� � ����� O(N)
    void emplace_back(Type&& item)
    {
        if (size + 1 > capacity)
        {
            size_t new_capacity = std::max(size + 1, capacity * 2);
            ArrayPtr<Type> temp(new_capacity);

            std::fill(temp.get(), temp.get() + new_capacity, Type());
            std::copy(items.get(), items.get() + size, temp.get());

            items.swap(temp);
            capacity = new_capacity;
        }
        items[size] = std::move(item);
        ++size;
    }

    // ���������� ��������� � ����� O(N)
    template <typename InputIterator>
    void append_range(InputIterator first, InputIterator last)
    {
        size_t range_size = std::distance(first, last);
        if (size + range_size > capacity)
        {
            size_t new_capacity = std::max(size + range_size, capacity * 2);
            ArrayPtr<Type> temp(new_capacity);

            std::fill(temp.get(), temp.get() + new_capacity, Type());
            std::copy(items.get(), items.get() + size, temp.get());

            items.swap(temp);
            capacity = new_capacity;
        }
        std::copy(first, last, items.get() + size);
        size += range_size;
    }

    // ������� � ��������� ����� c ������������ O(N)
    Iterator insert(ConstIterator pos, const Type& value)
    {
        size_t count = pos - items.get();
        if (capacity == 0)
        {
            ArrayPtr<Type> temp(1);
            temp[count] = value;
            items.swap(temp);
            ++capacity;
        }
        else if (size < capacity)
        {
            std::copy_backward(items.get() + count, items.get() + size, items.get() + size + 1);
            items[count] = value;
        }
        else
        {
            size_t new_capacity = std::max(size + 1, capacity * 2);
            ArrayPtr<Type> temp(capacity);

            std::copy(items.get(), items.get() + size, temp.get());
            std::copy_backward(items.get() + count, items.get() + size, temp.get() + size + 1);

            temp[count] = value;
            items.swap(temp);
            capacity = new_capacity;
        }
        ++size;
        return &items[count];
    }

    // ������� � ��������� ����� � ������������ O(N)
    Iterator insert(Iterator pos, Type&& value)
    {
        assert(pos >= begin() && pos <= end());

        size_t count = pos - items.get();

        if (capacity == 0)
        {
            ArrayPtr<Type> temp(1);

            temp[count] = std::move(value);
            items.swap(temp);
            ++capacity;
        }
        else if (size < capacity)
        {
            std::move_backward(items.get() + count, items.get() + size, items.get() + size + 1);
            items[count] = std::move(value);
        }
        else
        {
            size_t new_capacity = std::max(size + 1, capacity * 2);
            ArrayPtr<Type> temp(capacity);

            std::move(items.get(), items.get() + size, temp.get());
            std::move_backward(items.get() + count, items.get() + size, temp.get() + size + 1);

            temp[count] = std::move(value);
            items.swap(temp);
            capacity = new_capacity;
        }
        ++size;

        return &items[count];
    }

//-------------------------------------------------------------- ��������� �������� ------------------------------------------------------------------------
 
     // ������� ������ O(1)
    size_t get_size() const noexcept
    {
        return size;
    }

    // ������������ ������ O(1)
    size_t max_size() const
    {
        size_t max_size_vec = std::numeric_limits<size_t>::max / sizeof(Type);
        return max_size_vec;
    }

    // ����������� O(1)
    size_t get_capacity() const noexcept
    {
        return capacity;
    }

    // �������� �� ������� O(1)
    bool is_empty() const noexcept
    {
        return size == 0;
    }

    // ������ �� ������ ������� O(1)
    Type& front()
    {
        if (size == 0)
        {
            throw std::out_of_range("Vector is empty!");
        }
        return items[0];
    }

    // ����������� ������ �� ������ ������� O(1)
    const Type& front() const
    {
        if (size == 0)
        {
            throw std::out_of_range("Vector is empty!");
        }
        return items[0];
    }

    // ������ �� ��������� ������� O(1)
    Type& back()
    {
        if (size == 0)
        {
            throw std::out_of_range("Vector is empty!");
        }
        return items[size - 1];
    }

    // ����������� ������ ��������� ������� O(1)
    const Type& back() const
    {
        if (size == 0)
        {
            throw std::out_of_range("Vector is empty!");
        }
        return items[size - 1];
    }

    // ��������� �� ������ ������� O(1)
    Type* data()
    {
        return items.get();
    }

    // ����������� ��������� �� ������ ������� O(1)
    const Type* data() const
    {
        return items.get();
    }

    // ������ �� ������� �� ������� O(1)
    Type& at(size_t index) 
    {
        if (index >= size)
        {
            throw std::out_of_range("Out of range");
        }
        return items[index];
    }

    // ����������� ������ �� ������� �� ������� O(1)
    const Type& at(size_t index) const 
    {
        if (index >= size)
        {
            throw std::out_of_range("Out of range");
        }
        return items[index];
    }

//------------------------------------------------------------------- ������ � �������� --------------------------------------------------------------------

    // �������� ������ O(N)
    void resize(size_t new_size) 
    {
        if (new_size <= size) 
        {
            size = new_size;
        }
        if (new_size <= capacity) 
        {
            fill(items.get() + size, items.get() + size + new_size);
        }
        if (new_size > capacity) 
        {
            size_t new_capacity = std::max(new_size, capacity * 2);
            ArrayPtr<Type> temp(new_capacity);

            fill(temp.get(), temp.get() + new_capacity);
            std::move(items.get(), items.get() + capacity, temp.get());

            items.swap(temp);

            size = new_size;
            capacity = new_capacity;
        }
    }

    // ���������� ����������� � ������� O(N)
    void shrink_to_fit() 
    {
        if (size < capacity) 
        {
            ArrayPtr<Type> new_items(size);
            for (size_t i = 0; i < size; ++i) 
            {
                new(&new_items[i])Type(std::move(items[i]));
                items[i].~Type();
            }
            items.swap(new_items);
            capacity = size;
        }
    }

    // �������������� ����� O(N)
    void reserve(size_t new_capacity)
    {
        if (new_capacity > capacity)
        {
            ArrayPtr<Type> temp(new_capacity);

            std::fill(temp.get(), temp.get() + new_capacity, Type());
            std::copy(items.get(), items.get() + size, temp.get());

            items.swap(temp);
            capacity = new_capacity;
        }
    }

//------------------------------------------------------------------------ ������� � �������� --------------------------------------------------------------
    
    // �������� ������ O(1)
    void clear() noexcept
    {
        size = 0;
    }

    // �������� ���������� �������� O(1)
    void pop_back() noexcept
    {
        if (items)
        {
            --size;
        }
    }

    // �������� �������� � �������� ������� O(N)
    Iterator erase(ConstIterator pos)
    {
        assert(pos != this->end());

        size_t count = pos - items.get();

        std::move(items.get() + count + 1, items.get() + size, items.get() + count);
        --size;

        return &items[count];
    }

//--------------------------------------------------------------------- ������ ������ ----------------------------------------------------------------------

    // �������� ������ � ���������� O(N)
    void assign(size_t new_size, const Type& value) 
    {
        if (new_size > capacity)
        {
            ArrayPtr<Type> newData(new_size);
            items.swap(newData);
            capacity = new_size;
        }

        std::fill_n(items.get(), new_size, value);
        size = new_size;
    }

    // ����� �������� O(N)
    void swap(SimpleVector& other) noexcept 
    {
        std::swap(capacity, other.capacity);
        std::swap(size, other.size);

        items.swap(other.items);
    }

    // ������ ������� O(N)
    void print() const
    {
        for (size_t i = 0; i < size; ++i)
        {
            std::cout << items[i] << " ";
        }
        std::cout << std::endl;
    }

//----------------------------------------------------------------------------------------------------------------------------------------------------------

private:

    ArrayPtr<Type> items;
    size_t size = 0;
    size_t capacity = 0;
};

// ������� ��� �������� ������� ������ � ����������������� ����������� ������
ReserveProxyObj reserve(size_t capacity_to_reserve) 
{
    return ReserveProxyObj(capacity_to_reserve);
}

//================================================= ���� ������������� ���������� =========================================================

template <typename Type>
inline bool operator==(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs)
{
    return std::equal(lhs.begin(), lhs.end(), rhs.begin());
}

template <typename Type>
inline bool operator!=(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) 
{
    return !std::equal(lhs.begin(), lhs.end(), rhs.begin());
}

template <typename Type>
inline bool operator<(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) 
{
    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename Type>
inline bool operator<=(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) 
{
    return (lhs < rhs || lhs == rhs);
}

template <typename Type>
inline bool operator>(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) 
{
    return !(lhs <= rhs);
}

template <typename Type>
inline bool operator>=(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) 
{
    return !(lhs < rhs);
}