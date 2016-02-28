/* Modification by D.Howell for EDTracker2 sketch 28th Jan 2016
 *
 * Removed Mouse and Keyboard aspects of HID to make this a raw joystick-only device.
 * Main reason for this is size but also there are reports it makes it behave better
 * on Linux/Mac. Also wonder if it might be contributing to some reported issues on
 * Windows 10. If you want to expand/modify EDTracker to report as a mouse or keyboard
 * you'll need to restore the previous releases of this file
 */

/* Copyright (c) 2011, Peter Barrett  
**  
** Permission to use, copy, modify, and/or distribute this software for  
** any purpose with or without fee is hereby granted, provided that the  
** above copyright notice and this permission notice appear in all copies.  
** 
** THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL  
** WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED  
** WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR  
** BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES  
** OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,  
** WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,  
** ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS  
** SOFTWARE.  
*/

#include "USBAPI.h"

#if defined(USBCON)
#ifdef HID_ENABLED

Tracker_ Tracker;


//================================================================================
//================================================================================

//	HID report descriptor

#define LSB(_x) ((_x) & 0xFF)
#define MSB(_x) ((_x) >> 8)

#define RAWHID_USAGE_PAGE	0xFFC0
#define RAWHID_USAGE		0x0C00
#define RAWHID_TX_SIZE 64
#define RAWHID_RX_SIZE 64

extern const u8 _hidReportDescriptor[] PROGMEM;
const u8 _hidReportDescriptor[] = {
	
//Head Tracker - 3 axis joystick, no buttons
 0x05, 0x01, // Usage Page (Generic Desktop) /
 0x09, 0x04, // Usage (Joystick) / 

 0xa1, 0x01, // Collection (Application) 
 0x85, 0x01,	 // REPORT_ID 

 0x09, 0x01,   // Usage (Pointer) 
 0xa1, 0x00,   // Collection (Physical) 
 0x05, 0x01,     // Usage Page (Generic Desktop)
 0x09, 0x30,     // Usage (X) 
 0x09, 0x31,     // Usage (Y) 
 0x09, 0x32,     // Usage (Z) 

 //0x35, 0x81,     // PHYSICAL_MINIMUM (-127)
 //0x15, 0x81,     // LOGICAL_MINIMUM (-127)
 //0x26, 0x7f, 0x00,  //   LOGICAL_MAXIMUM (127)
 //0x46, 0x7f, 0x00,  //   PHYSICAL_MAXIMUM (127) 

 0x16, 0x00, 0x80, /* Logical Minimum (-32768) */
 0x26, 0xff, 0x7f, /* Logical Maximum (32767) */


 //0x75, 8, // Report Size (8) 
 0x75, 16, // Report Size (16) 
 0x95, 3, // Report Count (3) 
 0x81, 0x82, // Input (Data, Variable, Absolute, Volatile) 
 0xc0, // End Collection 


 0xc0	// END_COLLECTION
/**/

};

extern const HIDDescriptor _hidInterface PROGMEM;
const HIDDescriptor _hidInterface =
{
	D_INTERFACE(HID_INTERFACE,1,3,0,0),
	D_HIDREPORT(sizeof(_hidReportDescriptor)),
	D_ENDPOINT(USB_ENDPOINT_IN (HID_ENDPOINT_INT),USB_ENDPOINT_TYPE_INTERRUPT,0x40,0x01)
};

//================================================================================
//================================================================================
//	Driver

u8 _hid_protocol = 1;
u8 _hid_idle = 1;

#define WEAK __attribute__ ((weak))

int WEAK HID_GetInterface(u8* interfaceNum)
{
	interfaceNum[0] += 1;	// uses 1
	return USB_SendControl(TRANSFER_PGM,&_hidInterface,sizeof(_hidInterface));
}

int WEAK HID_GetDescriptor(int /* i */)
{
	return USB_SendControl(TRANSFER_PGM,_hidReportDescriptor,sizeof(_hidReportDescriptor));
}

void WEAK HID_SendReport(u8 id, const void* data, int len)
{
	USB_Send(HID_TX, &id, 1);
	USB_Send(HID_TX | TRANSFER_RELEASE,data,len);
}

bool WEAK HID_Setup(Setup& setup)
{
	u8 r = setup.bRequest;
	u8 requestType = setup.bmRequestType;
	if (REQUEST_DEVICETOHOST_CLASS_INTERFACE == requestType)
	{
		if (HID_GET_REPORT == r)
		{
			//HID_GetReport();
			return true;
		}
		if (HID_GET_PROTOCOL == r)
		{
			//Send8(_hid_protocol);	// TODO
			return true;
		}
	}
	
	if (REQUEST_HOSTTODEVICE_CLASS_INTERFACE == requestType)
	{
		if (HID_SET_PROTOCOL == r)
		{
			_hid_protocol = setup.wValueL;
			return true;
		}

		if (HID_SET_IDLE == r)
		{
			_hid_idle = setup.wValueL;
			return true;
		}
	}
	return false;
}


//================================================================================
//	Tracker
//  Usage: Tracker.setState(inputs go here)
//

Tracker_::Tracker_()
{
}


void Tracker_::setState(TrackState_t *trackerSt)
{
	int16_t data[3];
	
	data[0] = trackerSt->xAxis;		// X axis
	data[1] = trackerSt->yAxis;		// Y axis
	data[2] = trackerSt->zAxis;		// Z axis
	
	//HID_SendReport(Report number, array of values in same order as HID descriptor, length in bytes) 
	// NB: we're using 16-bit words here so double up to 6 for length
	HID_SendReport(1, data, 6);
}

#endif

#endif /* if defined(USBCON) */
