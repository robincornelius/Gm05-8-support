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
	
// HirstTimer.h : Declaration of the CHirstTimer

#ifndef __HIRSTTIMER_H_
#define __HIRSTTIMER_H_

#include "resource.h"       // main symbols
#include <atlctl.h>
#include "HMI_timerCP.h"





/////////////////////////////////////////////////////////////////////////////
// CHirstTimer
class ATL_NO_VTABLE CHirstTimer : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public IDispatchImpl<IHirstTimer, &IID_IHirstTimer, &LIBID_HMI_TIMERLib>,
	public CComControl<CHirstTimer>,
	public IPersistStreamInitImpl<CHirstTimer>,
	public IOleControlImpl<CHirstTimer>,
	public IOleObjectImpl<CHirstTimer>,
	public IOleInPlaceActiveObjectImpl<CHirstTimer>,
	public IViewObjectExImpl<CHirstTimer>,
	public IOleInPlaceObjectWindowlessImpl<CHirstTimer>,
	public IConnectionPointContainerImpl<CHirstTimer>,
	public IPersistStorageImpl<CHirstTimer>,
	public ISpecifyPropertyPagesImpl<CHirstTimer>,
	public IQuickActivateImpl<CHirstTimer>,
	public IDataObjectImpl<CHirstTimer>,
	public IProvideClassInfo2Impl<&CLSID_HirstTimer, &DIID__IHirstTimerEvents, &LIBID_HMI_TIMERLib>,
	public IPropertyNotifySinkCP<CHirstTimer>,
	public CComCoClass<CHirstTimer, &CLSID_HirstTimer>,
	public CProxy_IHirstTimerEvents< CHirstTimer >
{
public:
	int timerid;

	CHirstTimer()
	{
		m_pUnkMarshaler = NULL;
		timerid=0;
	}

DECLARE_GET_CONTROLLING_UNKNOWN()
DECLARE_REGISTRY_RESOURCEID(IDR_HIRSTTIMER)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CHirstTimer)
	COM_INTERFACE_ENTRY(IHirstTimer)
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

BEGIN_PROP_MAP(CHirstTimer)
	PROP_DATA_ENTRY("_cx", m_sizeExtent.cx, VT_UI4)
	PROP_DATA_ENTRY("_cy", m_sizeExtent.cy, VT_UI4)
	// Example entries
	// PROP_ENTRY("Property Description", dispid, clsid)
	// PROP_PAGE(CLSID_StockColorPage)
END_PROP_MAP()

BEGIN_CONNECTION_POINT_MAP(CHirstTimer)
	CONNECTION_POINT_ENTRY(IID_IPropertyNotifySink)
	CONNECTION_POINT_ENTRY(DIID__IHirstTimerEvents)
END_CONNECTION_POINT_MAP()

BEGIN_MSG_MAP(CHirstTimer)
	CHAIN_MSG_MAP(CComControl<CHirstTimer>)
	DEFAULT_REFLECTION_HANDLER()
	MESSAGE_HANDLER(WM_TIMER, OnTimer)
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

		::KillTimer(NULL,timerid);
		m_pUnkMarshaler.Release();
	}

	CComPtr<IUnknown> m_pUnkMarshaler;

// IViewObjectEx
	DECLARE_VIEW_STATUS(VIEWSTATUS_SOLIDBKGND | VIEWSTATUS_OPAQUE)

// IHirstTimer
public:
	STDMETHOD(StopTimer)();
	STDMETHOD(StartTimer)();
	STDMETHOD(get_active)(/*[out, retval]*/ BOOL *pVal);
	STDMETHOD(put_active)(/*[in]*/ BOOL newVal);
	STDMETHOD(get_interval)(/*[out, retval]*/ short *pVal);
	STDMETHOD(put_interval)(/*[in]*/ short newVal);

	HRESULT OnDraw(ATL_DRAWINFO& di)
	{
		RECT& rc = *(RECT*)di.prcBounds;
		Rectangle(di.hdcDraw, rc.left, rc.top, rc.right, rc.bottom);

		SetTextAlign(di.hdcDraw, TA_CENTER|TA_BASELINE);
		LPCTSTR pszText = _T("ATL 3.0 : HirstTimer");
		TextOut(di.hdcDraw, 
			(rc.left + rc.right) / 2, 
			(rc.top + rc.bottom) / 2, 
			pszText, 
			lstrlen(pszText));

		return S_OK;
	}

	LRESULT OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		// TODO : Add Code for message handler. Call DefWindowProc if necessary.

		
		return 0;
	}
};

#endif //__HIRSTTIMER_H_
