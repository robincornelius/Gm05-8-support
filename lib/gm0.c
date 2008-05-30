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
#include <stdio.h>
#include <time.h>

#ifndef _LINUX
	#include <objbase.h>
	#include <process.h>
	#include "windows.h"
#else
	#include <pthread.h>
	#include <unistd.h>
#endif

#include "gm0_private.h"
#include "gm0.h"


struct mode_array
{	
		char *pointer_text;
};

const float units_range_conversion[4][4]={

    //tesla					
    {1.0,1000.0,1000.0,	1000.0},		
    //gauss
    {0.001,0.001,1.0,1.0},		
    //a/m			
    {0.001,0.001,0.001,	0.001}, 
    //oersted
    {0.001,0.001,1.0,1.0}};
					
/* The DP position indexed by [units][range] */
const int DP_position[4][4] = {
 {4, 2, 3, 4},
 {3, 4, 2, 3},
 {1, 2, 3, 4},
 {3, 4, 2, 3}};
 
/* The divisor for each DP position */
int divisor[5] = {
    1, 1, 10, 100, 1000 };

int gm0_convertvalue(int range,int units,float value,float * newvalue,BOOL isusb)
{
	int dppos;
	int scalar;

	if(newvalue==NULL)
		return -1;

	range&=0x03; //mask out autorange

	if(isusb==TRUE)
	{
		dppos=DP_position[units][range];
		scalar=divisor[dppos];
		value /= (float)scalar;
	}


	value /= units_range_conversion[units][range];

	if(units==2 && isusb==TRUE)
	{
		value *=0.7957747;
	}

	(*newvalue)=value;

	return 0;


}

/******************** COMMAND WRAPPERS *******************/


GM0_API int gm0_gmcmd(HANDLEGM hand,unsigned char cmd,unsigned char data)
{
	int retdata;
	int oldstatus;

	if(pGMS[hand]->doingnull==TRUE)
		return 0;

	pGMS[hand]->disablepoll=TRUE;

	retdata=AMpacket(hand,cmd,data);

	if(pGMS[hand]->faultyfirmware==TRUE)
	{
		if( cmd==GMC_GETDATALO || cmd==GMC_GETDATAHI || ( cmd>=GMC_TIME0 && cmd <=GMC_TIME7))
		{
			oldstatus=pGMS[hand]->cmdstatus;
			pGMS[hand]->cmdstatus=retdata;
			retdata=oldstatus;
		}
	}

	return retdata;
}

int gm0_gmmode1(HANDLEGM hand) //ENTER DATA MODE AND START A DATA CAPTURE THREAD
{
	if(pGMS[hand]->m_Iportno<0)
	{
		pGMS[hand]->disablepoll=FALSE;
		//printf("Enabling poll\n");

		return 0;
	}

	packetbyte(hand,GMC_MODE1);
	packetbyte(hand,GMC_NULL);
	pGMS[hand]->gm0_usereadthread=TRUE;
	return 0;
}


int gm0_gmstar(HANDLEGM hand) // ENTER COMMAND MODE
{

	pGMS[hand]->disablepoll=TRUE;

	//printf("Disabling poll\n");

	while(pGMS[hand]->polldisabled==FALSE)
	{
		Sleep(1);
	}

	//printf("Poll disabled\n");


	if(pGMS[hand]->m_Iportno<0)
	{
		return AMpacket(hand,'*','*');
	}
	else
	{
		return gm0_dothestar(hand);
	}

}

int gm0_dothestar(HANDLEGM hand)
{
	int status;
	char flushdata;
	char star;
	int ret=-1;

	pGMS[hand]->gm0_usereadthread=FALSE;

	Sleep(50);
	star=42;
	rs232_write(&pGMS[hand]->com,&star,1);

	Sleep(250);
	status=0;
	while(status!=-1)
	{
		status=rs232_read(&pGMS[hand]->com,&flushdata,1,25);
		if(status!=-1)
		{
			ret=flushdata;
		}
	}
	
	return ret;


}

BOOL checkhand(int hand)
{

	if(hand<0 || hand > 255)
		return FALSE;
	
	if(pGMS[hand]==NULL)
		return FALSE;

	return TRUE;

}


// ****************** COMMANDS START HERE ********************************


GM0_API int gm0_getrange(HANDLEGM hand)
{
//	Sleep(1);
	if(!checkhand(hand))
		return -1;

	return pGMS[hand]->store.range;

}

