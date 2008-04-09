/*
   Copyright © 2004-2008 Hirst Magnetic Instruments Ltd and Robin Cornelius.

   The GNU GENERAL PUBLIC LICENSE Version 2, June 1991

   This documentation is part of GM0

   GM0  is  free  software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation; either version 2 of
   the License, or (at your option) any later version.

   This  program  is  distributed  in  the  hope  that it will be
   useful,  but  WITHOUT  ANY  WARRANTY; without even the implied
   warranty  of  MERCHANTABILITY  or  FITNESS  FOR  A  PARTICULAR
   PURPOSE. See the GNU General Public License for more details.

   You  should  have  received  a  copy of the GNU General Public
   License  along  with  this  program; if not, write to the Free
   Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,
   MA 02111-1307 USA

*/

#ifndef _LINUX
	#include <objbase.h>
	#include <process.h>
	#include "windows.h"
	#include <locale.h>
	#include "ntusb.h"
#else
	#include "linuxusb.h"
	#include <pthread.h>
	#include "malloc.h"
	#include <stdlib.h>

static inline void Beep(int frequency, int duration)
{
}

#endif


#include <stdio.h>
#include <time.h>
#include <math.h>
#include "gm0_private.h"
#include "gm0.h"


int gm0_openport232(HANDLEGM hand,int mode);
int gm0_openportUSB(HANDLEGM hand);

int processcount=0;
int handcount;

BOOL APIENTRY DllMain( HANDLE hModule, 
                    DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
			break;
		case DLL_THREAD_ATTACH:
			break;
		case DLL_THREAD_DETACH:
			break;
		case DLL_PROCESS_DETACH:
			for(handcount=0;handcount<255;handcount++)
			{
				if(pGMS[handcount]!=NULL)
				{
					gm0_killgm(handcount);
				}
			}

			break;
    }
    return TRUE;
}

int gm0_openport(HANDLEGM hand,int mode)
{

	#ifndef _LINUX
		/* Set the locale for this instance */
		setlocale( LC_ALL , "English" );
	#else
		/* POSIX warning */
		/* There may be a locale problem running on non english systems that use , for 
		decimal point, this was fixed on windows with above setlocale() code */
	#endif

	if(pGMS[hand]->m_Iportno>=0)
		gm0_openport232(hand,mode);

	if(pGMS[hand]->m_Iportno<0)
		gm0_openportUSB(hand);

	return 0;
}


int gm0_openport232(HANDLEGM hand,int mode)
{	
	char * pportstring; 

	
	pportstring=(char *)malloc(sizeof(char)*5);
	if(pportstring==NULL)
		return GM_MEMORY_ERROR;

#ifndef _LINUX
	sprintf(pportstring,"COM%d",pGMS[hand]->m_Iportno);
#else
	sprintf(pportstring,"/dev/ttyS%d",pGMS[hand]->m_Iportno);
#endif

	if(mode==0)
	{
		pGMS[hand]->com.fd=rs232_open(pportstring,4800,'N',8,1,0);
	}
	if(mode==1)
	{
		pGMS[hand]->com.fd=rs232_open(pportstring,9600,'N',8,1,0);
	}

	pGMS[hand]->port_open=TRUE;

	free(pportstring); 

	return 0;

}

int gm0_openportUSB(HANDLEGM hand)
{
	
	//FindTheHID(hand);

	return 0;
}


int gm0_closeport(HANDLEGM hand)
{

	char * pportstring; 


	if(pGMS[hand]->port_open!=TRUE)
		return 0; // NOTHING TO CLOSE

	pportstring=(char *)malloc(sizeof(char)*5);
	if(pportstring==NULL)
		return GM_MEMORY_ERROR;

	sprintf(pportstring,"COM%d",pGMS[hand]->m_Iportno);

	if (pGMS[hand]->port_open==TRUE)
	{
		rs232_exit(&pGMS[hand]->com);
	}

	free(pportstring);	

	return 0;
}


