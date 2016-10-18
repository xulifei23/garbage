#ifndef HBACKTRACE_H
#define HBACKTRACE_H

#ifdef HBACKTRACE_EXPORT
#define HBACKTRACE_CLASS __declspec(dllexport)
#else
#define HBACKTRACE_CLASS __declspec(dllimport)
#endif // HBACKTRACE_EXPORT

#include <windows.h>
#include <DbgHelp.h>

class HBACKTRACE_CLASS HBackTrace
{
public:  
    HBackTrace(const char* file, size_t maxFrameNameLen, size_t numOfBuffers = 20);
    ~HBackTrace();

public:
    unsigned short capture(unsigned int frameCapture, unsigned int frameSkip) const;

private:
    HANDLE m_hLogFile;
    HANDLE m_hProcess;
    mutable void** m_buffers;
    SYMBOL_INFO* m_symbolInfo;
};

#endif // HBACKTRACE_H