#if !defined( OLE_DATE_H )
#define OLE_DATE_H

#if !defined( DATE )
#define DATE double
#endif

#ifndef APIEXP
#define APIEXP 
#endif

#include "pch.h"

#include <ios>
#include <iomanip>
#include <iostream>
#include <sstream>


#define OLEDATETIME_1SECOND (1.0/((double)24*60*60))

#if !defined(_WIN32_WINDOWS)
#if defined(__BORLANDC__)
#define _WIN32_WINDOWS _WIN32_WINNT
#elif defined(_MSC_VER)
#define _WIN32_WINDOWS _WIN32_WINNT
#endif
#endif

#if defined(__BORLANDC__)
#define _timeb timeb
#define _ftime ftime
#endif


#if !defined(_WIN32_WINDOWS)
#define SIMULATE_WIN_SYSTEMTIME

typedef struct _SYSTEMTIME {
	__int16 wYear;
	__int16 wMonth;
	__int16 wDayOfWeek;
	__int16 wDay;
	__int16 wHour;
	__int16 wMinute;
	__int16 wSecond;
	__int16 wMilliseconds;

} SYSTEMTIME, *LPSYSTEMTIME;

void GetSystemTime(LPSYSTEMTIME);
void GetLocalTime(LPSYSTEMTIME);
BOOL SystemTimeToTzSpecificLocalTime(void*, LPSYSTEMTIME stUniversalTime, LPSYSTEMTIME stLocalTime);

#endif


BOOL APIEXP _AfxOleDateFromTm(WORD wYear, WORD wMonth, WORD wDay, WORD wHour, WORD wMinute, WORD wSecond, DATE& dtDest);

BOOL APIEXP _AfxTmFromOleDate(DATE dtSrc, LPSYSTEMTIME tmDest);

#if !defined(_UCC)
PTCHAR APIEXP GetTStringSystemTime(PTCHAR szBuf);

PTCHAR APIEXP GetTStringFromOLETime(DATE dtSrc, PTCHAR szBuf = 0);
PTCHAR APIEXP GetTStringFromOLEDate(DATE dtSrc, PTCHAR szBuf = 0);
#endif

DATE APIEXP GetOleCurrentDate(bool with_time = true);

DATE APIEXP GetOleCurrentUTCDate(bool with_time = true);

SYSTEMTIME APIEXP GetSystemtimeFromOleDate(DATE dtSrc);

DATE APIEXP GetOleDateFromSystemtime(SYSTEMTIME dtSrc);

__int64 APIEXP GetDayTimeInMilliseconds(SYSTEMTIME *pSystimeInp = 0);
__int64 APIEXP GetDeltaDayTimeInMilliseconds(__int64 timeInMilliseconds);

LONG APIEXP GetTimeInMilliseconds();
LONG APIEXP GetDeltaTimeInMilliseconds(LONG timeInMilliseconds);

void APIEXP ConvertUTCDateTime2LocalDateTime(double &dDate);

DATE APIEXP GetOleDateFromHHMMSS(const char* str, const char* format = 0);
DATE APIEXP GetOleDateFromYYMMDDHHMMSS(const char* str);
DATE APIEXP GetLocalOleDateFromUtcYYMMDDHHMMSS(const char* str);
std::string APIEXP GetYYMMDDHHMMSSFromOleDate(DATE dDate);
std::string APIEXP GetHHMMSSFromOleDate(DATE dDate);

std::string APIEXP GetDBDateStringFromOleDate(DATE dDate, const char *szFormat = "'%d-%02d-%02d %02d:%02d:%02d'");
std::string APIEXP GetDBTimeStringFromOleDate(DATE dDate, const char *szFormat = "'%02d:%02d:%02d'");

std::string APIEXP GetDateStringFromOleDate(DATE dDate);
std::string APIEXP GetTimeStringFromOleDate(DATE dDate);

DATE APIEXP GetOleDateFromDateString(const char* str);
DATE APIEXP GetOleDateFromTimeString(const char* str);
DATE APIEXP GetOleDateFromDateTimeString(const char* str, const char *szFormat = "%d-%d-%d%*c%d:%d:%d");

#endif // OLE_DATE_H




#if defined( OLE_DATE_IMPL )
#if !defined( OLE_DATE_IMPL_H )
#define OLE_DATE_IMPL_H