GM0_API int gm0_getmode(HANDLEGM hand)
{

	return pGMS[hand]->store.mode;
}

GM0_API int gm0_getunits(HANDLEGM hand)
{

	return pGMS[hand]->store.units;
}

GM0_API int gm0_setunits(HANDLEGM hand,unsigned char units)
{

	

	if(units >3)
		return GM_DATAERROR;

	if(pGMS[hand]->store.units==units) // no need to worry the comms
		return GM_OK;

	gm0_gmstar(hand);

	gm0_gmcmd(hand,GMC_UNITS,units+128);
	gm0_gmmode1(hand);

	return GM_OK;
}

GM0_API int gm0_setrange(HANDLEGM hand,unsigned char range)
{

	// if the range has been changed this function should not run for at least 2 sample periods

	if(pGMS[hand]==NULL)
		return GM_DATAERROR;

	if(hand<0 || hand>254)
		return GM_DATAERROR;

	if(pGMS[hand]->store.range==range) // no need to worry the comms
		return GM_OK;

	if(pGMS[hand]->store.range>4 && range>4) // both on auto no need to worry comms
		return GM_OK;



	gm0_gmstar(hand);
	gm0_gmcmd(hand,GMC_RANGE,range+128);

	gm0_gmmode1(hand);

	return GM_OK;
}

GM0_API int gm0_setmode(HANDLEGM hand,unsigned char mode)
{
	

	if(mode >5)
		return GM_DATAERROR;

	if(pGMS[hand]->store.mode==mode) // no need to worry the comms
		return GM_OK;
	
	gm0_gmstar(hand);
	
	gm0_gmcmd(hand,GMC_FUNCTION,mode+128);
	gm0_gmmode1(hand);

	return GM_OK;
}

GM0_API int gm0_setlanguage(HANDLEGM hand,unsigned char lan)
{
	

	if(lan >5)
		return GM_DATAERROR;

	gm0_gmstar(hand);

	gm0_gmcmd(hand,GMC_LANGUAGE,lan+128);
	gm0_gmmode1(hand);

	return GM_OK;
}


GM0_API int gm0_resetpeak(HANDLEGM hand)
{
	gm0_gmstar(hand);

	gm0_gmcmd(hand,GMC_RESPEAK,0);
	gm0_gmmode1(hand);

	return 0;
}

GM0_API int gm0_donull(HANDLEGM hand)
{
	gm0_gmstar(hand);

	gm0_gmcmd(hand,GMC_DONULL,0);
	Sleep(1000);
	gm0_resetnull(hand);

	if(pGMS[hand]->meter_mode==1)
	{
		Sleep(1000);
	}
	else
	{
		Sleep(9000);
	}
	
	gm0_gmmode1(hand);

	return 0;
}

GM0_API int gm0_doaz(HANDLEGM hand)
{
	gm0_gmstar(hand);
	gm0_gmcmd(hand,GMC_DOAZ,0);
	
	if(pGMS[hand]->meter_mode==0)
	{
		Sleep(33000);
	}
	else
	{
		Sleep(3500);
	}

	gm0_resetnull(hand);
	
	

	if(pGMS[hand]->meter_mode==0)
	{
		Sleep(33000);
	}
	else
	{
		Sleep(95000);
	}
	
	gm0_gmmode1(hand);

	return 0;
}

GM0_API int gm0_simkey(HANDLEGM hand,char keycode)
{
	gm0_gmcmd(hand,GMC_SIMKEY,keycode);
	return 0;
}


GM0_API int gm0_resetnull(HANDLEGM hand)
{
	gm0_gmcmd(hand,GMC_SIMKEY,'0');

	return 0;
}



GM0_API double gm0_getvalue(HANDLEGM hand)
{

	if(!checkhand(hand))
	{
		printf("CHECK FAILED\n");
		return -4;
	}

	return pGMS[hand]->store.value;
}



GM0_API long gm0_getsamplecount(HANDLEGM hand)
{
	return pGMS[hand]->samplecount;
}

GM0_API int gm0_setcallback(HANDLEGM hand,	void (* pCallback)(int,struct gm_store))
{
	pGMS[hand]->pCallback=pCallback;
	return (0);
}

GM0_API int gm0_setcallback2(HANDLEGM hand,	void (* pCallback)(int))
{
	pGMS[hand]->pCallback2=pCallback;
	return (0);
}

