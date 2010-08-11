/*
	gm0.dll/gm0.so shared gaussmeter function driver library for Hirst Gaussmeters
    Copyright © 2004-2008  Robin Cornelius/Hirst Magnetic Instruments Ltd

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.
   
    You  should  have  received  a  copy of the GNU General Public
    License  along  with  this  program; if not, write to the Free
    Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,
    MA 02111-1307 USA
*/

#include "gm0.h"
#include "comms.h"

#ifdef _WINDOWS
#include <windows.h>
#include <wtypes.h>
#include <initguid.h>
#include <setupapi.h>
#include <stdio.h>
#include <time.h>
#include "hidsdi.h"
#endif

#ifdef _LINUX
	#include <usb.h>
#endif

unsigned __int16 probee2buf[255];
unsigned __int16 gme2buf[255];

BOOL checkhand(int hand);

void debugprint(char * msg);
char AMpacket(HANDLEGM hand,char cmd,char data);
int gm0_convertvalue(int range,int units,float value,float * newvalue, BOOL isusb);
void filtercallbackdata(struct gm_store * pdata);

#define BUFFER_OWNER_INSTRUMENT 0x1
#define BUFFER_OWNER_USER 0x2
#define MAX_BUFFER 50

struct GM_STRUCT{


#ifdef _LINUX
	// LINUX USB HANDLES and friends
	usb_dev_handle * theusbdev;
	//****
#endif

	int	 meter_mode; //gm05=0 gm08=1
	BOOL disablepoll;
	BOOL polldisabled;
	BOOL faultyfirmware;
	BOOL fastUSBcapture;
	BOOL needrangepoll;
	BOOL sampleondemand;
	
	RS232_INFO com;

	int m_Iportno;

	BOOL port_open;

	BOOL gm0_threadrun;

	BOOL gm0_usereadthread;

	struct gm_store store;

	long samplecount;

	unsigned int index;

	unsigned int *pIncomming;

	char * pvalue,*punits,*pmode,*prange, *ptime_day,*ptime_month,*ptime_year,*ptime_sec,*ptime_min,*ptime_hour;

	void (__stdcall * pCallback)(int handle,struct gm_store);
	void (__stdcall * pCallback2)(int handle);

	void (__stdcall * pConnectCallback)(void);

	void (__stdcall * pDisConnectCallback)(HANDLEGM hand);

	void (__stdcall * pNullCallback)(HANDLEGM hand);

	struct gm_time time;

	BOOL connected;
	BOOL USBconnected;

	BOOL connecting;

	unsigned char cmdstatus;

	BOOL doingnull;

	BOOL shutdownstarted;

	int threadlockcount;

	BOOL datasignal;

	// the buffer will store in ring buffer format 
	struct gm_store store_buffer[MAX_BUFFER]; // a crude ring buffer
	int buffer_ringflag[MAX_BUFFER]; // a crude ring buffer
	int buffer_start;
	int buffer_end;
	int samples_avaiable;
	int buffer_enabled;

#ifdef _WINDOWS
	HIDP_CAPS		Capabilities;
	PSP_DEVICE_INTERFACE_DETAIL_DATA	detailData;
	HANDLE			DeviceHandle;
	DWORD			dwError;
	HANDLE			hEventObject;
	HANDLE			hDevInfo;
	GUID			HidGuid;
	OVERLAPPED		HIDOverlapped;
	ULONG			Length;
	LPOVERLAPPED	lpOverLap;
	DWORD			NumberOfBytesRead;
	HANDLE			ReadHandle;
	ULONG			Required;

#endif


}; 





FILE * logfile;

struct GM_STRUCT *pGMS[255];

#define true TRUE

#define false FALSE



void processgmcomms();

void __cdecl readthread(void *);
void __cdecl pollthread(void * pParam);

int gm0_gmmode1(int hand);

int gm0_gmstar(HANDLEGM hand);

void __cdecl connectthread(void * pParam);

void commerror(HANDLEGM hand);

char packetbyte(HANDLEGM hand,char datatosend);

int gm0_dothestar(HANDLEGM hand);





/* Global variables */

/*END OF GLOBALS */



#define GMC_NULL 0

#define GMC_MODE1 1

#define GMC_SETADDLO 2

#define GMC_SETADDHI 3

#define GMC_RDBYTE 4

#define GMC_WRBYTE 5

#define GMC_RDE2 6

#define GMC_WRE2 7

#define GMC_SETDATALO 8

#define GMC_SETDATAHI 9

#define GMC_GETDATALO 10

#define GMC_GETDATAHI 11

#define GMC_RANGE 12

#define GMC_PADDISABLE 13

#define GMC_SIMKEY 14

#define GMC_INCADDR 15

#define GMC_RTNDISP 16

#define GMC_LOBAT 17

#define GMC_LOCKMODE 18

#define GMC_UNITS 19

#define GMC_FUNCTION 20

#define GMC_LANGUAGE 21

#define GMC_RESPEAK 22

#define GMC_DONULL 23

#define GMC_DOAZ 24

#define GMC_APD 25

#define GMC_TIME0 26

#define GMC_TIME1 27

#define GMC_TIME2 28

#define GMC_TIME3 29

#define GMC_TIME4 30

#define GMC_TIME5 31

#define GMC_TIME6 32

#define GMC_TIME7 33

#define GMC_TIME 34

#define GMC_SAMPLENOW 35

#define GMC_COMMFLAG 36

#define GMC_GETREG 37

#define GMC_REGPTR 38

#define GMC_SAVESETUP 39

#define GMC_INTERVAL 40





//PlugE2 register assignments

#define rProbeType          0       //Probe type register

#define rCalibDate          1       //Last calibration date Month|Year Binary

#define rSerialNo           2       //Probe serial number





//GM04E2 register assignments

#define rGM04SerNo          0       //GM04 serial number




