#pragma once

#define _CRTDBG_MAP_ALLOC
#include <iostream>
#include <crtdbg.h>

#ifdef _DEBUG
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif

class MemoryLeakDetector
{
public:
    MemoryLeakDetector()
    {
        // Настройка режима отчета о памяти
        _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);

        // Настройка файла для записи отчетов
        _CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDERR);
    }

    ~MemoryLeakDetector()
    {
        // Проверка на наличие утечек памяти
        _CrtDumpMemoryLeaks();
    }
};