#include "math.h"



#if defined(SIMULATE_WIN_SYSTEMTIME)
#if defined(__GCC__)
#include <sys/timeb.h>
#endif


void timeb2systemtime(struct tm *x, LPSYSTEMTIME systime) {
	systime->wYear = x->tm_year + 1900;
	systime->wMonth = x->tm_mon + 1;
	systime->wDayOfWeek = x->tm_wday;
	systime->wDay = x->tm_mday;
	systime->wHour = x->tm_hour;
	systime->wMinute = x->tm_min;
	systime->wSecond = x->tm_sec;
}

void ConvertTimeb2Systemtime(LPSYSTEMTIME systime, bool utc = true) {
//	struct timeval tv;
//	gettimeofday(&tv, NULL);
//	time_t t = tv.tv_sec;

	struct _timeb tb;
	_ftime(&tb);
	time_t *t = &tb.time;

	struct tm x;
	if(utc) {
		x = *gmtime(t);
	}
	else {
		x = *localtime(t);
	}

	timeb2systemtime(&x, systime);
//	systime->wMilliseconds = tv.tv_usec / 1000;
	systime->wMilliseconds = tb.millitm;
}

void GetSystemTime(LPSYSTEMTIME systime) {
	ConvertTimeb2Systemtime(systime);
}

void GetLocalTime(LPSYSTEMTIME systime) {
	ConvertTimeb2Systemtime(systime, false);
}

BOOL SystemTimeToTzSpecificLocalTime(void*, LPSYSTEMTIME stUniversalTime, LPSYSTEMTIME stLocalTime) {
	struct tm x;

	x.tm_year = stUniversalTime->wYear - 1900;
	x.tm_mon = stUniversalTime->wMonth;
	x.tm_wday = stUniversalTime->wDayOfWeek;
	x.tm_mday = stUniversalTime->wDay;
	x.tm_hour = stUniversalTime->wHour;
	x.tm_min = stUniversalTime->wMinute;
	x.tm_sec = stUniversalTime->wSecond;

	time_t t = mktime(&x);
	x = *localtime(&t);

	timeb2systemtime(&x, stLocalTime);
	stLocalTime->wMilliseconds = stUniversalTime->wMilliseconds;

	return true;
}

#endif



// Half a second, expressed in days
#define HALF_SECOND  (1.0/172800.0)

// One-based array of days in year at month start
int _afxMonthDays[13] = {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365};

BOOL _AfxDaysFromTm(WORD wYear, WORD wMonth, WORD wDay, long& nDays)
{
//	Validate year and month (ignore day of week and milliseconds)
	if(wYear > 9999 || wMonth < 1 || wMonth > 12) {
		return false;
	}

//	Check for leap year and set the number of days in the month
	BOOL bLeapYear = ((wYear & 3) == 0) && ((wYear % 100) != 0 || (wYear % 400) == 0);

	int nDaysInMonth = _afxMonthDays[wMonth] - _afxMonthDays[wMonth-1] + ((bLeapYear && wDay == 29 && wMonth == 2) ? 1 : 0);

//	Finish validating the date
	if(wDay < 1 || wDay > nDaysInMonth) {
		return false;
	}

//	It is a valid date; make Jan 1, 1AD be 1
	nDays = wYear*365L + wYear/4 - wYear/100 + wYear/400 + _afxMonthDays[wMonth-1] + wDay;

//	If leap year and it's before March, subtract 1:
	if(wMonth <= 2 && bLeapYear) {
		--nDays;
	}

//	Offset so that 12/30/1899 is 0
	nDays -= 693959L;
	return true;
}

