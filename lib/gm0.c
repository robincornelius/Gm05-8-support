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
#include <stdio.h>
#include <time.h>

#ifndef _LINUX
	#include <objbase.h>
	#include <process.h>
	#include "windows.h"
#else
	#include <pthread.h>
#endif

#include "gm0_private.h"
#include "gm0.h"

struct mode_array
{	
		char *pointer_text;
};

const struct mode_array mode_text[5] = {"DC   \0", "AC   \0", "DCPK \0", "ACPK \0", "ACMAX\0"};

const struct units_struct units_range_conversion[4]={

						1.0,		//tesla					
						1000.0, 		//was 10 000 think it was fudge for /10
					 	10.0, 		
					 	100.0, 		
					 	1000.0,		

						1.0,	//gauss
					 	0.1,		
						1.0,		
						10.0,		
						100.0,		

						0.7957747, //a/m
						0.001,		
						0.01,		
						0.1,		
						1.0, 

						1.0,	//oersted
						0.1,		
						1.0,		
						10.0,		
						100.0 
						
};

int gm0_convertvalue(int range,int units,float value,float * newvalue)
{

	if(newvalue==NULL)
		return -1;

	range&=0x03; //mask out autorange

	value /= units_range_conversion[units].range_div[range];
	value *= units_range_conversion[units].global_mult;
	(*newvalue)=value;

	return 0;


}

/******************** COMMAND WRAPPERS *******************/


GM0_API int gm0_gmcmd(HANDLEGM hand,unsigned char cmd,unsigned char data)
{
	int retdata;
	int oldstatus;

	if(pGMS[hand]->doingnull==true)
		return 0;

	pGMS[hand]->disablepoll=TRUE;

	while(pGMS[hand]->polldisabled==FALSE)
		Sleep(1);

	retdata=AMpacket(hand,cmd,data);

	pGMS[hand]->disablepoll=FALSE;

	if(pGMS[hand]->faultyfirmware==true)
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
		return 0;

	packetbyte(hand,GMC_MODE1);
	packetbyte(hand,GMC_NULL);
	pGMS[hand]->gm0_usereadthread=true;
	return 0;
}


int gm0_gmstar(HANDLEGM hand) // ENTER COMMAND MODE
{
	if(pGMS[hand]->m_Iportno<0)
	{
		AMpacket(hand,'*','*');
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
	Sleep(1000);
	gm0_resetnull(hand);
	Sleep(9000);
	gm0_gmmode1(hand);

	return 0;

}

GM0_API int gm0_doaz(HANDLEGM hand)
{
	gm0_gmstar(hand);
	gm0_gmcmd(hand,GMC_DOAZ,0);
	Sleep(33000);
	gm0_resetnull(hand);
	Sleep(33000);
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

//	if(pGMS[hand]->faultyfirmware==true)
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

	gm0_convertvalue(store.range,store.units,tempvalue,&store.value);

	return(store);
}

void commerror(HANDLEGM hand)
{

}

