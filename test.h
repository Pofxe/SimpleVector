#pragma once

#include "simple_vector.h"

#include <cassert>
#include <iostream>

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
    // // push_back
    {
        SimpleVector<int> v(1);

        v.push_back(42);

        assert(v.get_size() == 2);
        assert(v.get_capacity() >= v.get_size());

        assert(v[0] == 0);
        assert(v[1] == 42);
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

void TestRun()
{
    Test1();
    Test2();
}