GM0_API int gm0_setconnectcallback(HANDLEGM hand,void ( * pCallback)(void))
{
	pGMS[hand]->pConnectCallback=pCallback;
	return (0);
}

GM0_API int gm0_setdisconnectcallback(HANDLEGM hand,void ( * pCallback)(HANDLEGM hand))
{
	pGMS[hand]->pDisConnectCallback=pCallback;
	return (0);
}

GM0_API int gm0_setnullcallback(HANDLEGM hand,void ( * pCallback)(void))
{
	pGMS[hand]->pNullCallback=(void*)pCallback;
	return (0);
}

GM0_API int gm0_fastUSBpoll(HANDLEGM hand,int enabled)
{	
	pGMS[hand]->needrangepoll=TRUE;

	if(enabled)
		pGMS[hand]->fastUSBcapture=TRUE;
	else
		pGMS[hand]->fastUSBcapture=FALSE;

}


GM0_API int gm0_setinterval(HANDLEGM hand,int interval)
{
	gm0_gmstar(hand);

	gm0_gmcmd(hand,GMC_INTERVAL,interval);
	gm0_gmmode1(hand);

	return 0;
}

GM0_API int gm0_getlanguage(HANDLEGM hand)
{
	int lang;
	gm0_gmstar(hand);
	lang=gm0_gmcmd(hand,GMC_LANGUAGE,0);
	gm0_gmmode1(hand);
	return(lang);

}

GM0_API int gm0_off(HANDLEGM hand)
{
	gm0_simkey(hand,'O');
	return 0;
}


GM0_API int gm0_setaddrlo(HANDLEGM hand,unsigned __int8 data)
{
	gm0_gmcmd(hand,GMC_SETADDLO,data);
	return 0;
}

GM0_API int gm0_setaddrhi(HANDLEGM hand,unsigned __int8 data)
{
	gm0_gmcmd(hand,GMC_SETADDHI,data);
	return 0;
}

GM0_API int gm0_setaddr(HANDLEGM hand,unsigned __int16 data)
{
	unsigned __int8 lo,hi;

	hi = data/256;
	lo=data&0x00FF;

	gm0_gmcmd(hand,GMC_SETADDHI,hi);
	gm0_gmcmd(hand,GMC_SETADDLO,lo);
	return 0;
}


GM0_API int gm0_incaddr(HANDLEGM hand)
{
	gm0_gmcmd(hand,GMC_INCADDR,0);
	return 0;
}

GM0_API BOOL gm0_getconnect(HANDLEGM hand)
{

	if(!checkhand(hand))
		return FALSE;

	return(pGMS[hand]->connected); 
}


GM0_API int gm0_setdatalo(HANDLEGM hand,unsigned __int8 data)
{
	gm0_gmcmd(hand,GMC_SETDATALO,data);
	return 0;
}

GM0_API int gm0_setdatahi(HANDLEGM hand,unsigned __int8 data)
{
	gm0_gmcmd(hand,GMC_SETDATAHI,data);
	return 0;
}


GM0_API int gm0_setdata(HANDLEGM hand,unsigned __int16 data)
{
	unsigned __int8 lo,hi;

	hi = data/256;
	lo=data&0x00FF;

	gm0_gmcmd(hand,GMC_SETDATAHI,hi);
	gm0_gmcmd(hand,GMC_SETDATALO,lo);
	return 0;
}


GM0_API unsigned __int8 gm0_getdatalo(HANDLEGM hand)
{
	return(gm0_gmcmd(hand,GMC_GETDATALO,0));
}

GM0_API unsigned __int8 gm0_getdatahi(HANDLEGM hand)
{
	return(gm0_gmcmd(hand,GMC_GETDATAHI,0));
}

GM0_API unsigned __int16 gm0_getdata(HANDLEGM hand)
{
	unsigned __int16 data;
	unsigned __int8 lo,hi;

//	if(pGMS[hand]->faultyfirmware==TRUE)
//	{
//		return(gm0_getdatafaulty(hand));
//	}

	lo=gm0_getdatalo(hand);
	hi=gm0_getdatahi(hand);

	data=hi;

	data=data*256;
	data=data+lo;

	return data;
}

