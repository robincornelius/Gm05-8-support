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

#ifndef __GM0H__
#define __GM0H__

#ifdef _LINUX
 #define GM0_API /*GM0_API*/
 #define __ /*__*/
 #define BOOL int
 #define TRUE 1
 #define FALSE 0
 #define __int16 int
 #define HANDLE int
 #define __cdecl /*__cdecl*/
 #define __int8 char
 #define APIENTRY /*APIENTRY*/
 #define DWORD int
 #define LPVOID void *
 #define DLL_PROCESS_ATTACH 1
 #define DLL_THREAD_ATTACH 2
 #define DLL_THREAD_DETACH 3
 #define DLL_PROCESS_DETACH 4
 #define OutputDebugString //printf
 #define BKET /**/
 #define LIBTYPE /*LIBTYPE*/
#else
 #ifdef NOVC
  #define GM0_API /**/
  #define LIBTYPE (WINAPI *
  #define BKET )
 #else
  #ifdef GM0_EXPORTS
   /*#define GM0_API  __declspec(dllexport)*/
	#define GM0_API  /**/
  #else
   #define GM0_API extern "C" __declspec(dllimport)
  #endif
 #define BKET /**/
 #define LIBTYPE WINAPI
 #endif
#endif


// The GM HANDLE TYPE
#define HANDLEGM int

// GM FUNCTION RETURN CODES
#define GM_OK 0
#define GM_MEMORY_ERROR -1
#define GM_PORTOPENERROR -2
#define GM_DATAERROR -3
#define GM_BLOCKED -4 



// GM KEYS (YOU CAN'T PRESS THE ON KEY FROM SOFTWARE!)
#define GM_KEY_ENTER 'E'
#define GM_KEY_MENU 'X'
#define GM_KEY_OFF 'O'
#define GM_KEY_NEXT 'N'
#define GM_KEY_RANGE 'R'
#define GM_KEY_RESET '0'

// GM RANGES
#define GM_RANGE_0 0
#define GM_RANGE_1 1
#define GM_RANGE_2 2
#define GM_RANGE_3 3
#define GM_RANGE_AUTO 4

// GM UNITS
#define GM_UNITS_TESLA 0
#define GM_UNITS_GAUSS 1
#define GM_UNITS_KAM 2
#define GM_UNITS_OE 3

// GM MODES
#define GM_DC 0
#define GM_DCPK 1
#define GM_AC 2
#define GM_ACMAX 3
#define GM_ACPK 4
#define GM_HOLD 5

// GM LANGUAGES
#define GM_ENGLISH 0
#define GM_FRENCH 1
#define GM_GERMAN 2
#define GM_ITALIAN 3
#define GM_SPANISH 4
#define GM_PORTUGUESE 5


// A struct for looking after gm_time responses
struct gm_time{
unsigned char sec;
unsigned char min;
unsigned char hour;
unsigned char day;
unsigned char month;
unsigned char year;
};

// A struct for dealing with data in the registers
struct gm_store{
 struct gm_time time;
 unsigned char range;
 unsigned char mode;
 unsigned char units;
 float value;
};

struct unit_range_struct {

	float range_div;
	char *formatter;
	//char *unit_string;
};

struct	units_struct {

	float global_mult;
	struct unit_range_struct unit_range[4];

};

const struct units_struct units_range_conversion[4];

// Thread and initaliseation fuctions
GM0_API HANDLEGM LIBTYPE gm0_newgm BKET (int port);
GM0_API HANDLEGM LIBTYPE gm0_startconnect BKET (HANDLEGM hand);
GM0_API int LIBTYPE gm0_killgm BKET (HANDLEGM hand);
GM0_API BOOL LIBTYPE gm0_getconnect BKET (HANDLEGM hand);

// Standard user Functions
GM0_API int LIBTYPE gm0_setrange BKET (HANDLEGM hand,unsigned char range);
GM0_API int LIBTYPE gm0_getrange BKET (HANDLEGM hand);

GM0_API int LIBTYPE gm0_setunits BKET (HANDLEGM hand,unsigned char units);
GM0_API int LIBTYPE gm0_getunits BKET (HANDLEGM hand);

GM0_API int LIBTYPE gm0_setmode BKET (HANDLEGM hand,unsigned char mode);
GM0_API int LIBTYPE gm0_getmode BKET (HANDLEGM hand);

GM0_API int LIBTYPE gm0_setlanguage BKET (HANDLEGM hand,unsigned char lan);
GM0_API int LIBTYPE gm0_getlanguage BKET (HANDLEGM hand);

GM0_API double LIBTYPE gm0_getvalue BKET (HANDLEGM hand);

GM0_API int LIBTYPE gm0_resetpeak BKET (HANDLEGM hand);

// Zero and null functions
GM0_API int LIBTYPE gm0_donull BKET (HANDLEGM hand);
GM0_API int LIBTYPE gm0_doaz BKET (HANDLEGM hand);
GM0_API int LIBTYPE gm0_resetnull BKET (HANDLEGM hand);

// Date time and register functions
GM0_API int LIBTYPE gm0_sendtime BKET (HANDLEGM hand,BOOL extended);
GM0_API int LIBTYPE gm0_settime BKET (HANDLEGM hand,struct gm_time);
GM0_API int LIBTYPE gm0_settime2 BKET (HANDLEGM hand,struct gm_time *time);
GM0_API struct gm_time LIBTYPE gm0_gettime BKET (HANDLEGM hand);
GM0_API struct gm_store LIBTYPE gm0_getstore BKET (HANDLEGM hand,int pos);

// Callback functions
GM0_API int LIBTYPE gm0_setcallback BKET (HANDLEGM hand, void (__stdcall * pCallback)(int,struct gm_store));
GM0_API int LIBTYPE gm0_setcallback2 BKET (HANDLEGM hand, void (__stdcall * pCallback)(int));

GM0_API int LIBTYPE gm0_setconnectcallback BKET (HANDLEGM hand,void (__stdcall * pCallback)(void));
GM0_API int LIBTYPE gm0_setdisconnectcallback BKET (HANDLEGM hand,void (__stdcall * pCallback)(HANDLEGM hand));
GM0_API int LIBTYPE gm0_setnullcallback BKET (HANDLEGM hand,void ( * pCallback)(void));

// Advanced
GM0_API int  LIBTYPE gm0_simkey BKET (HANDLEGM,char keycode);
GM0_API int LIBTYPE gm0_setinterval BKET (HANDLEGM hand,int interval);

GM0_API unsigned __int16 LIBTYPE gm0_getgmserial BKET (HANDLEGM hand);
GM0_API unsigned __int16 LIBTYPE gm0_getprobeserial BKET(HANDLEGM hand);
GM0_API unsigned __int16 LIBTYPE gm0_getprobetype BKET (HANDLEGM hand);
GM0_API unsigned __int16 LIBTYPE gm0_getprobecaldate BKET (HANDLEGM hand);


GM0_API int LIBTYPE gm0_setaddr BKET (HANDLEGM hand,unsigned __int16 data);
GM0_API int LIBTYPE gm0_setdata BKET (HANDLEGM hand,unsigned __int16 data);
GM0_API unsigned __int16 LIBTYPE gm0_getaddr BKET (HANDLEGM hand);
GM0_API unsigned __int16 LIBTYPE gm0_getdata BKET (HANDLEGM hand);
GM0_API unsigned __int16 LIBTYPE gm0_getdatafaulty BKET (HANDLEGM hand);


GM0_API long LIBTYPE gm0_startcmd BKET (HANDLEGM hand);
GM0_API long LIBTYPE gm0_endcmd BKET (HANDLEGM hand);

#endif
