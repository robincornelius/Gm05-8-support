
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


#define NOVC 

#ifdef _LINUX
	#define HMODULE int
	#define CLOCKS_PER_SEC 1
#else
	#include <windows.h>
	#define CLOCKS_PER_SEC 1000
#endif

#include <stdio.h>
#include "gm0.h"

void loadfunction (HANDLE lib,void ** functionpointer,char * functionname);

int main(int argv, char * argvc[])
{
	HANDLEGM mygm;
	double value;
	struct gm_store store;	
	int port;
	int iconnectcount;

  	printf("Which comm port would you like to use?\nUnder linux this translates to /dev/ttySn where n is what you are about to type\n=");
  	scanf("%d",&port);
 
  	if(port<0 || port >255)
  	{
		printf("\nDon't be silly, comport %d is out of range!\n",port);
		return 0;
	}
	
	printf("\nAttemptng to initalise a new instance of the gm0library functions ....");
	mygm=gm0_newgm(1); 
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
		printf("Communications have failed ... aborting\n");
		gm0_killgm(mygm);
		exit(-1);
	}
	else
	{
		printf("Communications have succeded\n");
	}
		
	store.value=gm0_getvalue(mygm);
	store.range=gm0_getrange(mygm);
	store.units=gm0_getunits(mygm);
	
	value=gm0_getvalue(mygm);
	printf("Reading %lf range %d % units %d\n",store.value,store.range,store.units);
	
	gm0_killgm(mygm);


}

// WIN32 SPECIFIC DLL FUNCTION LOADED //

#ifndef _LINUX

void loadallfunctions()
{
	HMODULE lib;
	lib=LoadLibrary("gm0.dll");
	
	if(lib==NULL)
	{
	printf("\nCould not load gm0.dll, Exiting\n");
	exit(-1);
	}
	
	loadfunction(lib,&gm0_newgm,"_gm0_newgm@4");
	loadfunction(lib,&gm0_killgm,"_ gm0_killgm@4");
	loadfunction(lib,&gm0_getvalue,"_ gm0_getvalue@4");
	loadfunction(lib,&gm0_startconnect,"_ gm0_startconnect@4");
	
}

void loadfunction (HANDLE lib,void ** functionpointer,char * functionname)
{
 (* functionpointer)=GetProcAddress(lib,functionname);
 if(functionpointer==NULL)
 {
  printf("Error loading function %s \n",functionname);
  exit(-1);
 }
 printf("Function %s loaded @ %d OK\n",functionname,functionpointer);
 return;
}
#endif
