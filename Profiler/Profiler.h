#pragma once
#include <Windows.h>
#include <tchar.h>

#ifdef PROFILE

#define PRO_BEGIN(TagName)	Profiler::ProfileBegin(_T(TagName))
#define PRO_END(TagName)	Profiler::ProfileEnd(_T(TagName))
#define PRO(Name, TagName)	Profiler::Profile Name = { _T(TagName) }
#else
#define PRO_BEGIN(TagName)
#define PRO_END(TagName)
#define PRO(TagName)

#endif

namespace Profiler
{
	const enum FLAG { EMPTY, BEGIN, END, RESET };

	struct PROFILE_SMAPLE
	{
		FLAG lFlag;
		TCHAR szName[64];
		LARGE_INTEGER lStartTime;

		__int64 iTotalTime;
		__int64 iMin[2];
		__int64 iMax[2];
		__int64 iCall;
	};

	void ProfileBegin(const TCHAR* TagName);
	void ProfileEnd(const TCHAR* TagName);
	void ProfileOutText(const TCHAR* fileName);
	void ProfileReset(void);
	void CreateProfileFileName(TCHAR* buffer);

	class Profile
	{
	public:
		Profile(const TCHAR* tag)
		{
			ProfileBegin(tag);
			_tag = tag;
		}
		~Profile()
		{
			ProfileEnd(_tag);
		}
	private:
		const TCHAR* _tag;
	};
}
