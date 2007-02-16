

#include <windows.h>
#include <wtypes.h>
#include <initguid.h>
#include <setupapi.h>
#include <stdio.h>
#include <time.h>
#include "m:/winddk/hidsdi.h"
#include "ntusb.h"
#include "gm0_private.h"
#include "gm0.h"


#define HANDLEGM int

#define	MAXREPORTSIZE 256
CHAR	OutputReport[MAXREPORTSIZE];
/*
struct unit_range_struct {

	float range_div;
	char *formatter;
	//char *unit_string;
};

struct	units_struct {

	float global_mult;
	struct unit_range_struct unit_range[4];

};
*/
/*
const struct units_struct units[4]={

						1.0,							
						1000.0, 	"% 05.3f  T ",	//was 10 000 think it was fudge for /10
					 	10.0, 		"% 05.1f mT ",
					 	100.0, 		"% 05.2f mT ",
					 	1000.0,		"% 05.3f mT ",

						1.0,
					 	100.0,		"% 05.2f kG ",
						1000.0,		"% 05.3f kG ",
						10.0,		"% 05.1f  G ",
						100.0,		"% 05.2f  G ",

						0.7957747,
						1.0,		"% 04.0f kA/m",
						10.0,		"% 05.1f kA/m",
						10.0,		"% 05.2f kA/m",
						1000.0, 	"% 05.3f kA/m",

						1.0,
						100.0,		"% 05.2f kOe ",
						1000.0,		"% 05.3f kOe ",
						10.0,		"% 05.1f Oe ",
						100.0, 		"% 05.2f Oe "
						
};
*/

//Application global variables 

	HIDP_CAPS		Capabilities;
	PSP_DEVICE_INTERFACE_DETAIL_DATA	detailData;
	HANDLE			DeviceHandle;
	DWORD			dwError;
	HANDLE			hEventObject;
	HANDLE			hDevInfo;
	GUID			HidGuid;
	OVERLAPPED		HIDOverlapped;
	char			InputReport[3];
	ULONG			Length;
	LPOVERLAPPED	lpOverLap;
	DWORD			NumberOfBytesRead;
	HANDLE			ReadHandle;
	ULONG			Required;


BOOL FindTheHID(HANDLEGM hand)
{
	//Use a series of API calls to find a HID with a matching Vendor and Product ID.

	HIDD_ATTRIBUTES						Attributes;
	SP_DEVICE_INTERFACE_DATA			devInfoData;
	BOOL							 	LastDevice = FALSE;
	int									MemberIndex = 0;
	LONG								Result;

	//These are the vendor and product IDs to look for.
	//Uses Lakeview Research's Vendor ID.
	const unsigned int VendorID = 0x04d8;
	const unsigned int ProductID = 0x0002;

	Length = 0;
	detailData = NULL;
	DeviceHandle=-1;
	ReadHandle=-1;

	/*
	API function: HidD_GetHidGuid
	Get the GUID for all system HIDs.
	Returns: the GUID in HidGuid.
	*/

	HidD_GetHidGuid(&HidGuid);	
	
	/*
	API function: SetupDiGetClassDevs
	Returns: a handle to a device information set for all installed devices.
	Requires: the GUID returned by GetHidGuid.
	*/
	
	hDevInfo=SetupDiGetClassDevs 
		(&HidGuid, 
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
			(hDevInfo, 
			0, 
			&HidGuid, 
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
				(hDevInfo, 
				&devInfoData, 
				NULL, 
				0, 
				&Length, 
				NULL);

			//Allocate memory for the hDevInfo structure, using the returned Length.
			detailData = (PSP_DEVICE_INTERFACE_DETAIL_DATA)malloc(Length);
			
			//Set cbSize in the detailData structure.
			detailData -> cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);

			//Call the function again, this time passing it the returned buffer size.
			Result = SetupDiGetDeviceInterfaceDetail 
				(hDevInfo, 
				&devInfoData, 
				detailData, 
				Length, 
				&Required, 
				NULL);

			//Open a handle to the device.

			/*
			API function: CreateFile
			Returns: a handle that enables reading and writing to the device.
			Requires:
			The DevicePath in the detailData structure
			returned by SetupDiGetDeviceInterfaceDetail.
			*/

			DeviceHandle=CreateFile 
				(detailData->DevicePath, 
				GENERIC_READ|GENERIC_WRITE, 
				FILE_SHARE_READ|FILE_SHARE_WRITE, 
				(LPSECURITY_ATTRIBUTES)NULL,
				OPEN_EXISTING, 
				0, 
				NULL);

			OutputDebugString("CreateFile: \n");

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
				(DeviceHandle, 
				&Attributes);
			
			OutputDebugString("HidD_GetAttributes: \n");
			
			//Is it the desired device?
			pGMS[hand]->USBconnected=FALSE;
			
			if (Attributes.VendorID == VendorID)
			{
				if (Attributes.ProductID == ProductID)
				{
					//Both the Product and Vendor IDs match.
					pGMS[hand]->USBconnected = TRUE;
					OutputDebugString("Device detected\n");
					//Get the device's capablities.
					GetDeviceCapabilities(hand);
					PrepareForOverlappedTransfer(hand);
				} //if (Attributes.ProductID == ProductID)

				else
					//The Product ID doesn't match.
					safeclosehandle(&DeviceHandle);
			} //if (Attributes.VendorID == VendorID)

			else
				//The Vendor ID doesn't match.
				safeclosehandle(&DeviceHandle);

		//Free the memory used by the detailData structure (no longer needed).
		free(detailData);
		}  //if (Result != 0)

		else
			//SetupDiEnumDeviceInterfaces returned 0, so there are no more devices to check.
			LastDevice=TRUE;

		//If we haven't found the device yet, and haven't tried every available device,
		//try the next one.
		MemberIndex = MemberIndex + 1;

	} //do
	while ((LastDevice == FALSE) && (pGMS[hand]->USBconnected == FALSE));

	if (pGMS[hand]->USBconnected == FALSE)
		OutputDebugString("Device not detected\n");
	else
		OutputDebugString("Device detected\n");

	//Free the memory reserved for hDevInfo by SetupDiClassDevs.
	SetupDiDestroyDeviceInfoList(hDevInfo);
	OutputDebugString("SetupDiDestroyDeviceInfoList\n");

	return pGMS[hand]->USBconnected;
}