GM0_API unsigned __int16 gm0_getdatafaulty(HANDLEGM hand)
{
	unsigned __int16 data;
	unsigned __int8 lo,hi;

	gm0_getdatalo(hand);
	lo=pGMS[hand]->cmdstatus;
	
	gm0_getdatahi(hand);
	hi=pGMS[hand]->cmdstatus;

	data=hi;

	data=data*256;
	data=data+lo;

	return data;
}


GM0_API int gm0_unlock(HANDLEGM hand,BOOL lockon)
{
	if(lockon==TRUE)
	{
		gm0_setaddrlo(hand,0);
		gm0_setaddrhi(hand,0);
	}
	else
	{
		gm0_setaddrhi(hand,0x3f);
		gm0_setaddrlo(hand,0x81);
	}

	gm0_gmcmd(hand,GMC_LOCKMODE,0);
	
	return 0;
}


GM0_API int gm0_settime2(HANDLEGM hand,struct gm_time *time)
{
	struct gm_time temp;
	temp=*time;
	gm0_settime(hand,temp);

	return 0;
}

GM0_API int gm0_settime(HANDLEGM hand,struct gm_time time)
{
	gm0_gmstar(hand);
	gm0_gmcmd(hand,GMC_TIME,1); // allow write to time reg

	time.sec=(time.sec % 10)+ (int)(((int)(time.sec/10))*16);
	time.min=(time.min % 10)+ (int)(((int)(time.min/10))*16);
	time.hour=(time.hour % 10)+ (int)(((int)(time.hour/10))*16);
	time.year=(time.year % 10)+ (int)(((int)(time.year/10))*16);
	time.month=(time.month % 10)+ (int)(((int)(time.month/10))*16);
	time.day=(time.day % 10)+ (int)(((int)(time.day/10))*16);

	gm0_gmcmd(hand,GMC_TIME0,0); 
	gm0_gmcmd(hand,GMC_TIME1,time.sec); 
	gm0_gmcmd(hand,GMC_TIME2,time.min); 
	gm0_gmcmd(hand,GMC_TIME3,time.hour); 
	gm0_gmcmd(hand,GMC_TIME4,0); 
	gm0_gmcmd(hand,GMC_TIME5,time.day); 
	gm0_gmcmd(hand,GMC_TIME6,time.month); 
	gm0_gmcmd(hand,GMC_TIME7,time.year); 

	gm0_gmcmd(hand,GMC_TIME,2); // save new data
	gm0_gmcmd(hand,GMC_TIME,0); // read only);

	gm0_gmmode1(hand);

	return 0;
}


GM0_API struct gm_time gm0_gettime(HANDLEGM hand)
{
	struct gm_time time;
	gm0_gmstar(hand);	
	gm0_gmcmd(hand,GMC_TIME,0); // read only);
	gm0_gmcmd(hand,GMC_TIME,3); // Get time
	
	time.sec=gm0_gmcmd(hand,GMC_TIME1,0); 
	
	if(pGMS[hand]->cmdstatus!=0)
		OutputDebugString("Clock error");
	time.min=gm0_gmcmd(hand,GMC_TIME2,0); 
	
	if(pGMS[hand]->cmdstatus!=0)
		OutputDebugString("Clock error");
	time.hour=gm0_gmcmd(hand,GMC_TIME3,0); 

	if(pGMS[hand]->cmdstatus!=0)
		OutputDebugString("Clock error");

	time.day=gm0_gmcmd(hand,GMC_TIME5,0); 
	if(pGMS[hand]->cmdstatus!=0)
		OutputDebugString("Clock error");

	time.month=gm0_gmcmd(hand,GMC_TIME6,0); 
	if(pGMS[hand]->cmdstatus!=0)
		OutputDebugString("Clock error");

	time.year=gm0_gmcmd(hand,GMC_TIME7,0); 
	if(pGMS[hand]->cmdstatus!=0)
		OutputDebugString("Clock error");

	time.sec = (time.sec & 15)+((int)(time.sec/16))*10;
	time.min =(time.min & 15)+((int)((int)(time.min/16)*10));
	time.hour =(time.hour & 15)+((int)((int)(time.hour/16)*10));
	time.day =(time.day & 15)+(int)((int)(time.day/16)*10);
	time.month =(time.month & 15)+(int)((int)(time.month/16)*10);
	time.year =(time.year & 15)+(int)((int)(time.year/16)*10);

	gm0_gmcmd(hand,GMC_TIME,0); // read only);

	gm0_gmmode1(hand);

	return (time);
}