BOOL _AfxOleDateFromTm(WORD wYear, WORD wMonth, WORD wDay, WORD wHour, WORD wMinute, WORD wSecond, DATE& dtDest)
{
//	Validate year and month (ignore day of week and milliseconds)
	if (wYear > 9999 || wMonth < 1 || wMonth > 12)
		return false;

//	Check for leap year and set the number of days in the month
	BOOL bLeapYear = ((wYear & 3) == 0) && ((wYear % 100) != 0 || (wYear % 400) == 0);

	int nDaysInMonth = _afxMonthDays[wMonth] - _afxMonthDays[wMonth-1] + ((bLeapYear && wDay == 29 && wMonth == 2) ? 1 : 0);

//	Finish validating the date
	if (wDay < 1 || wDay > nDaysInMonth || wHour > 23 || wMinute > 59 || wSecond > 59)
	{
		return false;
	}

//	Cache the date in days and time in fractional days
	long nDate;
	double dblTime;

//	It is a valid date; make Jan 1, 1AD be 1
	nDate = wYear*365L + wYear/4 - wYear/100 + wYear/400 + _afxMonthDays[wMonth-1] + wDay;

//	If leap year and it's before March, subtract 1:
	if (wMonth <= 2 && bLeapYear)
		--nDate;

//	Offset so that 12/30/1899 is 0
	nDate -= 693959L;

	dblTime = (((long)wHour * 3600L) +  // hrs in seconds
		((long)wMinute * 60L) +  // mins in seconds
		((long)wSecond)) / 86400.;

	dtDest = (double) nDate + ((nDate >= 0) ? dblTime : -dblTime);

	return true;
}

BOOL _AfxTmFromOleDate(DATE dtSrc, LPSYSTEMTIME tmDest)
{
	long nDays;             // Number of days since Dec. 30, 1899
	long nDaysAbsolute;     // Number of days since 1/1/0
	long nSecsInDay;        // Time in seconds since midnight
	long nMinutesInDay;     // Minutes in day

	long n400Years;         // Number of 400 year increments since 1/1/0
	long n400Century;       // Century within 400 year block (0,1,2 or 3)
	long n4Years;           // Number of 4 year increments since 1/1/0
	long n4Day;             // Day within 4 year block
							//  (0 is 1/1/yr1, 1460 is 12/31/yr4)
	long n4Yr;              // Year within 4 year block (0,1,2 or 3)
	BOOL bLeap4 = true;     // TRUE if 4 year block includes leap year

	double dblDate = dtSrc; // tempory serial date

	// If a valid date, then this conversion should not overflow
	nDays = (long)dblDate;

	// Round to the second
	dblDate += ((dtSrc > 0.0) ? HALF_SECOND : -HALF_SECOND);

	nDaysAbsolute = (long)dblDate + 693959L; // Add days from 1/1/0 to 12/30/1899

	dblDate = fabs(dblDate);
	nSecsInDay = (long)((dblDate - floor(dblDate)) * 86400.);

	// Calculate the day of week (sun=0, mon=1...)
	// -1 because 1/1/0 is Sat.
	tmDest->wDayOfWeek = (int)((nDaysAbsolute - 1) % 7L);

	// Leap years every 4 yrs except centuries not multiples of 400.
	n400Years = (long)(nDaysAbsolute / 146097L);

	// Set nDaysAbsolute to day within 400-year block
	nDaysAbsolute %= 146097L;

	// -1 because first century has extra day
	n400Century = (long)((nDaysAbsolute - 1) / 36524L);

	// Non-leap century
	if (n400Century != 0)
	{
		// Set nDaysAbsolute to day within century
		nDaysAbsolute = (nDaysAbsolute - 1) % 36524L;

		// +1 because 1st 4 year increment has 1460 days
		n4Years = (long)((nDaysAbsolute + 1) / 1461L);

		if (n4Years != 0)
			n4Day = (long)((nDaysAbsolute + 1) % 1461L);
		else
		{
			bLeap4 = false;
			n4Day = (long)nDaysAbsolute;
		}
	}
	else
	{
		// Leap century - not special case!
		n4Years = (long)(nDaysAbsolute / 1461L);
		n4Day = (long)(nDaysAbsolute % 1461L);
	}

	if (bLeap4)
	{
		// -1 because first year has 366 days
		n4Yr = (n4Day - 1) / 365;

		if (n4Yr != 0)
			n4Day = (n4Day - 1) % 365;
	}
	else
	{
		n4Yr = n4Day / 365;
		n4Day %= 365;
	}

	tmDest->wYear = (WORD)(n400Years * 400 + n400Century * 100 + n4Years * 4 + n4Yr);

	// Handle leap year: before, on, and after Feb. 29.
	if (n4Yr == 0 && bLeap4)
	{
		// Leap Year
		if (n4Day == 59)
		{
			/* Feb. 29 */
			tmDest->wMonth = 2;
			tmDest->wDay = 29;
			goto DoTime;
		}

		// Pretend it's not a leap year for month/day comp.
		if (n4Day >= 60)
			--n4Day;
	}

	// Make n4DaY a 1-based day of non-leap year and compute
	//  month/day for everything but Feb. 29.
	++n4Day;

	// Month number always >= n/32, so save some loop time */
	for (tmDest->wMonth = (WORD)((n4Day >> 5) + 1); n4Day > _afxMonthDays[tmDest->wMonth]; tmDest->wMonth++);

	tmDest->wDay = (int)(n4Day - _afxMonthDays[tmDest->wMonth-1]);

DoTime:
	if (nSecsInDay == 0)
		tmDest->wHour = tmDest->wMinute = tmDest->wSecond = 0;
	else
	{
		tmDest->wSecond = (WORD)(nSecsInDay % 60L);
		nMinutesInDay = nSecsInDay / 60L;
		tmDest->wMinute = (WORD)(nMinutesInDay % 60);
		tmDest->wHour = (WORD)(nMinutesInDay / 60);
	}

	return nDays >= 0;
}

