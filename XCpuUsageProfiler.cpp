#include "XCpuUsageProfiler.h"

static std::map<string, XPrinterFactory*> g_printerFactoryMap;

// RegisterPrinter
RegisterPrinter(XPrinterConsole);
RegisterPrinter(XPrinterFile);

XPrinter* GetPrinterByName(const string& strName)
{
	if (g_printerFactoryMap.find(strName) != g_printerFactoryMap.end())
	{
		return g_printerFactoryMap[strName]->GetPrinter();
	}
	
	return NULL;
}

XPrinterConsole::XPrinterConsole()
	:XPrinter()
	,m_hConsole(NULL)
{
	Init();
}
	
XPrinterConsole:: ~XPrinterConsole()
{
	FreeConsole();
}

void XPrinterConsole::Print(const string& msg)
{
	string strMsg = msg + "\n";
	unsigned long ulWriteNum = 0;
	if (WriteConsole(m_hConsole, strMsg.c_str(), strMsg.length(), &ulWriteNum, NULL) == FALSE)
	{
		assert(false);
	}
}
	
void XPrinterConsole::SetPath(const string& path)
{
	// nothing to set
	return;
}
	
void XPrinterConsole::Init()
{
	AllocConsole();
	m_hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	assert(m_hConsole != INVALID_HANDLE_VALUE);
	
	// some parent process would not like this child to use console
	if (m_hConsole == NULL)
	{
		m_hConsole = QueryConsoleHandle();
		if (m_hConsole == NULL)
		{
			assert(false);
		}
	}
	
	SetConsoleTextAttribute(m_hConsole, FOREGROUND_RED | BACKGROUND_RED | BACKGROUND_GREEN | 
		BACKGROUND_BLUE | BACKGROUND_INTENSITY);
}
	
// test console handle
HANDLE XPrinterConsole::QueryConsoleHandle()
{
	char pBuff[3] = "\n";
	DWORD writeBytes = 0;
	for (int i = 0; i < 65535; ++i)
	{			
		if (WriteConsole((HANDLE)i, pBuff, 1, &writeBytes, NULL) != FALSE)
		{
			return (HANDLE)i;
		}
	}
	
	return NULL;
}

XPrinterFile::XPrinterFile()
	:XPrinter()
	,m_strFilePath("")
	,m_hFile(INVALID_HANDLE_VALUE)
{
	
}
XPrinterFile::~XPrinterFile()
{
	CloseHandle(m_hFile);
}
	
void XPrinterFile::SetPath(const string& path)
{
	m_strFilePath = path;
	assert(CreateFile() == true);
}
	
void XPrinterFile::Print(const string& msg)
{
	string strMsg = msg + "\r\n";
	unsigned long ulWriteNum = 0;
	if (WriteFile(m_hFile, strMsg.c_str(), strMsg.length(), &ulWriteNum, NULL) == FALSE)
	{
		assert(false);
	}
}
	
