<<<<<<< HEAD

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

/*
 This code is compatable with win32 systems and POSIX systems (eg linux etc) 
 To run on linux make sure you compile with the _LINUX flag defined eg -D_LINUX this will
 patch some of the gaps between win32 and linux, mainly a few defines and the 
 method of dynamic library loading.
*/


#define NOVC 

/* This define avoids using Visual C stuff and compiles as a true win32 console app without
visual C extensions
*/


#ifdef _LINUX
	#define HMODULE int
	#define CLOCKS_PER_SEC 1000
	#include <unistd.h>
	#define _strdate strdate
	#define _strtime strtime
#else
	#include <windows.h>
	#define CLOCKS_PER_SEC 1000
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../lib/gm0.h"

void loadfunction (HANDLE lib,void ** functionpointer,char * functionname);
void loadallfunctions();
void showmenu();
void __stdcall callback(HANDLEGM hand,struct gm_store store);
int doconnect(int port,int mode);
void setgmtime();

	/* Descriptions of the display modes */
	char *mode_str[5] =
    {
		"DC     ",
		"DC Peak",
		"AC     ",
		"AC MAX ",
		"AC Peak"
    };

	char *units_str[4] =
    {
		"T",
		"G",
		"A/m",
		"Oe"
    };

	char *range_str[8] =
    {
		"Range 0",
		"Range 1",
		"Range 2",
		"Range 3",
		"Range 0 (auto range)",
		"Range 1 (auto range)",
		"Range 2 (auto range)",
		"Range 3 (auto range)"
	};


HANDLEGM mygm;

