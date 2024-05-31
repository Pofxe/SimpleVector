#include "test.h"
#include "log.h"
#include "detector.h"

int main()
{
	MemoryLeakDetector detector;

	LogDuration("Test");

	TestRun();
}