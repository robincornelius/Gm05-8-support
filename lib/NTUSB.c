/*

   Copyright © 2007 Hirst Magnetic Instruments Ltd 
   Copyright © 2007-2008 Robin Cornelius <robin.cornelius@hirst-magnetics.com>
   Copyright © 2007 Paul Knell <paul.knell@hirst-magnetics.com>

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

#include <windows.h>
#include <wtypes.h>
#include <initguid.h>
#include <setupapi.h>
#include <stdio.h>
#include <time.h>
#include "hidsdi.h"
#include "ntusb.h"
#include "gm0_private.h"
#include "gm0.h"


#define HANDLEGM int

#define	MAXREPORTSIZE 256


//Application global variables 


BOOL FindTheHID(HANDLEGM hand)
{
	//Use a series of API calls to find a HID with a matching Vendor and Product ID.

	HIDD_ATTRIBUTES						Attributes;
	SP_DEVICE_INTERFACE_DATA			devInfoData;
	BOOL							 	LastDevice = FALSE;
	int									MemberIndex = 0;
	LONG								Result;

	int									no_devices=0;

	//These are the vendor and product IDs to look for.
	//Uses Lakeview Research's Vendor ID.
	const unsigned int VendorID = 0x04d8;
	const unsigned int ProductID = 0x0002;

	pGMS[hand]->Length = 0;
	pGMS[hand]->detailData = NULL;
	pGMS[hand]->DeviceHandle=-1;
	pGMS[hand]->ReadHandle=-1;
	pGMS[hand]->hEventObject=0;


	/*
	API function: HidD_GetHidGuid
	Get the GUID for all system HIDs.
	Returns: the GUID in HidGuid.
	*/

	HidD_GetHidGuid(&pGMS[hand]->HidGuid);	
	
	/*
	API function: SetupDiGetClassDevs
	Returns: a handle to a device information set for all installed devices.
	Requires: the GUID returned by GetHidGuid.
	*/
	
	pGMS[hand]->hDevInfo=SetupDiGetClassDevs 
		(&pGMS[hand]->HidGuid, 
		NULL, 
		NULL, 
		DIGCF_PRESENT|DIGCF_INTERFACEDEVICE);
		
	devInfoData.cbSize = sizeof(devInfoData);

	//Step through the available devices looking for the one we want. 
	//Quit on detecting the desired device or checking all available devices without success.
	MemberIndex = 0;
	LastDevice = FALSE;

	do
	{
		pGMS[hand]->USBconnected=FALSE;

		/*
		API function: SetupDiEnumDeviceInterfaces
		On return, MyDeviceInterfaceData contains the handle to a
		SP_DEVICE_INTERFACE_DATA structure for a detected device.
		Requires:
		The DeviceInfoSet returned in SetupDiGetClassDevs.
		The HidGuid returned in GetHidGuid.
		An index to specify a device.
		*/

		Result=SetupDiEnumDeviceInterfaces 
			(pGMS[hand]->hDevInfo, 
			0, 
			&pGMS[hand]->HidGuid, 
			MemberIndex, 
			&devInfoData);

		if (Result != 0)
		{
			//A device has been detected, so get more information about it.

			/*
			API function: SetupDiGetDeviceInterfaceDetail
			Returns: an SP_DEVICE_INTERFACE_DETAIL_DATA structure
			containing information about a device.
			To retrieve the information, call this function twice.
			The first time returns the size of the structure in Length.
			The second time returns a pointer to the data in DeviceInfoSet.
			Requires:
			A DeviceInfoSet returned by SetupDiGetClassDevs
			The SP_DEVICE_INTERFACE_DATA structure returned by SetupDiEnumDeviceInterfaces.
			
			The final parameter is an optional pointer to an SP_DEV_INFO_DATA structure.
			This application doesn't retrieve or use the structure.			
			If retrieving the structure, set 
			MyDeviceInfoData.cbSize = length of MyDeviceInfoData.
			and pass the structure's address.
			*/
			
			//Get the Length value.
			//The call will return with a "buffer too small" error which can be ignored.
			Result = SetupDiGetDeviceInterfaceDetail 
				(pGMS[hand]->hDevInfo, 
				&devInfoData, 
				NULL, 
				0, 
				&pGMS[hand]->Length, 
				NULL);

			//Allocate memory for the hDevInfo structure, using the returned Length.
			pGMS[hand]->detailData = (PSP_DEVICE_INTERFACE_DETAIL_DATA)malloc(pGMS[hand]->Length);
			
			//Set cbSize in the detailData structure.
			pGMS[hand]->detailData -> cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);

			//Call the function again, this time passing it the returned buffer size.
			Result = SetupDiGetDeviceInterfaceDetail 
				(pGMS[hand]->hDevInfo, 
				&devInfoData, 
				pGMS[hand]->detailData, 
				pGMS[hand]->Length, 
				&pGMS[hand]->Required, 
				NULL);

			//Open a handle to the device.

			/*
			API function: CreateFile
			Returns: a handle that enables reading and writing to the device.
			Requires:
			The DevicePath in the detailData structure
			returned by SetupDiGetDeviceInterfaceDetail.
			*/

			pGMS[hand]->DeviceHandle=CreateFile 
				(pGMS[hand]->detailData->DevicePath, 
				GENERIC_READ|GENERIC_WRITE, 
				FILE_SHARE_READ|FILE_SHARE_WRITE, 
				(LPSECURITY_ATTRIBUTES)NULL,
				OPEN_EXISTING, 
				0, 
				NULL);

			if(pGMS[hand]->DeviceHandle==-1)
			{
				free(pGMS[hand]->detailData);
				MemberIndex++;			
				continue;
			}
			/*
			API function: HidD_GetAttributes
			Requests information from the device.
			Requires: the handle returned by CreateFile.
			Returns: a HIDD_ATTRIBUTES structure containing
			the Vendor ID, Product ID, and Product Version Number.
			Use this information to decide if the detected device is
			the one we're looking for.
			*/

			//Set the Size to the number of bytes in the structure.
			Attributes.Size = sizeof(Attributes);

			Result = HidD_GetAttributes 
				(pGMS[hand]->DeviceHandle, 
				&Attributes);
			
			OutputDebugString("HidD_GetAttributes: \n");
			
			//Is it the desired device?
			pGMS[hand]->USBconnected=FALSE;
			
			if (Attributes.VendorID == VendorID)
			{
				if (Attributes.ProductID == ProductID)
				{
					
					OutputDebugString("Device detected\n");

					no_devices++;
					if(pGMS[hand]->m_Iportno*-1==no_devices)
					{
						//Both the Product and Vendor IDs match.
						pGMS[hand]->USBconnected = TRUE;
						//Get the device's capablities.
						GetDeviceCapabilities(hand);
						PrepareForOverlappedTransfer(hand);
					}
					else
					{
						OutputDebugString("Not target device\n");
					}
				} //if (Attributes.ProductID == ProductID)
				else
				{
					//The Product ID doesn't match.
					safeclosehandle(&pGMS[hand]->DeviceHandle);
				}
			} //if (Attributes.VendorID == VendorID)
			else
			{
				//The Vendor ID doesn't match.
				safeclosehandle(&pGMS[hand]->DeviceHandle);
			}

		//Free the memory used by the detailData structure (no longer needed).
		free(pGMS[hand]->detailData);
		}  //if (Result != 0)

		else
			//SetupDiEnumDeviceInterfaces returned 0, so there are no more devices to check.
			LastDevice=TRUE;

		//If we haven't found the device yet, and haven't tried every available device,
		//try the next one.
		MemberIndex = MemberIndex + 1;

	} //do
	while ((LastDevice == FALSE) && (pGMS[hand]->USBconnected == FALSE));

	//Free the memory reserved for hDevInfo by SetupDiClassDevs.
	SetupDiDestroyDeviceInfoList(pGMS[hand]->hDevInfo);
	OutputDebugString("SetupDiDestroyDeviceInfoList\n");


	if (pGMS[hand]->USBconnected == FALSE)
	{
		OutputDebugString("Device not detected\n");
		return -1;
	}
	else
	{	
		OutputDebugString("Device detected\n");
		return 0;
	}
}