void PrepareForOverlappedTransfer(HANDLEGM hand)
{
	//Get another handle to the device for the overlapped ReadFiles.
	ReadHandle=CreateFile 
		(detailData->DevicePath, 
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

	if (hEventObject == 0)
	{
		hEventObject = CreateEvent 
			(NULL, 
			TRUE, 
			TRUE, 
			"");
	OutputDebugString("CreateEvent: \n") ;

	//Set the members of the overlapped structure.
	HIDOverlapped.hEvent = hEventObject;
	HIDOverlapped.Offset = 0;
	HIDOverlapped.OffsetHigh = 0;
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
		(DeviceHandle, 
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
		&Capabilities);
	OutputDebugString("HidP_GetCaps: \n");

	//Display the capabilities

//	ValueToDisplay.Format("%s%X", "Usage Page: ", Capabilities.UsagePage);
//	DisplayData(ValueToDisplay);
//	ValueToDisplay.Format("%s%d", "Input Report Byte Length: ", Capabilities.InputReportByteLength);
//	DisplayData(ValueToDisplay);
//	ValueToDisplay.Format("%s%d", "Output Report Byte Length: ", Capabilities.OutputReportByteLength);
//	DisplayData(ValueToDisplay);
//	ValueToDisplay.Format("%s%d", "Feature Report Byte Length: ", Capabilities.FeatureReportByteLength);
//	DisplayData(ValueToDisplay);
//	ValueToDisplay.Format("%s%d", "Number of Link Collection Nodes: ", Capabilities.NumberLinkCollectionNodes);
//	DisplayData(ValueToDisplay);
//	ValueToDisplay.Format("%s%d", "Number of Input Button Caps: ", Capabilities.NumberInputButtonCaps);
//	DisplayData(ValueToDisplay);
//	ValueToDisplay.Format("%s%d", "Number of InputValue Caps: ", Capabilities.NumberInputValueCaps);
//	DisplayData(ValueToDisplay);
//	ValueToDisplay.Format("%s%d", "Number of InputData Indices: ", Capabilities.NumberInputDataIndices);
//	DisplayData(ValueToDisplay);
//	ValueToDisplay.Format("%s%d", "Number of Output Button Caps: ", Capabilities.NumberOutputButtonCaps);
//	DisplayData(ValueToDisplay);
//	ValueToDisplay.Format("%s%d", "Number of Output Value Caps: ", Capabilities.NumberOutputValueCaps);
//	DisplayData(ValueToDisplay);
//	ValueToDisplay.Format("%s%d", "Number of Output Data Indices: ", Capabilities.NumberOutputDataIndices);
//	DisplayData(ValueToDisplay);
//	ValueToDisplay.Format("%s%d", "Number of Feature Button Caps: ", Capabilities.NumberFeatureButtonCaps);
//	DisplayData(ValueToDisplay);
//	ValueToDisplay.Format("%s%d", "Number of Feature Value Caps: ", Capabilities.NumberFeatureValueCaps);
//	DisplayData(ValueToDisplay);
//	ValueToDisplay.Format("%s%d", "Number of Feature Data Indices: ", Capabilities.NumberFeatureDataIndices);
//	DisplayData(ValueToDisplay);

	//No need for PreparsedData any more, so free the memory it's using.
	HidD_FreePreparsedData(PreparsedData);
	OutputDebugString("HidD_FreePreparsedData: \n") ;
}


DWORD ReadReport(HANDLEGM hand)
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
		(ReadHandle, 
		InputReport, 
		Capabilities.InputReportByteLength, 
		&NumberOfBytesRead,
		(LPOVERLAPPED) &HIDOverlapped); 
 
	//DisplayLastError("ReadFile: ") ;

	/*API call:WaitForSingleObject
	'Used with overlapped ReadFile.
	'Returns when ReadFile has received the requested amount of data or on timeout.
	'Requires an event object created with CreateEvent
	'and a timeout value in milliseconds.
	*/

	Result = WaitForSingleObject 
		(hEventObject, 
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
		
		Result = CancelIo(ReadHandle);
		
		//A timeout may mean that the device has been removed. 
		//Close the device handles and set DeviceDetected = False 
		//so the next access attempt will search for the device.
		safeclosehandle(&ReadHandle);
		safeclosehandle(&DeviceHandle);
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

	ResetEvent(hEventObject);

	//Display the report data.
	//DisplayInputReport();

	return(Result);
}

DWORD WriteReport(HANDLEGM hand)
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
		(DeviceHandle, 
		OutputReport, 
		Capabilities.OutputReportByteLength, 
		&BytesWritten, 
		NULL);

	if (Result == 0)
	{
		//The WriteFile failed, so close the handle, display a message,
		//and set DeviceDetected to FALSE so the next attempt will look for the device.
		safeclosehandle(&DeviceHandle);
		safeclosehandle(&ReadHandle);
		OutputDebugString("Can't write to device\n");
		pGMS[hand]->USBconnected=FALSE;
	}
	
	return(Result);
}

unsigned char WritepacketToDevice(HANDLEGM hand,unsigned char command, unsigned char data)
{
	unsigned char dataret;

	DWORD Result = 0;
	//Write a report to the device.
	//Get the values from the combo boxes.
	OutputReport[1]=command;
	Result = WriteReport(hand);

	//Read a report from the device.
	if (Result != 0)
	{
		Result = ReadReport(hand);
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
		Result = WriteReport(hand);
	}
	else
	{
		OutputDebugString("Read Error 1\n");
		return(0);
				
	}

	//Read a report from the device.
	if (Result != 0 )
	{
			Result = ReadReport(hand);
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

	int real_reading;

	float temp;

	unsigned char *p_uchar;

	unsigned char Reading[2];
	//Write a report to the device.
	//Get the values from the combo boxes.

	OutputReport[1]=49;
	Result = WriteReport(hand);

	//Read a report from the device.
	if (Result != 0)
	{
			Result = ReadReport(hand);
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
		Result = WriteReport(hand);
	}
	else
	{
		OutputDebugString("Read Error1");
		return(0);
				
	}
	//Read a report from the device.
	if (Result != 0 )
	{
			
			Result = ReadReport(hand);
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
	const struct units_struct units_range_conversion_baseunits[4]={

						1.0,							
						1000.0, 	"% 05.3f  T ",	//was 10 000 think it was fudge for /10
					 	10000.0, 		"% 05.4f  T ",
					 	100000.0, 		"% 05.5f  T ",
					 	1000000.0,		"% 05.6f  T ",

						1.0,
					 	0.1,		"% 06.0f  G ",
						1.0,		"% 05.0f  G ",
						10.0,		"% 05.1f  G ",
						100.0,		"% 05.2f  G ",

						0.7957747,
						1.0,		"% 04.0f kA/m",
						10.0,		"% 05.1f kA/m",
						100.0,		"% 05.2f kA/m",
						1000.0, 	"% 05.3f kA/m",

						1.0,
						0.1,		"% 05.2f Oe ",
						1.0,		"% 05.3f Oe ",
						10.0,		"% 05.4f Oe ",
						100.0, 		"% 05.5f Oe "
						
};

	double data;

	if(pGMS[hand]->m_Iportno>0)
		return;

	WritepacketToDevice(hand,42, 42);

	data = GetReadingFromGM08(hand);

	pGMS[hand]->store.range = WritepacketToDevice(hand,48, 0);

	pGMS[hand]->store.units = WritepacketToDevice(hand,47, 0);

	pGMS[hand]->store.mode = WritepacketToDevice(hand,46, 0);

	//pGMS[hand]->store.value= ((float) data / (float) units[pGMS[hand]->store.units].unit_range[pGMS[hand]->store.range].range_div);

	pGMS[hand]->store.value =
		(float) ((float) data * (float)units_range_conversion_baseunits[pGMS[hand]->store.units].global_mult/ (float) units_range_conversion_baseunits[pGMS[hand]->store.units].unit_range[pGMS[hand]->store.range].range_div);

}

void closeUSB(HANDLEGM hand)
{
	safeclosehandle(&DeviceHandle);
	safeclosehandle(&ReadHandle);
}

void safeclosehandle(HANDLE * hand)
{

	if((*hand)!=-1)
		CloseHandle((*hand));
	(*hand)=-1;
}