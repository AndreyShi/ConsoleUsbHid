#pragma once
#include <iostream>
#include <Windows.h>
#include <winusb.h>
#include <Setupapi.h>
#include <string> 
#include <hidsdi.h>
#include <string>
#include <string.h>

#define STM32_USBVID L"vid_0483"

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



/*������������� USB*/
void init();
/*��������� ���� ���������� USB*/
bool get_path();
/*�������� ������� ��� ������/������*/
void CreateReadWriteThread();

DWORD WINAPI ReadReport(void*);
DWORD WINAPI WriteReport(void*);