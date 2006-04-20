/*
   Copyright � 2004 Hirst Magnetic Instruments Ltd and Robin Cornelius.

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

#define debug_flag 0


#ifndef _LINUX
	#include <objbase.h>
	#include <process.h>
	#include "windows.h"
	#include <locale.h>
#else
	#include <pthread.h>
#endif

#ifdef _LINUX

 Sleep(int dwMs) // wrapper for sleep function 
 {
	sleep(dwMs);
 }

#endif

#include <stdio.h>
#include <time.h>
#include "gm0_private.h"
#include "gm0.h"


int processcount=0;
int handcount;

void debugwrite(char * msg)
{

if (debug_flag)
    {
    FILE *f;

    f = fopen("c:\\rs232.log", "a");
    fprintf(f, "MSG: %s\n",msg);
    fclose(f);
    }

}



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

int gm0_openport(HANDLEGM hand)
{
	char * pportstring; 

	#ifndef _LINUX
		/* Set the locale for this instance */
		setlocale( LC_ALL , "English" );
	#else
		/* POSIX warning */
		/* There may be a locale problem running on non english systems that use , for 
		decimal point, this was fixed on windows with above setlocale() code */
	#endif

	pportstring=(char *)malloc(sizeof(char)*5);
	if(pportstring==NULL)
		return GM_MEMORY_ERROR;

#ifndef _LINUX
	sprintf(pportstring,"COM%d",pGMS[hand]->m_Iportno);
#else
	sprintf(pportstring,"/dev/ttyS%d",pGMS[hand]->m_Iportno);
#endif
	pGMS[hand]->com.fd=rs232_open(pportstring,4800,'N',8,1,0);

	pGMS[hand]->port_open=true;

	free(pportstring); 

	return 0;
}


int gm0_closeport(HANDLEGM hand)
{

	char * pportstring; 


	if(pGMS[hand]->port_open!=true)
		return 0; // NOTHING TO CLOSE

	pportstring=(char *)malloc(sizeof(char)*5);
	if(pportstring==NULL)
		return GM_MEMORY_ERROR;

	sprintf(pportstring,"COM%d",pGMS[hand]->m_Iportno);

	if (pGMS[hand]->port_open==true)
	{
		rs232_exit(&pGMS[hand]->com);
	}

	free(pportstring);	

	return 0;
}


void __cdecl readthread(void * pParam)
{

	HANDLEGM hand;
	unsigned long length;
	static readthreadstarted=false;
	
#ifndef _LINUX
	CoInitializeEx(NULL,COINIT_MULTITHREADED);
#endif
	
	if (readthreadstarted==true)
		return;

	readthreadstarted=true;


	hand = (int)pParam;

	debugwrite("Starting a read thread");

	pGMS[hand]->threadlockcount++;

	OutputDebugString("this is read thread\n");
	

	while(pGMS[hand]->gm0_threadrun==true)
	{
		char data;

		if(pGMS[hand]->gm0_usereadthread==true)
		{
		length = rs232_read(&pGMS[hand]->com,&data,1,10);

			if(length!=-1) //read 1 char or timeout after 250ms
			{
				*(pGMS[hand]->pIncomming + pGMS[hand]->index)=data; // store data in buffer
				pGMS[hand]->index++;
				if(pGMS[hand]->index>=12 /*&& *(pGMS[hand]->pIncomming+12)==13*/)
				processgmcomms(hand);
			}
		}
		Sleep(1); //yield
	}
	
	if(pGMS[hand]!=NULL)
		pGMS[hand]->threadlockcount--;

	readthreadstarted=false;

		debugwrite("READ THEAD EXIT");


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
	char msg []="          \0";


	debugwrite("Packet byte start");

	oldindex=pGMS[hand]->index;
	target=oldindex+1;

	rs232_write(&pGMS[hand]->com, &dataout, 1);

    length = rs232_read(&pGMS[hand]->com, &retdata,1,250);

	if (length != 1)
	{

		OutputDebugString("WAIT TIMEOUT\n");
		printf("timeout recieve\n");
		commerror(hand);
	}

	
	sprintf(msg,"O:%d I:d",dataout,retdata);

	debugwrite(msg);



	return(retdata);
}



void processgmcomms(HANDLEGM hand)
{

	int offset;
	char *stop;

	if(pGMS[hand]->gm0_threadrun==false)
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
	pGMS[hand]->store.value=strtod(pGMS[hand]->pvalue,&stop);
	pGMS[hand]->store.time.day=(unsigned char)strtol(pGMS[hand]->ptime_day,&stop,10);
	pGMS[hand]->store.time.month=(unsigned char)strtol(pGMS[hand]->ptime_month,&stop,10);
	pGMS[hand]->store.time.year=(unsigned char)strtol(pGMS[hand]->ptime_year,&stop,10);
	pGMS[hand]->store.time.sec=(unsigned char)strtol(pGMS[hand]->ptime_sec,&stop,10);
	pGMS[hand]->store.time.min=(unsigned char)strtol(pGMS[hand]->ptime_min,&stop,10);
	pGMS[hand]->store.time.hour=(unsigned char)strtol(pGMS[hand]->ptime_hour,&stop,10);
	
	pGMS[hand]->samplecount++;
	pGMS[hand]->index=0; // reset the readthread buffer to zero

	// signal data has been collected
	pGMS[hand]->datasignal=true;

	if(pGMS[hand]->pCallback!=NULL && pGMS[hand]->gm0_threadrun==true)
	{
		pGMS[hand]->pCallback(hand,pGMS[hand]->store);
	}
}


