#pragma once

#include "simple_vector.h"

#include <cassert>
#include <iostream>
#include <utility>
#include <algorithm>
#include <numeric>

using namespace std;

inline void Test1()
{
    // Конструктор по умолчанию
    {
        SimpleVector<int> v;

        assert(v.get_size() == 0u);
        assert(v.is_empty());
        assert(v.get_capacity() == 0u);
    }

    // Инициализация вектора опеределнного размера
    {
        SimpleVector<int> v(5);

        assert(v.get_size() == 5u);
        assert(v.get_capacity() == 5u);
        assert(!v.is_empty());

        for (size_t i = 0; i < v.get_size(); ++i)
        {
            assert(v[i] == 0);
        }
    }

    // Инициализация вектора, заполненного значениями
    {
        SimpleVector<int> v(3, 42);

        assert(v.get_size() == 3);
        assert(v.get_capacity() == 3);

        for (size_t i = 0; i < v.get_size(); ++i)
        {
            assert(v[i] == 42);
        }
    }

    // Инициализация вектора при помощи initializer_list
    {
        SimpleVector<int> v{ 1, 2, 3 };

        assert(v.get_size() == 3);
        assert(v.get_capacity() == 3);
        assert(v[2] == 3);
    }

    // Доступ к элементам при помощи at
    {
        SimpleVector<int> v(3);

        assert(&v.at(2) == &v[2]);

        try
        {
            v.at(3);
            assert(false);
        }
        catch (const std::out_of_range&)
        {
        }
        catch (...)
        {
            assert(false);
        }
    }

    // Очистка вектора
    {
        SimpleVector<int> v(10);

        const size_t old_capacity = v.get_capacity();

        v.clear();

        assert(v.get_size() == 0);
        assert(v.get_capacity() == old_capacity);
    }

    // Изменение размера
    {
        {
            SimpleVector<int> v(3);

            v[2] = 17;
            v.resize(7);

            assert(v.get_size() == 7);
            assert(v.get_capacity() >= v.get_size());
            assert(v[2] == 17);
            assert(v[3] == 0);
        }

        {
            SimpleVector<int> v(3);

            v[0] = 42;
            v[1] = 55;

            const size_t old_capacity = v.get_capacity();

            v.resize(2);

            assert(v.get_size() == 2);
            assert(v.get_capacity() == old_capacity);
            assert(v[0] == 42);
            assert(v[1] == 55);
        }

        {
            const size_t old_size = 3;

            SimpleVector<int> v(3);

            v.resize(old_size + 5);
            v[3] = 42;
            v.resize(old_size);
            v.resize(old_size + 2);

            assert(v[3] == 0);
        }
    }

    // Итерирование по SimpleVector
    {
        {
            SimpleVector<int> v;

            assert(v.begin() == nullptr);
            assert(v.end() == nullptr);
        }

        {
            SimpleVector<int> v(10, 42);

            assert(v.begin());
            assert(*v.begin() == 42);
            assert(v.end() == v.begin() + v.get_size());
        }
    }

    // Получение первого, последнего элементов и адреса первого элемента
    {
        SimpleVector<int> v{ 1, 2, 3, 4, 5 };

        assert(v.front() == 1);
        assert(v.back() == 5);
        assert(*v.data() == 1);
    }

    // Пририванивание объема к размеру
    {
        SimpleVector<int> v;

        v.push_back(1);
        v.push_back(2);
        v.push_back(3);
        v.push_back(4);
        v.push_back(5);
        v.push_back(6);
        v.push_back(7);
        v.push_back(8);
        v.push_back(9);

        assert(v.get_size() == 9);
        assert(v.get_capacity() == 16);

        v.shrink_to_fit();

        assert(v.get_size() == 9);
        assert(v.get_capacity() == 9);
    }
}