bool XPrinterFile::CreateFile()
{
	if (m_strFilePath.empty())
	{
		return false;
	}
	
	m_hFile = ::CreateFile(m_strFilePath.c_str(), GENERIC_WRITE | GENERIC_READ, FILE_SHARE_READ, NULL, 
		CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (m_hFile == INVALID_HANDLE_VALUE)
	{
		return false;
	}
	
	return true;
}

// write thread load
void XCpuUsageProfiler::WriteLoad(const string& strLabel, int codeLine)
{
	double elapseTime = 0.0;
	double sysElapseTime = 0.0;
	string strTime = "";
	double processUsage = GetProcessCpuUsage(elapseTime, sysElapseTime, strTime);
	double threadUsage = GetCpuUsage(processUsage);
	m_ostr << strLabel << " " << codeLine << "\r\n" << "ProcessTotal : " << processUsage << 
		" Thread : "<< threadUsage << "\r\n" << "system time : " << sysElapseTime << 
		" process time : " << elapseTime << "\r\n" << strTime << "\r\n"; 
	m_pPrinter->Print(m_ostr.str());
	m_ostr.str("");
}

// write some debug msg
void XCpuUsageProfiler::WriteMsg(const string& msg)
{
	m_ostr.str("");
	m_ostr << msg << "\r\n" << GetCurrentTimeStr() << "\r\n";
	m_pPrinter->Print(m_ostr.str());
	m_ostr.str("");
}

string XCpuUsageProfiler::GetCurrentTimeStr()
{
	SYSTEMTIME sysTime;
	GetLocalTime(&sysTime);
	m_ostr.str("");
	m_ostr << sysTime.wYear << '-' << sysTime.wMonth << '-' << sysTime.wDay << ' ' << 
		sysTime.wHour << ':' << sysTime.wMinute << ':' << sysTime.wSecond << '.' << sysTime.wMilliseconds;
	string strTime = m_ostr.str();
	m_ostr.str("");
	return strTime;
}

// return elapse time in seconds
int XCpuUsageProfiler::GetTimeElapse()
{
	DWORD tickCount = GetTickCount();
	int timeElapse = tickCount - m_lastTickCount;
	m_lastTickCount = tickCount;
	
	return timeElapse;
}

// return cpu cores 
unsigned short XCpuUsageProfiler::GetCpuCores()
{
	SYSTEM_INFO sysInfo = {0};
	GetSystemInfo(&sysInfo);

	return sysInfo.dwNumberOfProcessors;
}

// SubstractFILETIME
__int64 XCpuUsageProfiler::SubstractFILETIME(const FILETIME& oldFileTime, const FILETIME& newFileTime)
{
	__int64 a = oldFileTime.dwHighDateTime;
	a = a << 32;
	a += oldFileTime.dwLowDateTime ;
	
	__int64 b = newFileTime.dwHighDateTime;
	b = b << 32;
	b += newFileTime.dwLowDateTime;
	
   return   (b - a);
}

// return process cpu usage
double XCpuUsageProfiler::GetProcessCpuUsage(double& elapseTime, double& sysElapseTime, string& strTime)
{
	FILETIME kernelTime;
	FILETIME userTime;
	GetProcessTimes(m_hCurrentProc, &m_lastDummyTime, &m_lastDummyTime, 
		&kernelTime, &userTime);

	__int64 elapseKernelTime = SubstractFILETIME(m_lastKernelTime, kernelTime);
	__int64 elapseUserTime = SubstractFILETIME(m_lastUserTime, userTime);
	elapseTime = (elapseKernelTime + elapseUserTime) / 10000;

	DWORD nowTick = GetTickCount();
	strTime = GetCurrentTimeStr();
	sysElapseTime = nowTick - m_lastTickCount;
	if (sysElapseTime == 0.0)
	{
		sysElapseTime = 15.0;
	}

	double usage = (elapseTime / sysElapseTime)  * 100.0 / m_cpuCores;
	m_lastKernelTime = kernelTime;
	m_lastUserTime = userTime;
	m_lastTickCount = nowTick;

	return usage;
}

// return thread cpu usage between elapse time
double XCpuUsageProfiler::GetCpuUsage(double processUsage)
{
	ULONG64 curTimeslotProcess = 0;
	ULONG64 curTimeslotThread = 0;

	m_pQueryProcessCycle(m_hCurrentProc, &curTimeslotProcess);
	m_pQueryThreadCycle(m_hCurrentThread, &curTimeslotThread);

	double timeslotProcess = (double)(__int64)(curTimeslotProcess - m_lastTimeSlotProcess);
	double timeslotThread = (double)(__int64)(curTimeslotThread - m_lastTimeSlotThread);

	return (timeslotThread / timeslotProcess) * processUsage;
}

// ctor
XCpuUsageProfiler::XCpuUsageProfiler(const string& strPrinter, const string& strPath)
	:m_pPrinter(GetPrinterByName(strPrinter))
{
	assert(m_pPrinter != NULL);
	m_pPrinter->SetPath(strPath);
	m_ostr.str("");
	m_cpuCores = GetCpuCores();
	m_hCurrentProc = GetCurrentProcess();
	m_hCurrentThread = GetCurrentThread();

	m_pQueryProcessCycle = (QUERYCYCLETIMES)GetProcAddress(GetModuleHandle("kernel32.dll"), "QueryProcessCycleTime");
	m_pQueryThreadCycle = (QUERYCYCLETIMES)GetProcAddress(GetModuleHandle("kernel32.dll"), "QueryThreadCycleTime");
	assert(m_pQueryProcessCycle != NULL);
	assert(m_pQueryThreadCycle != NULL);

	if (m_pQueryProcessCycle(m_hCurrentProc, &m_lastTimeSlotProcess) == FALSE)
	{
		assert(false);
	}

	if (m_pQueryThreadCycle(m_hCurrentThread, &m_lastTimeSlotThread) == FALSE)
	{
		assert(false);
	}

	m_lastTickCount = GetTickCount();
	GetProcessTimes(m_hCurrentProc, &m_lastDummyTime, &m_lastDummyTime, 
		&m_lastKernelTime, &m_lastUserTime);
}

// destor
XCpuUsageProfiler::~XCpuUsageProfiler()
{
	FREE_POINTER(m_pPrinter);
}



