/*
	a quick and dirty timer
    Copyright (C) 2004  Robin Cornelius/Hirst Magnetic Instruments Ltd

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

*/

#ifndef _HMI_TIMERCP_H_
#define _HMI_TIMERCP_H_

#include "CEventProxy.h"


template <class T>
class CProxy_IHirstTimerEvents : public IConnectionPointImpl<T, &DIID__IHirstTimerEvents, CComDynamicUnkArray_GIT>
{
	//Warning this class may be recreated by the wizard.
public:
	HRESULT Fire_TimerEvent()
	{
		CComVariant varResult;
		T* pT = static_cast<T*>(this);
		int nConnectionIndex;
		int nConnections = m_vec.GetSize();
		
		for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
		{
			pT->Lock();
			CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
			pT->Unlock();
		//	IDispatch* pDispatch = reinterpret_cast<IDispatch*>(sp.p);
			CComQIPtr< IDispatch > pDispatch( sp );;
			if (pDispatch != NULL)
			{
				VariantClear(&varResult);
				DISPPARAMS disp = { NULL, NULL, 0, 0 };
				pDispatch->Invoke(0x1, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &disp, &varResult, NULL, NULL);
			}
		}
		return varResult.scode;
	
	}
};
#endif