int main(int argv, char * argvc[])
{

	int port;
	int opt;
	unsigned char buffer[1024];
	unsigned char cmd;
	unsigned char data;
	int  integer;

#ifndef _LINUX

	printf("\nAttemptng to initalise a new instance of the gm0library functions ....\n");
	loadallfunctions();
	printf("\n");
#endif

	printf("Connection options \n Would you like to connect with :-\n 0 - RS232 Port \n 1 - USB\n");
	scanf("%d",&opt);
	if(opt<0 || opt >1) {
		printf("Invalid selection");
		return -1;
	}

	if(opt==1)
	{

		printf("Which meter would you like to use 1-1st 2-2nd etc ?\n=");
  		scanf("%d",&port);
		if(port< 1 || port>254) 
  		{
			printf("\n meter %d is out of range (0-254)\n",port);
			return -1;
		}
		

		printf("Attempting connect, using USB (GM08)\n");
		if(doconnect(port*-1,1)==-1)
			return -1;
	}

	if(opt==0)
	{
  		printf("Which comm port would you like to use (COMn or /dev/ttySn) ?\n=");
  		scanf("%d",&port);
		if(port< 0 || port>254) 
  		{
			printf("\n Com port %d is out of range (0-254)\n",port);
			return -1;
		}
		
		printf("Attempting connect at 9600 Baud (GM08)\n");
		if(doconnect(port,1)==-1)
		{
			printf("Attempting connect at 4800 Baud (GM05)\n");
			if(doconnect(port,0)==-1)
				return -1;
		}
	}

	showmenu();

	cmd=0;

	while(cmd!='X' && cmd!='x')
	{
		printf("Enter option :");
		scanf("%s",&buffer);
		cmd=buffer[0];
		fflush(stdin);
		switch(cmd)
		{
			case 'r':
			case 'R':
				printf(" ** Range ** \n 0 - Least sensitive \n 3 - Most sensitive \n 4 - Auto range\n");
				printf("Enter range 0-4 :");
				scanf("%s",&buffer);
				data=buffer[0];
				gm0_setrange(mygm,data-0x30);
				break;
			case 'U':
			case 'u':
				printf(" ** Units ** \n 0 - Tesla \n 1 - Gauss \n 2 - A/m \n 3 - Oersted\n");
				printf("Select Units 0-3 :");
				scanf("%s",&buffer);
				data=buffer[0];

				gm0_setunits(mygm,data-0x30);
				break;
			case 'M':
			case 'm':
				printf("** Modes ** \n 0 - DC\n 1 - DC Peak \n 2 - AC \n 3 - AC Peak \n 4 - AC MAX RMS \n");
				printf("Select mode 0-4 :");
				scanf("%s",&buffer);
				data=buffer[0];
				gm0_setmode(mygm,data-0x30);
				break;
			case ' ':
				showmenu();	
				break;
			case 'L':
			case 'l':
				printf("Select language 0-5 :");
				scanf("%s",&buffer);
				data=buffer[0];
				gm0_setlanguage(mygm,data-0x30);
				break;
			case 'V':
			case 'v':
				{
					struct gm_store store;
					store.value=gm0_getvalue(mygm);
					store.range=gm0_getrange(mygm);
					store.units=gm0_getunits(mygm);
					store.mode=gm0_getmode(mygm);
					callback(mygm,store);
				} //case v 
			break;
			case 'A':
			case 'a':
				printf("Doing Auto Zero \n");
				gm0_doaz(mygm);
				printf("Press enter to simulate enter key");
				scanf("%s",&buffer);
				data=buffer[0];
				printf("Before reset\n");
				gm0_resetnull(mygm);
				printf("AZ Done");
				break;
			case 'n':
			case 'N':

				gm0_donull(mygm);
				break;
			case '1':
				gm0_setcallback(mygm,&callback);
				break;
			case '0':
				gm0_setcallback(mygm,NULL);
				break;

			case 'i':
				printf("Enter 232 interval 1 upwards :");
				scanf("%s",&buffer);
				data=buffer[0];
				gm0_setinterval(mygm,data-0x30);
				break;
			case 'Q':
			case 'q':
				gm0_resetpeak(mygm);
				break;
			case 'T':
			case 't':

				gm0_sendtime(mygm,TRUE);				
				break;
			case 'Y':
			case 'y':
				gm0_sendtime(mygm,FALSE);				
				break;
			case 'G':
			case 'g':
				{
					struct gm_store mystore;
					int reg;
					gm0_startcmd(mygm);
					printf("Which register ?");
					scanf("%s",&buffer);
					reg=buffer[0]-0x30;
					mystore=gm0_getstore(mygm,reg);
	
					printf("%f"	,mystore.value);

					printf(" %s",units_str[mystore.units]);

					printf(" %s",mode_str[mystore.mode]);

					printf(" %s\n",range_str[mystore.range]);

					//printf("Value is %lf %d %d %d\n",mystore.value,mystore.units,mystore.range,mystore.mode);
					printf("Time stamp %d:%d:%d  %d/%d/%d\n",mystore.time.hour,mystore.time.min,mystore.time.sec,mystore.time.day,mystore.time.month,mystore.time.year);
					gm0_endcmd(mygm);
				}
				break;
		
			case 'h':
			case 'H':
				{	
				int startreg,endreg;
				int reg;
				struct gm_store mystore;
				gm0_startcmd(mygm);
				printf("\nStart register =");
				scanf("%s",&buffer);
				startreg=buffer[0]-0x30;
				
				printf("\nEnd register =");
				scanf("%s",&buffer);
				endreg=buffer[0]-0x30;
				//for(reg=startreg;reg<=endreg;reg++)
				reg = startreg;
				do
				{
					mystore=gm0_getstore(mygm,reg);

					if (mystore.mode != 255) //255 invalid
					{
						printf("%f",mystore.value);

						printf(" %s",units_str[mystore.units]);
						printf(" %s",mode_str[mystore.mode]);
						printf(" %s\n",range_str[mystore.range]);
						printf("Time stamp %d:%d:%d  %d/%d/%d\n",mystore.time.hour,mystore.time.min,mystore.time.sec,mystore.time.day,mystore.time.month,mystore.time.year);
					}	
					
				
				}
				while((reg++ < endreg) && (mystore.mode != 255)); //255 invalid data

				gm0_endcmd(mygm);

				}

				case '.':
			case '>':
				{
					setgmtime();								
				}
			break;
			case ',':
			case '<':
				{
					struct gm_time time;
					time=gm0_gettime(mygm);
					printf("GM TIME is %2d:%2d:%2d %2d/%2d/%2d\n",time.hour,time.min,time.sec,time.day,time.month,time.year);
					break;
				}
			case '9':
			{
				
				printf("Probe Serial %d\n",gm0_getprobeserial(mygm));
				printf("GM Serial %d\n",gm0_getgmserial(mygm));

				printf("Probe cal date %d\n",gm0_getprobecaldate(mygm));
				printf("Probe type %d\n",gm0_getprobetype(mygm));

			}
			case 'F':
			{
				
				int width;
				printf("\nNew filter width =");
				scanf("%s",&width);
				gm0_setfilterwidth(mygm,(unsigned char)width);
			}
		}
	}
	
	gm0_killgm(mygm);
	return 0;
}


