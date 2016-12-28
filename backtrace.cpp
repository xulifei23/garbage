#define HBACKTRACE_EXPORT
#include "backtrace.h"
#include <string>
#include <sstream>
#include <iomanip>

using namespace std;

HBackTrace::HBackTrace(const char* file, size_t maxFrameNameLen, size_t numOfBuffers)
    :m_hLogFile(INVALID_HANDLE_VALUE)
    ,m_buffers(new void*[numOfBuffers])
    ,m_symbolInfo(reinterpret_cast<SYMBOL_INFO*>(calloc(sizeof(SYMBOL_INFO) + maxFrameNameLen, 1)))
    ,m_hProcess(NULL)
{
    if (file == nullptr)
    {
        return;
    }

    m_hLogFile = CreateFileA(file, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, 
        nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (m_hLogFile == INVALID_HANDLE_VALUE)
    {
        return;
    }

    m_symbolInfo->MaxNameLen = maxFrameNameLen - 1;
    m_symbolInfo->SizeOfStruct = sizeof(SYMBOL_INFO);

    m_hProcess = GetCurrentProcess();
    SymInitialize(m_hProcess, nullptr, true);
}

HBackTrace::~HBackTrace()
{
    CloseHandle(m_hLogFile);
    delete [] m_buffers;
    free(m_symbolInfo);

}

template <class T>
static typename::std::enable_if<std::is_arithmetic<T>::value, std::string>::type
    toString(T val, bool toHex)
{
    std::stringstream ss;
    if (toHex)
    {
        ss << setiosflags(ios::uppercase) << hex << val;
    }
    else
    {
        ss << val;
    }
    string s;
    ss >> s;
    return s;
}

unsigned short HBackTrace::capture(unsigned int frameCapture, unsigned int frameSkip) const 
{
    auto makeupString = [](SYMBOL_INFO* symbol, unsigned short index)->string
    {
        string symbolName = symbol->Name;
        if (symbolName.empty())
        {
            symbolName = "unknow symbol";
        }

        string frameStr = toString(index, false);
        frameStr += " ";
        frameStr += symbolName;
        frameStr += " 0x";
        frameStr += toString(symbol->Address, true);
        frameStr += "\n";

        return frameStr;
    };

    // skip + 1: make sure that skip over (this capture call) itself.
    unsigned short frames = CaptureStackBackTrace(frameSkip + 1, frameCapture, m_buffers, nullptr);

    unsigned long dummy = 0;
    for (unsigned short i = 0; i < frames; ++i)
    {
        SymFromAddr(m_hProcess, (DWORD64)m_buffers[i], nullptr, m_symbolInfo);
        string frameStr = makeupString(m_symbolInfo, frames - i - 1);
        WriteFile(m_hLogFile, frameStr.c_str(), frameStr.length(), &dummy, nullptr);
    }

    string newLine = "\n";
    WriteFile(m_hLogFile, newLine.c_str(), newLine.length(), &dummy, nullptr);

    return frames;
}

