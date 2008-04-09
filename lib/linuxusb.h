
#include "gm0.h"

void polldata (HANDLEGM hand);

unsigned char WritepacketToDevice(HANDLEGM hand, unsigned char cmd, unsigned char data,char * status);

BOOL FindTheHID(HANDLEGM hand);
void closeUSB(HANDLEGM hand);