void showmenu()
{


printf("********************************************************************************\n");
printf("*                                                                              *\n");
printf("*                 GM05 Console control and DLL API Example                     *\n");
printf("*                                                                              *\n");
printf("********************************************************************************\n");
printf("\n");
printf(" (R) - Set Range        (U) - Set Units             (M) - Mode   \n");
printf(" (L) - language         (V) - Get Value             ( ) - Show menu\n");	
printf(" (A) - Auto Zero        (N) - Null                  (T) - Enable Time\n");	
printf(" (1) - Enable callback  (0) - Disable callback      (<) - Set GM Time\n");
printf(" (I) - Set interval     (Q) - Reset peak            (>) - Get GM Time\n");	
printf(" (W) - Power Off        (G) - Get reg               (H) - Get Reg(s)\n");
printf(" (B) - Reconnect        (X) - Exit                  (9) - Get E2\n");
printf(" (F) - FilterWidth\n");

printf("\n\n");

}


void __stdcall callback(HANDLEGM hand,struct gm_store store)
{

	//verify callback data
	if(store.units>3 || store.range>7 || store.mode>4)
	{
		printf("Invalid reading returned\n");
		return;
	}

	printf("Reading is ");

	printf("%f",store.value);
	
	printf(" %s",units_str[store.units]);

	printf(" %s ",mode_str[store.mode]);
	printf(" %s\n",range_str[store.range]);

}

// WIN32 SPECIFIC DLL FUNCTION LOADED //
// This avoids all the visual c macros that make the code platform specfic
// In stead we can just load the pointers for windows and let ld do the work for us
// on linux same code (minus function loader for win32 systems)

#ifndef _LINUX

void loadallfunctions()
{
	HMODULE lib;

	lib=LoadLibrary("gm0.dll");
	
	if(lib==NULL)
	{
		printf("\nCould not load gm0.dll going to try to find debug version\n");
		lib=LoadLibrary("gm0D.dll");
		if(lib==NULL)
		{
			printf("Sorry neither gm0.dll or gm0D.dll was found in DLL search path\n");
			exit (-1);
		}
	}
	
	loadfunction(lib,&gm0_newgm,"gm0_newgm");
	loadfunction(lib,&gm0_killgm,"gm0_killgm");
	loadfunction(lib,&gm0_getrange,"gm0_getrange");
	loadfunction(lib,&gm0_getunits,"gm0_getunits");
	loadfunction(lib,&gm0_getvalue,"gm0_getvalue"); 
	loadfunction(lib,&gm0_getmode,"gm0_getmode"); 

	loadfunction(lib,&gm0_startconnect,"gm0_startconnect");
	loadfunction(lib,&gm0_getconnect,"gm0_getconnect");

	loadfunction(lib,&gm0_setrange,"gm0_setrange");
	loadfunction(lib,&gm0_setunits,"gm0_setunits");
	loadfunction(lib,&gm0_setmode,"gm0_setmode"); 
	loadfunction(lib,&gm0_setlanguage,"gm0_setlanguage"); 

	loadfunction(lib,&gm0_doaz,"gm0_doaz"); 
	loadfunction(lib,&gm0_resetnull,"gm0_resetnull"); 

	loadfunction(lib,&gm0_setcallback,"gm0_setcallback"); 

	loadfunction(lib,&gm0_setinterval,"gm0_setinterval"); 
	loadfunction(lib,&gm0_resetpeak,"gm0_resetpeak"); 
	loadfunction(lib,&gm0_sendtime,"gm0_sendtime"); 

	loadfunction(lib,&gm0_startcmd,"gm0_startcmd"); 
	loadfunction(lib,&gm0_endcmd,"gm0_endcmd"); 
	loadfunction(lib,&gm0_getstore,"gm0_getstore"); 


	loadfunction(lib,&gm0_gettime,"gm0_gettime"); 
	loadfunction(lib,&gm0_settime,"gm0_settime"); 

	loadfunction(lib,&gm0_getprobeserial,"gm0_getprobeserial"); 
	loadfunction(lib,&gm0_getgmserial,"gm0_getgmserial"); 
	loadfunction(lib,&gm0_getprobetype,"gm0_getprobetype"); 
	loadfunction(lib,&gm0_getprobecaldate,"gm0_getprobecaldate"); 
	loadfunction(lib,&gm0_setfilterwidth,"gm0_setfilterwidth"); 

}