void PrepareForOverlappedTransfer(HANDLEGM hand)
{
	//Get another handle to the device for the overlapped ReadFiles.
	pGMS[hand]->ReadHandle=CreateFile 
		(pGMS[hand]->detailData->DevicePath, 
		GENERIC_READ|GENERIC_WRITE, 
		FILE_SHARE_READ|FILE_SHARE_WRITE,
		(LPSECURITY_ATTRIBUTES)NULL, 
		OPEN_EXISTING, 
		FILE_FLAG_OVERLAPPED, 
		NULL);
	OutputDebugString("CreateFile (ReadHandle): \n");

	//Get an event object for the overlapped structure.

	/*API function: CreateEvent
	Requires:
	  Security attributes or Null
	  Manual reset (true). Use ResetEvent to set the event object's state to non-signaled.
	  Initial state (true = signaled) 
	  Event object name (optional)
	Returns: a handle to the event object
	*/

	if (pGMS[hand]->hEventObject == 0)
	{
		pGMS[hand]->hEventObject = CreateEvent 
			(NULL, 
			TRUE, 
			TRUE, 
			"");
	OutputDebugString("CreateEvent: \n") ;

	//Set the members of the overlapped structure.
	pGMS[hand]->HIDOverlapped.hEvent = pGMS[hand]->hEventObject;
	pGMS[hand]->HIDOverlapped.Offset = 0;
	pGMS[hand]->HIDOverlapped.OffsetHigh = 0;
	}
}