void __cdecl pollthread(void * pParam)
{

	HANDLEGM hand;

	#ifndef _LINUX
		CoInitializeEx(NULL,COINIT_MULTITHREADED);
	#endif

	hand = (int)pParam;

	pGMS[hand]->threadlockcount++;

	while(pGMS[hand]->gm0_threadrun==TRUE)
	{

		if(pGMS[hand]->disablepoll==FALSE)
		{
			pGMS[hand]->polldisabled=FALSE;
			polldata(hand);
			// signal data has been collected
			pGMS[hand]->datasignal=TRUE;
	
			filtercallbackdata(&pGMS[hand]->store);

			if(pGMS[hand]->pCallback!=NULL && pGMS[hand]->gm0_threadrun==TRUE)
			{
				pGMS[hand]->pCallback(hand,pGMS[hand]->store);
			}

			if(pGMS[hand]->pCallback2!=NULL && pGMS[hand]->gm0_threadrun==TRUE)
			{
				pGMS[hand]->pCallback2(hand);
			}

		}
		else
		{
			pGMS[hand]->polldisabled=TRUE;	
			//OutputDebugString("Poll thread skipping beat\n");
		}


		Sleep(10); //333
	}

	pGMS[hand]->threadlockcount--;	

	#ifndef _LINUX
		CoUninitialize();
	#endif

}


void __cdecl readthread(void * pParam)
{

	HANDLEGM hand;
	unsigned long length;
	static int readthreadstarted=FALSE;
	
#ifndef _LINUX
	CoInitializeEx(NULL,COINIT_MULTITHREADED);
#endif
	
	if (readthreadstarted==TRUE)
		return;

	readthreadstarted=TRUE;

	hand = (int)pParam;

	pGMS[hand]->threadlockcount++;

	while(pGMS[hand]->gm0_threadrun==TRUE)
	{
		char data;

		if(pGMS[hand]->gm0_usereadthread==TRUE)
		{
		length = rs232_read(&pGMS[hand]->com,&data,1,10);

			if(length!=-1) //read 1 char or timeout after 250ms
			{
				*(pGMS[hand]->pIncomming + pGMS[hand]->index)=data; // store data in buffer
				//sprintf(buffer,"RX %d\n\0",data);
				//OutputDebugString(buffer);
				pGMS[hand]->index++;
				if(pGMS[hand]->index>=12 /*&& *(pGMS[hand]->pIncomming+12)==13*/)
				processgmcomms(hand);
			}
		}
		Sleep(1); //yield
	}
	
	if(pGMS[hand]!=NULL)
		pGMS[hand]->threadlockcount--;

	readthreadstarted=FALSE;


#ifndef _LINUX
	CoUninitialize();
#endif

}

char packetbyte(HANDLEGM hand, char dataout)
{
	char retdata;
	int length;
	int oldindex;
	int target;

	oldindex=pGMS[hand]->index;
	target=oldindex+1;

	rs232_write(&pGMS[hand]->com, &dataout, 1);

	length = rs232_read(&pGMS[hand]->com, &retdata,1,500);

	if (length != 1)
	{
		//printf("timeout recieve\n");
	}

	return(retdata);
}


char AMpacket(HANDLEGM hand,char cmd,char data)
{

	unsigned char ret;

	if(pGMS[hand]->m_Iportno>=0)
	{
		ret=packetbyte(hand,cmd);
		pGMS[hand]->cmdstatus=packetbyte(hand,data);
	}
	else
	{
		ret=WritepacketToDevice(hand,cmd,data,NULL);
		//pGMS[hand]->disablepoll=FALSE;
	}

	return ret;
}


