#include "test.h"
#include "log.h"

int main()
{
	MemoryLeakDetector detector;

	{
		LogDuration("Test");

		TestRun();
	}
}