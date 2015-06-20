#include "arduino_shim.h"

#define USE_I2CDEVLIB

#ifdef USE_I2CDEVLIB

#include "../I2Cdev/I2Cdev.h"

#else

#include "../Wire/Wire.h"

#endif

_EXTERN_ATTRIB int shim_i2c_write(unsigned char slave_addr, unsigned char reg_addr,unsigned char length, unsigned char const *data) {

#ifdef USE_I2CDEVLIB

	return (I2Cdev::writeBytes(slave_addr,reg_addr,length,(uint8_t *)data) ? 0 : -1); // return 0 on success
	
#else

	Wire.beginTransmission(slave_addr);
	Wire.write(reg_addr); // send address
    for (uint8_t i = 0; i < length; i++) {
		Wire.write((uint8_t) data[i]);
	}
	Wire.endTransmission();
    return 0;	

#endif	
	
}

_EXTERN_ATTRIB int shim_i2c_read(unsigned char slave_addr, unsigned char reg_addr,unsigned char length, unsigned char *data) {
#ifdef USE_I2CDEVLIB

	return (I2Cdev::readBytes(slave_addr,reg_addr,length,(uint8_t *)data) ? 0 : -1); // return 0 on success

#else


	uint16_t timeout = 3000;	 // Default
    int8_t count = 0;
    uint32_t t1 = millis();

	for (uint8_t k = 0; k < length; k += min(length, BUFFER_LENGTH)) {
	
		Wire.beginTransmission(slave_addr);
		Wire.write(reg_addr);
		Wire.endTransmission();
	
		Wire.beginTransmission(slave_addr);
		Wire.requestFrom(slave_addr, (uint8_t)min(length - k, BUFFER_LENGTH));

		for (; Wire.available() && (timeout == 0 || millis() - t1 < timeout); count++) {
			data[count] = Wire.read();
		}

		Wire.endTransmission();
	}

	    // check for timeout
    if (timeout > 0 && millis() - t1 >= timeout && count < length) count = -1; // timeout
	
	return (count == length);
	
#endif
	
}

_EXTERN_ATTRIB void shim_get_ms(unsigned long *count) {
	*count = millis();
}