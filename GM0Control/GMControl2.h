/* this ALWAYS GENERATED file contains the definitions for the interfaces */


/* File created by MIDL compiler version 5.01.0164 */
/* at Thu Aug 26 16:31:32 2004
 */
/* Compiler settings for M:\dlls\gm0\gm0\GMControl2\GMControl2.idl:
    Oicf (OptLev=i2), W1, Zp8, env=Win32, ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
*/
//@@MIDL_FILE_HEADING(  )


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 440
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __GMControl2_h__
#define __GMControl2_h__

#ifdef __cplusplus
extern "C"{
#endif 

/* Forward Declarations */ 

#ifndef __IGM0Control2_FWD_DEFINED__
#define __IGM0Control2_FWD_DEFINED__
typedef interface IGM0Control2 IGM0Control2;
#endif 	/* __IGM0Control2_FWD_DEFINED__ */


#ifndef ___IGM0Control2Events_FWD_DEFINED__
#define ___IGM0Control2Events_FWD_DEFINED__
typedef interface _IGM0Control2Events _IGM0Control2Events;
#endif 	/* ___IGM0Control2Events_FWD_DEFINED__ */


#ifndef __GM0Control2_FWD_DEFINED__
#define __GM0Control2_FWD_DEFINED__

#ifdef __cplusplus
typedef class GM0Control2 GM0Control2;
#else
typedef struct GM0Control2 GM0Control2;
#endif /* __cplusplus */

#endif 	/* __GM0Control2_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 

#ifndef __IGM0Control2_INTERFACE_DEFINED__
#define __IGM0Control2_INTERFACE_DEFINED__

/* interface IGM0Control2 */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IGM0Control2;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("AD946CAF-9C38-407C-8AD4-569A777FD721")
    IGM0Control2 : public IDispatch
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_ComPort( 
            /* [retval][out] */ short __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_ComPort( 
            /* [in] */ short newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_lastvalue( 
            /* [retval][out] */ float __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_lastmode( 
            /* [retval][out] */ short __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_lastunits( 
            /* [retval][out] */ short __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_store_value( 
            /* [retval][out] */ double __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_store_range( 
            /* [retval][out] */ short __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_store_units( 
            /* [retval][out] */ short __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_store_mode( 
            /* [retval][out] */ short __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_year( 
            /* [retval][out] */ short __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_year( 
            /* [in] */ short newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_month( 
            /* [retval][out] */ short __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_month( 
            /* [in] */ short newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_day( 
            /* [retval][out] */ short __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_day( 
            /* [in] */ short newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_hour( 
            /* [retval][out] */ short __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_hour( 
            /* [in] */ short newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_min( 
            /* [retval][out] */ short __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_min( 
            /* [in] */ short newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_sec( 
            /* [retval][out] */ short __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_sec( 
            /* [in] */ short newVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE AttemptConnect( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetMode( 
            short mode) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetRange( 
            short range) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetUnits( 
            short units) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetLanguage( 
            short lang) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE DoAutoZero( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE DoNull( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetRegister( 
            short reg) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ProbeInSheild( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Initalise( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE close( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE settime( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE gettime( void) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_lastrange( 
            /* [retval][out] */ short __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_gmserial( 
            /* [retval][out] */ short __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_probeserial( 
            /* [retval][out] */ short __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE startcmdseq( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE endcmdseq( void) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_sendtimeinfo( 
            /* [retval][out] */ BOOL __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_sendtimeinfo( 
            /* [in] */ BOOL newVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IGM0Control2Vtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IGM0Control2 __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IGM0Control2 __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IGM0Control2 __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IGM0Control2 __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IGM0Control2 __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IGM0Control2 __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IGM0Control2 __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_ComPort )( 
            IGM0Control2 __RPC_FAR * This,
            /* [retval][out] */ short __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_ComPort )( 
            IGM0Control2 __RPC_FAR * This,
            /* [in] */ short newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_lastvalue )( 
            IGM0Control2 __RPC_FAR * This,
            /* [retval][out] */ float __RPC_FAR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_lastmode )( 
            IGM0Control2 __RPC_FAR * This,
            /* [retval][out] */ short __RPC_FAR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_lastunits )( 
            IGM0Control2 __RPC_FAR * This,
            /* [retval][out] */ short __RPC_FAR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_store_value )( 
            IGM0Control2 __RPC_FAR * This,
            /* [retval][out] */ double __RPC_FAR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_store_range )( 
            IGM0Control2 __RPC_FAR * This,
            /* [retval][out] */ short __RPC_FAR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_store_units )( 
            IGM0Control2 __RPC_FAR * This,
            /* [retval][out] */ short __RPC_FAR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_store_mode )( 
            IGM0Control2 __RPC_FAR * This,
            /* [retval][out] */ short __RPC_FAR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_year )( 
            IGM0Control2 __RPC_FAR * This,
            /* [retval][out] */ short __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_year )( 
            IGM0Control2 __RPC_FAR * This,
            /* [in] */ short newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_month )( 
            IGM0Control2 __RPC_FAR * This,
            /* [retval][out] */ short __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_month )( 
            IGM0Control2 __RPC_FAR * This,
            /* [in] */ short newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_day )( 
            IGM0Control2 __RPC_FAR * This,
            /* [retval][out] */ short __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_day )( 
            IGM0Control2 __RPC_FAR * This,
            /* [in] */ short newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_hour )( 
            IGM0Control2 __RPC_FAR * This,
            /* [retval][out] */ short __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_hour )( 
            IGM0Control2 __RPC_FAR * This,
            /* [in] */ short newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_min )( 
            IGM0Control2 __RPC_FAR * This,
            /* [retval][out] */ short __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_min )( 
            IGM0Control2 __RPC_FAR * This,
            /* [in] */ short newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_sec )( 
            IGM0Control2 __RPC_FAR * This,
            /* [retval][out] */ short __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_sec )( 
            IGM0Control2 __RPC_FAR * This,
            /* [in] */ short newVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *AttemptConnect )( 
            IGM0Control2 __RPC_FAR * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetMode )( 
            IGM0Control2 __RPC_FAR * This,
            short mode);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetRange )( 
            IGM0Control2 __RPC_FAR * This,
            short range);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetUnits )( 
            IGM0Control2 __RPC_FAR * This,
            short units);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetLanguage )( 
            IGM0Control2 __RPC_FAR * This,
            short lang);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DoAutoZero )( 
            IGM0Control2 __RPC_FAR * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DoNull )( 
            IGM0Control2 __RPC_FAR * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetRegister )( 
            IGM0Control2 __RPC_FAR * This,
            short reg);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ProbeInSheild )( 
            IGM0Control2 __RPC_FAR * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Initalise )( 
            IGM0Control2 __RPC_FAR * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *close )( 
            IGM0Control2 __RPC_FAR * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *settime )( 
            IGM0Control2 __RPC_FAR * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *gettime )( 
            IGM0Control2 __RPC_FAR * This);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_lastrange )( 
            IGM0Control2 __RPC_FAR * This,
            /* [retval][out] */ short __RPC_FAR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_gmserial )( 
            IGM0Control2 __RPC_FAR * This,
            /* [retval][out] */ short __RPC_FAR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_probeserial )( 
            IGM0Control2 __RPC_FAR * This,
            /* [retval][out] */ short __RPC_FAR *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *startcmdseq )( 
            IGM0Control2 __RPC_FAR * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *endcmdseq )( 
            IGM0Control2 __RPC_FAR * This);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_sendtimeinfo )( 
            IGM0Control2 __RPC_FAR * This,
            /* [retval][out] */ BOOL __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_sendtimeinfo )( 
            IGM0Control2 __RPC_FAR * This,
            /* [in] */ BOOL newVal);
        
        END_INTERFACE
    } IGM0Control2Vtbl;

    interface IGM0Control2
    {
        CONST_VTBL struct IGM0Control2Vtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IGM0Control2_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IGM0Control2_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IGM0Control2_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IGM0Control2_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IGM0Control2_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IGM0Control2_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IGM0Control2_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IGM0Control2_get_ComPort(This,pVal)	\
    (This)->lpVtbl -> get_ComPort(This,pVal)

#define IGM0Control2_put_ComPort(This,newVal)	\
    (This)->lpVtbl -> put_ComPort(This,newVal)

#define IGM0Control2_get_lastvalue(This,pVal)	\
    (This)->lpVtbl -> get_lastvalue(This,pVal)

#define IGM0Control2_get_lastmode(This,pVal)	\
    (This)->lpVtbl -> get_lastmode(This,pVal)

#define IGM0Control2_get_lastunits(This,pVal)	\
    (This)->lpVtbl -> get_lastunits(This,pVal)

#define IGM0Control2_get_store_value(This,pVal)	\
    (This)->lpVtbl -> get_store_value(This,pVal)

#define IGM0Control2_get_store_range(This,pVal)	\
    (This)->lpVtbl -> get_store_range(This,pVal)

#define IGM0Control2_get_store_units(This,pVal)	\
    (This)->lpVtbl -> get_store_units(This,pVal)

#define IGM0Control2_get_store_mode(This,pVal)	\
    (This)->lpVtbl -> get_store_mode(This,pVal)

#define IGM0Control2_get_year(This,pVal)	\
    (This)->lpVtbl -> get_year(This,pVal)

#define IGM0Control2_put_year(This,newVal)	\
    (This)->lpVtbl -> put_year(This,newVal)

#define IGM0Control2_get_month(This,pVal)	\
    (This)->lpVtbl -> get_month(This,pVal)

#define IGM0Control2_put_month(This,newVal)	\
    (This)->lpVtbl -> put_month(This,newVal)

#define IGM0Control2_get_day(This,pVal)	\
    (This)->lpVtbl -> get_day(This,pVal)

#define IGM0Control2_put_day(This,newVal)	\
    (This)->lpVtbl -> put_day(This,newVal)

#define IGM0Control2_get_hour(This,pVal)	\
    (This)->lpVtbl -> get_hour(This,pVal)

#define IGM0Control2_put_hour(This,newVal)	\
    (This)->lpVtbl -> put_hour(This,newVal)

#define IGM0Control2_get_min(This,pVal)	\
    (This)->lpVtbl -> get_min(This,pVal)

#define IGM0Control2_put_min(This,newVal)	\
    (This)->lpVtbl -> put_min(This,newVal)

#define IGM0Control2_get_sec(This,pVal)	\
    (This)->lpVtbl -> get_sec(This,pVal)

#define IGM0Control2_put_sec(This,newVal)	\
    (This)->lpVtbl -> put_sec(This,newVal)

#define IGM0Control2_AttemptConnect(This)	\
    (This)->lpVtbl -> AttemptConnect(This)

#define IGM0Control2_SetMode(This,mode)	\
    (This)->lpVtbl -> SetMode(This,mode)

#define IGM0Control2_SetRange(This,range)	\
    (This)->lpVtbl -> SetRange(This,range)

#define IGM0Control2_SetUnits(This,units)	\
    (This)->lpVtbl -> SetUnits(This,units)

#define IGM0Control2_SetLanguage(This,lang)	\
    (This)->lpVtbl -> SetLanguage(This,lang)

#define IGM0Control2_DoAutoZero(This)	\
    (This)->lpVtbl -> DoAutoZero(This)

#define IGM0Control2_DoNull(This)	\
    (This)->lpVtbl -> DoNull(This)

#define IGM0Control2_GetRegister(This,reg)	\
    (This)->lpVtbl -> GetRegister(This,reg)

#define IGM0Control2_ProbeInSheild(This)	\
    (This)->lpVtbl -> ProbeInSheild(This)

#define IGM0Control2_Initalise(This)	\
    (This)->lpVtbl -> Initalise(This)

#define IGM0Control2_close(This)	\
    (This)->lpVtbl -> close(This)

#define IGM0Control2_settime(This)	\
    (This)->lpVtbl -> settime(This)

#define IGM0Control2_gettime(This)	\
    (This)->lpVtbl -> gettime(This)

#define IGM0Control2_get_lastrange(This,pVal)	\
    (This)->lpVtbl -> get_lastrange(This,pVal)

#define IGM0Control2_get_gmserial(This,pVal)	\
    (This)->lpVtbl -> get_gmserial(This,pVal)

#define IGM0Control2_get_probeserial(This,pVal)	\
    (This)->lpVtbl -> get_probeserial(This,pVal)

#define IGM0Control2_startcmdseq(This)	\
    (This)->lpVtbl -> startcmdseq(This)

#define IGM0Control2_endcmdseq(This)	\
    (This)->lpVtbl -> endcmdseq(This)

#define IGM0Control2_get_sendtimeinfo(This,pVal)	\
    (This)->lpVtbl -> get_sendtimeinfo(This,pVal)

#define IGM0Control2_put_sendtimeinfo(This,newVal)	\
    (This)->lpVtbl -> put_sendtimeinfo(This,newVal)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IGM0Control2_get_ComPort_Proxy( 
    IGM0Control2 __RPC_FAR * This,
    /* [retval][out] */ short __RPC_FAR *pVal);


void __RPC_STUB IGM0Control2_get_ComPort_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IGM0Control2_put_ComPort_Proxy( 
    IGM0Control2 __RPC_FAR * This,
    /* [in] */ short newVal);


void __RPC_STUB IGM0Control2_put_ComPort_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IGM0Control2_get_lastvalue_Proxy( 
    IGM0Control2 __RPC_FAR * This,
    /* [retval][out] */ float __RPC_FAR *pVal);


void __RPC_STUB IGM0Control2_get_lastvalue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IGM0Control2_get_lastmode_Proxy( 
    IGM0Control2 __RPC_FAR * This,
    /* [retval][out] */ short __RPC_FAR *pVal);


void __RPC_STUB IGM0Control2_get_lastmode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IGM0Control2_get_lastunits_Proxy( 
    IGM0Control2 __RPC_FAR * This,
    /* [retval][out] */ short __RPC_FAR *pVal);


void __RPC_STUB IGM0Control2_get_lastunits_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IGM0Control2_get_store_value_Proxy( 
    IGM0Control2 __RPC_FAR * This,
    /* [retval][out] */ double __RPC_FAR *pVal);


void __RPC_STUB IGM0Control2_get_store_value_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IGM0Control2_get_store_range_Proxy( 
    IGM0Control2 __RPC_FAR * This,
    /* [retval][out] */ short __RPC_FAR *pVal);


void __RPC_STUB IGM0Control2_get_store_range_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IGM0Control2_get_store_units_Proxy( 
    IGM0Control2 __RPC_FAR * This,
    /* [retval][out] */ short __RPC_FAR *pVal);


void __RPC_STUB IGM0Control2_get_store_units_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IGM0Control2_get_store_mode_Proxy( 
    IGM0Control2 __RPC_FAR * This,
    /* [retval][out] */ short __RPC_FAR *pVal);


void __RPC_STUB IGM0Control2_get_store_mode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IGM0Control2_get_year_Proxy( 
    IGM0Control2 __RPC_FAR * This,
    /* [retval][out] */ short __RPC_FAR *pVal);


void __RPC_STUB IGM0Control2_get_year_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IGM0Control2_put_year_Proxy( 
    IGM0Control2 __RPC_FAR * This,
    /* [in] */ short newVal);


void __RPC_STUB IGM0Control2_put_year_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IGM0Control2_get_month_Proxy( 
    IGM0Control2 __RPC_FAR * This,
    /* [retval][out] */ short __RPC_FAR *pVal);


void __RPC_STUB IGM0Control2_get_month_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IGM0Control2_put_month_Proxy( 
    IGM0Control2 __RPC_FAR * This,
    /* [in] */ short newVal);


void __RPC_STUB IGM0Control2_put_month_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IGM0Control2_get_day_Proxy( 
    IGM0Control2 __RPC_FAR * This,
    /* [retval][out] */ short __RPC_FAR *pVal);


void __RPC_STUB IGM0Control2_get_day_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IGM0Control2_put_day_Proxy( 
    IGM0Control2 __RPC_FAR * This,
    /* [in] */ short newVal);


void __RPC_STUB IGM0Control2_put_day_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IGM0Control2_get_hour_Proxy( 
    IGM0Control2 __RPC_FAR * This,
    /* [retval][out] */ short __RPC_FAR *pVal);


void __RPC_STUB IGM0Control2_get_hour_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IGM0Control2_put_hour_Proxy( 
    IGM0Control2 __RPC_FAR * This,
    /* [in] */ short newVal);


void __RPC_STUB IGM0Control2_put_hour_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IGM0Control2_get_min_Proxy( 
    IGM0Control2 __RPC_FAR * This,
    /* [retval][out] */ short __RPC_FAR *pVal);


void __RPC_STUB IGM0Control2_get_min_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IGM0Control2_put_min_Proxy( 
    IGM0Control2 __RPC_FAR * This,
    /* [in] */ short newVal);


void __RPC_STUB IGM0Control2_put_min_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IGM0Control2_get_sec_Proxy( 
    IGM0Control2 __RPC_FAR * This,
    /* [retval][out] */ short __RPC_FAR *pVal);


void __RPC_STUB IGM0Control2_get_sec_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IGM0Control2_put_sec_Proxy( 
    IGM0Control2 __RPC_FAR * This,
    /* [in] */ short newVal);


void __RPC_STUB IGM0Control2_put_sec_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IGM0Control2_AttemptConnect_Proxy( 
    IGM0Control2 __RPC_FAR * This);


void __RPC_STUB IGM0Control2_AttemptConnect_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IGM0Control2_SetMode_Proxy( 
    IGM0Control2 __RPC_FAR * This,
    short mode);


void __RPC_STUB IGM0Control2_SetMode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IGM0Control2_SetRange_Proxy( 
    IGM0Control2 __RPC_FAR * This,
    short range);


void __RPC_STUB IGM0Control2_SetRange_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IGM0Control2_SetUnits_Proxy( 
    IGM0Control2 __RPC_FAR * This,
    short units);


void __RPC_STUB IGM0Control2_SetUnits_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IGM0Control2_SetLanguage_Proxy( 
    IGM0Control2 __RPC_FAR * This,
    short lang);


void __RPC_STUB IGM0Control2_SetLanguage_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IGM0Control2_DoAutoZero_Proxy( 
    IGM0Control2 __RPC_FAR * This);


void __RPC_STUB IGM0Control2_DoAutoZero_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IGM0Control2_DoNull_Proxy( 
    IGM0Control2 __RPC_FAR * This);


void __RPC_STUB IGM0Control2_DoNull_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IGM0Control2_GetRegister_Proxy( 
    IGM0Control2 __RPC_FAR * This,
    short reg);


void __RPC_STUB IGM0Control2_GetRegister_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IGM0Control2_ProbeInSheild_Proxy( 
    IGM0Control2 __RPC_FAR * This);


void __RPC_STUB IGM0Control2_ProbeInSheild_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IGM0Control2_Initalise_Proxy( 
    IGM0Control2 __RPC_FAR * This);


void __RPC_STUB IGM0Control2_Initalise_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IGM0Control2_close_Proxy( 
    IGM0Control2 __RPC_FAR * This);


void __RPC_STUB IGM0Control2_close_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IGM0Control2_settime_Proxy( 
    IGM0Control2 __RPC_FAR * This);


void __RPC_STUB IGM0Control2_settime_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IGM0Control2_gettime_Proxy( 
    IGM0Control2 __RPC_FAR * This);


void __RPC_STUB IGM0Control2_gettime_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IGM0Control2_get_lastrange_Proxy( 
    IGM0Control2 __RPC_FAR * This,
    /* [retval][out] */ short __RPC_FAR *pVal);


void __RPC_STUB IGM0Control2_get_lastrange_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IGM0Control2_get_gmserial_Proxy( 
    IGM0Control2 __RPC_FAR * This,
    /* [retval][out] */ short __RPC_FAR *pVal);


void __RPC_STUB IGM0Control2_get_gmserial_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IGM0Control2_get_probeserial_Proxy( 
    IGM0Control2 __RPC_FAR * This,
    /* [retval][out] */ short __RPC_FAR *pVal);


void __RPC_STUB IGM0Control2_get_probeserial_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IGM0Control2_startcmdseq_Proxy( 
    IGM0Control2 __RPC_FAR * This);


void __RPC_STUB IGM0Control2_startcmdseq_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IGM0Control2_endcmdseq_Proxy( 
    IGM0Control2 __RPC_FAR * This);


void __RPC_STUB IGM0Control2_endcmdseq_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IGM0Control2_get_sendtimeinfo_Proxy( 
    IGM0Control2 __RPC_FAR * This,
    /* [retval][out] */ BOOL __RPC_FAR *pVal);


void __RPC_STUB IGM0Control2_get_sendtimeinfo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IGM0Control2_put_sendtimeinfo_Proxy( 
    IGM0Control2 __RPC_FAR * This,
    /* [in] */ BOOL newVal);


void __RPC_STUB IGM0Control2_put_sendtimeinfo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IGM0Control2_INTERFACE_DEFINED__ */



#ifndef __GMCONTROL2Lib_LIBRARY_DEFINED__
#define __GMCONTROL2Lib_LIBRARY_DEFINED__

/* library GMCONTROL2Lib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_GMCONTROL2Lib;

#ifndef ___IGM0Control2Events_DISPINTERFACE_DEFINED__
#define ___IGM0Control2Events_DISPINTERFACE_DEFINED__

/* dispinterface _IGM0Control2Events */
/* [helpstring][uuid] */ 


EXTERN_C const IID DIID__IGM0Control2Events;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("9975CC1B-7CBE-4D8A-9753-D66521AE4B4A")
    _IGM0Control2Events : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _IGM0Control2EventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            _IGM0Control2Events __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            _IGM0Control2Events __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            _IGM0Control2Events __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            _IGM0Control2Events __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            _IGM0Control2Events __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            _IGM0Control2Events __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            _IGM0Control2Events __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        END_INTERFACE
    } _IGM0Control2EventsVtbl;

    interface _IGM0Control2Events
    {
        CONST_VTBL struct _IGM0Control2EventsVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _IGM0Control2Events_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define _IGM0Control2Events_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define _IGM0Control2Events_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define _IGM0Control2Events_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define _IGM0Control2Events_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define _IGM0Control2Events_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define _IGM0Control2Events_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___IGM0Control2Events_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_GM0Control2;

#ifdef __cplusplus

class DECLSPEC_UUID("CD25C73B-B31D-41A5-B792-439006542309")
GM0Control2;
#endif
#endif /* __GMCONTROL2Lib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif
