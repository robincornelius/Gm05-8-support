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
using System.IO;

namespace gm0_sharp
{
    public enum faults
    {
        OK=0,
        MEMORY_ERROR=-1,
        PORT_OPEN_ERROR=-2,
        DATA_ERROR=-3,
        BLOCKED=-4
    }

    public enum connectiontype
    {
        Connection_Serial_GM05,
        Connection_Serial_GM08  
    }

    public enum ranges
    {
        RANGE_0=0,
        RANGE_1=1,
        RANGE_2=2,
        RANGE_3=3,
        RANGE_AUTO=4
    }

    public enum meter_keys
    {
        ENTER='E',
        MENU='X',
        OFF='O',
        NEXT='N',
        RANGE='R',
        RESET='0'
    }

    public enum units
    {
        TESLA=0,
        GAUSS=1,
        KAm=2,
        Oe=3
    }

    public enum modes
    {
         DC=0,
         DCPEAK=1,
         AC=2,
         ACMAX=3,
         ACPEAK=4,
         HOLD=5
    }

    public enum languages
    {
         ENGLISH=0,
         FRENCH=1,
         GERMAN=2,
         ITALIAN=3,
         SPANISH=4,
         PORTUGUESE=5
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
     public byte probeoffset;
    };

    public class gmtime
    {
        public gmtime(gm_time data)
        {
            sec = data.sec;
            min=data.min;
            hour = data.hour;
            day = data.day;
            month = data.month;
            year = data.year;
        }

        public gmtime()
        {

        }

        public byte sec;
        public byte min;
        public byte hour;
        public byte day;
        public byte month;
        public byte year;

        public override string ToString()
        {
            if (year == 0)
            {
                return "";
            }
            else
            {
                return day.ToString() + "/" + month.ToString() + "/" + year.ToString() + " " + hour.ToString() + ":" + min.ToString() + ":" + sec.ToString();
            }
       }

        public static implicit operator gmtime(gm_time i)
        {
            return(new gmtime(i));
        }
    };

    public class gmstore
    {

        string[] unitsdisp = new string[]{
            "T",
            "G",
            "A/m",
            "Oe"
        };

        public gmstore(gm_store data)
        {
            range = data.range;
            mode = (modes)Enum.Parse(typeof(modes),data.mode.ToString());
            units = (units)Enum.Parse(typeof(units), data.units.ToString());
            value = data.value;
            time = new gmtime(data.time);
        }

        public gmstore()
        {
            time = new gmtime();
        }

        public gmtime time;
        public byte range;
        public modes mode;
        public units units;
        public float value;

        public override string ToString()
        {
            return value.ToString() + " " + range.ToString() + " " + mode.ToString() + " " + units.ToString() + " " + time.ToString();
        }

        public static implicit operator gmstore(gm_store i)
        {
            return (new gmstore(i));
        }

        public string getUnitsDisp( units units)
        {
            return unitsdisp[(int)units];
        }

        public string getCurrentUnitsBaseDisp()
        {
            return unitsdisp[(int)units];
        }
    }

    public class gm0
    {

        public const string DLL_FILE_NAME = "gm0.dll";
 
        int connectionport;
        int hand=-1;

        public gmstore currentreading;
        public gmstore lastreading;
        
        public dllConnectedCallBack meh;
        public DataCallBack meh2;

        [DllImport("kernel32.dll")]
        private static extern IntPtr LoadLibrary(string dllToLoad);

        [DllImport(DLL_FILE_NAME)]
        static extern int gm0_newgm(int port, int mode);
        public gm0(connectiontype contype,int port)
        {

            //Try to load the gm0.dll from current application folder bin32 or bin64 dirs
            //then try and load from Program Files (x86)\Hirst Magnetic Instruments Ltd\gm0
            var myPath = new Uri(typeof(gm0).Assembly.CodeBase).LocalPath;
            var myFolder = Path.GetDirectoryName(myPath);

            var is64 = IntPtr.Size == 8;
            var subfolder = is64 ? "bin64" : "bin32";

            LoadLibrary(Path.Combine(myPath, subfolder, "gm0.dll"));

            string pf = Environment.GetFolderPath(Environment.SpecialFolder.ProgramFilesX86);

            myPath = Path.Combine(pf, "Hirst Magnetic Instruments Ltd","gm0");

            LoadLibrary(Path.Combine(myPath,subfolder,"gm0.dll"));


            lastreading = new gmstore();
            connectionport = port;

            hand = gm0_newgm(port, (int)Enum.Parse(typeof(connectiontype), contype.ToString()));
            checkvalidhandorthrow();

            meh = new dllConnectedCallBack(connectedcallback);
            meh2 = new DataCallBack(datacallback);

            gm0_setconnectcallback(hand, meh);
            gm0_setcallback(hand, meh2);

            //gm0_setnullcallback(hand, nullcallback);
        }

