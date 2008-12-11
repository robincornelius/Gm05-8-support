/*

   Copyright © 2008 Hirst Magnetic Instruments Ltd and Robin Cornelius.

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

// This file provides .Net wrappers for gm0.dll for access in C# VB.Net etc 

using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;

namespace gm0_sharp
{
    public enum connectiontype
    {
        Connection_Serial,
        Connection_USB  
    }

   // A struct for looking after gm_time responses
    public struct gm_time{
    public byte sec;
    public byte min;
    public byte hour;
    public byte day;
    public byte month;
    public byte year;
    };

    // A struct for dealing with data in the registers
    public struct gm_store{
     public gm_time time;
     public byte range;
     public byte mode;
     public byte units;
     public float value;
    };

    public class gm0
    {
        connectiontype contype;
        int connectionport;
        int hand=-1;

        public gm_store lastreading;

        static void Main()
        {
           
        }

        [DllImport("gm0.dll")]
        static extern int gm0_newgm(int port, int mode);
        public gm0(connectiontype contype,int port)
        {
            contype = contype;
            connectionport = port;
            
            hand=gm0_newgm(port,contype==connectiontype.Connection_Serial?0:1);
            checkvalidhandorthrow();

            gm0_setconnectcallback(hand, connectedcallback);
            gm0_setcallback(hand, datacallback);
            gm0_setnullcallback(hand, nullcallback);
        }

        [DllImport("gm0.dll")]
        static extern int gm0_killgm(int hand);
        public void terminate()
        {
            checkvalidhandorthrow();
            gm0_killgm(hand);
            hand = -1;
        }

        [DllImport("gm0.dll")]
        static extern int gm0_startconnect(int hand);
        public void StartConnect()
        {
            checkvalidhandorthrow();
            gm0_startconnect(hand);
        }

        [DllImport("gm0.dll")]
        static extern int gm0_getconnect(int hand);
        public bool GetConnectionStatus()
        {
            if(hand<0)
                return false;

            int status = gm0_getconnect(hand);
            return (status==1?true:false);
        }

       


        delegate void dllConnectedCallBack();
        [DllImport("gm0.dll")]
        static extern int gm0_setconnectcallback(int hand, dllConnectedCallBack x);
        public delegate void ConnectedCallback();
        public event ConnectedCallback onConnectedCallback;
        void connectedcallback()
        {
            if(onConnectedCallback!=null)
            {
                try
                {
                    onConnectedCallback();
                }
                catch
                {

                }
            }
        }


        public delegate void NewValueCallback(float value);
        public event NewValueCallback onNewValue;

        public delegate void NewRangeCallback(int range);
        public event NewRangeCallback onNewRange;

        public delegate void NewModeCallback(int mode);
        public event NewModeCallback onNewMode;

        public delegate void NewUnitsCallback(int units);
        public event NewUnitsCallback onNewUnits;

        public delegate void DataCallBack(int hand, gm_store data);
        [DllImport("gm0.dll")]
        public static extern int gm0_setcallback(int hand, DataCallBack x);

        void datacallback(int hand, gm_store data)
        {
            // new data avaiable

            lastreading.value = data.value;
            if (this.onNewValue != null)
                onNewValue(data.value);

            if (lastreading.range != data.range)
            {
                lastreading.range = data.range;
                if (this.onNewRange != null)
                    onNewRange(data.range);
            }

            if (lastreading.mode != data.mode)
            {
                lastreading.mode = data.mode;
                if (this.onNewMode != null)
                    onNewMode(data.mode);
            }

            if (lastreading.units != data.units)
            {
                lastreading.units = data.units;
                if (this.onNewUnits != null)
                    onNewUnits(data.units);
            }

        }

        [DllImport("gm0.dll")]
        static extern int gm0_setlanguage(int hand, byte lan);
        public void setMeterLanguage(byte lan)
        {
            checkvalidhandorthrow();
            gm0_setlanguage(hand, lan);
        }

        [DllImport("gm0.dll")]
        static extern int gm0_getlanguage(int hand);
        public byte GetMeterLanguage()
        {
            checkvalidhandorthrow();
            return (byte)gm0.gm0_getlanguage(hand);
        }


        [DllImport("gm0.dll")]
        static extern int gm0_setrange(int hand, byte range);
        public void setrange(byte range)
        {
            checkvalidhandorthrow();
            gm0_setrange(hand, range);
        }

        [DllImport("gm0.dll")]
        static extern int gm0_setmode(int hand, byte mode);
        public void setmode(byte mode)
        {
            checkvalidhandorthrow();
            gm0_setunits(hand, mode);
        }

        [DllImport("gm0.dll")]
        static extern int gm0_setunits(int hand, byte units);
        public void setunits(byte range)
        {
            checkvalidhandorthrow();
            gm0_setmode(hand, range);
        }

        [DllImport("gm0.dll")]
        static extern int gm0_resetpeak(int hand);
        public void resetpeak()
        {
             checkvalidhandorthrow();
             gm0_resetpeak(hand);

        }

        [DllImport("gm0.dll")]
        static extern int gm0_donull(int hand);
        public void donull()
        {
            checkvalidhandorthrow();
            gm0.gm0_donull(hand);

        }

        [DllImport("gm0.dll")]
        static extern int gm0_doaz(int hand);
        public void autozero()
        {
             checkvalidhandorthrow();
             gm0_doaz(hand);

        }

        [DllImport("gm0.dll")]
        static extern int gm0_resetnull(int hand);
        public void resetnull()
        {


        }

        delegate void dllNullCallBack();
        [DllImport("gm0.dll")]
        static extern int gm0_setnullcallback(int hand, dllNullCallBack x);
        void nullcallback()
        {
            

        }

        [DllImport("gm0.dll")]
        static extern int gm0_sendtime(int hand, bool extended);
        public void send_time_with_value()
        {
            checkvalidhandorthrow();

        }

        [DllImport("gm0.dll")]
        static extern int gm0_settime(int hand, gm_time time);
        public void set_meter_time(DateTime time)
        {
            checkvalidhandorthrow();
            gm_time gtime = new gm_time();
            gtime.day = (byte)time.Day;
            gtime.month = (byte)time.Month;
            gtime.year = (byte)(time.Year - 2000);
            gtime.hour = (byte)time.Hour;
            gtime.min = (byte)time.Minute;
            gtime.sec = (byte)time.Second;
            gm0_settime(hand, gtime);

        }

        [DllImport("gm0.dll")]
        static extern gm_time gm0_gettime(int hand);
        public DateTime get_meter_time()
        {
            checkvalidhandorthrow();
            gm_time gtime = new gm_time();
            gtime = gm0_gettime(hand);
            DateTime time = new DateTime(gtime.year+2000,gtime.month,gtime.day,gtime.hour,gtime.min,gtime.sec);
            return time;
        }

        [DllImport("gm0.dll")]
        static extern gm_store gm0_getstore(int hand, int pos);
        public gm_store get_saved_value(int pos)
        {
            checkvalidhandorthrow();
            return gm0_getstore(hand, pos);
        }

        [DllImport("gm0.dll")]
        static extern int gm0_simkey(int hand, byte keycode);
        public void simkey(byte key)
        {
            checkvalidhandorthrow();
            gm0_simkey(hand, key);
        }

        [DllImport("gm0.dll")]
        static extern int gm0_setinterval(int hand, int interval);
        public void SetDataRate(int rate)
        {
            checkvalidhandorthrow();
            gm0_setinterval(hand, rate);
        }

        [DllImport("gm0.dll")]
        static extern int gm0_fastUSBpoll(int hand, int enabled);
        public void enable_fastUSBpoll(bool enabled)
        {
            checkvalidhandorthrow();
            gm0_fastUSBpoll(hand, enabled==true?1:0);
        }

        [DllImport("gm0.dll")]
        static extern void gm0_sampleondemand(int hand);
        public void enable_sampleOnDemand()
        {
            checkvalidhandorthrow();
            gm0_sampleondemand(hand);
        }

        [DllImport("gm0.dll")]
        static extern gm_store gm0_demandsample(int hand, int extra);
        public gm_store demand_sample(bool extended)
        {
            checkvalidhandorthrow();
            return(gm0_demandsample(hand,extended==true?1:0));
        }


        [DllImport("gm0.dll")]
        static extern UInt16 gm0_getgmserial(int hand);
        public UInt16 GetMeterSerial()
        {
            checkvalidhandorthrow();
            return gm0_getgmserial(hand);

        }

        [DllImport("gm0.dll")]
        static extern UInt16 gm0_getprobeserial(int hand);
        public UInt16 GetProbeSerial()
        {
            checkvalidhandorthrow();
            return gm0_getprobeserial(hand);
        }

        [DllImport("gm0.dll")]
        static extern UInt16 gm0_getprobetype(int hand);
        public UInt16 GetProbeType()
        {
            checkvalidhandorthrow();
            return gm0_getprobetype(hand);
        }

        [DllImport("gm0.dll")]
        static extern UInt16 gm0_getprobecaldate(int hand);
        public UInt16 GetProbeCalDate()
        {
            checkvalidhandorthrow();
            return gm0_getprobecaldate(hand);
        }



        public void checkvalidhandorthrow()
        {
            if(hand==-1)
            {
                Exception e=new Exception("Attempt to access a gm0 function with an invalid handle");
                throw(e);
             }
        }

    }
}