void processgmcomms(HANDLEGM hand)
{

	int offset;
	char *stop;
	double value;
	char msg[1024];

	Beep(2000,20);

	if(pGMS[hand]->gm0_threadrun==FALSE)
		return;

	offset=0;
	if (pGMS[hand]->index==12)
		offset=0;
	if (pGMS[hand]->index==31)
		offset=19;

	if(*(pGMS[hand]->pIncomming+(pGMS[hand]->index-1))!=10)
	{
		return;
	}
	

	// messy string movement
	*(pGMS[hand]->pvalue)=*(pGMS[hand]->pIncomming+(pGMS[hand]->index-(12+offset))+0);
	*(pGMS[hand]->pvalue+1)=*(pGMS[hand]->pIncomming+(pGMS[hand]->index-(12+offset))+1);
	*(pGMS[hand]->pvalue+2)=*(pGMS[hand]->pIncomming+(pGMS[hand]->index-(12+offset))+2);
	*(pGMS[hand]->pvalue+3)=*(pGMS[hand]->pIncomming+(pGMS[hand]->index-(12+offset))+3);
	*(pGMS[hand]->pvalue+4)=*(pGMS[hand]->pIncomming+(pGMS[hand]->index-(12+offset))+4);
	*(pGMS[hand]->pvalue+5)=*(pGMS[hand]->pIncomming+(pGMS[hand]->index-(12+offset))+5);
	*(pGMS[hand]->pvalue+6)=*(pGMS[hand]->pIncomming+(pGMS[hand]->index-(12+offset))+6);
	*(pGMS[hand]->pvalue+7)=0;
	

	*(pGMS[hand]->prange)=*(pGMS[hand]->pIncomming+(pGMS[hand]->index-(5+offset)));

	*(pGMS[hand]->pmode)=*(pGMS[hand]->pIncomming+(pGMS[hand]->index-(3+offset)));

	*(pGMS[hand]->punits)=*(pGMS[hand]->pIncomming+(pGMS[hand]->index-(4+offset)));

	if(offset!=0)
	{	
		*(pGMS[hand]->ptime_sec)=*(pGMS[hand]->pIncomming+(pGMS[hand]->index-(20)));
		*(pGMS[hand]->ptime_sec+1)=*(pGMS[hand]->pIncomming+(pGMS[hand]->index-(19)));

		*(pGMS[hand]->ptime_min)=*(pGMS[hand]->pIncomming+(pGMS[hand]->index-(17)));
		*(pGMS[hand]->ptime_min+1)=*(pGMS[hand]->pIncomming+(pGMS[hand]->index-(16)));

		*(pGMS[hand]->ptime_hour)=*(pGMS[hand]->pIncomming+(pGMS[hand]->index-(14)));
		*(pGMS[hand]->ptime_hour+1)=*(pGMS[hand]->pIncomming+(pGMS[hand]->index-(13)));

		*(pGMS[hand]->ptime_year)=*(pGMS[hand]->pIncomming+(pGMS[hand]->index-(5)));
		*(pGMS[hand]->ptime_year+1)=*(pGMS[hand]->pIncomming+(pGMS[hand]->index-(4)));

		*(pGMS[hand]->ptime_month)=*(pGMS[hand]->pIncomming+(pGMS[hand]->index-(8)));
		*(pGMS[hand]->ptime_month+1)=*(pGMS[hand]->pIncomming+(pGMS[hand]->index-(7)));

		*(pGMS[hand]->ptime_day)=*(pGMS[hand]->pIncomming+(pGMS[hand]->index-(11)));
		*(pGMS[hand]->ptime_day+1)=*(pGMS[hand]->pIncomming+(pGMS[hand]->index-(10)));
	}

	pGMS[hand]->store.units=(unsigned char)strtol(pGMS[hand]->punits,&stop,10);
	pGMS[hand]->store.range=(unsigned char)strtol(pGMS[hand]->prange,&stop,10);
	pGMS[hand]->store.mode=(unsigned char)strtol(pGMS[hand]->pmode,&stop,10);

	value=strtod(pGMS[hand]->pvalue,&stop);

	gm0_convertvalue(pGMS[hand]->store.range,pGMS[hand]->store.units,value,&pGMS[hand]->store.value,FALSE);
	
	pGMS[hand]->store.time.day=(unsigned char)strtol(pGMS[hand]->ptime_day,&stop,10);
	pGMS[hand]->store.time.month=(unsigned char)strtol(pGMS[hand]->ptime_month,&stop,10);
	pGMS[hand]->store.time.year=(unsigned char)strtol(pGMS[hand]->ptime_year,&stop,10);
	pGMS[hand]->store.time.sec=(unsigned char)strtol(pGMS[hand]->ptime_sec,&stop,10);
	pGMS[hand]->store.time.min=(unsigned char)strtol(pGMS[hand]->ptime_min,&stop,10);
	pGMS[hand]->store.time.hour=(unsigned char)strtol(pGMS[hand]->ptime_hour,&stop,10);
	
	pGMS[hand]->samplecount++;
	pGMS[hand]->index=0; // reset the readthread buffer to zero

	if(pGMS[hand]->buffer_enabled==1)
	{

		if(pGMS[hand]->buffer_ringflag[pGMS[hand]->buffer_end]==BUFFER_OWNER_INSTRUMENT)
		{	
			//yay we hit an empty spot!
			pGMS[hand]->store_buffer[pGMS[hand]->buffer_end]=pGMS[hand]->store;
			pGMS[hand]->samples_avaiable++;
			pGMS[hand]->buffer_ringflag[pGMS[hand]->buffer_end]=BUFFER_OWNER_USER;

			pGMS[hand]->buffer_end++;
			if(pGMS[hand]->buffer_end>=MAX_BUFFER)
				pGMS[hand]->buffer_end=0;
		}
	}

	// signal data has been collected
	pGMS[hand]->datasignal=TRUE;

	filtercallbackdata(&pGMS[hand]->store);

	if(pGMS[hand]->pCallback!=NULL && pGMS[hand]->gm0_threadrun==TRUE)
	{
		pGMS[hand]->pCallback(hand,pGMS[hand]->store);
	}

	//Beep(3000,50);
	if(pGMS[hand]->pCallback2!=NULL && pGMS[hand]->gm0_threadrun==TRUE)
	{
		pGMS[hand]->pCallback2(hand);
	}


}


