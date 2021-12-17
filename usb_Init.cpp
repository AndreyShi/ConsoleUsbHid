

#include "usbhid.h"


GUID Guid;
HANDLE HidDeviceObject;
HIDP_CAPS Capabilities;
OVERLAPPED	HIDOverlapped;
DWORD NumberOfBytesRead;
DWORD NumberOfBytesWriten;
wchar_t DevicePath[MAX_PATH];
HANDLE ReadThread;
DWORD ReadThreadId;
HANDLE WriteThread;
DWORD WriteThreadId;
bool WRITE_ROPRT;
BYTE InputReport[256];
BYTE OutputReport[256];
BYTE ProductStr[256];
int Use_Setxxx = 0;
/*
* const wchar_t* device - юникод строка с устройством (PID or VID)
*/
void init(const wchar_t* device) {

	if (get_path(device) == 0) {
		std::wcout << "device not found: " << DevicePath << '\n';
		return;
	}
	else
		std::wcout << "device: " << DevicePath << '\n';
		

	HidDeviceObject = CreateFile(
		(LPCTSTR)DevicePath,
		GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL, //&SecurityAttributes,  //no SECURITY_ATTRIBUTES structure
		OPEN_EXISTING,  //No special create flags
		FILE_FLAG_OVERLAPPED, // No special attributes
		NULL); // No template file

	if (HidDeviceObject != INVALID_HANDLE_VALUE)
	{
		PHIDD_ATTRIBUTES Attributes = (PHIDD_ATTRIBUTES)malloc(sizeof(HIDD_ATTRIBUTES));
		if (HidD_GetAttributes(HidDeviceObject, Attributes))
		{
			PHIDP_PREPARSED_DATA	PreparsedData;
			HidD_GetPreparsedData(HidDeviceObject, &PreparsedData);
			HidP_GetCaps(PreparsedData, &Capabilities);
			HidD_FreePreparsedData(PreparsedData);
		}
	}

	HidD_GetProductString(HidDeviceObject, &ProductStr, Capabilities.OutputReportByteLength);
	std::wcout << "ProductString: " << (wchar_t*)ProductStr << '\n';
	CreateReadWriteThread();
}
/*
* const wchar_t* device - юникод строка с устройством (PID or VID)
*/
bool get_path(const wchar_t* device)
{
	char	Product[253];
	std::string	Prod;
	bool result = 0;

	HDEVINFO info;
	HidD_GetHidGuid(&Guid);
	info = SetupDiGetClassDevs(&Guid, NULL, NULL, DIGCF_PRESENT | DIGCF_INTERFACEDEVICE);
	if (info != INVALID_HANDLE_VALUE)
	{
		DWORD devIndex;
		SP_INTERFACE_DEVICE_DATA ifData;
		ifData.cbSize = sizeof(ifData);

		for (devIndex = 0; SetupDiEnumDeviceInterfaces(info, NULL, &Guid, devIndex, &ifData); ++devIndex)
		{
			DWORD needed;
			SetupDiGetDeviceInterfaceDetail(info, &ifData, NULL, 0, &needed, NULL);

			PSP_INTERFACE_DEVICE_DETAIL_DATA detail = (PSP_INTERFACE_DEVICE_DETAIL_DATA)new BYTE[needed];
			detail->cbSize = sizeof(SP_INTERFACE_DEVICE_DETAIL_DATA);
			SP_DEVINFO_DATA did = { sizeof(SP_DEVINFO_DATA) };

			if (SetupDiGetDeviceInterfaceDetail(info, &ifData, detail, needed, NULL, &did))
			{
				if (wcsstr(detail->DevicePath, UNKNOWN_DEVICE) || wcsstr(detail->DevicePath, device))
				{
					memcpy(DevicePath, detail->DevicePath, MAX_PATH * 2);
					result = 1;
				}
			}

			if (wcsstr(detail->DevicePath, UNKNOWN_DEVICE) != NULL)
			{
				if (SetupDiGetDeviceRegistryProperty(info, &did, SPDRP_DEVICEDESC, NULL, (PBYTE)Product, 253, NULL))
					Prod = Product;
				else
					Prod = "(Unnamed HID device)";
			}
			delete[](PBYTE)detail;
		}
		SetupDiDestroyDeviceInfoList(info);
	}
	return result;
}

void CreateReadWriteThread() {

	if (ReadThread == NULL)
		ReadThread = CreateThread(NULL,0,ReadReport,NULL,0,&ReadThreadId);
	if (ReadThread == NULL)
		MessageBox(NULL,L"Error occured when creating Read thread (%d)\n",L"error",NULL);
	if (WriteThread == NULL)
		WriteThread = CreateThread(NULL,0,WriteReport,NULL,0,&WriteThreadId);
	if (WriteThread == NULL)
		MessageBox(NULL, L"Error occured when creating Write thread (%d)\n", L"error", NULL);
}