void loadfunction (HANDLE lib,void ** functionpointer,char * functionname)
{
 (* functionpointer)=GetProcAddress(lib,functionname);
 if(functionpointer==NULL)
 {
  printf("Error loading function %s \n",functionname);
  exit(-1);
 }
 printf("Function %s loaded @ %x OK\n",functionname,functionpointer);
 return;
}
#endif


int doconnect(int port,int mode)
{
	int iconnectcount;

	mygm=gm0_newgm(port,mode); 
	if(mygm>=0 && mygm<255)
	{
		printf(" SUCCESS, handle %d\n",mygm);
	}
	else
	{
		printf(" ERROR, error value %d\n",mygm);
		exit(-1);
	}
	
	printf("Attempting to initalise communications\n");
	gm0_startconnect(mygm);
	
  	iconnectcount=0;
	while(gm0_getconnect(mygm)==FALSE && iconnectcount <5)
  	{
		printf("Attempt %d of 5 has FAILED\n",iconnectcount);
		iconnectcount++;
		Sleep(1000);
	}
	
	if(gm0_getconnect(mygm)==FALSE)
	{
		printf("\n ** Communications have failed ... aborting ** \n");
		gm0_killgm(mygm);
		return -1;
	}
	else
	{
		printf("\n** Communications have succeded **\n");
	}

	return 0;
	
}


#ifdef _LINUX

void strdate(char * buffer)
{

    time_t curtime;
    struct tm * loctime;

    curtime= time(NULL);
    
    loctime=localtime(&curtime);
    strftime(buffer,256,"%D",loctime);
}


void strtime(char * buffer)
{

    time_t curtime;
    struct tm * loctime;

    curtime= time(NULL);
    
    loctime=localtime(&curtime);
    strftime(buffer,256,"%T",loctime);
}
        
#endif

void setgmtime()
{

    struct gm_time gaussmeter_time;
			    
    char dbuffer [256];
    char tbuffer [256];
    char tempbuffer[3];
    char * stop;

    tempbuffer[2]='\0';
				
    _strdate( dbuffer );
    printf( "The current date is %s (US Month Format)\n", dbuffer );
    _strtime( tbuffer );
    printf( "The current time is %s \n", tbuffer );
				
    tempbuffer[0]=tbuffer[0];
    tempbuffer[1]=tbuffer[1];
    gaussmeter_time.hour=strtol(tempbuffer,&stop,10);
    tempbuffer[0]=tbuffer[3];
    tempbuffer[1]=tbuffer[4];
    gaussmeter_time.min=strtol(tempbuffer,&stop,10);
    tempbuffer[0]=tbuffer[6];
    tempbuffer[1]=tbuffer[7];
    gaussmeter_time.sec=strtol(tempbuffer,&stop,10);
    tempbuffer[0]=dbuffer[3];
    tempbuffer[1]=dbuffer[4];
    gaussmeter_time.day=strtol(tempbuffer,&stop,10);
    tempbuffer[0]=dbuffer[0];
    tempbuffer[1]=dbuffer[1];
    gaussmeter_time.month=strtol(tempbuffer,&stop,10);
    tempbuffer[0]=dbuffer[6];
    tempbuffer[1]=dbuffer[7];
    gaussmeter_time.year=strtol(tempbuffer,&stop,10);


    printf("Setting GM time to %d:%d:%d %d/%d/%d \n",gaussmeter_time.hour,gaussmeter_time.min,gaussmeter_time.sec,gaussmeter_time.day,gaussmeter_time.month,gaussmeter_time.year);
    gm0_settime(mygm,gaussmeter_time);


}
=======

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