#if !defined(_UCC)
PTCHAR GetTStringSystemTime(PTCHAR szBuf)
{
	SYSTEMTIME systimeSrc;
	GetLocalTime(&systimeSrc);

	if(szBuf == 0)
	{
		static TCHAR x[32];
		szBuf = x;
	}

	_stprintf(szBuf, TEXT("%d-%d-%d %d:%d:%d"),
		(int)systimeSrc.wMonth, (int)systimeSrc.wDay, (int)systimeSrc.wYear, (int)systimeSrc.wHour, (int)systimeSrc.wMinute, (int)systimeSrc.wSecond);
	return szBuf;
}

PTCHAR GetTStringFromOLEDate(DATE dtSrc, PTCHAR szBuf) {
	SYSTEMTIME systimeSrc = GetSystemtimeFromOleDate(dtSrc) ;
	if(szBuf == 0) {
		static TCHAR x[32];
		szBuf = x;
	}
#ifdef __G_LIB_H__
	GDate *date = g_date_new_dmy((GDateDay)systimeSrc.wDay, (GDateMonth)systimeSrc.wMonth, systimeSrc.wYear);
	g_date_strftime(szBuf, 32, "%x", date);
#else
	_stprintf(szBuf, TEXT("%d-%d-%d"), (int)systimeSrc.wMonth, (int)systimeSrc.wDay, (int)systimeSrc.wYear);
#endif
	return szBuf;
}

PTCHAR GetTStringFromOLETime(DATE dtSrc, PTCHAR szBuf)
{
	SYSTEMTIME systimeSrc = GetSystemtimeFromOleDate(dtSrc) ;

	if(szBuf == 0)
	{
		static TCHAR x[32];
		szBuf = x;
	}

	_stprintf(szBuf, TEXT("%d-%d-%d %d:%d:%d"), (int)systimeSrc.wMonth, (int)systimeSrc.wDay, (int)systimeSrc.wYear, (int)systimeSrc.wHour, (int)systimeSrc.wMinute, (int)systimeSrc.wSecond);
	return szBuf;
}
#endif

SYSTEMTIME GetSystemtimeFromOleDate(DATE dtSrc)
{
	SYSTEMTIME tmDest;
	_AfxTmFromOleDate(dtSrc, &tmDest);

	tmDest.wMilliseconds = 0;
	return tmDest;
}

DATE GetOleCurrentDate(bool with_time) {
	SYSTEMTIME systimeSrc;
	GetLocalTime(&systimeSrc);
	if(!with_time) {
		systimeSrc.wHour = systimeSrc.wMinute = systimeSrc.wSecond = 0;
	}

	DATE dtDest;
	BOOL dtValid = _AfxOleDateFromTm(systimeSrc.wYear, systimeSrc.wMonth, systimeSrc.wDay,
		systimeSrc.wHour, systimeSrc.wMinute, systimeSrc.wSecond, dtDest);

	return dtValid? dtDest: 0;
}