GM0_API HANDLEGM gm0_newgm(int port,int mode)
{
	HANDLEGM newhand=0;
	int portret;
	int x=0;
	while(pGMS[newhand]!=NULL && newhand<255)
	{
		if(pGMS[newhand]->m_Iportno==port)
			return newhand;
		newhand++;
	}

	debugprint("Creating a new GM\n");

	if(newhand==255)
		return GM_MEMORY_ERROR;

	pGMS[newhand]=malloc(sizeof(struct GM_STRUCT));

	pGMS[newhand]->pvalue=malloc(8);
	pGMS[newhand]->punits=malloc(2);
	pGMS[newhand]->pmode=malloc(2);
	pGMS[newhand]->prange=malloc(2);
	pGMS[newhand]->ptime_day=malloc(3);
	pGMS[newhand]->ptime_month=malloc(3);
	pGMS[newhand]->ptime_year=malloc(3);
	pGMS[newhand]->ptime_sec=malloc(3);
	pGMS[newhand]->ptime_min=malloc(3);
	pGMS[newhand]->ptime_hour=malloc(3);

	*(pGMS[newhand]->pvalue+7)='\0';
	*(pGMS[newhand]->punits+1)='\0';
	*(pGMS[newhand]->pmode+1)='\0';
	*(pGMS[newhand]->prange+1)='\0';
	*(pGMS[newhand]->ptime_day+2)='\0';
	*(pGMS[newhand]->ptime_month+2)='\0';
	*(pGMS[newhand]->ptime_year+2)='\0';
	*(pGMS[newhand]->ptime_sec+2)='\0';
	*(pGMS[newhand]->ptime_min+2)='\0';
	*(pGMS[newhand]->ptime_hour+2)='\0';


	pGMS[newhand]->disablepoll=FALSE;
	pGMS[newhand]->polldisabled=FALSE;
	pGMS[newhand]->faultyfirmware=FALSE;
	
	pGMS[newhand]->threadlockcount=0;

	pGMS[newhand]->samplecount=0;
	pGMS[newhand]->port_open=FALSE;
	pGMS[newhand]->index=0;
	pGMS[newhand]->pIncomming=malloc(255);
	pGMS[newhand]->m_Iportno=port;
	pGMS[newhand]->gm0_threadrun=TRUE;

	pGMS[newhand]->pCallback=NULL;
	pGMS[newhand]->pCallback2=NULL;
	pGMS[newhand]->pConnectCallback=NULL;
	pGMS[newhand]->pDisConnectCallback=NULL;
	pGMS[newhand]->pNullCallback=NULL;
	pGMS[newhand]->connected=FALSE;	
	pGMS[newhand]->connecting=FALSE;

	pGMS[newhand]->shutdownstarted=FALSE;
	
	for(x=0;x<MAX_BUFFER;x++)
	{
		pGMS[newhand]->buffer_ringflag[x]=BUFFER_OWNER_INSTRUMENT;
	}

	pGMS[newhand]->samples_avaiable=0;
	pGMS[newhand]->buffer_start=0;
	pGMS[newhand]->buffer_end=0;

	if(port<0)
		mode=1; // force to GM08 if USB is selected
				// else mode will ste com port speed anyway

	pGMS[newhand]->meter_mode=mode;

	portret= gm0_openport(newhand,mode);
	
	pGMS[newhand]->doingnull=FALSE;
	gm0_startbuffersamples(newhand);

	if(portret!=0)
	{
		gm0_killgm(newhand); // clean memory up before exit
		return (GM_PORTOPENERROR);
	}


	return newhand;
}