/*
 This code is compatable with win32 systems and POSIX systems (eg linux etc) 
 To run on linux make sure you compile with the _LINUX flag defined eg -D_LINUX this will
 patch some of the gaps between win32 and linux, mainly a few defines and the 
 method of dynamic library loading.
*/


#define NOVC 

/* This define avoids using Visual C stuff and compiles as a true win32 console app without
visual C extensions
*/


#ifdef _LINUX
	#define HMODULE int
	#define CLOCKS_PER_SEC 1000
	#include <unistd.h>
	#define _strdate strdate
	#define _strtime strtime
#else
	#include <windows.h>
	#define CLOCKS_PER_SEC 1000
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../lib/gm0.h"

void loadfunction (HANDLE lib,void ** functionpointer,char * functionname);
void loadallfunctions();
void showmenu();
void __stdcall callback(HANDLEGM hand,struct gm_store store);
int doconnect(int port,int mode);
void setgmtime();

	/* Descriptions of the display modes */
	char *mode_str[5] =
    {
		"DC     ",
		"DC Peak",
		"AC     ",
		"AC MAX ",
		"AC Peak"
    };

	char *units_str[4] =
    {
		"T",
		"G",
		"A/m",
		"Oe"
    };

	char *range_str[8] =
    {
		"Range 0",
		"Range 1",
		"Range 2",
		"Range 3",
		"Range 0 (auto range)",
		"Range 1 (auto range)",
		"Range 2 (auto range)",
		"Range 3 (auto range)"
	};


HANDLEGM mygm;

