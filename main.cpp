#include "usbhid.h"

#pragma comment(lib, "hid.lib")
#pragma comment(lib, "setupapi.lib")
#pragma comment(lib, "winusb.lib")

int main() {	
    int t;
    init();
    n:
    std::cin >> t; 
    OutputReport[4] = t;
    WRITE_ROPRT = true;   
    goto n;

    return 0;
}