        [DllImport(DLL_FILE_NAME)]
        static extern int gm0_killgm(int hand);
        public void terminate()
        {
            checkvalidhandorthrow();
            gm0_killgm(hand);
            hand = -1;
        }

        [DllImport(DLL_FILE_NAME)]
        static extern int gm0_startconnect(int hand);
        public void StartConnect()
        {
            checkvalidhandorthrow();
            gm0_startconnect(hand);
        }

        [DllImport(DLL_FILE_NAME)]
        static extern int gm0_getconnect(int hand);
        public bool GetConnectionStatus()
        {
            if(hand<0)
                return false;

            int status = gm0_getconnect(hand);
            return (status==1?true:false);
        }

        public delegate void dllConnectedCallBack();
        [DllImport(DLL_FILE_NAME)]
        static extern int gm0_setconnectcallback(int hand, dllConnectedCallBack x);
        public delegate void ConnectedCallback();
        public event ConnectedCallback onConnectedCallback;
        public void connectedcallback()
        {
            if(onConnectedCallback!=null)
            {
                try
                {
                    onConnectedCallback();
                    gm0_setconnectcallback(hand, null);
                }
                catch
                {

                }
            }
        }

        public delegate void NewDataCallback(gmstore value);
        public event NewDataCallback onNewData;

        public delegate void NewValueCallback(float value);
        public event NewValueCallback onNewValue;

        public delegate void NewRangeCallback(int range);
        public event NewRangeCallback onNewRange;

        public delegate void NewModeCallback(int mode);
        public event NewModeCallback onNewMode;

        public delegate void NewUnitsCallback(int units);
        public event NewUnitsCallback onNewUnits;

        public delegate void DataCallBack(int hand, gm_store data);
        [DllImport(DLL_FILE_NAME)]
        static extern int gm0_setcallback(int hand, DataCallBack x);

        void datacallback(int hand, gm_store data)
        {
            // new data avaiable

            currentreading = data;

            if (this.onNewValue != null)
                onNewValue(data.value);

            if (this.onNewData != null)
                onNewData(new gmstore(data));

            if (lastreading.range != currentreading.range)
            {
                if (this.onNewRange != null)
                    onNewRange(data.range);
            }

            if (lastreading.mode != currentreading.mode)
            {
                if (this.onNewMode != null)
                    onNewMode(data.mode);
            }

            if (lastreading.units != currentreading.units)
            {
                if (this.onNewUnits != null)
                    onNewUnits(data.units);
            }

            lastreading = data;
        }

        [DllImport(DLL_FILE_NAME)]
        static extern int gm0_setlanguage(int hand, byte lan);
        public void setMeterLanguage(languages lan)
        {
            checkvalidhandorthrow();
            gm0_setlanguage(hand, (byte)(int)Enum.Parse(typeof(languages), lan.ToString()));
        }

        [DllImport(DLL_FILE_NAME)]
        static extern int gm0_getlanguage(int hand);
        public languages GetMeterLanguage()
        {
            checkvalidhandorthrow();
            int lan=gm0.gm0_getlanguage(hand);
            return (languages) Enum.Parse(typeof(languages), lan.ToString());
        }

        [DllImport(DLL_FILE_NAME)]
        static extern int gm0_setrange(int hand, byte range);
        public void setrange(byte range)
        {
            checkvalidhandorthrow();
            gm0_setrange(hand, range);
        }

        public void setrange(ranges range)
        {
            checkvalidhandorthrow();
            gm0_setrange(hand, (byte)(int)Enum.Parse(typeof(ranges), range.ToString()));
        }

        [DllImport(DLL_FILE_NAME)]
        static extern int gm0_setmode(int hand, byte mode);
        public void setmode(modes mode)
        {
            checkvalidhandorthrow();
            gm0_setmode(hand, (byte)(int)Enum.Parse(typeof(modes), mode.ToString()));
        }

        [DllImport(DLL_FILE_NAME)]
        static extern int gm0_setunits(int hand, byte units);
        public void setunits(units units)
        {
            checkvalidhandorthrow();
            gm0_setunits(hand, (byte)(int)Enum.Parse(typeof(units), units.ToString()));
        }

        [DllImport(DLL_FILE_NAME)]
        static extern int gm0_resetpeak(int hand);
        public void resetpeak()
        {
             checkvalidhandorthrow();
             gm0_resetpeak(hand);

        }

        [DllImport(DLL_FILE_NAME)]
        static extern int gm0_donull(int hand);
        public void donull()
        {
            checkvalidhandorthrow();
            gm0.gm0_donull(hand);

        }

        [DllImport(DLL_FILE_NAME)]
        static extern int gm0_doaz(int hand);
        public void autozero()
        {
             checkvalidhandorthrow();
             gm0_doaz(hand);

        }

        [DllImport(DLL_FILE_NAME)]
        static extern int gm0_resetnull(int hand);
        public void resetnull()
        {


        }

