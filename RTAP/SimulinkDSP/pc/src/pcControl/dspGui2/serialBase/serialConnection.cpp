/***********************************************************************
 * DESCRIPTION:
 * serial connection: Base class for handling all RS-232 events
 ***********************************************************************
 * This class provides functions to list up all RS-232 ports on the current PC.
 * Afterwards each serialElemementDescriptor element stands for one port that can be activated
 * and openend.
 * To the parental reference if nonzero, events are reached by calling associated functions passing purpose
 * and also the id from which port the message comes from.
 * IMPORTANT: It seems that Windows uses the name of an event to trigger an event to the 
 * waiting queue, that is, be sure that different names are used for each event!
 *
 * Copyright Hauke Krueger, IND, 2004, 02-06-04
 *********************************************************************/

#include <Windows.h>
#include <Setupapi.h>
#include <string>
//#include <ntddser.h>
#include "serialConnection.h"

//! Define the GUID for access to the COMPORT, taken from somewhere in the internet
#ifndef GUID_CLASS_COMPORT
DEFINE_GUID(GUID_CLASS_COMPORT, 0x86e0d1e0L, 0x8089, 0x11d0, 0x9c, 0xe4,\
			0x08, 0x00, 0x3e, 0x30, 0x1f, 0x73)
#endif

/**
 * Constructor: Assume default values for maxChar (maximum fields in report buffer
 * from device, used whenever using stop character, timeout or whenever the maximum message
 * field is larger than maxChar.
 *///==================================================================================
serialConnection::serialConnection()
{
	// Default value for character buffer length
	maxChar = 1024;

	// Parental reference set to NULL first
	parentalApp = NULL;

	portState = PORTS_ON;
}

/**
 * Initialize the com ports. That is set maxChar and scan for available com ports
 *///==============================================================================
bool 
serialConnection::initialize(int mChar, serialConnectionUseRef* parApp)
{
	maxChar = mChar;
	parentalApp = parApp;
	return(getAvailableComPorts());
}

/**
 * Open the port specified with id. Otherwhise return false.
 *///======================================================================
bool
serialConnection::openPort(unsigned id)
{
	if(id < elementList.size())
	{
		return(elementList[id]->openPort());
	}
	return(false);
}

/**
 * Open the port specified with id. Otherwhise return false.
 *///======================================================================
bool
serialConnection::closePort(unsigned id)
{
	if(id < elementList.size())
	{
		return(elementList[id]->closePort());
	}
	return(false);
}

/** 
 * Activate the COM port, follow the DCB INIT struct, I do not know exactly what everything means,
 * using the default values with only slight modifications is useful. 
 * stopConstr: Variable specifies when to report a new message: Report it after detecting
 *			   a timeout in the signal, after receiving a stop character or after a fixed amount of bytes.
 * timeoutReadSample: Timeout inbetween arriving samples to report arriving buffer (good for non continuous link)
 * fieldLength: Length of field waited for before reporting (this is good for streaming applications)
 * stopChar: Wait for a specific character to be transimtted before reporting the received field ( good for both but costs overhead of a byte)
 *///=====================================================================================================
bool 
serialConnection::activateComPort(unsigned id, int baudrate, 
								  int bits4Byte, int stopBitsEnum, 
								  int parityEnum, serialElementDescriptor::flowControlEnum enFlow,
								  serialElementDescriptor::messageCompleteEnum stopConstr,
								  int timeoutReadSample, int fieldLength, char stopChar, int syncword,
								  serialElementDescriptor::reportEnum stRep)
{
	if(id < elementList.size())
	{
		// Activate the associated port
		return(elementList[id]->activateComPort(baudrate, bits4Byte, stopBitsEnum, parityEnum,
			enFlow, stopConstr, timeoutReadSample,
			fieldLength, stopChar, syncword, stRep));
	}
	return(false);
}

/**
 * Scan for new available com ports. Before doing that, close all existing ports.
 * Most of the source code is from somewhere in the internet, understood and
 * modified for my own purposes.
 *///=============================================================================
