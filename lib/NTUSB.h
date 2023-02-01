#define HANDLEGM int

void PrepareForOverlappedTransfer(HANDLEGM hand);
BOOL FindTheHID(HANDLEGM hand);
void GetDeviceCapabilities(HANDLEGM hand);
unsigned char WritepacketToDevice(HANDLEGM hand, unsigned char command, unsigned char data, char* status);
DWORD WriteReport(HANDLEGM hand, char* Report);
DWORD ReadReport(HANDLEGM hand, char* Report);
int GetReadingFromGM08(HANDLEGM hand);
void polldata(HANDLEGM hand);
void closeUSB(HANDLEGM hand);
void safeclosehandle(HANDLE* hand);