GM0_API HANDLEGM gm0_newgm(int port)
{
	HANDLEGM newhand=0;
	int portret;

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


	pGMS[newhand]->threadlockcount=0;

	pGMS[newhand]->samplecount=0;
	pGMS[newhand]->port_open=false;
	pGMS[newhand]->index=0;
	pGMS[newhand]->pIncomming=malloc(255);
	pGMS[newhand]->m_Iportno=port;
	pGMS[newhand]->gm0_threadrun=true;

	pGMS[newhand]->pCallback=NULL;
	pGMS[newhand]->pConnectCallback=NULL;
	pGMS[newhand]->pDisConnectCallback=NULL;
	pGMS[newhand]->pNullCallback=NULL;
	pGMS[newhand]->connected=false;	
	pGMS[newhand]->connecting=false;

	pGMS[newhand]->shutdownstarted=false;
	
	portret= gm0_openport(newhand);
	
	pGMS[newhand]->doingnull=false;


	if(portret!=0)
	{
		gm0_killgm(newhand); // clean memory up before exit
		return (GM_PORTOPENERROR);
	}


	return newhand;
}



void debugprint(char * msg)
{

	FILE * debug;
	debug=fopen("c:\\gm0dll.txt","a+");
	if(debug==NULL)
		return;
	fprintf(debug,msg);
	fflush(debug);

	return;
}



GM0_API HANDLEGM gm0_startconnect(HANDLEGM hand)
{

	char msg[255];

	if(pGMS[hand]==NULL)
		return GM_DATAERROR;

	if(hand<0 || hand > 255)
		return GM_DATAERROR;

	if(pGMS[hand]->connected==true)
		return -1;

	if(pGMS[hand]->connecting==true)
		return -1;
	
	pGMS[hand]->connecting=true;

	sprintf(msg,"Hand is %d\n",hand);

	debugprint("Start Connect Activated ");
	debugprint(msg);

#ifndef _LINUX
	_beginthread(connectthread,((void*)NULL),((void*)hand));
#else
	{
		pthread_t  p_thread; 
		pthread_create(&p_thread, NULL, connectthread, (void*)hand);
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


	pGMS[hand]->gm0_threadrun=false;


	if(pGMS[hand]->shutdownstarted==true)
	{
		return 0;
	}

	pGMS[hand]->shutdownstarted=true;

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

	gm0_closeport(hand);
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

	timeout=false;
	connected=false;
	pGMS[hand]->gm0_threadrun=true; // if this shoudn't be true then this function shoudn't have been started!

	pGMS[hand]->gm0_usereadthread=false; // if this shoudn't be true then this function shoudn't have been started!

	#ifndef _LINUX
		_beginthread(readthread,0,(void*)hand);
	#else
	{
		pthread_t  p_thread; 
		pthread_create(&p_thread, NULL, readthread, (void*)hand);
	}
	#endif


#ifndef _LINUX
	CoInitializeEx(NULL,COINIT_MULTITHREADED);
#endif
	
	pGMS[hand]->threadlockcount++;

	debugwrite("Starting connect thread");
	
	while(connected==false && timeout==false && pGMS[hand]->gm0_threadrun==true)
	{
		ret=-1; // default to allow entry below

		while(ret!=0 && pGMS[hand]->gm0_threadrun==true)
		{
			ret=gm0_dothestar(hand); // this does not need threadlock counting 
		}

		if(pGMS[hand]->gm0_threadrun==false)
		{
			#ifndef _LINUX
				CoUninitialize();
			#endif
			pGMS[hand]->threadlockcount--;
			return;
		}

		gm0_setdata(hand,0xABCD);
		
		retdata=gm0_getdata(hand);

		if(retdata==0xABCD)
		{
			connected=true;	
		}
		else
		{
			#ifndef _LINUX
				CoUninitialize();
			#endif
			pGMS[hand]->threadlockcount--;
			return;
		}
	}
	
	gm0_gmmode1(hand);

	if(pGMS[hand]->gm0_threadrun==false)
	{
	#ifndef _LINUX
		CoUninitialize();
	#endif
			pGMS[hand]->threadlockcount--;

		return;
	}

	pGMS[hand]->connected=true;
 
	gm0_setinterval(hand,1);

	if(pGMS[hand]->pConnectCallback!=NULL)
	{
		pGMS[hand]->pConnectCallback();
	}

	#ifndef _LINUX
		CoUninitialize();
	#endif

	pGMS[hand]->threadlockcount--;

}