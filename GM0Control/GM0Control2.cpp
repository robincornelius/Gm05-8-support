// GM0Control.cpp : Implementation of CGM0Control2

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

#include "stdafx.h"

#include "GMControl2.h"

#include "GM0Control2.h"

#include "..\lib\gm0.h"



void __stdcall connectcallback();

void __stdcall disconnectcallback(int handgm);

void __stdcall datacallback(HANDLEGM hand,gm_store store);

void _stdcall nullcallback(HANDLEGM hand);



CGM0Control2 * plocalptr;

struct gm_store thestore;

BOOL timeinfo;



/////////////////////////////////////////////////////////////////////////////

// CGM0Control2





STDMETHODIMP CGM0Control2::get_ComPort(short *pVal)

{

	// TODO: Add your implementation code here

// what?



	*pVal=m_iComPort;

	return S_OK;

}



STDMETHODIMP CGM0Control2::put_ComPort(short newVal)

{

	// TODO: Add your implementation code here



	if(newVal>0 &&newVal<255)

	{

		m_iComPort=newVal;

		return S_OK;

	}

	else

	{

		return S_OK;

	}



}



STDMETHODIMP CGM0Control2::AttemptConnect()

{

	// TODO: Add your implementation code here



	if(thegm<0)

		return S_OK;

	

	gm0_startconnect(thegm);

	return S_OK;

}



STDMETHODIMP CGM0Control2::SetMode(short lmode)

{

	// TODO: Add your implementation code here

	if(connected==false || thegm <0)

		return S_OK;

	

	if(mode != lmode)

		gm0_setmode(thegm,lmode);



	return S_OK;

}



STDMETHODIMP CGM0Control2::SetRange(short lrange)

{

	// TODO: Add your implementation code here



	if(connected==false|| thegm <0)

		return S_OK;



	if(range != lrange)

	gm0_setrange(thegm,lrange);



	return S_OK;

}



STDMETHODIMP CGM0Control2::SetUnits(short lunits)

{

	// TODO: Add your implementation code here

	if(connected==false|| thegm <0)

		return S_OK;



	if(units != lunits)

		gm0_setunits(thegm,lunits);



	return S_OK;

}



STDMETHODIMP CGM0Control2::SetLanguage(short lang)

{

	// TODO: Add your implementation code here

	if(connected==false|| thegm <0)

		return S_OK;





	return S_OK;

}



STDMETHODIMP CGM0Control2::DoAutoZero()

{

	// TODO: Add your implementation code here

	if(connected==false|| thegm <0)

		return S_OK;



	gm0_doaz(thegm);

	Fire_RequireSheild();



	return S_OK;

}



STDMETHODIMP CGM0Control2::DoNull()

{

	// TODO: Add your implementation code here

	if(connected==false || thegm <0)

		return S_OK;



	gm0_donull(thegm);

	Fire_RequireSheild();



	return S_OK;

}



STDMETHODIMP CGM0Control2::GetRegister(short reg)

{

	gm_store mystore;

	// TODO: Add your implementation code here

	if(connected==false|| thegm <0)

		return S_OK;



	thestore=gm0_getstore(thegm,reg);

	

	return S_OK;

}



STDMETHODIMP CGM0Control2::get_lastvalue(float *pVal)

{

	// TODO: Add your implementation code here



	*pVal=value;

	return S_OK;

}



STDMETHODIMP CGM0Control2::get_lastmode(short *pVal)

{

	// TODO: Add your implementation code here

	*pVal=mode;

	return S_OK;

}



STDMETHODIMP CGM0Control2::get_lastrange(short *pVal)

{

	// TODO: Add your implementation code here

	*pVal=range;

	return S_OK;

}





STDMETHODIMP CGM0Control2::get_lastunits(short *pVal)

{

	// TODO: Add your implementation code here

	*pVal=units;

	return S_OK;

}



