	

// GM0Control2.h : Declaration of the CGM0Control2

/*

   Copyright © 2004 Hirst Magnetic Instruments Ltd and Robin Cornelius.



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





#ifndef __GM0CONTROL2_H_

#define __GM0CONTROL2_H_



#include "resource.h"       // main symbols

#include <atlctl.h>

#include "GMControl2CP.h"

#include "../lib/gm0.h"





/////////////////////////////////////////////////////////////////////////////

// CGM0Control2

class ATL_NO_VTABLE CGM0Control2 : 

	public CComObjectRootEx<CComSingleThreadModel>,

	public IDispatchImpl<IGM0Control2, &IID_IGM0Control2, &LIBID_GMCONTROL2Lib>,

	public CComControl<CGM0Control2>,

	public IPersistStreamInitImpl<CGM0Control2>,

	public IOleControlImpl<CGM0Control2>,

	public IOleObjectImpl<CGM0Control2>,

	public IOleInPlaceActiveObjectImpl<CGM0Control2>,

	public IViewObjectExImpl<CGM0Control2>,

	public IOleInPlaceObjectWindowlessImpl<CGM0Control2>,

	public ISupportErrorInfo,

	public IConnectionPointContainerImpl<CGM0Control2>,

	public IPersistStorageImpl<CGM0Control2>,

	public ISpecifyPropertyPagesImpl<CGM0Control2>,

	public IQuickActivateImpl<CGM0Control2>,

	public IDataObjectImpl<CGM0Control2>,

	public IProvideClassInfo2Impl<&CLSID_GM0Control2, &DIID__IGM0Control2Events, &LIBID_GMCONTROL2Lib>,

	public IPropertyNotifySinkCP<CGM0Control2>,

	public CComCoClass<CGM0Control2, &CLSID_GM0Control2>,

	public CProxy_IGM0Control2Events< CGM0Control2 >

{

public:

	FILE * mystderr;

	BOOL threadlock;

	bool abort;

	bool connected;

	HANDLEGM thegm;

	float value;

	int range,mode,units;

	int m_iComPort;

//	int hour,min,sec,day,month,year;



	CGM0Control2()

	{

		threadlock=false;

		thegm=-1;

		mode=-1;

		range=-1;

		units=-1;

		value=0.000;

		connected=false;

		abort=false;

		m_pUnkMarshaler = NULL;

	}



	~CGM0Control2()

	{

	//	::MessageBox(NULL,"OLE ABORT","ABORTING",MB_OK);

		abort=true;

		

	if (thegm>=0)

	{

	//	::MessageBox(NULL,"Starting ABORT Shutdown","ABORTING",MB_OK);



	//	while(threadlock==true)

	//	{

	//		Sleep(20);

	//	}

		

		gm0_killgm(thegm);

		thegm=-1;

		//Sleep(2000);

	}





	}





DECLARE_GET_CONTROLLING_UNKNOWN()

DECLARE_REGISTRY_RESOURCEID(IDR_GM0CONTROL2)



DECLARE_PROTECT_FINAL_CONSTRUCT()



BEGIN_COM_MAP(CGM0Control2)

	COM_INTERFACE_ENTRY(IGM0Control2)

	COM_INTERFACE_ENTRY(IDispatch)

	COM_INTERFACE_ENTRY(IViewObjectEx)

	COM_INTERFACE_ENTRY(IViewObject2)

	COM_INTERFACE_ENTRY(IViewObject)

	COM_INTERFACE_ENTRY(IOleInPlaceObjectWindowless)

	COM_INTERFACE_ENTRY(IOleInPlaceObject)

	COM_INTERFACE_ENTRY2(IOleWindow, IOleInPlaceObjectWindowless)

	COM_INTERFACE_ENTRY(IOleInPlaceActiveObject)

	COM_INTERFACE_ENTRY(IOleControl)

	COM_INTERFACE_ENTRY(IOleObject)

	COM_INTERFACE_ENTRY(IPersistStreamInit)

	COM_INTERFACE_ENTRY2(IPersist, IPersistStreamInit)

	COM_INTERFACE_ENTRY(ISupportErrorInfo)

	COM_INTERFACE_ENTRY(IConnectionPointContainer)

	COM_INTERFACE_ENTRY(ISpecifyPropertyPages)

	COM_INTERFACE_ENTRY(IQuickActivate)

	COM_INTERFACE_ENTRY(IPersistStorage)

	COM_INTERFACE_ENTRY(IDataObject)

	COM_INTERFACE_ENTRY(IProvideClassInfo)

	COM_INTERFACE_ENTRY(IProvideClassInfo2)

	COM_INTERFACE_ENTRY_AGGREGATE(IID_IMarshal, m_pUnkMarshaler.p)

	COM_INTERFACE_ENTRY_IMPL(IConnectionPointContainer)

END_COM_MAP()



BEGIN_PROP_MAP(CGM0Control2)

	PROP_DATA_ENTRY("_cx", m_sizeExtent.cx, VT_UI4)

	PROP_DATA_ENTRY("_cy", m_sizeExtent.cy, VT_UI4)

	// Example entries

	// PROP_ENTRY("Property Description", dispid, clsid)

	// PROP_PAGE(CLSID_StockColorPage)

END_PROP_MAP()



BEGIN_CONNECTION_POINT_MAP(CGM0Control2)

	CONNECTION_POINT_ENTRY(IID_IPropertyNotifySink)

	CONNECTION_POINT_ENTRY(DIID__IGM0Control2Events)

END_CONNECTION_POINT_MAP()



BEGIN_MSG_MAP(CGM0Control2)

	CHAIN_MSG_MAP(CComControl<CGM0Control2>)

	DEFAULT_REFLECTION_HANDLER()

END_MSG_MAP()

// Handler prototypes:

//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);







	HRESULT FinalConstruct()

	{

		return CoCreateFreeThreadedMarshaler(

			GetControllingUnknown(), &m_pUnkMarshaler.p);

	}



	void FinalRelease()

	{

		m_pUnkMarshaler.Release();

	}



	CComPtr<IUnknown> m_pUnkMarshaler;



// ISupportsErrorInfo

	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid)

	{

		static const IID* arr[] = 

		{

			&IID_IGM0Control2,

		};

		for (int i=0; i<sizeof(arr)/sizeof(arr[0]); i++)

		{

			if (InlineIsEqualGUID(*arr[i], riid))

				return S_OK;

		}

		return S_FALSE;

	}



// IViewObjectEx

	DECLARE_VIEW_STATUS(VIEWSTATUS_SOLIDBKGND | VIEWSTATUS_OPAQUE)



// IGM0Control2

public:

	STDMETHOD(get_sendtimeinfo)(/*[out, retval]*/ BOOL *pVal);

	STDMETHOD(put_sendtimeinfo)(/*[in]*/ BOOL newVal);

	STDMETHOD(endcmdseq)();

	STDMETHOD(startcmdseq)();

	STDMETHOD(get_probeserial)(/*[out, retval]*/ short *pVal);

	STDMETHOD(get_gmserial)(/*[out, retval]*/ short *pVal);

	STDMETHOD(get_lastrange)(/*[out, retval]*/ short *pVal);

	STDMETHOD(gettime)();

	STDMETHOD(settime)();

	STDMETHOD(close)();

	STDMETHOD(Initalise)();

	STDMETHOD(ProbeInSheild)();

	STDMETHOD(GetRegister)(short reg);

	STDMETHOD(DoNull)();

	STDMETHOD(DoAutoZero)();

	STDMETHOD(SetLanguage)(short lang);

	STDMETHOD(SetUnits)(short units);

	STDMETHOD(SetRange)(short range);

	STDMETHOD(SetMode)(short mode);

	STDMETHOD(AttemptConnect)();

	STDMETHOD(get_sec)(/*[out, retval]*/ short *pVal);

	STDMETHOD(put_sec)(/*[in]*/ short newVal);

	STDMETHOD(get_min)(/*[out, retval]*/ short *pVal);

	STDMETHOD(put_min)(/*[in]*/ short newVal);

	STDMETHOD(get_hour)(/*[out, retval]*/ short *pVal);

	STDMETHOD(put_hour)(/*[in]*/ short newVal);

	STDMETHOD(get_day)(/*[out, retval]*/ short *pVal);

	STDMETHOD(put_day)(/*[in]*/ short newVal);

	STDMETHOD(get_month)(/*[out, retval]*/ short *pVal);

	STDMETHOD(put_month)(/*[in]*/ short newVal);

	STDMETHOD(get_year)(/*[out, retval]*/ short *pVal);

	STDMETHOD(put_year)(/*[in]*/ short newVal);

	STDMETHOD(get_store_mode)(/*[out, retval]*/ short *pVal);

	STDMETHOD(get_store_units)(/*[out, retval]*/ short *pVal);

	STDMETHOD(get_store_range)(/*[out, retval]*/ short *pVal);

	STDMETHOD(get_store_value)(/*[out, retval]*/ double *pVal);

	STDMETHOD(get_lastunits)(/*[out, retval]*/ short *pVal);

	STDMETHOD(get_lastmode)(/*[out, retval]*/ short *pVal);

	STDMETHOD(get_lastvalue)(/*[out, retval]*/ float *pVal);

	STDMETHOD(get_ComPort)(/*[out, retval]*/ short *pVal);

	STDMETHOD(put_ComPort)(/*[in]*/ short newVal);



	HRESULT OnDraw(ATL_DRAWINFO& di)

	{

		RECT& rc = *(RECT*)di.prcBounds;

		Rectangle(di.hdcDraw, rc.left, rc.top, rc.right, rc.bottom);



		SetTextAlign(di.hdcDraw, TA_CENTER|TA_BASELINE);

		LPCTSTR pszText = _T("ATL 3.0 : GM0Control2");

		TextOut(di.hdcDraw, 

			(rc.left + rc.right) / 2, 

			(rc.top + rc.bottom) / 2, 

			pszText, 

			lstrlen(pszText));



		return S_OK;

	}

};



#endif //__GM0CONTROL2_H_

