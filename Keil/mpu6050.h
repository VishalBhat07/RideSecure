#include <lpc214x.h>
#include <stdint.h>
#include "i2c.h"  // Your custom I2C functions header

#define MPU_WRITE_ADDR  0xD0  // 0x68 << 1
#define MPU_READ_ADDR   0xD1  // 0x68 << 1 | 1


void MPU6050_Write_Register(uint8_t reg, uint8_t data) {
		i2c_start();
    uart0_tx_string("I2C Start\n");
    i2c_write(MPU_WRITE_ADDR);
    uart0_tx_string("MPU WRITE\n");
    i2c_write(reg);
    uart0_tx_string("write reg\n");
    i2c_write(data);
		uart0_tx_string("write data\n");
    i2c_stop();
		uart0_tx_string("I2C stop\n");
    
}

void MPU6050_Init(void) {
		uart0_tx_string("MPU6050 Initialising\n");
    delay_ms(20);
    MPU6050_Write_Register(0x6B, 0x00); // Wake up
		uart0_tx_string("Wake up register\n");
    MPU6050_Write_Register(0x19, 0x07); // Sample rate
    uart0_tx_string("Sample rate\n");
		MPU6050_Write_Register(0x1A, 0x00); // No DLPF
    uart0_tx_string("NO DLPF\n");
		MPU6050_Write_Register(0x1B, 0x00); // Gyro scale
		uart0_tx_string("Gyro scale\n");
		MPU6050_Write_Register(0x1C, 0x00); // Accel scale
		uart0_tx_string("Accel scale\n");
}

void MPU6050_Read_All(int16_t* ax, int16_t* ay, int16_t* az,
                      int16_t* gx, int16_t* gy, int16_t* gz,
                      int16_t* temp) {
    uint8_t raw[14];

    i2c_start();
    if (!i2c_write(MPU_WRITE_ADDR)) {
        uart0_tx_string("Failed at write address\n");
        return;
    }
    if (!i2c_write(0x3B)) {
        uart0_tx_string("Failed at register write\n");
        return;
    }

    i2c_start(); // repeated start
    if (!i2c_write(MPU_READ_ADDR)) {
        uart0_tx_string("Failed at read address\n");
        return;
    }

    uart0_tx_string("i2c multiread starting\n");
    i2c_multiread((char*)raw, 14);
    i2c_stop();

    *ax = ((int16_t)(raw[0] << 8) | raw[1]);
    *ay = ((int16_t)(raw[2] << 8) | raw[3]);
    *az = ((int16_t)(raw[4] << 8) | raw[5]);
    *temp = ((int16_t)(raw[6] << 8) | raw[7]);
    *gx = ((int16_t)(raw[8] << 8) | raw[9]);
    *gy = ((int16_t)(raw[10] << 8) | raw[11]);
    *gz = ((int16_t)(raw[12] << 8) | raw[13]);
}
