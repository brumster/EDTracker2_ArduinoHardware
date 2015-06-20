/*
 $License:
    Copyright (C) 2011-2012 InvenSense Corporation, All Rights Reserved.
    See included License.txt for License information.
 $
 */
/**
 *  @addtogroup  DRIVERS Sensor Driver Layer
 *  @brief       Hardware drivers to communicate with sensors via I2C.
 *
 *  @{
 *      @file       inv_mpu.h
 *      @brief      An I2C-based driver for Invensense gyroscopes.
 *      @details    This driver currently works for uint8_t mpu_ following devices:
 *                  MPU6050
 *                  MPU6500
 *                  MPU9150 (or MPU6050 w/ AK8975 on uint8_t mpu_ auxiliary bus)
 *                  MPU9250 (or MPU6500 w/ AK8963 on uint8_t mpu_ auxiliary bus)
 */

#ifndef _INV_MPU_H_
#define _INV_MPU_H_

#define INV_X_GYRO      (0x40)
#define INV_Y_GYRO      (0x20)
#define INV_Z_GYRO      (0x10)
#define INV_XYZ_GYRO    (INV_X_GYRO | INV_Y_GYRO | INV_Z_GYRO)
#define INV_XYZ_ACCEL   (0x08)
#define INV_XYZ_COMPASS (0x01)

struct int_param_s {
#if defined EMPL_TARGET_MSP430 || defined MOTION_DRIVER_TARGET_MSP430
    void (*cb)(void);
    unsigned short pin;
    unsigned char lp_exit;
    unsigned char active_low;
#elif defined EMPL_TARGET_UC3L0
    unsigned long pin;
    void (*cb)(volatile void*);
    void *arg;
#elif defined EMPL_TARGET_ATMEGA328 || defined EMPL_TARGET_STM32_LIBMAPLE
    void (*cb)(void);
    unsigned short pin;
#endif
};

#define MPU_INT_STATUS_DATA_READY       (0x0001)
#define MPU_INT_STATUS_DMP              (0x0002)
#define MPU_INT_STATUS_PLL_READY        (0x0004)
#define MPU_INT_STATUS_I2C_MST          (0x0008)
#define MPU_INT_STATUS_FIFO_OVERFLOW    (0x0010)
#define MPU_INT_STATUS_ZMOT             (0x0020)
#define MPU_INT_STATUS_MOT              (0x0040)
#define MPU_INT_STATUS_FREE_FALL        (0x0080)
#define MPU_INT_STATUS_DMP_0            (0x0100)
#define MPU_INT_STATUS_DMP_1            (0x0200)
#define MPU_INT_STATUS_DMP_2            (0x0400)
#define MPU_INT_STATUS_DMP_3            (0x0800)
#define MPU_INT_STATUS_DMP_4            (0x1000)
#define MPU_INT_STATUS_DMP_5            (0x2000)

/* Set up APIs */
void  mpu_init(unsigned char* revision);
uint8_t mpu_init_slave(void);
uint8_t mpu_set_bypass(unsigned char bypass_on);
void mpu_force_reset();

/* Configuration APIs */
uint8_t mpu_lp_accel_mode(unsigned char rate);
uint8_t mpu_lp_motion_interrupt(unsigned short thresh, unsigned char time,
    unsigned char lpa_freq);
uint8_t mpu_set_int_level(unsigned char active_low);
uint8_t mpu_set_int_latched(unsigned char enable);

void  mpu_set_dmp_state(unsigned char enable);
void  mpu_get_dmp_state(unsigned char *enabled);

void mpu_get_lpf(unsigned short *lpf);
void mpu_set_lpf(unsigned short lpf);

void  mpu_get_gyro_fsr(unsigned short *fsr);
void  mpu_set_gyro_fsr(unsigned short fsr);

void    mpu_get_accel_fsr(unsigned char *fsr);
void    mpu_set_accel_fsr(unsigned char fsr);

uint8_t mpu_get_compass_fsr(unsigned short *fsr);

void  mpu_get_gyro_sens(float *sens);
void  mpu_get_accel_sens(unsigned short *sens);

void mpu_get_sample_rate(unsigned short *rate);
void mpu_set_sample_rate(unsigned short rate);
uint8_t mpu_get_compass_sample_rate(unsigned short *rate);
uint8_t mpu_set_compass_sample_rate(unsigned short rate);


void  mpu_set_gyro_bias_reg(long *gyro_bias);

void  mpu_set_accel_bias_6050_reg(const long *accel_bias, unsigned char relative); 

void  mpu_read_6050_accel_bias(long *accel_bias);



uint8_t mpu_get_fifo_config(unsigned char *sensors);
void mpu_configure_fifo(unsigned char sensors);

uint8_t mpu_get_power_state(unsigned char *power_on);
void mpu_set_sensors(unsigned char sensors);

void  mpu_set_accel_bias(const long *accel_bias);

/* Data getter/setter APIs */
void mpu_get_gyro_reg(short *data, unsigned long *timestamp);
void mpu_get_accel_reg(short *data, unsigned long *timestamp);

uint8_t mpu_get_compass_reg(short *data);
uint8_t mpu_get_temperature(long *data);

uint8_t mpu_get_int_status(short *status);
uint8_t mpu_read_fifo(short *gyro, short *accel, unsigned long *timestamp,
    unsigned char *sensors, unsigned char *more);
uint8_t mpu_read_fifo_stream(unsigned short length, unsigned char *data,
    unsigned char *more);
uint8_t mpu_reset_fifo(void);

void mpu_write_mem(unsigned short mem_addr, unsigned short length,
    unsigned char *data);
void mpu_read_mem(unsigned short mem_addr, unsigned short length,
    unsigned char *data);
void mpu_load_firmware(unsigned short length, const unsigned char *firmware,
    unsigned short start_addr, unsigned short sample_rate);

uint8_t mpu_reg_dump(void);
uint8_t mpu_read_reg(unsigned char reg, unsigned char *data);
uint8_t mpu_run_self_test(long *gyro, long *accel);
uint8_t mpu_register_tap_cb(void (*func)(unsigned char, unsigned char));
void mpu_get_biases(long *gyro, long *accel);





#endif  /* #ifndef _INV_MPU_H_ */

