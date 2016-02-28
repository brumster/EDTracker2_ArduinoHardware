# EDTracker2_ArduinoHardware
www.edtracker.org.uk

Packaged up hardware files for EDTracker2. Place the "edtracker" folder into
your <Arduino Install>/hardware/ folder.

When compiling, choose the appropriate device from Tools -> Board

Summary of changes/inclusions :
 - Arduino core files USBAPI.h/HID.cpp modified to include a USB joystick as
   part of the HID descriptor
 - EEPROM library included (David A. Mellis - GNU license)
 - I2C library included (Jeff Rowberg - MIT license)
 - Invensense Motion Driver (Invensense)
 - Wire (TWI) library included (Nicholas Zambetti - GNU license)
 
NB: We removed the keyboard and mouse components from the USB HID descriptor
as of 28th Jan (pushed to Git 28th Feb) in the interests of space and neatness.
If these matter to you (maybe you have customised the firmware to report as
a mouse, for example) then please revert to the previous release of the
hardware files.