STDMETHODIMP CGM0Control2::ProbeInSheild()

{

	// TODO: Add your implementation code here

	if(connected==false|| thegm <0)

		return S_OK;



	gm0_resetnull(thegm);



	return S_OK;

}



STDMETHODIMP CGM0Control2::Initalise()

{

	// TODO: Add your implementation code here

//		AfxMessageBox("Initalising");

	if(connected==true)

		return S_OK;



	if(thegm>-1)

		return S_OK;



		thegm=gm0_newgm(m_iComPort);

		if(thegm==GM_PORTOPENERROR)

		{

//			AfxMessageBox("Arrrrg cannot open port\n");

			::MessageBox(NULL,"Error opening requested port","GM0 COM OBJECT",MB_OK);

			return E_FAIL;

		}

			printf("Waiting for a GM to respond");



		// register callbacks

		plocalptr=this;

		plocalptr->abort=false;



//		AfxMessageBox("registering callbacks");



		gm0_setdisconnectcallback(thegm,&disconnectcallback);	

		gm0_setconnectcallback(thegm,&connectcallback);

		gm0_setcallback(thegm,&datacallback);



//		AfxMessageBox("done");





	return S_OK;

}





// Callback helpers



void __stdcall connectcallback()

{

	if(plocalptr->thegm<0 || plocalptr->abort==true)

		return;



	plocalptr->connected=true;

	gm0_setinterval(plocalptr->thegm,1);

	Sleep(200);

	plocalptr->Fire_Connected();

	

}



void __stdcall disconnectcallback(int handgm)

{

//	if(plocalptr->thegm<0 || plocalptr->abort==true)

//		return;

	

//	plocalptr->connected=false;

//	plocalptr->Fire_DisConnected();

}



void __stdcall datacallback(HANDLEGM hand,struct gm_store store)

{

	int temprange,tempunits,tempmode;



	plocalptr->threadlock=true;



	if(plocalptr->connected==false || plocalptr->thegm<0 || plocalptr->abort==true)

		return;



	tempmode=plocalptr->mode;

	temprange=plocalptr->range;

	tempunits=plocalptr->units;

		

	plocalptr->value=store.value;

	plocalptr->range=store.range;

	plocalptr->mode=store.mode;

	plocalptr->units=store.units;



	if(plocalptr->connected==false)

		return;

	plocalptr->Fire_NewData();



	if (tempmode!=plocalptr->mode && plocalptr->connected==true)

		plocalptr->Fire_NewMode();

	if (tempunits!=plocalptr->units && plocalptr->connected==true)

		plocalptr->Fire_NewUnits();

	if (temprange!=plocalptr->range && plocalptr->connected==true)

		plocalptr->Fire_NewRange();



	plocalptr->threadlock=false;

}



void __stdcall nullcallback(HANDLEGM hand)

{



}



STDMETHODIMP CGM0Control2::close()

{

	// TODO: Add your implementation code here



	if(thegm>=0)

	{

	//	::MessageBox(NULL,"Starting Shutdown","ABORTING",MB_OK);

		plocalptr->connected=false;

		plocalptr->abort=true;

		gm0_killgm(thegm);

		thegm=-1;

	//	::MessageBox(NULL,"Starting Shutdown","ABORTING",MB_OK);

	}

	return S_OK;

}



STDMETHODIMP CGM0Control2::get_store_value(double *pVal)

{

	// TODO: Add your implementation code here

	*(pVal)=thestore.value;



	return S_OK;

}



STDMETHODIMP CGM0Control2::get_store_range(short *pVal)

{

	// TODO: Add your implementation code here

	*(pVal)=thestore.range;



	return S_OK;

}



STDMETHODIMP CGM0Control2::get_store_units(short *pVal)

{

	// TODO: Add your implementation code here

	*(pVal)=thestore.units;



	return S_OK;

}





STDMETHODIMP CGM0Control2::get_store_mode(short *pVal)