        delegate void dllNullCallBack();
        [DllImport(DLL_FILE_NAME)]
        static extern int gm0_setnullcallback(int hand, dllNullCallBack x);
        void nullcallback()
        {
            

        }

        [DllImport(DLL_FILE_NAME)]
        static extern int gm0_sendtime(int hand, bool extended);
        public void send_time_with_value(bool enabled)
        {
            checkvalidhandorthrow();
            gm0_sendtime(hand, enabled);
        }

        [DllImport(DLL_FILE_NAME)]
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

        [DllImport(DLL_FILE_NAME)]
        static extern gm_time gm0_gettime(int hand);
        public DateTime get_meter_time()
        {
            checkvalidhandorthrow();
            gm_time gtime = new gm_time();
            gtime = gm0_gettime(hand);
            DateTime time = new DateTime(gtime.year+2000,gtime.month,gtime.day,gtime.hour,gtime.min,gtime.sec);
            return time;
        }

        [DllImport(DLL_FILE_NAME)]
        static extern gm_store gm0_getstore(int hand, int pos);
        public gmstore get_saved_value(int pos)
        {
            checkvalidhandorthrow();
            gmstore data=new gmstore(gm0_getstore(hand, pos));
            return (data);
        }

        [DllImport(DLL_FILE_NAME)]
        static extern int gm0_simkey(int hand, byte meterkey);
        public void simkey(gm0_sharp.meter_keys key)
        {
            checkvalidhandorthrow();
            gm0_simkey(hand, (byte)(int)Enum.Parse(typeof(meter_keys), key.ToString()));
        }

        [DllImport(DLL_FILE_NAME)]
        static extern int gm0_setinterval(int hand, int interval);
        public void SetDataRate(int rate)
        {
            checkvalidhandorthrow();
            gm0_setinterval(hand, rate);
        }

        [DllImport(DLL_FILE_NAME)]
        static extern int gm0_fastUSBpoll(int hand, int enabled);
        public void enable_fastUSBpoll(bool enabled)
        {
            checkvalidhandorthrow();
            gm0_fastUSBpoll(hand, enabled==true?1:0);
        }

        [DllImport(DLL_FILE_NAME)]
        static extern void gm0_sampleondemand(int hand);
        public void enable_sampleOnDemand()
        {
            checkvalidhandorthrow();
            gm0_sampleondemand(hand);
        }

        [DllImport(DLL_FILE_NAME)]
        static extern gm_store gm0_demandsample(int hand, int extra);
        public gmstore demand_sample(bool extended)
        {
            checkvalidhandorthrow();
            gmstore data = new gmstore(gm0_demandsample(hand,extended==true?1:0));
            return (data);
        }


        [DllImport(DLL_FILE_NAME)]
        static extern UInt16 gm0_getgmserial(int hand);
        public UInt16 GetMeterSerial()
        {
            checkvalidhandorthrow();
            return gm0_getgmserial(hand);

        }

        [DllImport(DLL_FILE_NAME)]
        static extern UInt16 gm0_getprobeserial(int hand);
        public UInt16 GetProbeSerial()
        {
            checkvalidhandorthrow();
            return gm0_getprobeserial(hand);
        }

        [DllImport(DLL_FILE_NAME)]
        static extern UInt16 gm0_getprobetype(int hand);
        public UInt16 GetProbeType()
        {
            checkvalidhandorthrow();
            return gm0_getprobetype(hand);
        }

        [DllImport(DLL_FILE_NAME)]
        static extern UInt16 gm0_getprobecaldate(int hand);
        public UInt16 GetProbeCalDate()
        {
            checkvalidhandorthrow();
            return gm0_getprobecaldate(hand);
        }

        [DllImport(DLL_FILE_NAME)]
        static extern UInt16 gm0_readprobee2wordfast(int hand,byte pos);
        public UInt16 GetProbeE2Word(byte location)
        {
             checkvalidhandorthrow();
             return gm0_readprobee2wordfast(hand, location);
        }

        [DllImport(DLL_FILE_NAME)]
        static extern UInt16 gm0_writeprobee2word(int hand, byte pos,int data);
        public UInt16 WriteProbeE2Word(byte location,ushort data)
        {
            checkvalidhandorthrow();
            return gm0_writeprobee2word(hand, location,data);
        }

        [DllImport(DLL_FILE_NAME)]
        static extern UInt16 gm0_startproberead(int hand);
        public UInt16 BeginProbeRead()
        {
            checkvalidhandorthrow();
            return gm0_startproberead(hand);
        }

        [DllImport(DLL_FILE_NAME)]
        static extern UInt16 gm0_endproberead(int hand);
        public UInt16 EndProbeRead()
        {
            checkvalidhandorthrow();
            return gm0_endproberead(hand);
        }



        public void checkvalidhandorthrow()
        {
            if(hand<0)
            {
                faults fault;
                fault=(faults)hand;
                Exception e=new Exception("Attempt to access a gm0 function with an invalid handle = "+fault.ToString());
                throw(e);
             }
        }

    }
}
