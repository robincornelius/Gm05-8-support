

#include <stdlib.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <asm/types.h>
#include <usb.h>

#include "gm0_private.h"
#include "gm0.h"

struct usb_bus *bus; 
struct usb_device *dev=NULL;
usb_dev_handle * thedev=NULL;


unsigned char WritepacketToDevice(HANDLEGM hand, unsigned char cmd, unsigned char data, unsigned char *statdata);


int writereport(unsigned char data)
{
	unsigned char rdata[8];
	rdata[0]=data;
	rdata[1]=0;
	rdata[2]=0;
	rdata[3]=0;
	rdata[4]=0;
	rdata[5]=0;
	rdata[6]=0;
	rdata[7]=0;

	int status;

	
	//printf("TX: %x:%x:%x:%x:%x:%x:%x:%x ",rdata[0],rdata[1],rdata[2],rdata[3],rdata[4],rdata[5],rdata[6],rdata[7]);


	status=usb_interrupt_write(thedev, 1, &rdata[0], 8,200);
	//status=usb_bulk_write(thedev, 0, &rdata[0], 8, 1000);

	if(status<0)
	{
		printf("TX ERROR %d ",status);
		exit(-1);
	}

	//printf("<- TX\n");

	return status;
}

unsigned char readreport()
{
	int status;
	unsigned char rdata[8];

	//printf("RX -> ");

	status=usb_interrupt_read(thedev, 1, &rdata[0], 8, 200);

	if(status<0)
	{
		printf(" RX ERROR %d ",status);
		
		if(status!=-110)
			exit(-1);
	}

	//printf("RX: %x:%x:%x:%x:%x:%x:%x:%x\n",rdata[0],rdata[1],rdata[2],rdata[3],rdata[4],rdata[5],rdata[6],rdata[7]);

	return rdata[0];
}

void polldata(HANDLEGM hand)
{

	double data;
	int tempmode;

	
	if(pGMS[hand]->m_Iportno>0)
		return;

	//WritepacketToDevice(hand,42, 42,NULL);

	data = GetReadingFromGM08(hand);

	pGMS[hand]->store.range = WritepacketToDevice(hand,48, 0,NULL)&0x07;

	pGMS[hand]->store.units = WritepacketToDevice(hand,47, 0,NULL)&0x03;

	tempmode=WritepacketToDevice(hand,46, 0,NULL);

	pGMS[hand]->store.mode = tempmode>4 || tempmode <0 ? 0 : tempmode;

	gm0_convertvalue(pGMS[hand]->store.range,pGMS[hand]->store.units,(float)data,&pGMS[hand]->store.value,TRUE);

}

int GetReadingFromGM08(HANDLEGM hand)
{

	unsigned char Reading[2];
	char * stop;
	signed __int16 intreading;
	unsigned char *p_uchar;

	Reading[0]=WritepacketToDevice(hand,49,0,&Reading[1]);

	intreading=0;	
	
	intreading=(256*Reading[1])+Reading[0];

	return ((int)intreading);

}

unsigned char WritepacketToDevice(HANDLEGM hand, unsigned char cmd, unsigned char data, unsigned char *statdata)
{

	unsigned char rdata,rdata2;

	writereport(cmd);
	rdata=readreport();
	writereport(data);
	rdata2=readreport();

	if(statdata!=NULL)
		*statdata=rdata2;


	return rdata;	
}



BOOL FindTheHID(HANDLEGM hand)
{
 
/* looking for

vendor 0x04d8 product 0x0002 version 0x0000 has 1 application and is on bus: 2 devnum: 3 ifnum: 0
 
*/

struct usb_bus *busses;
    
usb_init();
usb_find_busses();
usb_find_devices();
bus = usb_get_busses();

for (bus = usb_busses; bus; bus = bus->next) 
{ 
	for (dev = bus->devices; dev; dev = dev->next) 
	{ 
		if(dev->descriptor.idVendor==0x04d8 && dev->descriptor.idProduct==0x0002)
		{
			//printf("Found the gaussmeter\n");

			thedev = usb_open(dev);

			int stat=usb_detach_kernel_driver_np(thedev, 0);

			//printf("Releasestatus is %d\n",stat);

			usb_set_configuration(thedev, 3);

			int status;
			if((status=usb_claim_interface(thedev, 0))<0)
			{
				printf("Could not claim interface, status %d\n",status);
				exit(-1);
			}

			return TRUE;
		}
	}
}

    dev=NULL;
    printf("DID NOT FIND GAUSSMETER\n");
    return FALSE;
}

void closeUSB(HANDLEGM hand)
{

	usb_close(thedev);


}