DATE GetOleCurrentUTCDate(bool with_time) {
	SYSTEMTIME systimeSrc;
	GetSystemTime(&systimeSrc);
	if(!with_time) {
		systimeSrc.wHour = systimeSrc.wMinute = systimeSrc.wSecond = 0;
	}

	DATE dtDest;
	BOOL dtValid = _AfxOleDateFromTm(systimeSrc.wYear, systimeSrc.wMonth, systimeSrc.wDay,
		systimeSrc.wHour, systimeSrc.wMinute, systimeSrc.wSecond, dtDest);

	return dtValid? dtDest: 0;
}


DATE GetOleDateFromSystemtime(SYSTEMTIME dtSrc)
{
	DATE dtDest;
	BOOL dtValid = _AfxOleDateFromTm(dtSrc.wYear, dtSrc.wMonth, dtSrc.wDay, dtSrc.wHour, dtSrc.wMinute, dtSrc.wSecond, dtDest);

	return dtValid? dtDest: 0;
}

__int64 _AfxGetDayTimeInMilliseconds(SYSTEMTIME *pSystimeInp) {
	SYSTEMTIME systimeSrc;
	if(pSystimeInp != 0) {
		systimeSrc = *pSystimeInp;
	}
	else {
		GetLocalTime(&systimeSrc);
	}

	long days = 0;
	if(!_AfxDaysFromTm(systimeSrc.wYear, systimeSrc.wMonth, systimeSrc.wDay, days)) {
		return 0;
	}

	static long boot_day = 0;
	if(boot_day == 0) {
		_AfxDaysFromTm(systimeSrc.wYear, systimeSrc.wMonth, systimeSrc.wDay, boot_day);
	}

	days -= boot_day;

	__int64 millisecs = LONG(systimeSrc.wMilliseconds) + LONG(LONG(systimeSrc.wHour) * 3600L + systimeSrc.wMinute * 60L + systimeSrc.wSecond) * 1000L;
	return ((__int64)days) * 24 * 3600 * 1000 + millisecs;
}


__int64 GetDayTimeInMilliseconds(SYSTEMTIME *pSystimeInp) { 
	if(pSystimeInp) { 
		return _AfxGetDayTimeInMilliseconds(pSystimeInp); 
	} 

	_timeb tmnow;
	memset(&tmnow, 0, sizeof(tmnow));
	_ftime(&tmnow);

	__int64 secs = tmnow.time - 40 * (__int64)31556926; 

	return (secs * 1000) + tmnow.millitm;
}

__int64 GetDeltaDayTimeInMilliseconds(__int64 timeInMilliseconds) {
	return GetDayTimeInMilliseconds() - timeInMilliseconds;
}

LONG GetTimeInMilliseconds() {
	SYSTEMTIME systimeSrc;
	GetLocalTime(&systimeSrc);
	return LONG(systimeSrc.wMilliseconds) + LONG(LONG(systimeSrc.wHour) * 3600L + systimeSrc.wMinute * 60L + systimeSrc.wSecond) * 1000L;
}

LONG GetDeltaTimeInMilliseconds(LONG timeInMilliseconds) {
	LONG delta = GetTimeInMilliseconds() - timeInMilliseconds;
	if(delta < 0) {
		delta = -delta;
	}

	return delta;
}

void ConvertUTCDateTime2LocalDateTime(double &dDate) {
	if(dDate == 0) {
		return;
	}

	SYSTEMTIME stUniversalTime = GetSystemtimeFromOleDate(dDate);
	SYSTEMTIME stLocalTime;

	BOOL rc = SystemTimeToTzSpecificLocalTime(NULL, &stUniversalTime, &stLocalTime);
	if(rc == false) {
//		OutputDebugStringA("OleDate::ConvertUTCDateTime2LocalDateTime()->Error\r\n");
		return;
	}

	dDate = GetOleDateFromSystemtime(stLocalTime);
}


DATE GetOleDateFromYYMMDDHHMMSS(const char* str) {
	SYSTEMTIME st;
	memset(&st, 0, sizeof(st));
	sscanf(str, "%2hd%2hd%2hd%2hd%2hd%2hd", &st.wYear, &st.wMonth, &st.wDay, &st.wHour, &st.wMinute, &st.wSecond);
	st.wYear += 2000;

	DATE dDate = GetOleDateFromSystemtime(st);
	return dDate;
}


