/*
	gm0.dll/gm0.so shared gaussmeter function driver library for Hirst Gaussmeters
    Copyright (C) 2004  Robin Cornelius/Hirst Magnetic Instruments Ltd

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include "gm0.h"
#include "comms.h"

unsigned __int16 probee2buf[255];
unsigned __int16 gme2buf[255];

BOOL checkhand(int hand);

void debugprint(char * msg);
char AMpacket(HANDLEGM hand,char cmd,char data);


struct GM_STRUCT{

	BOOL disablepoll;
	BOOL polldisabled;
	BOOL faultyfirmware;
	
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