inline void Test2() 
{
    // push_back
    {
        SimpleVector<int> v(1);

        v.push_back(42);

        assert(v.get_size() == 2);
        assert(v.get_capacity() >= v.get_size());

        assert(v[0] == 0);
        assert(v[1] == 42);
    }

    // emplace_back
    {
        SimpleVector<int> v(1);

        v.emplace_back(42);

        assert(v.get_size() == 2);
        assert(v.get_capacity() >= v.get_size());

        assert(v[0] == 0);
        assert(v[1] == 42);
    }

    // append_range
    {
        SimpleVector<int> v(2);
        v[0] = 10;
        v[1] = 20;

        int range[] = { 30, 40, 50 };
        size_t range_size = sizeof(range) / sizeof(range[0]);

        v.append_range(range, range + range_size);

        assert(v.get_size() == 2 + range_size);

        assert(v[0] == 10);
        assert(v[1] == 20);

        for (size_t i = 0; i < range_size; ++i) 
        {
            assert(v[2 + i] == range[i]);
        }
    }

    // assign
    {
        {
            SimpleVector<int> v1{ 1, 2, 3, 4, 5 };

            v1.assign(3, 0);

            assert(v1.get_size() == 3);
            assert(v1.get_capacity() == 5);
            assert(v1[0] == 0);
            assert(v1[1] == 0);
            assert(v1[2] == 0);
        }

        {
            SimpleVector<int> v2{ 1, 2, 3 };

            v2.assign(5, 0);

            assert(v2.get_size() == 5);
            assert(v2.get_capacity() == 5);
            assert(v2[0] == 0);
            assert(v2[1] == 0);
            assert(v2[2] == 0);
            assert(v2[3] == 0);
            assert(v2[4] == 0);
        }

        {
            SimpleVector<int> v3{ 1, 2, 3, 4, 5 };

            v3.assign(5, 0);

            assert(v3.get_size() == 5);
            assert(v3.get_capacity() == 5);

            for (size_t i = 0; i < v3.get_size(); ++i) 
            {
                assert(v3[i] == 0);
            }
        }

        {
            SimpleVector<int> v4{ 1, 2, 3, 4, 5 };

            v4.assign(0, 0);

            assert(v4.get_size() == 0);
            assert(v4.get_capacity() == 5);
        }

        {
            SimpleVector<int> v5{ 1, 2, 3 };

            v5.assign(10, 0);

            assert(v5.get_size() == 10);
            assert(v5.get_capacity() >= 10); 

            for (size_t i = 0; i < v5.get_size(); ++i) 
            {
                assert(v5[i] == 0);
            }
        }
    }

    // Если хватает места, push_back не увеличивает capacity
    {
        SimpleVector<int> v(2);

        v.resize(1);

        const size_t old_capacity = v.get_capacity();

        v.push_back(123);

        assert(v.get_size() == 2);
        assert(v.get_capacity() == old_capacity);
    }

    // pop_back
    {
        SimpleVector<int> v{ 0, 1, 2, 3 };

        const size_t old_capacity = v.get_capacity();
        const auto old_begin = v.begin();

        v.pop_back();

        assert(v.get_capacity() == old_capacity);
        assert(v.begin() == old_begin);
        assert((v == SimpleVector<int>{0, 1, 2}));
    }

    // Конструктор копирования
    {
        SimpleVector<int> numbers{ 1, 2 };

        auto numbers_copy(numbers);

        assert(&numbers_copy[0] != &numbers[0]);
        assert(numbers_copy.get_size() == numbers.get_size());

        for (size_t i = 0; i < numbers.get_size(); ++i)
        {
            assert(numbers_copy[i] == numbers[i]);
            assert(&numbers_copy[i] != &numbers[i]);
        }
    }

    // Сравнение
    {
        assert((SimpleVector{ 1, 2, 3 } == SimpleVector{ 1, 2, 3 }));
        assert((SimpleVector{ 1, 2, 3 } != SimpleVector{ 1, 2, 2 }));

        assert((SimpleVector{ 1, 2, 3 } < SimpleVector{ 1, 2, 3, 1 }));
        assert((SimpleVector{ 1, 2, 3 } > SimpleVector{ 1, 2, 2, 1 }));

        assert((SimpleVector{ 1, 2, 3 } >= SimpleVector{ 1, 2, 3 }));
        assert((SimpleVector{ 1, 2, 4 } >= SimpleVector{ 1, 2, 3 }));
        assert((SimpleVector{ 1, 2, 3 } <= SimpleVector{ 1, 2, 3 }));
        assert((SimpleVector{ 1, 2, 3 } <= SimpleVector{ 1, 2, 4 }));
    }

    // Обмен значений векторов
    {
        SimpleVector<int> v1{ 42, 666 };
        SimpleVector<int> v2;

        v2.push_back(0);
        v2.push_back(1);
        v2.push_back(2);

        const int* const begin1 = &v1[0];
        const int* const begin2 = &v2[0];

        const size_t capacity1 = v1.get_capacity();
        const size_t capacity2 = v2.get_capacity();

        const size_t size1 = v1.get_size();
        const size_t size2 = v2.get_size();

        static_assert(noexcept(v1.swap(v2)));

        v1.swap(v2);

        assert(&v2[0] == begin1);
        assert(&v1[0] == begin2);
        assert(v1.get_size() == size2);
        assert(v2.get_size() == size1);
        assert(v1.get_capacity() == capacity2);
        assert(v2.get_capacity() == capacity1);
    }

    // Присваивание
    {
        SimpleVector<int> src_vector{ 1, 2, 3, 4 };
        SimpleVector<int> dst_vector{ 1, 2, 3, 4, 5, 6 };

        dst_vector = src_vector;

        assert(dst_vector == src_vector);
    }

    // Вставка элементов
    {
        SimpleVector<int> v{ 1, 2, 3, 4 };

        v.insert(v.begin() + 2, 42);

        assert((v == SimpleVector<int>{1, 2, 42, 3, 4}));
    }

    // Удаление элементов
    {
        SimpleVector<int> v{ 1, 2, 3, 4 };

        v.erase(v.cbegin() + 2);

        assert((v == SimpleVector<int>{1, 2, 4}));
    }
}