void debugprint(char * msg)
{
/*
	FILE * debug;
	debug=fopen("c:\\gm0dll.txt","a+");
	if(debug==NULL)
		return;
	fprintf(debug,msg);
	fflush(debug);
*/
	return;
}



GM0_API HANDLEGM gm0_startconnect(HANDLEGM hand)
{

	char msg[255];

	if(pGMS[hand]==NULL)
		return GM_DATAERROR;

	if(hand<0 || hand > 255)
		return GM_DATAERROR;

	if(pGMS[hand]->connected==TRUE)
		return -1;

	if(pGMS[hand]->connecting==TRUE)
		return -1;
	
	pGMS[hand]->connecting=TRUE;

	sprintf(msg,"Hand is %d\n",hand);

	debugprint("Start Connect Activated");
	debugprint(msg);

#ifndef _LINUX
	_beginthread(connectthread,((void*)NULL),((void*)hand));
#else
	{
		pthread_t  p_thread; 
		pthread_create(&p_thread, NULL, (void*)connectthread, (void*)hand);
	}
#endif
	return 0;
}


GM0_API int gm0_killgm(HANDLEGM hand)
{
	clock_t increment;
	int timeout = 0;
	
	if(hand<0 || hand >255)
		return 0;
	
#ifndef _LINUX // does linux have an equlivent ?
	if(IsBadWritePtr(pGMS[hand],sizeof(struct GM_STRUCT)))
	return 0;
#endif


	pGMS[hand]->gm0_threadrun=FALSE;


	if(pGMS[hand]->shutdownstarted==TRUE)
	{
		return 0;
	}

	pGMS[hand]->shutdownstarted=TRUE;

	increment = clock() + 6000;
	timeout=0;

	while(pGMS[hand]->threadlockcount!=0 && timeout == 0)
	{

		if (clock() > increment)
		{
			timeout = 1;
			/* we timed out waiting for the read thread to terminate
			 this is bad but what can we do about it now? */
		}


		Sleep(20);
	}

	
	if(pGMS[hand]->m_Iportno>=0)
	{
		gm0_closeport(hand);
	}
	else
	{
		// USB Shut down ?
		closeUSB(hand);
	}

	free(pGMS[hand]->pIncomming);
	free(pGMS[hand]->pvalue);
	free(pGMS[hand]->punits);
	free(pGMS[hand]->pmode);
	free(pGMS[hand]->prange);
	free(pGMS[hand]->ptime_sec);
	free(pGMS[hand]->ptime_min);
	free(pGMS[hand]->ptime_hour);
	free(pGMS[hand]->ptime_day);
	free(pGMS[hand]->ptime_year);
	free(pGMS[hand]->ptime_month);
	free(pGMS[hand]);
	pGMS[hand]=NULL;

return 0;

}