bool
serialConnection::getAvailableComPorts()
{
	// TODO: Close all ports prior to reascanning:	
	int id = 0;

	// Enumerate all available COM-Ports first
	GUID *guiddev = (GUID*) &GUID_CLASS_COMPORT;

	HDEVINFO hDevInfo = INVALID_HANDLE_VALUE;
	SP_DEVICE_INTERFACE_DETAIL_DATA* pDetData = NULL;

	// Obtain a hw device handle with the associated COM GUID
	hDevInfo = SetupDiGetClassDevs(guiddev, NULL, NULL,
		DIGCF_PRESENT|DIGCF_DEVICEINTERFACE);
	if(hDevInfo == INVALID_HANDLE_VALUE)
	{
		;
	}

	char fname[256];
	char desc[256];
	char locInfo[256];

	std::string sname;
	int ind1;
	int ind2;

	BOOL bOK = true;
	bool result = true;
	
	SP_DEVICE_INTERFACE_DATA ifcData;
	DWORD dwDetDataSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA)+256;
	pDetData = (SP_DEVICE_INTERFACE_DETAIL_DATA*) new char[dwDetDataSize];

	ifcData.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);
	pDetData->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);

	for(unsigned i = 0; bOK; i++)
	{
		// Return handle of device
		bOK = SetupDiEnumDeviceInterfaces(hDevInfo, 
			NULL, guiddev, i, &ifcData);
		if(bOK)
		{
			SP_DEVINFO_DATA devData = {sizeof(SP_DEVINFO_DATA)};
			bOK = SetupDiGetDeviceInterfaceDetail(hDevInfo, &ifcData,
				pDetData, dwDetDataSize, NULL, &devData);
			if(bOK)
			{
				std::string pathName = pDetData->DevicePath;

				if(SetupDiGetDeviceRegistryProperty(hDevInfo,
					&devData, SPDRP_FRIENDLYNAME, NULL, (byte*) fname, sizeof(fname),
					NULL))
				{
					if(SetupDiGetDeviceRegistryProperty(hDevInfo,
					&devData, SPDRP_DEVICEDESC, NULL, (byte*)desc, sizeof(desc), NULL))
					{
						serialElementDescriptor* newElement = new serialElementDescriptor(id++);
						newElement->initialize(maxChar, this, parentalApp);

						// Obtain a part of the name
						newElement->setFName(fname);
						sname = fname;
						ind1 = sname.find("(");
						ind1 ++;
						ind2 = sname.find(")");
						if((ind2 > ind1)&&(ind2 < (signed)sname.size()))
						{
							sname = sname.substr(ind1, (ind2-ind1));
						}
						newElement->setShortCut(sname);
						newElement->setDescription(desc);
						//newElement->setID(id++);

						if(SetupDiGetDeviceRegistryProperty(hDevInfo, &devData,
							SPDRP_LOCATION_INFORMATION, NULL,
							(byte*)locInfo, sizeof(locInfo), NULL))
						{
							newElement->setLocation(locInfo);
						}
						elementList.push_back(newElement);
					}
				}
			}
			else
			{
				result = false;
			}
		}
	}
	if(hDevInfo != INVALID_HANDLE_VALUE)
		SetupDiDestroyDeviceInfoList(hDevInfo);

	if(pDetData)
		delete[](pDetData);

	portState = PORTS_INITIALIZED;
	return(result);
}

/**
 * Deactivate the current com port (ID).
 *///==========================================================
bool
serialConnection::deactivateComPort(unsigned id)
{
	if(id < elementList.size())
	{
		// Activate the associated port
		return(elementList[id]->deactivateComPort());
	}
	return(false);
}

/**
 * Function to set the class into initial state: Terminate all ports 
 * and delete the list of ports
 *///===============================================================
bool 
serialConnection::terminate()
{
	for(unsigned i = 0; i < elementList.size(); i++)
	{
		elementList[i]->terminate();
		delete(elementList[i]);
	}
	elementList.clear();
	portState = PORTS_ON;
	return(true);
}

/**
 * Return the number of available Com Ports found on the system.
 * getAvailableComPorts should always have been called previously
 *///==============================================================
int 
serialConnection::getAmountAvailableComPorts()
{
	return(elementList.size());
}

/** 
 * Return the expression for the COM port shortcut
 *///=============================================================
const char* 
serialConnection::getShortcutComPort(unsigned id)
{
	if(id < elementList.size())
	{
		return(elementList[id]->getShortCut().c_str());
	}
	else
		return(NULL);
}

/**
 * Return the name of the COM port
 *///==================================================================
