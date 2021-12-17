#include "usbhid.h"

#pragma comment(lib, "hid.lib")
#pragma comment(lib, "setupapi.lib")
#pragma comment(lib, "winusb.lib")

int main() {	
    int index;
    int data;
    init(STM32_USBVID);
n:
    std::cout << "index:";
    std::cin >> index;
    if (index == 0) {
        std::cout << "reading input buffer...\n";
        char byte[4] = {'0','0','0','\0'};
        for (int i = 0; i < 20; i++) {
            sprintf_s(byte, "%x", InputReport[i]);
            std::cout << byte << '_';
        }
        std::cout << "\n";
goto n;
    }
    std::cout << "data:";
    std::cin >> data;

    if (index >= 1 && index <= 255) {
        OutputReport[index] = data;
        WRITE_ROPRT = true;
        std::cout << "sending...";
        while (WRITE_ROPRT);
        std::cout << "OK\n";
    }
    else
        std::cout << "wrong index\n";
      
goto n;
    CloseHandle(HidDeviceObject);
    return 0;
}
