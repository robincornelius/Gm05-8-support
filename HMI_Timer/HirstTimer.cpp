// HirstTimer.cpp : Implementation of CHirstTimer
/*
	a quick and dirty timer
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


#include "stdafx.h"

#include "HMI_timer.h"
#include "HMI_timerCP.h"
#include "HirstTimer.h"




/////////////////////////////////////////////////////////////////////////////
// CHirstTimer

BOOL active=false;

int interval=0;


void __stdcall timerfunc(struct HWND__ *,unsigned int,unsigned int,unsigned long);

CHirstTimer * thethis;


STDMETHODIMP CHirstTimer::get_interval(short *pVal)
{
	// TODO: Add your implementation code here
	thethis=this;
	*(pVal)=interval;
		
	return S_OK;
}

STDMETHODIMP CHirstTimer::put_interval(short newVal)
{
	// TODO: Add your implementation code here
	thethis=this;
	interval=newVal;

	if(active==TRUE)
	{
		::KillTimer(NULL,IDT_TIMER);
		::SetTimer(NULL,IDT_TIMER,interval,timerfunc);
	}


	return S_OK;
}

STDMETHODIMP CHirstTimer::get_active(BOOL *pVal)
{
	// TODO: Add your implementation code here
thethis=this;

	*(pVal)=active;

	return S_OK;
}

STDMETHODIMP CHirstTimer::put_active(BOOL newVal)
{
	// TODO: Add your implementation code here

thethis=this;

	active=newVal;

	if(active==TRUE)
	{
		::KillTimer(NULL,timerid);
		timerid=::SetTimer(NULL,IDT_TIMER,interval,timerfunc);
	}

	return S_OK;
}



void __stdcall timerfunc(struct HWND__ *,unsigned int,unsigned int,unsigned long)
{

//	::MessageBox(NULL,"TIMER EVENT","OK",MB_OK);
	thethis->Fire_TimerEvent();

}

STDMETHODIMP CHirstTimer::StartTimer()
{
	// TODO: Add your implementation code here
	thethis=this;

//	::MessageBox(NULL,"TIMER STARTED","OK",MB_OK);

	::KillTimer(NULL,timerid);
	timerid=::SetTimer(NULL,IDT_TIMER,interval,timerfunc);

	active=TRUE;

	return S_OK;
}

STDMETHODIMP CHirstTimer::StopTimer()
{
	// TODO: Add your implementation code here

	thethis=this;

	active=FALSE;

		::KillTimer(NULL,timerid);


	return S_OK;
}
