
import time
from gm0 import *
 
# Connect to the first USB GM08
#  -1 is the first USB device, -2 is the 2nd

handle = gm0_newgm(-1,0)
gm0_startconnect(handle)

while gm0_getconnect(handle) == 0:
    print("Connecting ....")
    time.sleep(1)

if gm0_getconnect(handle) == 1:
    print("Connected")

if gm0_getconnect(handle) <0:
    print("Error connecting")
    time.sleep(5)
    exit()

probesn = gm0_getprobesn(handle)
metersn = gm0_getmetersn(handle)

print("Meter :",metersn," Probe :",probesn)

print("Setting Mode DC")
gm0_setmode(handle,0)
time.sleep(1)

print("Setting Units Tesla")
gm0_setunits(handle,0)
time.sleep(1)

for x in range(4):
    print("Nulling range :",x)
    gm0_setrange(handle,x) 
    time.sleep(1)
    gm0_donull(handle)
    time.sleep(1)
    gm0_resetnull(handle)
    time.sleep(1)

print("Setting Range Auto")
gm0_setrange(handle,4) #4 is auto range
time.sleep(1)

for x in range(10):

    val = gm0_getvalue(handle)
    range = gm0_getrange(handle)
    autorange=0;
    if range>0x0:
        autorange=1
        
    range &= 0x03
    
    mode = gm0_getmode(handle)
    units = gm0_getunits(handle)

    
    print("{:.6f}".format(val),gm0_Units[units],gm0_Mode[mode],"Range :",range," Auto :",autorange)
    time.sleep(2)

  


    
