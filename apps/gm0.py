import ctypes

#load the correct 32 or 64 bit dll
if  ctypes.sizeof(ctypes.c_voidp) == 8:
    clibrary = ctypes.CDLL('C:\\Program Files (x86)\\Hirst Magnetic Instruments Ltd\\gm0\\bin64\\gm0.dll')
else:
    clibrary = ctypes.CDLL('C:\\Program Files (x86)\\Hirst Magnetic Instruments Ltd\\gm0\\bin32\\gm0.dll')


gm0_newgm = clibrary.gm0_newgm
gm0_newgm.argtypes = [ctypes.c_int, ctypes.c_int]
gm0_newgm.restype = ctypes.c_int

gm0_startconnect = clibrary.gm0_startconnect
gm0_startconnect.argtypes = [ctypes.c_int]
gm0_startconnect.restype = ctypes.c_int

gm0_getconnect = clibrary.gm0_getconnect
gm0_getconnect.argtypes = [ctypes.c_int]
gm0_getconnect.restype = ctypes.c_int

gm0_getvalue = clibrary.gm0_getvalue
gm0_getvalue.argtypes = [ctypes.c_int]
gm0_getvalue.restype = ctypes.c_double

gm0_getrange = clibrary.gm0_getrange
gm0_getrange.argtypes = [ctypes.c_int]
gm0_getrange.restype = ctypes.c_int

gm0_getmode = clibrary.gm0_getmode
gm0_getmode.argtypes = [ctypes.c_int]
gm0_getmode.restype = ctypes.c_int

gm0_getunits = clibrary.gm0_getunits
gm0_getunits.argtypes = [ctypes.c_int]
gm0_getunits.restype = ctypes.c_int

gm0_setrange = clibrary.gm0_setrange
gm0_setrange.argtypes = [ctypes.c_int,ctypes.c_byte]
gm0_setrange.restype = ctypes.c_int

gm0_setmode = clibrary.gm0_setmode
gm0_setmode.argtypes = [ctypes.c_int,ctypes.c_byte]
gm0_setmode.restype = ctypes.c_int

gm0_setunits = clibrary.gm0_setunits
gm0_setunits.argtypes = [ctypes.c_int,ctypes.c_byte]
gm0_setunits.restype = ctypes.c_int

gm0_getprobesn = clibrary.gm0_getprobeserial
gm0_getprobesn.argtypes = [ctypes.c_int]
gm0_getprobesn.restype = ctypes.c_int

gm0_getmetersn = clibrary.gm0_getgmserial
gm0_getmetersn.argtypes = [ctypes.c_int]
gm0_getmetersn.restype = ctypes.c_int


gm0_donull = clibrary.gm0_donull
gm0_donull.argtypes = [ctypes.c_int]
gm0_donull.restype = ctypes.c_int

gm0_doaz = clibrary.gm0_doaz
gm0_doaz.argtypes = [ctypes.c_int]
gm0_doaz.restype = ctypes.c_int

gm0_resetnull = clibrary.gm0_resetnull
gm0_resetnull.argtypes = [ctypes.c_int]
gm0_resetnull.restype = ctypes.c_int


gm0_Units = ['T', 'G', 'A/m','Oe']
gm0_Mode = ['DC','DC Peak','AC','AC MAX','AC PEAK','HOLD']


