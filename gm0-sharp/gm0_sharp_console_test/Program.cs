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

using System;
using System.Collections.Generic;
using System.Text;
using gm0_sharp;

namespace gm0_sharp_console_test
{
    class Program
    {
        static gm0 mygm;
        static System.Threading.AutoResetEvent waitconnect = new System.Threading.AutoResetEvent(false);

        static void Main(string[] args)
        {
            Console.WriteLine("Creating a new gm0");
            
            // connectiontype is only important for RS232 comms;
            // they use different baud rates

            // the port should be -1 for the first USB device -2 for the 2nd etc
            // 1 for com1 or /dev/ttyS0 etc..
            mygm = new gm0(connectiontype.Connection_Serial_GM08, -1);
            Console.WriteLine("Registering callbacks");

            waitconnect.Reset();
 
            mygm.onConnectedCallback += new gm0.ConnectedCallback(mygm_onConnectedCallback);

            Console.WriteLine("Trying to connect to meter");
            mygm.StartConnect();

            if (!waitconnect.WaitOne(10000,false))
            {
                Console.WriteLine("Failed to connect to meter");
                System.Threading.Thread.Sleep(2500);
                return;
            }

            showmenu();
            douserinput();
            mygm.terminate();

        }

        static void mygm_onNewData(gmstore value)
        {
            Console.WriteLine("New data :" + value.ToString());
        }

        static void mygm_onConnectedCallback()
        {
            Console.WriteLine("Gaussmeter is connected");
            waitconnect.Set();
        }

        static void showmenu()
        {
            Console.WriteLine("********************************************************************************");
            Console.WriteLine("*                                                                              *");
            Console.WriteLine("*                 GM05 Console control and DLL API Example                     *");
            Console.WriteLine("*                                                                              *");
            Console.WriteLine("********************************************************************************");
            Console.WriteLine("\n");
            Console.WriteLine(" (R) - Set Range        (U) - Set Units             (M) - Mode   ");
            Console.WriteLine(" (L) - language         (V) - Get Value             ( ) - Show menu");	
            Console.WriteLine(" (A) - Auto Zero        (N) - Null                                  ");	
            Console.WriteLine(" (1) - Enable callback  (0) - Disable callback      (<) - Set GM Time");
            Console.WriteLine(" (I) - Set interval     (Q) - Reset peak            (>) - Get GM Time");	
            Console.WriteLine(" (W) - Power Off        (G) - Get reg               (H) - Get Reg(s)");
            Console.WriteLine("                        (X) - Exit                  (9) - Get E2");	
            Console.WriteLine("\n\n");

        }

        static void douserinput()
        {
            bool run=true;
            string input;
            int value;

            while(run)
            {
                Console.Write("Your selection :");
                ConsoleKeyInfo key=Console.ReadKey();
                Console.Write("\n");
                switch (key.KeyChar)
                {

                    case 'r':
                    case 'R':
                        Console.Write("New range (0-4) :");
                        input=Console.ReadLine();
                        if (int.TryParse(input, out value))
                        {
                            mygm.setrange((byte)value);
                        }
                        break;

                    case 'U':
                    case 'u':
                        Console.Write("Select new units :-\n");
                       
                        foreach(string unit in Enum.GetNames(typeof(units)))
                        {
                            Console.WriteLine("Units: {0} - {1}", unit, (int)Enum.Parse(typeof(units), unit));
                        }
                        try
                        {
                            mygm.setunits((units)Enum.Parse(typeof(units), Console.ReadLine()));
                        }
                        catch(Exception e)
                        {
                            Console.WriteLine(e.Message);
                        }

                        break;

                    case 'M':
                    case 'm':
                       
                        Console.Write("Select new mode :-\n");

                        foreach (string mode in Enum.GetNames(typeof(modes)))
                        {
                            Console.WriteLine("Mode: {0} - {1}", mode, (int)Enum.Parse(typeof(modes), mode));
                        }
                        try
                        {
                            mygm.setmode((modes)Enum.Parse(typeof(modes), Console.ReadLine()));
                        }
                        catch (Exception e)
                        {
                            Console.WriteLine(e.Message);
                        }

                        break;

                    case 'L':
                    case 'l':

                        Console.Write("Select new meter language :-\n");

                        foreach (string lan in Enum.GetNames(typeof(languages)))
                        {
                            Console.WriteLine("Language: {0} - {1}", lan, (int)Enum.Parse(typeof(languages), lan));
                        }
                        try
                        {
                            mygm.setMeterLanguage((languages)Enum.Parse(typeof(languages), Console.ReadLine()));
                        }
                        catch (Exception e)
                        {
                            Console.WriteLine(e.Message);
                        }

                        break;
                    case 'V':
                    case 'v':
                        Console.WriteLine("Reading is " + mygm.lastreading.ToString());
                        break;

                    case ' ':
                        showmenu();
                        break;

                    case 'A':
                    case 'a':
                        mygm.autozero();
                        break;

                    case 'N':
                    case 'n':
                        mygm.donull();
                        break;

                    case 'T':
                    case 't':
                        mygm.send_time_with_value(true);
                        break;

                    case '1':
                        mygm.onNewData += new gm0.NewDataCallback(mygm_onNewData);
                        break;

                    case '0':
                        mygm.onNewData -= new gm0.NewDataCallback(mygm_onNewData);
                        break;

                    case '<':
                        mygm.set_meter_time(DateTime.Now);
                        break;

                    case 'I':
                    case 'i':
                        Console.Write("New data rate :");
                        input = Console.ReadLine();
                        
                        if (int.TryParse(input, out value))
                        {
                            mygm.SetDataRate(value);
                        }
                       
                        break;

                    case 'Q':
                    case 'q':
                        mygm.resetpeak();
                        break;

                    case '>':
                        try
                        {
                            DateTime dt;
                            dt = mygm.get_meter_time();
                            Console.WriteLine("Current meter time is " + dt.ToString());
                        }
                        catch { Console.WriteLine("Error geting Date from meter"); }

                        break;

                    case 'W':
                    case 'w':
                        mygm.simkey(gm0_sharp.meter_keys.OFF);
                        break;

                    case 'G':
                    case 'g':
                        Console.Write("Register (0-99) :");
                        input = Console.ReadLine();
                        
                        if (int.TryParse(input, out value))
                        {
                            gmstore data=mygm.get_saved_value(value);
                            Console.WriteLine(data.ToString());
                        }       
                        break;

                    case 'H':
                    case 'h':
                        Console.Write("Start Register (0-99) :");
                        int start;
                        int.TryParse(Console.ReadLine(), out start);

                        Console.Write("End Register (0-99) :");
                        int end;
                        int.TryParse(Console.ReadLine(),out end);

                        for(int x=start;x<=end;x++)
                        {
                            gmstore data=mygm.get_saved_value(x);
                            Console.WriteLine("Register "+x.ToString()+" is "+data.ToString());
                        }

                        break;

                    case 'B':
                    case 'b':
                        break;

                    case 'x':
                    case 'X':
                        run = false;
                        break;

                    case '9':
                        break;
                }


            }
        




        }

    }
}
