#define PROFILE
#include "Profiler.h"

int main()
{
	int i = 0;
	while (i < 10)
	{
		{
			PRO_BEGIN("dd");
			Sleep(100);
			PRO_END("dd");
		}

		i++;
	}

	TCHAR fileName[256];
	Profiler::CreateProfileFileName(fileName);
	Profiler::ProfileOutText(fileName);
}