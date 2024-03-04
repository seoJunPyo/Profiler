#define PROFILE
#include "Profiler.h"

int main()
{
	int i = 0;
	while (i < 10)
	{
		{
			PRO(AAA, "AAA");
			Sleep(100);
		}

		i++;
	}

	TCHAR fileName[256];
	Profiler::CreateProfileFileName(fileName);
	Profiler::ProfileOutText(fileName);
}