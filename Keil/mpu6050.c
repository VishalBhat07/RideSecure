#include "mpu6050_NEW.h"

#define I2C_SCL_PIN 0  // Pin for I2C SCL
#define I2C_SDA_PIN 1  // Pin for I2C SDA

void I2C_init(void) {
    // Setup I2C pins and I2C configuration for LPC2148 (assuming pins are configured elsewhere)
    I2C0CONCLR = 0x6C;  // Clear all flags
    I2C0CONSET = 0x40;  // Enable the I2C0 module
    I2C0SCLL = 100;     // SCL clock low period (set to 100KHz)
    I2C0SCLH = 100;     // SCL clock high period (set to 100KHz)
}

void I2C_start(void) {
    I2C0CONSET = 0x20; // Set START condition
    while (!(I2C0CONSET & 0x08)); // Wait for the START condition to complete
}

void I2C_stop(void) {
    I2C0CONSET = 0x10; // Set STOP condition
    while (I2C0CONSET & 0x10); // Wait for STOP condition to complete
}

void I2C_write(uint8_t data) {
    I2C0DAT = data;               // Load data into I2C0DAT register
    I2C0CONSET = 0x04;            // Set the "I2C start" bit
    while (!(I2C0CONSET & 0x08)); // Wait until transmission is complete
}

uint8_t I2C_read(void) {
    I2C0CONCLR = 0x04; // Clear the I2C "start" bit
    I2C0CONSET = 0x08; // Enable "ack" on data reception
    while (!(I2C0CONSET & 0x08)); // Wait for read completion
    return I2C0DAT;
}

void mpu6050_init(void) {
    I2C_start();
    I2C_write(MPU6050_ADDR << 1);  // Write address (8-bit address)
    I2C_write(MPU6050_PWR_MGMT_1); // Write to power management register
    I2C_write(0x00);               // Wake up the MPU6050
    I2C_stop();
}

int16_t mpu6050_read_accel_x(void) {
    uint8_t high_byte, low_byte;
    int16_t accel_x;

    I2C_start();
    I2C_write(MPU6050_ADDR << 1);
    I2C_write(MPU6050_ACCEL_XOUT_H);  // Address of the accel X high byte
    I2C_start();
    I2C_write((MPU6050_ADDR << 1) | 0x01);  // Send device address with read bit
    high_byte = I2C_read();              // Read high byte
    low_byte = I2C_read();               // Read low byte
    I2C_stop();

    accel_x = (high_byte << 8) | low_byte;  // Combine high and low byte
    return accel_x;
}

int16_t mpu6050_read_gyro_x(void) {
    uint8_t high_byte, low_byte;
    int16_t gyro_x;

    I2C_start();
    I2C_write(MPU6050_ADDR << 1);
    I2C_write(MPU6050_GYRO_XOUT_H);  // Address of the gyro X high byte
    I2C_start();
    I2C_write((MPU6050_ADDR << 1) | 0x01);  // Send device address with read bit
    high_byte = I2C_read();              // Read high byte
    low_byte = I2C_read();               // Read low byte
    I2C_stop();

    gyro_x = (high_byte << 8) | low_byte;  // Combine high and low byte
    return gyro_x;
}
