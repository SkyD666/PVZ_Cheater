#include "MemoryUtil.h"

MemoryUtil::MemoryUtil()
{

}

HANDLE MemoryUtil::getProcessHandle()
{
    HWND hWindow = FindWindow(L"MainWindow", L"植物大战僵尸中文版");   //获取窗口句柄
    DWORD processId;
    GetWindowThreadProcessId(hWindow, &processId);            //获取进程id
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);    //打开进程
    return hProcess;
}
