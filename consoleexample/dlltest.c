
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

/*
 This code is compatable with win32 systems and POSIX systems (eg linux etc) 
 To run on linux make sure you compile with the _LINUX flag defined this will
 patch some of the gaps between win32 and linux, mainly a few defines and the 
 method of dynamic library loading.
*/


#define NOVC 

/* This define avoids using Visual C stuff and compiles as a true win32 console app without
visual C extensions
*/


#ifdef _LINUX
	#define HMODULE int
	#define CLOCKS_PER_SEC 1
#else
	#include <windows.h>
	#define CLOCKS_PER_SEC 1000
#endif

#include <stdio.h>
#include <time.h>
#include "../lib/gm0.h"

void loadfunction (HANDLE lib,void ** functionpointer,char * functionname);
void loadallfunctions();
void showmenu();
void __stdcall callback(HANDLEGM hand,struct gm_store store);

const struct units_struct units_range_conversion[4]={

						1.0,							
						1000.0, 	"% 05.3f  T ",	//was 10 000 think it was fudge for /10
					 	10.0, 		"% 05.1f mT ",
					 	100.0, 		"% 05.2f mT ",
					 	1000.0,		"% 05.3f mT ",

						1.0,
					 	100.0,		"% 05.2f kG ",
						1000.0,		"% 05.3f kG ",
						10.0,		"% 05.1f  G ",
						100.0,		"% 05.2f  G ",

						0.7957747,
						1.0,		"% 04.0f kA/m",
						10.0,		"% 05.1f kA/m",
						100.0,		"% 05.2f kA/m",
						1000.0, 	"% 05.3f kA/m",

						1.0,
						100.0,		"% 05.2f kOe ",
						1000.0,		"% 05.3f kOe ",
						10.0,		"% 05.1f Oe ",
						100.0, 		"% 05.2f Oe "
						
};

const struct units_struct units_range_conversion_baseunits[4]={

						1.0,							
						1000.0, 	"% 05.3f  T ",	//was 10 000 think it was fudge for /10
					 	10000.0, 		"% 05.4f  T ",
					 	100000.0, 		"% 05.5f  T ",
					 	1000000.0,		"% 05.6f  T ",

						1.0,
					 	0.1,		"% 06.0f  G ",
						1.0,		"% 05.0f  G ",
						10.0,		"% 05.1f  G ",
						100.0,		"% 05.2f  G ",

						0.7957747,
						1.0,		"% 04.0f kA/m",
						10.0,		"% 05.1f kA/m",
						100.0,		"% 05.2f kA/m",
						1000.0, 	"% 05.3f kA/m",

						1.0,
						0.1,		"% 05.2f Oe ",
						1.0,		"% 05.3f Oe ",
						10.0,		"% 05.4f Oe ",
						100.0, 		"% 05.5f Oe "
						
};

	/* Descriptions of the display modes */
	char *mode_str[5] =
    {
		"DC     ",
		"DC Peak",
		"AC     ",
		"AC MAX ",
		"AC Peak"
    };