int main(int argv, char * argvc[])
{

	int port;
	int opt;
	unsigned char buffer[1024];
	unsigned char cmd;
	unsigned char data;
	int  integer;

#ifndef _LINUX

	printf("\nAttemptng to initalise a new instance of the gm0library functions ....\n");
	loadallfunctions();
	printf("\n");
#endif

	printf("Connection options \n Would you like to connect with :-\n 0 - RS232 Port \n 1 - USB\n");
	scanf("%d",&opt);
	if(opt<0 || opt >1) {
		printf("Invalid selection");
		return -1;
	}

	if(opt==1)
	{

		printf("Which meter would you like to use 1-1st 2-2nd etc ?\n=");
  		scanf("%d",&port);
		if(port< 1 || port>254) 
  		{
			printf("\n meter %d is out of range (0-254)\n",port);
			return -1;
		}
		

		printf("Attempting connect, using USB (GM08)\n");
		if(doconnect(port*-1,1)==-1)
			return -1;
	}

	if(opt==0)
	{
  		printf("Which comm port would you like to use (COMn or /dev/ttySn) ?\n=");
  		scanf("%d",&port);
		if(port< 0 || port>254) 
  		{
			printf("\n Com port %d is out of range (0-254)\n",port);
			return -1;
		}
		
		printf("Attempting connect at 9600 Baud (GM08)\n");
		if(doconnect(port,1)==-1)
		{
			printf("Attempting connect at 4800 Baud (GM05)\n");
			if(doconnect(port,0)==-1)
				return -1;
		}
	}

	showmenu();

	cmd=0;

	while(cmd!='X' && cmd!='x')
	{
		printf("Enter option :");
		scanf("%s",&buffer);
		cmd=buffer[0];
		fflush(stdin);
		switch(cmd)
		{
			case 'r':
			case 'R':
				printf(" ** Range ** \n 0 - Least sensitive \n 3 - Most sensitive \n 4 - Auto range\n");
				printf("Enter range 0-4 :");
				scanf("%s",&buffer);
				data=buffer[0];
				gm0_setrange(mygm,data-0x30);
				break;
			case 'U':
			case 'u':
				printf(" ** Units ** \n 0 - Tesla \n 1 - Gauss \n 2 - A/m \n 3 - Oersted\n");
				printf("Select Units 0-3 :");
				scanf("%s",&buffer);
				data=buffer[0];

				gm0_setunits(mygm,data-0x30);
				break;
			case 'M':
			case 'm':
				printf("** Modes ** \n 0 - DC\n 1 - DC Peak \n 2 - AC \n 3 - AC Peak \n 4 - AC MAX RMS \n");
				printf("Select mode 0-4 :");
				scanf("%s",&buffer);
				data=buffer[0];
				gm0_setmode(mygm,data-0x30);
				break;
			case ' ':
				showmenu();	
				break;
			case 'L':
			case 'l':
				printf("Select language 0-5 :");
				scanf("%s",&buffer);
				data=buffer[0];
				gm0_setlanguage(mygm,data-0x30);
				break;
			case 'V':
			case 'v':
				{
					struct gm_store store;
					store.value=gm0_getvalue(mygm);
					store.range=gm0_getrange(mygm);
					store.units=gm0_getunits(mygm);
					store.mode=gm0_getmode(mygm);
					callback(mygm,store);
				} //case v 
			break;
			case 'A':
			case 'a':
				printf("Doing Auto Zero \n");
				gm0_doaz(mygm);
				printf("Press enter to simulate enter key");
				scanf("%s",&buffer);
				data=buffer[0];
				printf("Before reset\n");
				gm0_resetnull(mygm);
				printf("AZ Done");
				break;
			case 'n':
			case 'N':

				gm0_donull(mygm);
				break;
			case '1':
				gm0_setcallback(mygm,&callback);
				break;
			case '0':
				gm0_setcallback(mygm,NULL);
				break;

			case 'i':
				printf("Enter 232 interval 1 upwards :");
				scanf("%s",&buffer);
				data=buffer[0];
				gm0_setinterval(mygm,data-0x30);
				break;
			case 'Q':
			case 'q':
				gm0_resetpeak(mygm);
				break;
			case 'T':
			case 't':

				gm0_sendtime(mygm,TRUE);				
				break;
			case 'Y':
			case 'y':
				gm0_sendtime(mygm,FALSE);				
				break;
			case 'G':
			case 'g':
				{
					struct gm_store mystore;
					int reg;
					gm0_startcmd(mygm);
					printf("Which register ?");
					scanf("%s",&buffer);
					reg=buffer[0]-0x30;
					mystore=gm0_getstore(mygm,reg);
	
					printf("%f"	,mystore.value);

					printf(" %s",units_str[mystore.units]);

					printf(" %s",mode_str[mystore.mode]);

					printf(" %s\n",range_str[mystore.range]);

					//printf("Value is %lf %d %d %d\n",mystore.value,mystore.units,mystore.range,mystore.mode);
					printf("Time stamp %d:%d:%d  %d/%d/%d\n",mystore.time.hour,mystore.time.min,mystore.time.sec,mystore.time.day,mystore.time.month,mystore.time.year);
					gm0_endcmd(mygm);
				}
				break;
		
			case 'h':
			case 'H':
				{	
				int startreg,endreg;
				int reg;
				struct gm_store mystore;
				gm0_startcmd(mygm);
				printf("\nStart register =");
				scanf("%s",&buffer);
				startreg=buffer[0]-0x30;
				
				printf("\nEnd register =");
				scanf("%s",&buffer);
				endreg=buffer[0]-0x30;
				//for(reg=startreg;reg<=endreg;reg++)
				reg = startreg;
				do
				{
					mystore=gm0_getstore(mygm,reg);

					if (mystore.mode != 255) //255 invalid
					{
						printf("%f",mystore.value);

						printf(" %s",units_str[mystore.units]);
						printf(" %s",mode_str[mystore.mode]);
						printf(" %s\n",range_str[mystore.range]);
						printf("Time stamp %d:%d:%d  %d/%d/%d\n",mystore.time.hour,mystore.time.min,mystore.time.sec,mystore.time.day,mystore.time.month,mystore.time.year);
					}	
					
				
				}
				while((reg++ < endreg) && (mystore.mode != 255)); //255 invalid data

				gm0_endcmd(mygm);

				}

				case '.':
			case '>':
				{
					setgmtime();								
				}
			break;
			case ',':
			case '<':
				{
					struct gm_time time;
					time=gm0_gettime(mygm);
					printf("GM TIME is %2d:%2d:%2d %2d/%2d/%2d\n",time.hour,time.min,time.sec,time.day,time.month,time.year);
					break;
				}
			case '9':
			{
				
				printf("Probe Serial %d\n",gm0_getprobeserial(mygm));
				printf("GM Serial %d\n",gm0_getgmserial(mygm));

				printf("Probe cal date %d\n",gm0_getprobecaldate(mygm));
				printf("Probe type %d\n",gm0_getprobetype(mygm));

			}
		}
	}
	
	gm0_killgm(mygm);
	return 0;
}


