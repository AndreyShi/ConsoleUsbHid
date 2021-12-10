#pragma once
#include <iostream>
#include <Windows.h>
#include <winusb.h>
#include <Setupapi.h>
#include <string> 
#include <hidsdi.h>
#include <string>
#include <string.h>

#define UNKNOWN_DEVICE    L"vid_0483"
#define STM32_USBVID      L"vid_03eb"

extern GUID Guid;
extern HANDLE HidDeviceObject;
extern HIDP_CAPS Capabilities;
extern OVERLAPPED	HIDOverlapped;
extern DWORD NumberOfBytesRead;
extern DWORD NumberOfBytesWriten;
extern wchar_t DevicePath[MAX_PATH];
extern HANDLE ReadThread;
extern DWORD ReadThreadId;
extern HANDLE WriteThread;
extern DWORD WriteThreadId;

extern bool WRITE_ROPRT;
extern BYTE InputReport[256];
extern BYTE OutputReport[256];
extern int Use_Setxxx;



/*инициализация USB*/
void init();
/*получение пути устройства USB*/
bool get_path();
/*создание потоков для чтения/записи*/
void CreateReadWriteThread();

DWORD WINAPI ReadReport(void*);
DWORD WINAPI WriteReport(void*);