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
 #include "windows.h"
#endif

#ifndef __IFM01COMMSH__
#define __IFM01COMMSH__

#define NAME_LENGTH 255
#define SUCCESS TRUE

typedef struct {
  char port[NAME_LENGTH];
  int  baud;
  char parity;
  int  data_bit;
  int  stop_bit;
  int  flow_control;
} RS232_SETTINGS;

#define RS232_SETTINGS_STR "\
RS232 Port = STRING : [32] com1\n\
Baud = INT : 9600\n\
Parity = CHAR : N\n\
Data Bit = INT : 8\n\
Stop Bit = INT : 1\n\
Flow control = INT : 0\n\
"

typedef struct {
  RS232_SETTINGS settings;
  int fd;                         /* device handle for RS232 device */
} RS232_INFO;


#endif

int rs232_open(char *port, int baud, char parity, int data_bit, int stop_bit, int flow_control);
int rs232_exit(RS232_INFO *info);
int rs232_write(RS232_INFO *info, char *data, int size);
int rs232_read(RS232_INFO *info, char *data, int size, int timeout);
int rs232_puts(RS232_INFO *info, char *str);
int rs232_gets(RS232_INFO *info, char *str, int size, char *pattern, int timeout);