GM0_API int gm0_sendtime(HANDLEGM hand,BOOL extended)
{
	gm0_gmstar(hand);
	gm0_gmcmd(hand,GMC_COMMFLAG,extended);
	gm0_gmmode1(hand);
	return 0;
}

GM0_API void gm0_enabledebug()
{
	//rs232_enable_debug();
}

GM0_API void gm0_disabledebug()
{
	//rs232_disable_debug();
}

GM0_API void gm0_stopbuffersamples(HANDLEGM hand)
{
		pGMS[hand]->buffer_enabled=0;
}

GM0_API void gm0_startbuffersamples(HANDLEGM hand)
{
	pGMS[hand]->buffer_enabled=1;
}


GM0_API int gm0_resetbuffersamples(HANDLEGM hand)
{
	int counter=0;

	gm0_stopbuffersamples(hand);

	for (counter=0;counter<MAX_BUFFER;counter++)
	{
		pGMS[hand]->buffer_ringflag[counter]=BUFFER_OWNER_INSTRUMENT;
	}
	pGMS[hand]->samples_avaiable=0;

	gm0_startbuffersamples(hand);
}


GM0_API int gm0_getnobuffersamples(HANDLEGM hand)
{
	return pGMS[hand]->samples_avaiable;
}

GM0_API struct gm_store gm0_get_next_buffer(HANDLEGM hand)
{
	char msg[1024];

	//sprintf(msg,"get_buffer() aval %d, start %d, end %d \n\0",pGMS[hand]->samples_avaiable,pGMS[hand]->buffer_start,pGMS[hand]->buffer_end);
	//debugwrite(msg);

	if (pGMS[hand]->buffer_ringflag[pGMS[hand]->buffer_start]==BUFFER_OWNER_USER)
	{
		int oldindex=pGMS[hand]->buffer_start;
		pGMS[hand]->buffer_start++;
		if(pGMS[hand]->buffer_start>=MAX_BUFFER)
		{
			pGMS[hand]->buffer_start=0;
			//sprintf(msg,"get_buffer() read wrap\n\0");
			//debugwrite(msg);
		}

		pGMS[hand]->buffer_ringflag[oldindex]=BUFFER_OWNER_INSTRUMENT;

		pGMS[hand]->samples_avaiable--;
		//sprintf(msg,"get_buffer() sent one reading\n\0");
		//debugwrite(msg);

		return pGMS[hand]->store_buffer[oldindex];

	}
	else
	{
		//sprintf(msg,"get_buffer() NOTHING TO DO!\n\0");
		//debugwrite(msg);
		// we need to return *something* and have no error system
		return pGMS[hand]->store_buffer[pGMS[hand]->buffer_start];
	}
}

GM0_API void gm0_sampleondemand(HANDLEGM hand)
{

	pGMS[hand]->sampleondemand=TRUE;

}

GM0_API struct gm_store gm0_demandsample(HANDLEGM hand,int extra)
{

	// sanity checking here

	if(extra==1)
		pGMS[hand]->needrangepoll=TRUE;
	else
		pGMS[hand]->needrangepoll=FALSE;

	polldata(hand);

	return pGMS[hand]->store;


}


//******************* E2 FUNCTIONS *************//
// DO NOT ATTEMPT TO WRITE TO THE E2 YOU WILL KILL YOUR METER
// NO WRITING FUNCTIONS ARE EXPOSED HERE FOR THAT VERY REASON.


GM0_API unsigned __int16 gm0_writeprobee2word(HANDLEGM hand,char pos,__int16 data)
{
	unsigned __int16 temp;
	unsigned char hi,lo;

	gm0_gmstar(hand);
	gm0_unlock(hand,FALSE); //unlock gm0

	gm0_setaddrlo(hand,pos);
	gm0_setaddrhi(hand,1);
	gm0_setdatalo(hand,data&0x00FF);
	gm0_setdatahi(hand,0x00FF&((data&0xFF00)>>8));
	gm0_gmcmd(hand,GMC_WRE2,0);
	
	gm0_unlock(hand,TRUE); // lock gm0
	gm0_gmmode1(hand);

	return (temp);
}