{

	// TODO: Add your implementation code here

	*(pVal)=thestore.mode;



	return S_OK;

}







STDMETHODIMP CGM0Control2::settime()

{



	// TODO: Add your implementation code here



	gm0_settime(thegm,thestore.time);



	return S_OK;

}



STDMETHODIMP CGM0Control2::gettime()

{

	// TODO: Add your implementation code here

	

	thestore.time=gm0_gettime(thegm);

 

	return S_OK;

}





STDMETHODIMP CGM0Control2::get_year(short *pVal)

{

	// TODO: Add your implementation code here

	*(pVal)=(short)thestore.time.year;



	return S_OK;

}



STDMETHODIMP CGM0Control2::put_year(short newVal)

{

	// TODO: Add your implementation code here

	thestore.time.year=newVal;





	return S_OK;

}



STDMETHODIMP CGM0Control2::get_month(short *pVal)

{

	// TODO: Add your implementation code here



	*(pVal)=(short)thestore.time.month;



	return S_OK;

}



STDMETHODIMP CGM0Control2::put_month(short newVal)

{

	// TODO: Add your implementation code here

	thestore.time.month=newVal;





	return S_OK;

}



STDMETHODIMP CGM0Control2::get_day(short *pVal)

{

	// TODO: Add your implementation code here



		*(pVal)=(short)thestore.time.day;



	return S_OK;

}



STDMETHODIMP CGM0Control2::put_day(short newVal)

{

	// TODO: Add your implementation code here



		thestore.time.day=newVal;



	return S_OK;

}



STDMETHODIMP CGM0Control2::get_hour(short *pVal)

{

	// TODO: Add your implementation code here



		*(pVal)=(short)thestore.time.hour;



	return S_OK;

}



STDMETHODIMP CGM0Control2::put_hour(short newVal)

{

	// TODO: Add your implementation code here

	thestore.time.hour=newVal;



	return S_OK;

}



STDMETHODIMP CGM0Control2::get_min(short *pVal)

{

	// TODO: Add your implementation code here



		*(pVal)=(short)thestore.time.min;



	return S_OK;

}



STDMETHODIMP CGM0Control2::put_min(short newVal)

{

	// TODO: Add your implementation code here

	thestore.time.min=newVal;



	return S_OK;

}



STDMETHODIMP CGM0Control2::get_sec(short *pVal)

{

	// TODO: Add your implementation code here

	*(pVal)=(short)thestore.time.sec;



	return S_OK;

}



STDMETHODIMP CGM0Control2::put_sec(short newVal)

{

	// TODO: Add your implementation code here

	thestore.time.sec=newVal;



	return S_OK;

}





STDMETHODIMP CGM0Control2::get_gmserial(short *pVal)

{

	// TODO: Add your implementation code here



	*(pVal)=gm0_getgmserial(thegm);	

	return S_OK;

}



STDMETHODIMP CGM0Control2::get_probeserial(short *pVal)

{

	// TODO: Add your implementation code here

		*(pVal)=gm0_getprobeserial(thegm);





	return S_OK;

}



STDMETHODIMP CGM0Control2::startcmdseq()

{

	// TODO: Add your implementation code here



	gm0_startcmd(thegm);





	return S_OK;

}



STDMETHODIMP CGM0Control2::endcmdseq()

{

	// TODO: Add your implementation code here



	gm0_endcmd(thegm);



	return S_OK;

}







STDMETHODIMP CGM0Control2::get_sendtimeinfo(BOOL *pVal)

{

	// TODO: Add your implementation code here



	*(pVal)=timeinfo;



	return S_OK;

}



STDMETHODIMP CGM0Control2::put_sendtimeinfo(BOOL newVal)

{

	// TODO: Add your implementation code here



	timeinfo=newVal;

	gm0_sendtime(thegm,timeinfo);



	return S_OK;

}