// Функция заполнения вектора последовательными значениями
SimpleVector<int> GenerateVector(size_t size) 
{
    SimpleVector<int> v(size);
    iota(v.begin(), v.end(), 1);
    return v;
}

// Вспомогательный класс для проверки правильности перемещения
// Класс запрещает копирование, но позволяет их перемещать
class X 
{
public:
    X() : X(5) {}

    X(size_t num) : x(num) {}

    X(const X& other) = delete;
    X& operator=(const X& other) = delete;

    X(X&& other)
    {
        x = exchange(other.x, 0);
    }

    X& operator=(X&& other) 
    {
        x = exchange(other.x, 0);
        return *this;
    }
    size_t get_x() const 
    {
        return x;
    }

private:
    size_t x;
};

void Test3()
{
    // создание
    {
        const size_t size = 1000000;
        SimpleVector<int> moved_vector(GenerateVector(size));
        assert(moved_vector.get_size() == size);
    }

    // присваивание
    {
        const size_t size = 1000000;
        SimpleVector<int> moved_vector;

        assert(moved_vector.get_size() == 0);

        moved_vector = GenerateVector(size);

        assert(moved_vector.get_size() == size);
    }

    // перемещение в конструктор
    {
        const size_t size = 1000000;
        SimpleVector<int> vector_to_move(GenerateVector(size));

        assert(vector_to_move.get_size() == size);

        SimpleVector<int> moved_vector(move(vector_to_move));

        assert(moved_vector.get_size() == size);
        assert(vector_to_move.get_size() == 0);
    }

    // перемещение в оператор присваивания
    {
        const size_t size = 1000000;
        SimpleVector<int> vector_to_move(GenerateVector(size));

        assert(vector_to_move.get_size() == size);

        SimpleVector<int> moved_vector = move(vector_to_move);

        assert(moved_vector.get_size() == size);
        assert(vector_to_move.get_size() == 0);
    }

    // перемещение с объектом класса Х
    {
        const size_t size = 5;
        SimpleVector<X> vector_to_move;

        for (size_t i = 0; i < size; ++i) 
        {
            vector_to_move.push_back(X(i));
        }

        SimpleVector<X> moved_vector = move(vector_to_move);

        assert(moved_vector.get_size() == size);
        assert(vector_to_move.get_size() == 0);

        for (size_t i = 0; i < size; ++i) 
        {
            assert(moved_vector[i].get_x() == i);
        }
    }

    // различные варианты вставок
    {
        const size_t size = 5;
        SimpleVector<X> v;

        for (size_t i = 0; i < size; ++i) 
        {
            v.push_back(X(i));
        }

        // в начало
        v.insert(v.begin(), X(size + 1));

        assert(v.get_size() == size + 1);
        assert(v.begin()->get_x() == size + 1);

        // в конец
        v.insert(v.end(), X(size + 2));

        assert(v.get_size() == size + 2);
        assert((v.end() - 1)->get_x() == size + 2);

        // в середину
        v.insert(v.begin() + 3, X(size + 3));

        assert(v.get_size() == size + 3);
        assert((v.begin() + 3)->get_x() == size + 3);
    }

    // удаление
    {
        const size_t size = 3;
        SimpleVector<X> v;

        for (size_t i = 0; i < size; ++i) 
        {
            v.push_back(X(i));
        }

        auto it = v.erase(v.begin());

        assert(it->get_x() == 1);
    }
}

void TestRun()
{
    Test1();
    Test2();
    Test3();
}