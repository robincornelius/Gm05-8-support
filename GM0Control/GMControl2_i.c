/* this file contains the actual definitions of */
/* the IIDs and CLSIDs */

/* link this file in with the server and any clients */


/* File created by MIDL compiler version 5.01.0164 */
/* at Thu Aug 26 16:31:32 2004
 */
/* Compiler settings for M:\dlls\gm0\gm0\GMControl2\GMControl2.idl:
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

const IID IID_IGM0Control2 = {0xAD946CAF,0x9C38,0x407C,{0x8A,0xD4,0x56,0x9A,0x77,0x7F,0xD7,0x21}};


const IID LIBID_GMCONTROL2Lib = {0x427A7646,0xA9FD,0x4CB6,{0x8F,0x90,0x5A,0xBC,0x8B,0x67,0x86,0x6A}};


const IID DIID__IGM0Control2Events = {0x9975CC1B,0x7CBE,0x4D8A,{0x97,0x53,0xD6,0x65,0x21,0xAE,0x4B,0x4A}};


const CLSID CLSID_GM0Control2 = {0xCD25C73B,0xB31D,0x41A5,{0xB7,0x92,0x43,0x90,0x06,0x54,0x23,0x09}};


#ifdef __cplusplus
}
#endif