void GetDeviceCapabilities(HANDLEGM hand)
{
	//Get the Capabilities structure for the device.
	PHIDP_PREPARSED_DATA	PreparsedData;

	/*
	API function: HidD_GetPreparsedData
	Returns: a pointer to a buffer containing the information about the device's capabilities.
	Requires: A handle returned by CreateFile.
	There's no need to access the buffer directly,
	but HidP_GetCaps and other API functions require a pointer to the buffer.
	*/

	HidD_GetPreparsedData 
		(pGMS[hand]->DeviceHandle, 
		&PreparsedData);
	OutputDebugString("HidD_GetPreparsedData: \n");

	/*
	API function: HidP_GetCaps
	Learn the device's capabilities.
	For standard devices such as joysticks, you can find out the specific
	capabilities of the device.
	For a custom device, the software will probably know what the device is capable of,
	and the call only verifies the information.
	Requires: the pointer to the buffer returned by HidD_GetPreparsedData.
	Returns: a Capabilities structure containing the information.
	*/
	
	HidP_GetCaps 
		(PreparsedData, 
		&pGMS[hand]->Capabilities);
	OutputDebugString("HidP_GetCaps: \n");

	//No need for PreparsedData any more, so free the memory it's using.
	HidD_FreePreparsedData(PreparsedData);
	OutputDebugString("HidD_FreePreparsedData: \n") ;
}


