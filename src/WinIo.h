#ifndef WINIO_H
#define WINIO_H

extern "C"
{
  _declspec(dllimport) bool _stdcall InitializeWinIo();
  _declspec(dllimport) void _stdcall ShutdownWinIo();
  _declspec(dllimport) PBYTE _stdcall MapPhysToLin(PBYTE pbPhysAddr, DWORD dwPhysSize, HANDLE *pPhysicalMemoryHandle);
  _declspec(dllimport) bool _stdcall UnmapPhysicalMemory(HANDLE PhysicalMemoryHandle, PBYTE pbLinAddr);
  _declspec(dllimport) bool _stdcall GetPhysLong(PBYTE pbPhysAddr, PDWORD pdwPhysVal);
  _declspec(dllimport) bool _stdcall SetPhysLong(PBYTE pbPhysAddr, DWORD dwPhysVal);
  _declspec(dllimport) bool _stdcall GetPortVal(WORD wPortAddr, PDWORD pdwPortVal, BYTE bSize);
  _declspec(dllimport) bool _stdcall SetPortVal(WORD wPortAddr, DWORD dwPortVal, BYTE bSize);
  _declspec(dllimport) bool _stdcall InstallWinIoDriver(PSTR pszWinIoDriverPath, bool IsDemandLoaded = false);
  _declspec(dllimport) bool _stdcall RemoveWinIoDriver();
}

extern bool IsNT;
extern HANDLE hDriver;
extern bool IsWinIoInitialized;

bool _stdcall StartWinIoDriver();
bool _stdcall StopWinIoDriver();

#endif
