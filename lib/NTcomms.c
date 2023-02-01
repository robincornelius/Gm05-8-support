/*

   This code was copied from the MIDAS project http://midas.psi.ch/ which has graciously
   released its code under the GPL the original author is Stefan Ritt. The file has been
   modified by Hirst Magnetic Instruments Ltd./*

   Copyright © Stefan Ritt and Hirst Magnetic Instruments Ltd and Robin Cornelius.

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

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include "comms.h"
static int debug_flag;

void rs232_enable_debug(void)
{
	debug_flag = 1;
}

void rs232_disable_debug(void)
{
	debug_flag = 0;
}


int rs232_open(char* port, int baud, char parity, int data_bit, int stop_bit, int flow_control)
{
	char          str[80];
	DCB           dcb;
	HANDLE        hDev;
	COMMTIMEOUTS  CommTimeOuts;

	sprintf(str, "\\\\.\\%s", port);

	hDev = CreateFile(str, GENERIC_READ | GENERIC_WRITE,
		0,                    // exclusive access
		NULL,                 // no security attrs
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL |
		0,
		NULL);

	if (hDev == (HANDLE)-1)
		return -1;

	GetCommState(hDev, &dcb);

	sprintf(str, "baud=%d parity=%c data=%d stop=%d", baud, parity, data_bit, stop_bit);
	BuildCommDCB(str, &dcb);

	if (flow_control == 1)
	{
		dcb.fOutX = dcb.fInX = FALSE;
		//dcb.fDtrControl = DTR_CONTROL_HANDSHAKE;
		//dcb.fRtsControl = RTS_CONTROL_HANDSHAKE;
		dcb.fOutxCtsFlow = dcb.fOutxDsrFlow = TRUE;
	}
	else if (flow_control == 2)
	{
		dcb.fOutX = dcb.fInX = TRUE;
		dcb.fOutxCtsFlow = dcb.fOutxDsrFlow = FALSE;
	}
	else
	{
		dcb.fOutX = dcb.fInX = FALSE;
		dcb.fOutxCtsFlow = dcb.fOutxDsrFlow = FALSE;
	}

	if (SetCommState(hDev, &dcb) == FALSE)
		return -1;

	SetupComm(hDev, 4096, 4096);

	CommTimeOuts.ReadIntervalTimeout = 0;
	CommTimeOuts.ReadTotalTimeoutMultiplier = 0;
	CommTimeOuts.ReadTotalTimeoutConstant = 0;
	CommTimeOuts.WriteTotalTimeoutMultiplier = 0;
	CommTimeOuts.WriteTotalTimeoutConstant = 5000;

	SetCommTimeouts((HANDLE)hDev, &CommTimeOuts);

	return (INT)hDev;
}

/*----------------------------------------------------------------------------*/

INT rs232_exit(RS232_INFO* info)
{
	EscapeCommFunction((HANDLE)info->fd, CLRDTR);

	PurgeComm((HANDLE)info->fd, PURGE_TXABORT | PURGE_RXABORT |
		PURGE_TXCLEAR | PURGE_RXCLEAR);
	CloseHandle((HANDLE)info->fd);

	return SUCCESS;
}

/*----------------------------------------------------------------------------*/

int rs232_write(RS232_INFO* info, char* data, int size)
{
	DWORD written, i;

	if (debug_flag)
	{
		FILE* f;

		f = fopen("c:\\rs232.log", "a");
		fprintf(f, "write: ");
		for (i = 0; (int)i < size; i++)
			fprintf(f, "%X ", data[i]);
		fprintf(f, "\n");
		fclose(f);
	}

	WriteFile((HANDLE)info->fd, data, size, &written, NULL);
	return written;
}

/*----------------------------------------------------------------------------*/

int rs232_read(RS232_INFO* info, char* data, int size, int timeout)
{
	int           status, i, l;
	COMMTIMEOUTS  CommTimeOuts;

	GetCommTimeouts((HANDLE)info->fd, &CommTimeOuts);

	CommTimeOuts.ReadIntervalTimeout = 0;
	CommTimeOuts.ReadTotalTimeoutMultiplier = 0;
	CommTimeOuts.ReadTotalTimeoutConstant = timeout;

	SetCommTimeouts((HANDLE)info->fd, &CommTimeOuts);

	memset(data, 0, size);
	for (l = 0; l < size; l++)
	{
		status = ReadFile((HANDLE)info->fd, data + l, 1, &i, NULL);
		if (!status || i == 0)
			break;
	}

	if (!status || i == 0)
		return -1;

	if (debug_flag)
	{
		FILE* f;

		f = fopen("c:\\rs232.log", "a");
		fprintf(f, "read: ");
		for (i = 0; i < size; i++)
			fprintf(f, "%X ", data[i]);
		fprintf(f, "\n");
		fclose(f);
	}

	return l;
}

/*----------------------------------------------------------------------------*/

int rs232_puts(RS232_INFO* info, char* str)
{
	DWORD written;

	if (debug_flag)
	{
		FILE* f;

		f = fopen("rs232.log", "a");
		fprintf(f, "puts: %s\n", str);
		fclose(f);
	}

	WriteFile((HANDLE)info->fd, str, strlen(str), &written, NULL);
	return written;
}

/*----------------------------------------------------------------------------*/

int rs232_gets(RS232_INFO* info, char* str, int size, char* pattern, int timeout)
{
	int           status, i, l;
	COMMTIMEOUTS  CommTimeOuts;

	GetCommTimeouts((HANDLE)info->fd, &CommTimeOuts);

	CommTimeOuts.ReadIntervalTimeout = 0;
	CommTimeOuts.ReadTotalTimeoutMultiplier = 0;
	CommTimeOuts.ReadTotalTimeoutConstant = timeout;

	SetCommTimeouts((HANDLE)info->fd, &CommTimeOuts);

	memset(str, 0, size);
	for (l = 0; l < size - 1; l++)
	{
		status = ReadFile((HANDLE)info->fd, str + l, 1, &i, NULL);
		if (!status || i == 0)
		{
			if (pattern && pattern[0])
				return 0;
			break;
		}

		if (pattern && pattern[0])
			if (strstr(str, pattern) != NULL)
				break;
	}

	if (debug_flag && l > 0)
	{
		FILE* f;

		f = fopen("rs232.log", "a");
		fprintf(f, "gets %s: %s\n", pattern, str);
		fclose(f);
	}

	return l;
}


#endif