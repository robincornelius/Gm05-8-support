
#define HANDLEGM int

void polldata (HANDLEGM hand);
unsigned char WritepacketToDevice(HANDLEGM hand, unsigned char cmd, unsigned char data);
BOOL FindTheHID(HANDLEGM hand);


