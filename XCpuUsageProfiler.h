#ifndef XCpuUsageProfiler_H
#define XCpuUsageProfiler_H

/********************************************
*
*	usage:
*	
*	xProfilerInit(XPrinterConsole, "");  // XPrinterConsole ：输出到控制台，路径随意
*												 // XPrinterFile ：输出到文件，给个文件全路径
*	somecode....
*
*	xProfilerCheck("Label1");     // 从 loadInit 到 Label1 之间代码 CPU使用率
*
*	somecode...
*
*	xProfilerCheck("Label2");		// 从 Label1 到 Label2 之间代码 CPU使用率
*
*********************************************/
#include <windows.h>
#include <sstream>
#include <map>
#include "XSmartPtr.hpp"

#define xProfilerInit(printerName, path) \
	XSMARTPTR::XSharedPtr<XCpuUsageProfiler> profiler(new XCpuUsageProfiler(printerName, path))

#define xProfilerCheck(label) \
	profiler->WriteLoad(label, __LINE__)

#define xProfilerMsg(msg) \
	profiler->WriteMsg(msg)

#ifndef FREE_POINTER
#define FREE_POINTER(p) \
	if (p != NULL) { delete p; p = NULL; }
#endif // FREE_POINTER

#ifndef PURE
#define PURE = 0
#endif // PURE

typedef BOOL (WINAPI *QUERYCYCLETIMES)(HANDLE, PULONG64);
typedef BOOL (WINAPI *GETSYSTEMTIMES)(LPFILETIME, LPFILETIME, LPFILETIME);

// printer interface class
class XPrinter
{
public:

	XPrinter(){}
	virtual ~XPrinter(){}
	
public:

	virtual void Print(const string& msg) PURE;
	virtual void SetPath(const string& path) PURE;
};

class XPrinterFactory
{
public:
	virtual XPrinter* GetPrinter() PURE;
};

#define RegisterPrinter(name) \
class XPrinterFactory##name : public XPrinterFactory \
{ \
public: \
	XPrinterFactory##name() \
	{\
		g_printerFactoryMap.insert(make_pair(string(#name), this)); \
	}\
	virtual XPrinter* GetPrinter() \
	{\
		return new name(); \
	}\
}; \
static XPrinterFactory##name factory##name;

XPrinter* GetPrinterByName(const string& strName);

// output to console class
class XPrinterConsole : public XPrinter
{
public:

	XPrinterConsole();
	virtual ~XPrinterConsole();
	
public:

	virtual void Print(const string& msg);
	virtual void SetPath(const string& path);

protected:

	void Init();
	HANDLE QueryConsoleHandle();
	
private:
	HANDLE m_hConsole;
};

class XPrinterFile : public XPrinter
{
public:

	XPrinterFile();
	virtual ~XPrinterFile();

public:

	virtual void SetPath(const string& path);
	virtual void Print(const string& msg);

protected:

	bool CreateFile();

private:
	string m_strFilePath;
	HANDLE m_hFile;
};

class XCpuUsageProfiler
{
public:

	// write thread load
	void WriteLoad(const string& strLabel, int codeLine);
	// write some debug msg
	void WriteMsg(const string& msg);
	string GetCurrentTimeStr();
	// return elapse time in seconds
	int GetTimeElapse();
	// return cpu cores 
	unsigned short GetCpuCores();
	// SubstractFILETIME
	__int64 SubstractFILETIME(const FILETIME& oldFileTime, const FILETIME& newFileTime);
	// return process cpu usage
	double GetProcessCpuUsage(double& elapseTime, double& sysElapseTime, string& strTime);
	// return thread cpu usage between elapse time
	double GetCpuUsage(double processUsage);

public:

	// ctor
	XCpuUsageProfiler(const string& strPrinter, const string& strPath);
	// destor
	~XCpuUsageProfiler();

private:

	XPrinter* m_pPrinter;
	unsigned short m_cpuCores;
	ostringstream m_ostr;
	HANDLE m_hCurrentProc;
	HANDLE m_hCurrentThread;
	DWORD m_lastTickCount;

	QUERYCYCLETIMES m_pQueryThreadCycle;
	QUERYCYCLETIMES m_pQueryProcessCycle;
	ULONG64 m_lastTimeSlotProcess;
	ULONG64 m_lastTimeSlotThread;

	FILETIME m_lastDummyTime;
	FILETIME m_lastKernelTime;
	FILETIME m_lastUserTime;
};



#endif // XCpuUsageProfiler_H