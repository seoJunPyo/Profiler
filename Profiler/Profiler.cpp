#include <time.h>
#include <iostream>
#include "Profiler.h"
#define MAX_SAMPLE 64

namespace Profiler
{
	const int MICRO_SECOND = 1000000;

	PROFILE_SMAPLE list[MAX_SAMPLE];

	void ProfileBegin(const TCHAR* TagName)
	{
		int idx;

		for (idx = 0; idx < MAX_SAMPLE; idx++)
		{
			if (_tcscmp(TagName, list[idx].szName) == 0)
			{
				break;
			}

			if (list[idx].lFlag == EMPTY)
			{
				_tcscpy_s(list[idx].szName, TagName);
				list[idx].iMin[0] = LLONG_MAX;
				list[idx].iMin[1] = LLONG_MAX;
				break;
			}
		}

		// -------------------------------------------------------------------------
		// End�� ���������� ȣ��Ǿ��ٸ�, End���� lFlag�� BEGIN�� �ƴ� ������ �����Ѵ�.
		// ���� lFlag�� BEGIN�̶�� Begin�� �� �� ���� ȣ��� ������ ��Ȳ���� ���ܸ� ������.
		// -------------------------------------------------------------------------
		if (list[idx].lFlag == BEGIN)
		{
			// ���� �����ؼ� ����� ������
			throw 1;
		}

		list[idx].lFlag = BEGIN;
		QueryPerformanceCounter(&list[idx].lStartTime);
	}

	void ProfileEnd(const TCHAR* TagName)
	{
		LARGE_INTEGER end;
		QueryPerformanceCounter(&end);

		int idx;
		bool isFound = false;

		for (idx = 0; idx < MAX_SAMPLE; idx++)
		{
			if (_tcscmp(TagName, list[idx].szName) == 0)
			{
				isFound = true;
				break;
			}
		}

		// -------------------------------------------------------------------------
		// �±� ������ �˻��� �����ϸ� ���ܸ� ������.
		// -------------------------------------------------------------------------
		if (!isFound)
		{
			// �±� ������ ã�� ���ߴ�.
			throw 2;
		}

		// -------------------------------------------------------------------------
		// End�� ���������� ȣ��Ǿ����� �˸��� ���� lFlag ���� �����Ѵ�.
		// -------------------------------------------------------------------------
		list[idx].lFlag = END;

		// -------------------------------------------------------------------------
		// �ҿ� �ð��� iTotalTime�� ���س�����.
		// -------------------------------------------------------------------------
		__int64 diff = (end.QuadPart - list[idx].lStartTime.QuadPart);
		list[idx].iTotalTime += diff;

		list[idx].iCall++;

		// -------------------------------------------------------------------------
		//	�ִ밪 ����.
		// -------------------------------------------------------------------------
		if (diff > list[idx].iMax[0])
		{
			list[idx].iMax[1] = list[idx].iMax[0];
			list[idx].iMax[0] = diff;
		}

		// -------------------------------------------------------------------------
		//	�ּҰ� ����.
		// -------------------------------------------------------------------------
		if (diff < list[idx].iMin[0])
		{
			if (diff < list[idx].iMin[1])
			{
				list[idx].iMin[0] = list[idx].iMin[1];
				list[idx].iMin[1] = diff;
			}
			else
			{
				list[idx].iMin[0] = diff;
			}
		}
	}

	void ProfileOutText(const TCHAR* fileName)
	{
		_tsetlocale(LC_ALL, _T("Korean"));

		FILE* fp;
		errno_t err = _tfopen_s(&fp, fileName, _T("w"));
		if (err != 0)
		{
			throw 3;
		}

		// -------------------------------------------------------------------------
		//	���� ��� �Է�.
		// -------------------------------------------------------------------------
		_ftprintf(fp, _T("-------------------------------------------------------------------------------------------------------------\n"));
		_ftprintf(fp, _T("%20s | %20s | %20s | %20s | %20s |\n"), _T("Name"), _T("Average"), _T("Min"), _T("Max"), _T("Call"));
		_ftprintf(fp, _T("-------------------------------------------------------------------------------------------------------------\n"));

		LARGE_INTEGER fre;
		QueryPerformanceFrequency(&fre);

		for (int i = 0; i < MAX_SAMPLE; i++)
		{
			if (list[i].lFlag == 0)
				break;

			long double Average = ((long double)((list[i].iTotalTime - list[i].iMax[0] - list[i].iMax[1] - list[i].iMin[0] - list[i].iMin[1]) / (list[i].iCall - 4)) / fre.QuadPart) * MICRO_SECOND;
			long double Max = ((long double)list[i].iMax[0] / fre.QuadPart) * MICRO_SECOND;
			long double Min = ((long double)list[i].iMin[0] / fre.QuadPart) * MICRO_SECOND;

			_ftprintf(fp, _T("%20s |  %20.3lf�� | %20.3lf�� | %20.3lf�� | %20lld |\n"), list[i].szName, Average, Min, Max, list[i].iCall);
		}
	}

	void ProfileReset(void)
	{
		for (int i = 0; i < MAX_SAMPLE; i++)
		{
			if (list[i].lFlag == 0)
				break;

			list[i].lFlag = RESET;
			list[i].iTotalTime = 0;
			list[i].iCall = 0;
			list[i].iMax[0] = 0;
			list[i].iMin[0] = LLONG_MAX;
			list[i].iMin[1] = LLONG_MAX;
		}
	}

	void CreateProfileFileName(TCHAR* buffer)
	{
		tm now_time;
		__time64_t long_time;

		_time64(&long_time);
		errno_t err = _localtime64_s(&now_time, &long_time);

		_stprintf_s(buffer, 128, _T("Profiling_%d%02d%02d_%02d%02d%02d.txt"),
			now_time.tm_year + 1900, now_time.tm_mon + 1, now_time.tm_mday, now_time.tm_hour, now_time.tm_min, now_time.tm_sec);
	}
}

