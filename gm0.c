/*
   Copyright © 2004 Hirst Magnetic Instruments Ltd and Robin Cornelius.

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
#else
 #include <pthread.h>
#endif


#include <stdio.h>
#include <time.h>


#include "gm0_private.h"
#include "gm0.h"


/******************** COMMAND WRAPPERS *******************/


GM0_API int gm0_gmcmd(HANDLEGM hand,unsigned char cmd,unsigned char data)
{
 int retdata;

 if(pGMS[hand]->doingnull==true)
  return 0;

 retdata=packetbyte(hand,cmd);
 pGMS[hand]->cmdstatus=packetbyte(hand,data); 

 return retdata;
}

int gm0_gmmode1(HANDLEGM hand) //ENTER DATA MODE AND START A DATA CAPTURE THREAD
{

 packetbyte(hand,GMC_MODE1);
 packetbyte(hand,GMC_NULL);

//	pGMS[hand]->gm0_threadrun=true;

	pGMS[hand]->gm0_usereadthread=true;


//	#ifndef _LINUX
//		_beginthread(readthread,0,(void*)hand);
//	#else
//	{
//		pthread_t  p_thread; 
//		pthread_create(&p_thread, NULL, readthread, (void*)hand);
//	}

//#endif

	return 0;

}


int gm0_gmstar(HANDLEGM hand) // ENTER COMMAND MODE
{
	return gm0_dothestar(hand);
}

int gm0_dothestar(HANDLEGM hand)
{
	int status;
	char flushdata;
	char star;
	int ret;

	pGMS[hand]->gm0_usereadthread=false;

	Sleep(50);

	OutputDebugString("Attempt Connect\n");
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
		return false;
	
	if(pGMS[hand]==NULL)
		return false;

	return true;

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

	gm0_gmstar(hand);

	if(units<0 || units >3)
		return GM_DATAERROR;

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


	gm0_gmstar(hand);
	gm0_gmcmd(hand,GMC_RANGE,range+128);
	gm0_gmmode1(hand);

	return GM_OK;
}

GM0_API int gm0_setmode(HANDLEGM hand,unsigned char mode)
{
	gm0_gmstar(hand);

	if(mode<0 || mode >5)
		return GM_DATAERROR;

	gm0_gmcmd(hand,GMC_FUNCTION,mode+128);
	gm0_gmmode1(hand);

	return GM_OK;
}

GM0_API int gm0_setlanguage(HANDLEGM hand,unsigned char lan)
{
	gm0_gmstar(hand);

	if(lan<0 || lan >5)
		return GM_DATAERROR;

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
	return 0;
	
	Sleep(15000);

}

GM0_API int gm0_doaz(HANDLEGM hand)
{
	gm0_gmstar(hand);
	gm0_gmcmd(hand,GMC_DOAZ,0);
	Sleep(30000);

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
	Sleep(30000);
	gm0_gmmode1(hand);
	return 0;
}




GM0_API double gm0_getvalue(HANDLEGM hand)
{
//	Sleep(1);

	if(!checkhand(hand))
		return -4;

//	Beep(1000,10);
	return pGMS[hand]->store.value;

}



GM0_API long gm0_getsamplecount(HANDLEGM hand)
{
	return pGMS[hand]->samplecount;
}



GM0_API int gm0_setcallback(HANDLEGM hand,	void (__stdcall * pCallback)(int,struct gm_store))
{
	pGMS[hand]->pCallback=pCallback;
	return (0);
}

GM0_API int gm0_setconnectcallback(HANDLEGM hand,void (__stdcall * pCallback)(void))
{
	pGMS[hand]->pConnectCallback=pCallback;
	return (0);
}

GM0_API int gm0_setdisconnectcallback(HANDLEGM hand,void (__stdcall * pCallback)(HANDLEGM hand))
{
	pGMS[hand]->pDisConnectCallback=pCallback;
	return (0);
}

