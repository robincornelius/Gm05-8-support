#ifndef __CEVENT_PROXY__
#define __CEVENT_PROXY__

#pragma warning(disable:4530)	//Template memory warning
#pragma warning(disable:4786)	//Template memory warning

#include <map>
#include <comdef.h>
#include <atlcom.h>
using namespace std;

class CComDynamicUnkArray_GIT : public CComDynamicUnkArray
{
    private:
        IGlobalInterfaceTable*  GIT;

    public:

        CComDynamicUnkArray_GIT() : CComDynamicUnkArray()
        { 
            GIT = NULL;

            CoCreateInstance( CLSID_StdGlobalInterfaceTable, 
                              NULL, 
                              CLSCTX_INPROC_SERVER, 
                              __uuidof(IGlobalInterfaceTable), 
                              reinterpret_cast< void** >(&GIT) );
        }

	    ~CComDynamicUnkArray_GIT()
	    {
            clear();
            if( GIT != NULL )
            {
                GIT->Release();
            }
	    }
	    DWORD Add(IUnknown* pUnk);
	    BOOL Remove(DWORD dwCookie);

        CComPtr<IUnknown> GetAt(int nIndex)
	    {
            DWORD dwCookie = (DWORD)CComDynamicUnkArray::GetAt( nIndex );

            if( dwCookie == 0 )
			    return NULL;

            if( CookieMap.find( dwCookie ) == CookieMap.end() )
            {
    		    return (IUnknown*)dwCookie;
            }
            if( GIT != NULL )
            {
                CComPtr<IUnknown>   ppv;

                HRESULT hr = GIT->GetInterfaceFromGlobal(
                  CookieMap[dwCookie],              //Cookie identifying the desired global 
                                                    //interface and its object
                  __uuidof(IUnknown),               //IID of the registered global interface
                  reinterpret_cast< void** >(&ppv)  //Indirect pointer to the desired interface
                );
                if( hr == S_OK )
                {
                    return ppv;
                }
            }
		    return (IUnknown*)dwCookie;
	    }

	    void clear()
	    {
            CComDynamicUnkArray::clear();

            if( GIT != NULL )
            {
                map< DWORD, DWORD >::iterator iter;
                for( iter = CookieMap.begin(); iter != CookieMap.end(); ++iter )
                {
                    GIT->RevokeInterfaceFromGlobal(
                      iter->second              //Cookie that was returned from 
                                                //RegisterInterfaceInGlobal
                    );
                }
            }
            CookieMap.clear();
	    }
    protected:
        map< DWORD, DWORD > CookieMap;
};

inline DWORD CComDynamicUnkArray_GIT::Add(IUnknown* pUnk)
{
    DWORD Result = CComDynamicUnkArray::Add( pUnk );

    HRESULT hr;
    DWORD   pdwCookie = 0;
    if( GIT != NULL )
    {
        hr = GIT->RegisterInterfaceInGlobal(
          pUnk,                 //Pointer to interface of type riid of object 
                                //containing global interface
          __uuidof(IUnknown),   //IID of the interface to be registered
          &pdwCookie            //Supplies a pointer to the cookie that provides 
                                //a caller in another apartment access to the 
                                //interface pointer
        );
    }
    if( hr == S_OK )
    {
        CookieMap[Result] = pdwCookie;
    }

	return Result;
}

inline BOOL CComDynamicUnkArray_GIT::Remove(DWORD dwCookie)
{
    BOOL Result = CComDynamicUnkArray::Remove( dwCookie );

    if( GIT != NULL )
    {
        if( CookieMap.find( dwCookie ) != CookieMap.end() )
        {
            GIT->RevokeInterfaceFromGlobal(
              CookieMap[dwCookie]   //Cookie that was returned from 
                                    //RegisterInterfaceInGlobal
            );
            CookieMap.erase(dwCookie);
        }
    }
    return Result;
}

#endif