const char* 
serialConnection::getNameComPort(unsigned id)
{
	if(id < elementList.size())
	{
		return(elementList[id]->getFName().c_str());
	}
	else
		return(NULL);
}

/**
 * Return the description of the COM port
 *///================================================================
const char* 
serialConnection::getDescriptionComPort(unsigned id)
{
	if(id < elementList.size())
	{
		return(elementList[id]->getDescription().c_str());
	}
	else
		return(NULL);
}

/**
 * Return the location property of the com port
 *///==================================================
const char* 
serialConnection::getLocationComPort(unsigned id)
{
	if(id < elementList.size())
	{
		return(elementList[id]->getLocation().c_str());
	}
	else
		return(NULL);
}


serialConnection::~serialConnection()
{
	// TODO: Unbuild the list of available COM ports
	for(unsigned i = 0; i< elementList.size(); i++)
	{
		elementList[i]->deactivateComPort();
		elementList[i]->closePort();
		delete(elementList[i]);
	}
}

/**
 * Pass a characters buffer to the specific port and send it to the other device
 * via RS232. The timeout is used to specify when a message is considered to be lost
 *///=================================================================================
bool
serialConnection::initSend(unsigned portID, const char* message, int amBytes, long messageID, DWORD timeout)
{
	if(portID < elementList.size())
	{
		return(elementList[portID]->initSend(message, amBytes, messageID, timeout));
	}
	else
		return(false);
}

/** 
 * Set the stop mode to timeout: That is as soon as a break that is longer than newTimeout
 * happens to lie inbetween the arrival of two characters the received bytes are reported to the 
 * host reference.
 *///==============================================================================
bool 
serialConnection::setStopModeTimeout(unsigned portID, int newTimeout)
{
	if(portID < elementList.size())
	{
		return(elementList[portID]->setStopModeTimeout(newTimeout));
	}
	else
		return(false);
}
	
/** 
 * Set the stop mode to timeout: That is as soon a field of specified length has arrived, the 
 * buffer is reported to the host reference.
 *///==============================================================================
bool 
serialConnection::setStopModeFieldLength(unsigned portID,int newLength)
{
	if(portID < elementList.size())
	{
		return(elementList[portID]->setStopModeFieldLength(newLength));
	}
	else
		return(false);
}

/** 
 * Set the stop mode to timeout: That is as soon a field is terminated by a special character, the 
 * buffer is reported to the host reference.
 *///==============================================================================
bool 
serialConnection::setStopModeSpecialChar(unsigned portID, char newChar)
{
	if(portID < elementList.size())
	{
		return(elementList[portID]->setStopModeSpecialChar(newChar));
	}
	else
		return(false);
}

/** 
 * Function to test the CTS flag for verification (at first it will be 
 * indicated by a callback).
 *///====================================================================
bool 
serialConnection::verifyCTSSet(unsigned portID)
{
	if(portID < elementList.size())
	{
		return(elementList[portID]->verifyCTSSet());
	}
	else
		return(false);
}

/** 
 * Function to test the DSR flag for verification (at first it will be 
 * indicated by a callback).
 *///====================================================================
bool 
serialConnection::verifyDSRSet(unsigned portID)
{
	if(portID < elementList.size())
	{
		return(elementList[portID]->verifyDSRSet());
	}
	else
		return(false);
}

/** 
 * Function to test the RING flag for verification (at first it will be 
 * indicated by a callback).
 *///====================================================================
bool 
serialConnection::verifyRINGSet(unsigned portID)
{
	if(portID < elementList.size())
	{
		return(elementList[portID]->verifyRINGSet());
	}
	else
		return(false);
}

/** 
 * Function to test the RLSDD flag for verification (at first it will be 
 * indicated by a callback).
 *///====================================================================
bool 
serialConnection::verifyRLSDSet(unsigned portID)
{
	if(portID < elementList.size())
	{
		return(elementList[portID]->verifyRLSDSet());
	}
	else
		return(false);
}

/**
 * Function to erase all messages from send queue for a port, the return value
 * specifies the number of messages killed from queue.
 *///=========================================================================
int
serialConnection::killAllMessagesQueue(unsigned portID)
{
	if(portID < elementList.size())
	{
		return(elementList[portID]->killElementsSendQueue());
	}
	else
		return(0);
}