int main(int argv, char * argvc[])
{
	HANDLEGM mygm;
	double value;
	struct gm_store store;	
	int port;
	int iconnectcount;
	char cmd,data;

#ifndef _LINUX

	printf("\nAttemptng to initalise a new instance of the gm0library functions ....\n");
	loadallfunctions();
	printf("\n");
#endif

  	printf("Which comm port would you like to use (COMn or /dev/ttySn) ?\n=");
  	scanf("%d",&port);
	if(port<-5 || port>254) 
  	{
		printf("\nDon't be silly, comport %d is out of range!\n",port);
		return 0;
	}
	

	mygm=gm0_newgm(port); 
	if(mygm>=0 || mygm<255)
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
	while(gm0_getconnect(mygm)==FALSE && iconnectcount <10)
  	{
		printf("Attempt %d of 10 has FAILED\n",iconnectcount);
		iconnectcount++;
		Sleep(CLOCKS_PER_SEC*1);
	}
	
	if(gm0_getconnect(mygm)==FALSE)
	{
		printf("\n ** Communications have failed ... aborting ** \n");
		gm0_killgm(mygm);
		exit(-1);
	}
	else
	{
		printf("\n** Communications have succeded **\n");
	}

	showmenu();

	cmd=0;

	while(cmd!='X' && cmd!='x')
	{
		printf("Enter option :");
		scanf("%c",&cmd);
		printf("\n");
		switch(cmd)
		{
			case 'r':
			case 'R':
				printf("Enter range 0-4 :");
				scanf("%d",&data);
				gm0_setrange(mygm,data);
				break;
			case 'U':
			case 'u':
				printf("Select Units 0-3 :");
				scanf("%d",&data);

				gm0_setunits(mygm,data);
				break;
			case 'M':
			case 'm':
				printf("Select mode 0-5 :");
				scanf("%d",&data);
				gm0_setmode(mygm,data);
				break;
			case ' ':
				showmenu();	
				break;
			case 'L':
			case 'l':
				printf("Select language 0-5 :");
				scanf("%d",&data);
				gm0_setlanguage(mygm,data);
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
				scanf("%d",&data);
				printf("Before reset\n");
				gm0_resetnull(mygm);
				printf("AZ Done");
				break;
			case '1':
				gm0_setcallback(mygm,&callback);
				break;
			case '0':
				gm0_setcallback(mygm,NULL);
				break;

			case 'i':
				printf("Enter 232 interval 1 upwards :");
				scanf("%d",&data);
				gm0_setinterval(mygm,data);
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
					scanf("%d",&reg);
					mystore=gm0_getstore(mygm,reg);
	
					printf(units_range_conversion[mystore.units].unit_range[mystore.range].formatter
						,mystore.value);

					printf(" %s \n",mode_str[mystore.mode]);


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
				scanf("%d",&startreg);
				printf("\nEnd register =");
				scanf("%d",&endreg);
				//for(reg=startreg;reg<=endreg;reg++)
				reg = startreg;
				do
				{
					mystore=gm0_getstore(mygm,reg);

					if (mystore.mode != 255) //255 invalid
					{
						//printf("Reg %d is %lf %d %d %d\n",reg,mystore.value,mystore.units,mystore.range,mystore.mode);
						printf(units_range_conversion[mystore.units].unit_range[mystore.range].formatter
								,mystore.value);

						printf(" %s \n",mode_str[mystore.mode]);
						printf("Time stamp %d:%d:%d  %d/%d/%d\n",mystore.time.hour,mystore.time.min,mystore.time.sec,mystore.time.day,mystore.time.month,mystore.time.year);
					}	
					
				
				}
				while((reg++ < endreg) && (mystore.mode != 255)); //255 invalid data

				gm0_endcmd(mygm);

				}

				case '.':
			case '>':
				{
					char dbuffer [256];
					char tbuffer [256];
					char tempbuffer[3];
					struct gm_time time;
					char * stop;
		
					tempbuffer[2]='\0';
				
					_strdate( dbuffer );
					printf( "The current date is %s (US Month Format)\n", dbuffer );
					_strtime( tbuffer );
					printf( "The current time is %s \n", tbuffer );
					//settosystime
				
					tempbuffer[0]=tbuffer[0];
					tempbuffer[1]=tbuffer[1];
					time.hour=strtol(tempbuffer,&stop,10);
					tempbuffer[0]=tbuffer[3];
					tempbuffer[1]=tbuffer[4];
					time.min=strtol(tempbuffer,&stop,10);
					tempbuffer[0]=tbuffer[6];
					tempbuffer[1]=tbuffer[7];
					time.sec=strtol(tempbuffer,&stop,10);

					tempbuffer[0]=dbuffer[3];
					tempbuffer[1]=dbuffer[4];
					time.day=strtol(tempbuffer,&stop,10);
					tempbuffer[0]=dbuffer[0];
					tempbuffer[1]=dbuffer[1];
					time.month=strtol(tempbuffer,&stop,10);
					tempbuffer[0]=dbuffer[6];
					tempbuffer[1]=dbuffer[7];
					time.year=strtol(tempbuffer,&stop,10);

					printf("Setting GM time to %d:%d:%d %d/%d/%d \n",time.hour,time.min,time.sec,time.day,time.month,time.year);
					gm0_settime(mygm,time);
								
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

	/*
struct unit_range_struct {

	float range_div;
	char *formatter;
	//char *unit_string;
};

struct	units_struct {

	float global_mult;
	struct unit_range_struct unit_range[4];

};
*/
/*
const struct units_struct units_range_conversion[4]={

						1.0,							
						1000.0, 	"% 05.3f  T ",	//was 10 000 think it was fudge for /10
					 	10.0, 		"% 05.1f mT ",
					 	100.0, 		"% 05.2f mT ",
					 	1000.0,		"% 05.3f mT ",

						1.0,
					 	100.0,		"% 05.2f kG ",
						1000.0,		"% 05.3f kG ",
						10.0,		"% 05.1f  G ",
						100.0,		"% 05.2f  G ",

						0.7957747,
						1.0,		"% 04.0f kA/m",
						10.0,		"% 05.1f kA/m",
						100.0,		"% 05.2f kA/m",
						1000.0, 	"% 05.3f kA/m",

						1.0,
						100.0,		"% 05.2f kOe ",
						1000.0,		"% 05.3f kOe ",
						10.0,		"% 05.1f Oe ",
						100.0, 		"% 05.2f Oe "
						
};
*/

	char *units_str[4][4] =
  {
	  {" T  ", "mT  ", "mT  ", "mT  "},
	  {"kG  ", "kG  ", "G   ", "G   "},
	  {"kA/m", "kA/m", "kA/m", "kA/m"},
	  {"kOe ", "kOe ", "Oe  ", "Oe  "}
  };




/* The DP position indexed by [units][range] */
	int DP_position[4][4] =
	{
	  {4, 2, 3, 4},
	  {3, 4, 2, 3},
	  {1, 2, 3, 4},
	  {3, 4, 2, 3}
	 };

	/* The divisor for each DP position */
	int divisor[5] =
	{
		1, 1, 10, 100, 1000
	};

	//units_range_conversion[pGMS[hand]->store.units].unit_range[pGMS[hand]->store.range].formatter

	printf("Reading is ");
	
	printf(units_range_conversion_baseunits[store.units].unit_range[store.range&0x03].formatter
			,store.value);

	printf(" %s \n",mode_str[store.mode]);

}



// WIN32 SPECIFIC DLL FUNCTION LOADED //
// This avoids all the visual c macros that make the code platform specfic
// In stead we can just load the pointers for windows and let ld do the work for us
// on linux same code (minus function loader for win32 systems)

#ifndef _LINUX

void loadallfunctions()
{
	HMODULE lib;
	lib=LoadLibrary("gm0D.dll");
	
	if(lib==NULL)
	{
	printf("\nCould not load gm0.dll, Exiting\n");
	exit(-1);
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