GM0_API unsigned __int16 gm0_readprobee2word(HANDLEGM hand,char pos)
{
	unsigned __int16 temp;
	unsigned char hi,lo;

	gm0_gmstar(hand);
	gm0_unlock(hand,FALSE); //unlock gm0

	gm0_setaddrlo(hand,pos);
	gm0_setaddrhi(hand,1);
	gm0_gmcmd(hand,GMC_RDE2,0);
	lo=gm0_getdatalo(hand);
	hi=gm0_getdatahi(hand);
	temp=hi*256+lo;

	gm0_unlock(hand,TRUE); // lock gm0
	gm0_gmmode1(hand);

	return (temp);
}

GM0_API unsigned __int16 gm0_readgme2word(HANDLEGM hand,char pos)
{
	unsigned __int16 temp;
	unsigned char hi,lo;

	gm0_gmstar(hand);
	gm0_unlock(hand,FALSE); //unlock gm0

	gm0_setaddrlo(hand,pos);
	gm0_setaddrhi(hand,0);
	gm0_gmcmd(hand,GMC_RDE2,0);
	lo=gm0_getdatalo(hand);
	hi=gm0_getdatahi(hand);
	temp=hi*256+lo;

	gm0_unlock(hand,TRUE); // lock gm0
	gm0_gmmode1(hand);

	return (temp);
}


GM0_API unsigned __int16 gm0_getgmserial(HANDLEGM hand)
{
	return gm0_readgme2word(hand,rGM04SerNo);
}

GM0_API unsigned __int16 gm0_getprobeserial(HANDLEGM hand)
{
	return gm0_readprobee2word(hand,rSerialNo);
}

GM0_API unsigned __int16 gm0_getprobetype(HANDLEGM hand)
{
	return gm0_readprobee2word(hand,rProbeType);
}

GM0_API unsigned __int16 gm0_getprobecaldate(HANDLEGM hand)
{
	return gm0_readprobee2word(hand,rCalibDate);
}


GM0_API int gm0_probebutton(HANDLEGM hand)
{
	gm0_simkey(hand,'S');
	gm0_gmmode1(hand);
	return 0;
}


GM0_API	long gm0_startcmd(HANDLEGM hand)
{
	gm0_gmstar(hand);
	Sleep(250);
	return 0;
}

GM0_API long gm0_endcmd(HANDLEGM hand)
{
	gm0_gmmode1(hand);
	return 0;
}


GM0_API struct gm_store gm0_getstore(HANDLEGM hand,int pos)
{	

	struct gm_store store;
	unsigned __int8 reg[8];
	signed __int16 tempvalue;

	gm0_gmcmd(hand,GMC_REGPTR,pos+128);
	gm0_gmcmd(hand,GMC_GETREG,0);

	reg[0]=gm0_gmcmd(hand,GMC_TIME0,0); 
	reg[1]=gm0_gmcmd(hand,GMC_TIME1,0); 
	reg[2]=gm0_gmcmd(hand,GMC_TIME2,0); 
	reg[3]=gm0_gmcmd(hand,GMC_TIME3,0); 
	reg[4]=gm0_gmcmd(hand,GMC_TIME4,0); 
	reg[5]=gm0_gmcmd(hand,GMC_TIME5,0); 
	reg[6]=gm0_gmcmd(hand,GMC_TIME6,0); 
	reg[7]=gm0_gmcmd(hand,GMC_TIME7,0); 

	if(reg[3]==255)
		printf("THIS REGISTER IS EMPTY\n");

	// decode the compressed data

	tempvalue=(int)(reg[0]*256)+reg[1];
	store.range=reg[2] & 3;
	store.units=(int)(reg[2] /4) &7;
	store.mode=reg[3];
	store.time.sec=reg[4] & 63;
	store.time.min=(reg[5] & 15) +(((int)(reg[4]/4)&48));
	store.time.hour=((int)(reg[5]/16)+(int)(reg[6]&1)*16);
	store.time.day=(int)(reg[6]/2) & 31;
	store.time.month=(int)((reg[7] & 3)*4)+(int)(reg[6]/64);
	store.time.year=(int)(reg[7]/4)+(int)((reg[3] & 128)/2);

	/* Scale the value appropriately for the units and range */

	gm0_convertvalue(store.range,store.units,tempvalue,&store.value,TRUE);

	return(store);
}

void commerror(HANDLEGM hand)
{

}


GM0_API int gm0_getmetertype(HANDLEGM hand)
{
	// At present we only have GM05 and GM08 to worry about
	// IF we are using USB we are a GM08

	return pGMS[hand]->meter_mode;

}