void showmenu()
{


printf("********************************************************************************\n");
printf("*                                                                              *\n");
printf("*                 GM05 Console control and DLL API Example                     *\n");
printf("*                                                                              *\n");
printf("********************************************************************************\n");
printf("\n");
printf(" (R) - Set Range        (U) - Set Units             (M) - Mode   \n");
printf(" (L) - language         (V) - Get Value             ( ) - Show menu\n");	
printf(" (A) - Auto Zero        (N) - Null                  (T) - Enable Time\n");	
printf(" (1) - Enable callback  (0) - Disable callback      (<) - Set GM Time\n");
printf(" (I) - Set interval     (Q) - Reset peak            (>) - Get GM Time\n");	
printf(" (W) - Power Off        (G) - Get reg               (H) - Get Reg(s)\n");
printf(" (B) - Reconnect        (X) - Exit                  (9) - Get E2\n");	
printf("\n\n");

}


void __stdcall callback(HANDLEGM hand,struct gm_store store)
{

	//verify callback data
	if(store.units>3 || store.range>7 || store.mode>4)
	{
		printf("Invalid reading returned\n");
		return;
	}

	printf("Reading is ");

	printf("%f",store.value);
	
	printf(" %s",units_str[store.units]);

	printf(" %s ",mode_str[store.mode]);
	printf(" %s\n",range_str[store.range]);

}

// WIN32 SPECIFIC DLL FUNCTION LOADED //
// This avoids all the visual c macros that make the code platform specfic
// In stead we can just load the pointers for windows and let ld do the work for us
// on linux same code (minus function loader for win32 systems)

#ifndef _LINUX

void loadallfunctions()
{
	HMODULE lib;

	lib=LoadLibrary("gm0.dll");
	
	if(lib==NULL)
	{
		printf("\nCould not load gm0.dll going to try to find debug version\n");
		lib=LoadLibrary("gm0D.dll");
		if(lib==NULL)
		{
			printf("Sorry neither gm0.dll or gm0D.dll was found in DLL search path\n");
			exit (-1);
		}
	}
	
	loadfunction(lib,&gm0_newgm,"gm0_newgm");
	loadfunction(lib,&gm0_killgm,"gm0_killgm");
	loadfunction(lib,&gm0_getrange,"gm0_getrange");
	loadfunction(lib,&gm0_getunits,"gm0_getunits");
	loadfunction(lib,&gm0_getvalue,"gm0_getvalue"); 
	loadfunction(lib,&gm0_getmode,"gm0_getmode"); 

	loadfunction(lib,&gm0_startconnect,"gm0_startconnect");
	loadfunction(lib,&gm0_getconnect,"gm0_getconnect");

	loadfunction(lib,&gm0_setrange,"gm0_setrange");
	loadfunction(lib,&gm0_setunits,"gm0_setunits");
	loadfunction(lib,&gm0_setmode,"gm0_setmode"); 
	loadfunction(lib,&gm0_setlanguage,"gm0_setlanguage"); 

	loadfunction(lib,&gm0_doaz,"gm0_doaz"); 
	loadfunction(lib,&gm0_resetnull,"gm0_resetnull"); 

	loadfunction(lib,&gm0_setcallback,"gm0_setcallback"); 

	loadfunction(lib,&gm0_setinterval,"gm0_setinterval"); 
	loadfunction(lib,&gm0_resetpeak,"gm0_resetpeak"); 
	loadfunction(lib,&gm0_sendtime,"gm0_sendtime"); 

	loadfunction(lib,&gm0_startcmd,"gm0_startcmd"); 
	loadfunction(lib,&gm0_endcmd,"gm0_endcmd"); 
	loadfunction(lib,&gm0_getstore,"gm0_getstore"); 


	loadfunction(lib,&gm0_gettime,"gm0_gettime"); 
	loadfunction(lib,&gm0_settime,"gm0_settime"); 

	loadfunction(lib,&gm0_getprobeserial,"gm0_getprobeserial"); 
	loadfunction(lib,&gm0_getgmserial,"gm0_getgmserial"); 
	loadfunction(lib,&gm0_getprobetype,"gm0_getprobetype"); 
	loadfunction(lib,&gm0_getprobecaldate,"gm0_getprobecaldate"); 

}

