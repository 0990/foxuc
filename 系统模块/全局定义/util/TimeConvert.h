#ifndef _TIMECONVERT_H_
#define _TIMECONVERT_H_

namespace Util
{
	class TimeConvert
	{
	public:
		TimeConvert();
		~TimeConvert();

		enum ConvertType
		{
			HH_MM,					// תΪʱ�� hh:mm
			HH_MM_SS,				// תΪʱ�� hh:mm:ss
			MM_DD_HH_MM,			// ץΪʱ�� mmddhhmm
			YYYY_MM_DD,				// תΪʱ�� yyyymmdd
			YYYY_MM_DD_HH_MM_SS,	// תΪʱ�� yyyymmddhhmmss
		
		};

		static time_t GetCurrentTaskFreshTime();
		static time_t GetTaskFreshTime(SYSTEMTIME& sys);

		static WORD GetDaysInMonth(WORD year, WORD month);

		static time_t SysTime2Time(const SYSTEMTIME& st);
		static SYSTEMTIME Timet2SystemTime(const time_t& tt);

		static string SysTime2Str(const SYSTEMTIME& st, ConvertType CType = YYYY_MM_DD_HH_MM_SS);
		static string SysTime2Str(const time_t& tt, ConvertType CType = YYYY_MM_DD_HH_MM_SS);

		static SYSTEMTIME Str2SystemTime(const string& strTime, ConvertType CType = YYYY_MM_DD_HH_MM_SS);
		static time_t Str2Time(const string& strTime, ConvertType CType = YYYY_MM_DD_HH_MM_SS);

	};
}

#endif //_TIMECONVERT_H_