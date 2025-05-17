#include <LPC214x.h>
#include<stdio.h>
#include "mpu6050.h"
#include <stdint.h>

int main() {
		init_pll();
    uart0_init();
		delay_ms(50);
		uart0_tx_string("Uart Initialized...\n");
    i2c_init();
    uart0_tx_string("I2C Initialized...\n");
		delay_ms(50);
		MPU6050_Init();
    int16_t ax, ay, az, gx, gy, gz;
    uart0_tx_string("MPU6050 Initialized...\n");

    while (1) {
				if (U0LSR & 0x01) {
            volatile char dummy = U0RBR;
        }
				int16_t temp_raw;
				//uart0_tx_string('a')
				MPU6050_Read_All(&ax, &ay, &az, &gx, &gy, &gz, &temp_raw);
        char buffer[64];
        sprintf(buffer, "AX=%d AY=%d AZ=%d\n", ax, ay, az);
        uart0_tx_string(buffer);
        sprintf(buffer, "GX=%d GY=%d GZ=%d\n\n", gx, gy, gz);
        uart0_tx_string(buffer);

        delay_ms(500);
    }
}