DWORD ReadReport(HANDLEGM hand,char	* InputReport)
{
//	CString	ByteToDisplay = "";
//	CString	MessageToDisplay = "";
	DWORD	Result;
	
	//Read a report from the device.
 	
	/*API call:ReadFile
	'Returns: the report in InputReport.
	'Requires: a device handle returned by CreateFile
	'(for overlapped I/O, CreateFile must be called with FILE_FLAG_OVERLAPPED),
	'the Input report length in bytes returned by HidP_GetCaps,
	'and an overlapped structure whose hEvent member is set to an event object.
	*/

	Result = ReadFile 
		(pGMS[hand]->ReadHandle, 
		InputReport, 
		pGMS[hand]->Capabilities.InputReportByteLength, 
		&pGMS[hand]->NumberOfBytesRead,
		(LPOVERLAPPED) &pGMS[hand]->HIDOverlapped); 
 
	//DisplayLastError("ReadFile: ") ;

	/*API call:WaitForSingleObject
	'Used with overlapped ReadFile.
	'Returns when ReadFile has received the requested amount of data or on timeout.
	'Requires an event object created with CreateEvent
	'and a timeout value in milliseconds.
	*/

	Result = WaitForSingleObject 
		(pGMS[hand]->hEventObject, 
		5000);
	//DisplayLastError("WaitForSingleObject: ") ;
 
	switch (Result)
	{
	case WAIT_OBJECT_0:
		{
		//ValueToDisplay.Format("%s", "ReadFile Completed");
		//DisplayData(ValueToDisplay);
		break;
		}
	case WAIT_TIMEOUT:
		{
//		ValueToDisplay.Format("%s", );
		OutputDebugString("ReadFile timeout");
		//Cancel the Read operation.

		/*API call: CancelIo
		Cancels the ReadFile
        Requires the device handle.
        Returns non-zero on success.
		*/
		
		Result = CancelIo(pGMS[hand]->ReadHandle);
		
		//A timeout may mean that the device has been removed. 
		//Close the device handles and set DeviceDetected = False 
		//so the next access attempt will search for the device.
		safeclosehandle(&pGMS[hand]->ReadHandle);
		safeclosehandle(&pGMS[hand]->DeviceHandle);
		OutputDebugString("Can't read from device");
		pGMS[hand]->USBconnected=FALSE;
		break;
	default:
		OutputDebugString("Undefined error");
		break;
		}
	}

	/*
	API call: ResetEvent
	Sets the event object to non-signaled.
	Requires a handle to the event object.
	Returns non-zero on success.
	*/

	ResetEvent(pGMS[hand]->hEventObject);

	//Display the report data.
	//DisplayInputReport();

	return(Result);
}

DWORD WriteReport(HANDLEGM hand,CHAR * OutputReport)
{
	//Send a report to the device.
	DWORD	BytesWritten = 0;
	INT		Index =0;
	ULONG	Result;

	//The first byte is the report number.
	OutputReport[0]=0;

	/*
	API Function: WriteFile
	Sends a report to the device.
	Returns: success or failure.
	Requires:
	The device handle returned by CreateFile.
	The Output Report length returned by HidP_GetCaps,
	A report to send.
	*/

	Result = WriteFile 
		(pGMS[hand]->DeviceHandle, 
		OutputReport, 
		pGMS[hand]->Capabilities.OutputReportByteLength, 
		&BytesWritten, 
		NULL);

	if (Result == 0)
	{
		//The WriteFile failed, so close the handle, display a message,
		//and set DeviceDetected to FALSE so the next attempt will look for the device.
		safeclosehandle(&pGMS[hand]->DeviceHandle);
		safeclosehandle(&pGMS[hand]->ReadHandle);
		OutputDebugString("Can't write to device\n");
		pGMS[hand]->USBconnected=FALSE;
	}
	
	return(Result);
}

unsigned char WritepacketToDevice(HANDLEGM hand,unsigned char command, unsigned char data,char * status)
{
	unsigned char dataret;
	CHAR OutputReport[MAXREPORTSIZE];
	CHAR InputReport[MAXREPORTSIZE];
	DWORD Result = 0;

	memset(OutputReport,0,MAXREPORTSIZE);
	memset(InputReport,0,MAXREPORTSIZE);

	//Write a report to the device.
	//Get the values from the combo boxes.
	OutputReport[1]=command;
	Result = WriteReport(hand,OutputReport);


	//Read a report from the device.
	if (Result != 0)
	{
		Result = ReadReport(hand,InputReport);
		dataret=InputReport[1];
	}
	else
	{
		OutputDebugString("Write Error 1\n");
		return(0);
	}

	//Write a report to the device.
	//Get the values from the combo boxes.

	OutputReport[1]=data;
		
	if (Result != WAIT_TIMEOUT)
	{
		Result = WriteReport(hand,OutputReport);
	}
	else
	{
		OutputDebugString("Read Error 1\n");
		return(0);
				
	}

	//Read a report from the device.
	if (Result != 0 )
	{
			Result = ReadReport(hand,InputReport);
			pGMS[hand]->cmdstatus = InputReport[1];
	}
	else
	{
		OutputDebugString("Write Error 2\n");
		return(0);
				
	}

	if (Result == WAIT_TIMEOUT)
	{
		OutputDebugString("Read Error 2\n");
		return(0);
	}

	return(dataret);

}