DATE GetOleDateFromHHMMSS(const char* str, const char* format) {
	if(!format) {
		format = "%2hd%2hd%2hd";
	}
	SYSTEMTIME st;
	memset(&st, 0, sizeof(st));
	sscanf(str, format, &st.wHour, &st.wMinute, &st.wSecond);
	st.wYear = 2000;
	st.wMonth = 1;
	st.wDay = 1;

	DATE dDate = GetOleDateFromSystemtime(st);
	int nDate = (int)dDate;
	return dDate - nDate;
}


DATE GetLocalOleDateFromUtcYYMMDDHHMMSS(const char* str) {
	DATE dDate = GetOleDateFromYYMMDDHHMMSS(str);
	ConvertUTCDateTime2LocalDateTime(dDate);
	return dDate;
}


std::string GetYYMMDDHHMMSSFromOleDate(DATE dDate) {
	SYSTEMTIME tmDate = GetSystemtimeFromOleDate(dDate);
	std::ostringstream otime;
	{
		using namespace std;
		otime << setfill('0')
			<< setw(2) << (tmDate.wYear - 2000) << setw(2) << tmDate.wMonth << setw(2) << tmDate.wDay
			<< setw(2) << tmDate.wHour << setw(2) << tmDate.wMinute << setw(2) << tmDate.wSecond;
	}

	return otime.str();
}

std::string GetHHMMSSFromOleDate(DATE dDate) {
	SYSTEMTIME tmDate = GetSystemtimeFromOleDate(dDate);
	std::ostringstream otime;
	{
		using namespace std;
		otime << setfill('0')
			<< setw(2) << tmDate.wHour << setw(2) << tmDate.wMinute << setw(2) << tmDate.wSecond;
	}

	return otime.str();
}

std::string GetDBDateStringFromOleDate(DATE dDate, const char *szFormat) {
	SYSTEMTIME tmDate = GetSystemtimeFromOleDate(dDate);
	CHAR buf[64];
	sprintf(buf, szFormat, (int)tmDate.wMonth, (int)tmDate.wDay, (int)tmDate.wYear, (int)tmDate.wHour, (int)tmDate.wMinute, (int)tmDate.wSecond);
	return std::string(buf);
}

std::string GetDBTimeStringFromOleDate(DATE dDate, const char *szFormat) {
	SYSTEMTIME tmDate = GetSystemtimeFromOleDate(dDate);
	CHAR buf[64];
	sprintf(buf, szFormat, (int)tmDate.wHour, (int)tmDate.wMinute, (int)tmDate.wSecond);
	return std::string(buf);
}



std::string GetDateStringFromOleDate(DATE dDate) {
	SYSTEMTIME tmDate = GetSystemtimeFromOleDate(dDate);
	CHAR buf[64];
	sprintf(buf, "%d-%d-%d", (int)tmDate.wMonth, (int)tmDate.wDay, (int)tmDate.wYear);
	return std::string(buf);
}

std::string GetTimeStringFromOleDate(DATE dDate) {
	SYSTEMTIME tmDate = GetSystemtimeFromOleDate(dDate);
	CHAR buf[64];
	sprintf(buf, "%d:%d:%d", (int)tmDate.wHour, (int)tmDate.wMinute, (int)tmDate.wSecond);
	return std::string(buf);
}

DATE GetOleDateFromDateString(const char* str) {
	SYSTEMTIME st;
	memset(&st, 0, sizeof(st));
	sscanf(str, "%hd-%hd-%hd", &st.wMonth, &st.wDay, &st.wYear);
	if(st.wYear < 100) {
		st.wYear += 2000;
	}

	DATE dDate = GetOleDateFromSystemtime(st);
	return dDate;
}

DATE GetOleDateFromTimeString(const char* str) {
	DATE dDate = GetOleDateFromHHMMSS(str, "%hd:%hd:%hd");
	return dDate;
}

DATE APIEXP GetOleDateFromDateTimeString(const char* str, const char *szFormat) {
	SYSTEMTIME st;
	memset(&st, 0, sizeof(st));
	sscanf(str, szFormat, &st.wYear, &st.wMonth, &st.wDay, &st.wHour, &st.wMinute, &st.wSecond);
	if(st.wYear < 100) {
		st.wYear += 2000;
	}

	DATE dDate = GetOleDateFromSystemtime(st);
	return dDate;
}

#endif // OLE_DATE_IMPL_H
#endif // OLE_DATE_IMPL