void loadfunction (HANDLE lib,void ** functionpointer,char * functionname)
{
 (* functionpointer)=GetProcAddress(lib,functionname);
 if(functionpointer==NULL)
 {
  printf("Error loading function %s \n",functionname);
  exit(-1);
 }
 printf("Function %s loaded @ %x OK\n",functionname,functionpointer);
 return;
}
#endif


int doconnect(int port,int mode)
{
	int iconnectcount;

	mygm=gm0_newgm(port,mode); 
	if(mygm>=0 && mygm<255)
	{
		printf(" SUCCESS, handle %d\n",mygm);
	}
	else
	{
		printf(" ERROR, error value %d\n",mygm);
		exit(-1);
	}
	
	printf("Attempting to initalise communications\n");
	gm0_startconnect(mygm);
	
  	iconnectcount=0;
	while(gm0_getconnect(mygm)==FALSE && iconnectcount <5)
  	{
		printf("Attempt %d of 5 has FAILED\n",iconnectcount);
		iconnectcount++;
		Sleep(1000);
	}
	
	if(gm0_getconnect(mygm)==FALSE)
	{
		printf("\n ** Communications have failed ... aborting ** \n");
		gm0_killgm(mygm);
		return -1;
	}
	else
	{
		printf("\n** Communications have succeded **\n");
	}

	return 0;
	
}


#ifdef _LINUX

void strdate(char * buffer)
{

    time_t curtime;
    struct tm * loctime;

    curtime= time(NULL);
    
    loctime=localtime(&curtime);
    strftime(buffer,256,"%D",loctime);
}


void strtime(char * buffer)
{

    time_t curtime;
    struct tm * loctime;

    curtime= time(NULL);
    
    loctime=localtime(&curtime);
    strftime(buffer,256,"%T",loctime);
}
        
#endif

void setgmtime()
{

    struct gm_time gaussmeter_time;
			    
    char dbuffer [256];
    char tbuffer [256];
    char tempbuffer[3];
    char * stop;

    tempbuffer[2]='\0';
				
    _strdate( dbuffer );
    printf( "The current date is %s (US Month Format)\n", dbuffer );
    _strtime( tbuffer );
    printf( "The current time is %s \n", tbuffer );
				
    tempbuffer[0]=tbuffer[0];
    tempbuffer[1]=tbuffer[1];
    gaussmeter_time.hour=strtol(tempbuffer,&stop,10);
    tempbuffer[0]=tbuffer[3];
    tempbuffer[1]=tbuffer[4];
    gaussmeter_time.min=strtol(tempbuffer,&stop,10);
    tempbuffer[0]=tbuffer[6];
    tempbuffer[1]=tbuffer[7];
    gaussmeter_time.sec=strtol(tempbuffer,&stop,10);
    tempbuffer[0]=dbuffer[3];
    tempbuffer[1]=dbuffer[4];
    gaussmeter_time.day=strtol(tempbuffer,&stop,10);
    tempbuffer[0]=dbuffer[0];
    tempbuffer[1]=dbuffer[1];
    gaussmeter_time.month=strtol(tempbuffer,&stop,10);
    tempbuffer[0]=dbuffer[6];
    tempbuffer[1]=dbuffer[7];
    gaussmeter_time.year=strtol(tempbuffer,&stop,10);


    printf("Setting GM time to %d:%d:%d %d/%d/%d \n",gaussmeter_time.hour,gaussmeter_time.min,gaussmeter_time.sec,gaussmeter_time.day,gaussmeter_time.month,gaussmeter_time.year);
    gm0_settime(mygm,gaussmeter_time);


}
>>>>>>> a7f7fcefe8571222392baf17e1f007d1ccacb12c