int GetReadingFromGM08(HANDLEGM hand)
{
	char counter = 0;
	DWORD Result = 0;

	char inttext[16];

	__int16 intreading;

	unsigned char *p_uchar;

	unsigned char Reading[2];
	//Write a report to the device.
	//Get the values from the combo boxes.

	CHAR OutputReport[MAXREPORTSIZE];
	CHAR InputReport[MAXREPORTSIZE];

	OutputReport[1]=49;
	Result = WriteReport(hand,OutputReport);

	//Read a report from the device.
	if (Result != 0)
	{
			Result = ReadReport(hand,InputReport);
			Reading[0] = InputReport[1];
	}
	else
	{
		OutputDebugString("Write Error1");
		return(0);
	}

				//Write a report to the device.
				//Get the values from the combo boxes.

	OutputReport[1]=0;
		

	if (Result != WAIT_TIMEOUT)
	{
		Result = WriteReport(hand,OutputReport);
	}
	else
	{
		OutputDebugString("Read Error1");
		return(0);
				
	}
	//Read a report from the device.
	if (Result != 0 )
	{
			
			Result = ReadReport(hand,InputReport);
			Reading[1] = InputReport[1];
	}
	else
	{
		OutputDebugString("Write Error2");
		return(0);
				
	}

	if (Result == WAIT_TIMEOUT)
	{
		OutputDebugString("Read Error2");
		return(0);
	}

	p_uchar = (unsigned char *) &intreading;

	*p_uchar = (unsigned char) Reading[0];

	*(p_uchar+1) = (unsigned char) Reading[1];

	sprintf(inttext, "%d",   intreading);

	//DisplayData(inttext);

	return((int) intreading);

}


void polldata(HANDLEGM hand)
{
	double data;
	int tempmode;

	if(pGMS[hand]->m_Iportno>0)
		return;

	WritepacketToDevice(hand,42, 42,NULL);

	data = GetReadingFromGM08(hand);

	pGMS[hand]->store.range = WritepacketToDevice(hand,48, 0,NULL)&0x07;

	pGMS[hand]->store.units = WritepacketToDevice(hand,47, 0,NULL)&0x03;

	pGMS[hand]->store.probeoffset = (unsigned char) probe_offset;

	pGMS[hand]->store.time.hour=0;
	pGMS[hand]->store.time.min=0;
	pGMS[hand]->store.time.sec=0;
	pGMS[hand]->store.time.year=0;
	pGMS[hand]->store.time.month=0;
	pGMS[hand]->store.time.day=0;

	tempmode=WritepacketToDevice(hand,46, 0,NULL);

	pGMS[hand]->store.mode = tempmode>4 || tempmode <0 ? 0 : tempmode;

	gm0_convertvalue(pGMS[hand]->store.range,pGMS[hand]->store.units,(float)data,&pGMS[hand]->store.value,TRUE);

}

void closeUSB(HANDLEGM hand)
{
	safeclosehandle(&pGMS[hand]->DeviceHandle);
	safeclosehandle(&pGMS[hand]->ReadHandle);
}

void safeclosehandle(HANDLE * hand)
{

	if((*hand)!=-1)
		CloseHandle((*hand));
	(*hand)=-1;
}
