GM05 Device drivers and applications

28-Feb-2007

Code for USB has been done for win32 systems for GM08
GM08 support is complete internally
RS232 ports can run at 4800 or 9600 for GM08
Linux build has been fixed again but lacks USB support

Old com objects have been removed
GM0Control is dead


STATUS as at 20-April-2006

gm0.dll and consoleapplication are working and should compile on windows and linux

installsheild stuff has been removed from cvs, its not code its just packaging and 
should not have been here

GM0Control does not compile on visual C.net only v6 so I am going to scrap it and *maybe* 
rewrite it but i cannot see the point at the moment. It is easier to avoid the com step
altogether in the middle.

As GM0Control is broken the excel application and VB code is broken, this is currently
being rewritten to avoid the com object

Robin Cornelius