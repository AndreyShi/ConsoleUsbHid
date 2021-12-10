#include "usbhid.h"

DWORD WINAPI ReadReport(void*) {

	while (TRUE)
	{
		if (!WRITE_ROPRT)
		{
			if (HidDeviceObject != INVALID_HANDLE_VALUE)
			{
				CancelIo(HidDeviceObject);
				int Result = ReadFile(HidDeviceObject,&InputReport,Capabilities.InputReportByteLength,
					&NumberOfBytesRead,(LPOVERLAPPED)&HIDOverlapped);
			}
		}
		Sleep(25);
	}

}
DWORD WINAPI WriteReport(void*) {
	BOOL Result = TRUE;
	int n;
	while (TRUE)
	{
		if (WRITE_ROPRT)
		{
			if (HidDeviceObject != INVALID_HANDLE_VALUE)
			{
				CancelIo(HidDeviceObject);
				if (Use_Setxxx == 1){
					Result = HidD_SetFeature(HidDeviceObject,OutputReport,Capabilities.FeatureReportByteLength);
				}
				else{
					Result = WriteFile(HidDeviceObject,&OutputReport,Capabilities.OutputReportByteLength,
						&NumberOfBytesWriten,(LPOVERLAPPED)&HIDOverlapped);
				}
				WRITE_ROPRT = FALSE;
				memset(&OutputReport, 0, 256);
			}
		}
		Sleep(25);
	}
}