GM0_API int gm0_setnullcallback(HANDLEGM hand,void (__stdcall * pCallback)(void))
{
	pGMS[hand]->pNullCallback=pCallback;
	return (0);
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
		return false;

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

	lo=gm0_getdatalo(hand);
	hi=gm0_getdatahi(hand);

	data=hi;

	data=data*256;
	data=data+lo;

	return data;
}


GM0_API int gm0_unlock(HANDLEGM hand,BOOL lockon)
{
	if(lockon==true)
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

//******************* E2 FUNCTIONS *************//
// DO NOT ATTEMPT TO WRITE TO THE E2 YOU WILL KILL YOUR METER
// NO WRITING FUNCTIONS ARE EXPOSED HERE FOR THAT VERY REASON.

GM0_API unsigned __int16 gm0_readprobee2word(HANDLEGM hand,char pos)
{
	unsigned __int16 temp;
	unsigned char hi,lo;

	gm0_gmstar(hand);
	gm0_unlock(hand,false); //unlock gm0

	gm0_setaddrlo(hand,pos);
	gm0_setaddrhi(hand,1);
	gm0_gmcmd(hand,GMC_RDE2,0);
	lo=gm0_getdatalo(hand);
	hi=gm0_getdatahi(hand);
	temp=hi*256+lo;

	gm0_unlock(hand,true); // lock gm0
	gm0_gmmode1(hand);

	return (temp);
}

GM0_API unsigned __int16 gm0_readgme2word(HANDLEGM hand,char pos)
{
	unsigned __int16 temp;
	unsigned char hi,lo;

	gm0_gmstar(hand);
	gm0_unlock(hand,false); //unlock gm0

	gm0_setaddrlo(hand,pos);
	gm0_setaddrhi(hand,0);
	gm0_gmcmd(hand,GMC_RDE2,0);
	lo=gm0_getdatalo(hand);
	hi=gm0_getdatahi(hand);
	temp=hi*256+lo;

	gm0_unlock(hand,true); // lock gm0
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


GM0_API void gm0_startcmd(HANDLEGM hand)
{
	gm0_gmstar(hand);
	Sleep(250);
}

GM0_API void gm0_endcmd(HANDLEGM hand)
{
	gm0_gmmode1(hand);
}


GM0_API struct gm_store gm0_getstore(HANDLEGM hand,int pos)
{	

	struct gm_store store;
	unsigned __int8 temp[8];
	
	gm0_gmcmd(hand,GMC_REGPTR,pos+127);
	gm0_gmcmd(hand,GMC_GETREG,0);

	temp[0]=gm0_gmcmd(hand,GMC_TIME0,0); 
	temp[1]=gm0_gmcmd(hand,GMC_TIME1,0); 
	temp[2]=gm0_gmcmd(hand,GMC_TIME2,0); 
	temp[3]=gm0_gmcmd(hand,GMC_TIME3,0); 
	temp[4]=gm0_gmcmd(hand,GMC_TIME4,0); 
	temp[5]=gm0_gmcmd(hand,GMC_TIME5,0); 
	temp[6]=gm0_gmcmd(hand,GMC_TIME6,0); 
	temp[7]=gm0_gmcmd(hand,GMC_TIME7,0); 

	// decode the compressed data

	store.value=(int)(temp[0]*256)+temp[1];
 store.range=temp[2] & 3;
 store.units=(int)(temp[2] /4) &7;
 store.mode=temp[3];
 store.time.sec=temp[4] & 63;
 store.time.min=(temp[5] & 15) +(((int)(temp[4]/4)&48));
 store.time.hour=((int)(temp[5]/15)+(int)(temp[6]&1)*16);
 store.time.day=(int)(temp[6]/2) & 31;
 store.time.month=(int)((temp[7] & 3)*4)+(int)(temp[6]/64);
 store.time.year=(int)(temp[7]/4)+(int)((temp[3] & 128)/2);


 return(store);
}


void commerror(HANDLEGM hand)
{

}

