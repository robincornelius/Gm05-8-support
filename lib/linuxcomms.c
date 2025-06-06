/*

   This code was copied from the MIDAS project http://midas.psi.ch/ which has graciously 
   released its code under the GPL the original author is Stefan Ritt. The file has been 
   modified by Hirst Magnetic Instruments Ltd./*

   Copyright � Stefan Ritt
   Copyright � 2004 Hirst Magnetic Instruments Ltd
   Copyright � 2004 Robin Cornelius.

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

#ifdef _LINUX

#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/timeb.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdarg.h>
#include "comms.h"

#define TRUE 1
#define FALSE 0
#define min(x,y) ((x) < (y) ? (x) : (y))  /* this is not type safe and naughty etc but i don't care!*/

#define debug_flag 0

int rs232_open(char *port, int baud, char parity, int data_bit, int stop_bit, int flow_control)
{
int  fd, i;
struct termios tio;

struct {
  int speed;
  int code;
} baud_table[] = {
  {300,  B300}, 
  {600,  B600}, 
  {1200, B1200},
  {1800, B1800}, 
  {2400, B2400}, 
  {4800, B4800}, 
  {9600, B9600},
  {19200,B19200}, 
  {38400,B38400}, 
  {0,0}
};


  printf("************************************\n");

  fd = open(port, O_RDWR);
  if (fd < 0)
    {
    perror("rs232_open");
    return fd;
    }

  tio.c_iflag = 0;
  tio.c_oflag = 0;
  tio.c_cflag = CREAD | CLOCAL;
  if (data_bit == 7)
    tio.c_cflag |= CS7;
  else
    tio.c_cflag |= CS8;

  if (stop_bit == 2)
    tio.c_cflag |= CSTOPB;

  if (parity == 'E')
    tio.c_cflag |= PARENB;
  if (parity == 'O')
    tio.c_cflag |= PARENB | PARODD;

  if (flow_control == 1)
    tio.c_cflag |= CRTSCTS;
  if (flow_control == 2)
    tio.c_iflag |= IXON | IXOFF; 

  tio.c_lflag = 0;
  tio.c_cc[VMIN] = 1;
  tio.c_cc[VTIME] = 0;

  /* check baud argument */
  for (i=0 ; baud_table[i].speed ; i++)
    {
    if (baud == baud_table[i].speed)
      break;
    }

  if (!baud_table[i].speed)
    i = 6; /* 9600 baud by default */

  cfsetispeed(&tio, baud_table[i].code);
  cfsetospeed(&tio, baud_table[i].code);

  tcsetattr(fd, TCSANOW, &tio);

  return fd;
}

/*----------------------------------------------------------------------------*/

int rs232_exit(RS232_INFO *info)
{
  close(info->fd);

  return SUCCESS;
}

/*----------------------------------------------------------------------------*/

int rs232_write(RS232_INFO *info, char *data, int size)
{
int i;
 
  //printf("write: %x\n",data[0]);

  if (debug_flag)
    {
    FILE *f;
 
    f = fopen("rs232.log", "a");
    fprintf(f, "write: ");
    for (i=0 ; (int)i<size ; i++)
      fprintf(f, "%X ", data[i]);
    fprintf(f, "\n");
    fclose(f);
    }
 
  i = write(info->fd, data, size);

  return i;
}

/*----------------------------------------------------------------------------*/ 

int rs232_read(RS232_INFO *info, char *str, int size, int timeout)
{
int    i, l;
struct timeb start, now;
double fstart, fnow; 

  ftime(&start);
  fstart = start.time+start.millitm/1000.0;
  
  memset(str, 0, size);
  for (l=0 ; l<=size-1 ;)
    {
    ioctl(info->fd, FIONREAD, &i);
    if (i > 0)
      {
      i = read(info->fd, str+l, 1);
 
      if (i == 1)
        l++;
      else
        perror("read");
      }
 
    ftime(&now);
    fnow = now.time+now.millitm/1000.0;

    if (fnow - fstart >= timeout/1000.0)
      break;
 
    if (i == 0)
      usleep(min(100000, timeout*1000));
    }
 
  if (debug_flag && l>0)
    {
    FILE *f;

    f = fopen("rs232.log", "a");
    fprintf(f, "read: ");
    for (i=0 ; i<size ; i++)
      fprintf(f, "%X ", str[i]);
    fprintf(f, "\n");
    fclose(f);
    }

  if(i==0)
  {
	return -1;  
  }
  else
 // printf("read: %x\n",str[0]);
  
  return i;
}

/*----------------------------------------------------------------------------*/

int rs232_puts(RS232_INFO *info, char *str)
{
int i;
 
  if (debug_flag)
    {
    FILE *f;
 
    f = fopen("rs232.log", "a");
    fprintf(f, "puts: %s\n", str);
    fclose(f);
    }
 
  i = write(info->fd, str, strlen(str));

  return i;
}

/*----------------------------------------------------------------------------*/ 

int rs232_gets(RS232_INFO *info, char *str, int size, char *pattern, int timeout)
{
int    i, l;
struct timeb start, now;
double fstart, fnow; 

  ftime(&start);
  fstart = start.time+start.millitm/1000.0;
  
  memset(str, 0, size);
  for (l=0 ; l<size-1 ;)
    {
    ioctl(info->fd, FIONREAD, &i);
    if (i > 0)
      {
      i = read(info->fd, str+l, 1);
 
      if (i == 1)
        l++;
      else
        perror("read");
      }
 
    if (pattern && pattern[0])
      if (strstr(str, pattern) != NULL)
        break;
 
    ftime(&now);
    fnow = now.time+now.millitm/1000.0;

    if (fnow - fstart >= timeout/1000.0)
      {
      if (pattern && pattern[0])
        return 0;
      break;
      }
 
    if (i == 0)
      usleep(min(100000, timeout*1000));

    };
 
  if (debug_flag && l>0)
    {
    FILE *f;

    f = fopen("rs232.log", "a");
    fprintf(f, "getstr %s: %s\n", pattern, str);
    fclose(f);
    }
 
  return l;
}

#endif

/*----------------------------------------------------------------------------*/