void __cdecl connectthread(void * pParam)
{

	char ret;
	HANDLEGM hand;
	int connected,timeout;
	int retdata;
	hand=(HANDLEGM)pParam;

	timeout=FALSE;
	connected=FALSE;
	pGMS[hand]->gm0_threadrun=TRUE; // if this shoudn't be TRUE then this function shoudn't have been started!
	pGMS[hand]->gm0_usereadthread=FALSE; 
	pGMS[hand]->disablepoll=TRUE;
	pGMS[hand]->polldisabled=TRUE;

	printf("Starting a CONNECT THREAD\n");
	

		#ifndef _LINUX
			if(pGMS[hand]->m_Iportno>=0)
			{
				_beginthread(readthread,0,(void*)hand);
			}
			else
			{
				_beginthread(pollthread,0,(void*)hand);
			}
		#else
		{
			if(pGMS[hand]->m_Iportno>=0)
			{
				pthread_t  p_thread; 
				pthread_create(&p_thread, NULL, (void*)readthread, (void*)hand);
			}
			else
			{
				
				pthread_t  p_thread; 
				pthread_create(&p_thread, NULL, (void*)pollthread, (void*)hand);
			}		
		}
		#endif

#ifndef _LINUX
	CoInitializeEx(NULL,COINIT_MULTITHREADED);
#endif
	
	pGMS[hand]->threadlockcount++;
	
	while(connected==FALSE && timeout==FALSE && pGMS[hand]->gm0_threadrun==TRUE)
	{
		haveanothego:

		Sleep(1);
		

		if(pGMS[hand]->m_Iportno<0)
		{
			FindTheHID(hand);
		}
		
		ret=-1; // default to allow entry below

		while(ret!=0 && pGMS[hand]->gm0_threadrun==TRUE)
		{
			Beep(1000,25);
			printf("Seting star\n");
			ret=gm0_gmstar(hand); // this does not need threadlock counting				
			Sleep(50);
		}

		Beep(1000,100);

		if(pGMS[hand]->gm0_threadrun==FALSE)
		{
			#ifndef _LINUX
				CoUninitialize();
			#endif
			pGMS[hand]->threadlockcount--;
			return;
		}


		gm0_setdata(hand,0xABCD);

		pGMS[hand]->faultyfirmware=FALSE;
		retdata=gm0_getdata(hand);

		if(retdata==0xABCD)
		{
			connected=TRUE;	
			OutputDebugString("******* NORMAL COM DETECTED *******\n");
		
		}
		else
		{

			pGMS[hand]->faultyfirmware=TRUE;
			retdata=gm0_getdata(hand);
		
			if(retdata==0xABCD)
			{
				connected=TRUE;	
				OutputDebugString("******* FAULT COM DETECTED *******\n");
			}
		}
		if(connected!=TRUE)
		{

			Beep(500,100);
			goto haveanothego;

			#ifndef _LINUX
				CoUninitialize();
			#endif
			pGMS[hand]->threadlockcount--;
			
			return;
		}
	}
	
	if(pGMS[hand]->gm0_threadrun==FALSE)
	{
	#ifndef _LINUX
		CoUninitialize();
	#endif
			pGMS[hand]->threadlockcount--;

		return;
	}

	Beep(2000,100);
	pGMS[hand]->connected=TRUE;

	gm0_gmmode1(hand);

	Sleep(1000);

	// BUG in GM08 RS232,need to do interval commands as below
	gm0_setinterval(hand,2);
	gm0_setinterval(hand,1);

	if(pGMS[hand]->pConnectCallback!=NULL)
	{
		pGMS[hand]->pConnectCallback();
	}

	#ifndef _LINUX
		CoUninitialize();
	#endif

	pGMS[hand]->threadlockcount--;

	if(pGMS[hand]->m_Iportno<0)
	{
		//printf("Connected,re-enable poll\n");
		pGMS[hand]->disablepoll=FALSE;
	}
	

}

void filtercallbackdata(struct gm_store * pdata)
{

	pdata->range&=0x07; //including autorange
	pdata->units&=0x03; 
	if(pdata->mode>5)
		pdata->mode=5;

}

