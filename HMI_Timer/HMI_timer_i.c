/* this file contains the actual definitions of */
/* the IIDs and CLSIDs */

/* link this file in with the server and any clients */


/* File created by MIDL compiler version 5.01.0164 */
/* at Thu Aug 26 09:18:05 2004
 */
/* Compiler settings for M:\dlls\gm0\gm0\HMI_timer\HMI_timer.idl:
    Oicf (OptLev=i2), W1, Zp8, env=Win32, ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
*/
//@@MIDL_FILE_HEADING(  )
#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __IID_DEFINED__
#define __IID_DEFINED__

typedef struct _IID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;

#endif // __IID_DEFINED__

#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif // CLSID_DEFINED

const IID IID_IHirstTimer = {0xE52981AC,0xDF62,0x11D6,{0x98,0xFA,0x00,0x50,0x04,0xB8,0x4D,0x6A}};


const IID LIBID_HMI_TIMERLib = {0xE52981A0,0xDF62,0x11D6,{0x98,0xFA,0x00,0x50,0x04,0xB8,0x4D,0x6A}};


const IID DIID__IHirstTimerEvents = {0xE52981AE,0xDF62,0x11D6,{0x98,0xFA,0x00,0x50,0x04,0xB8,0x4D,0x6A}};


const CLSID CLSID_HirstTimer = {0xE52981AD,0xDF62,0x11D6,{0x98,0xFA,0x00,0x50,0x04,0xB8,0x4D,0x6A}};


#ifdef __cplusplus
}
#endif

