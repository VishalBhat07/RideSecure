#include <LPC214x.h>
#include <stdint.h>
#include "uart.h"
void i2c_init() {
    PINSEL0 = PINSEL0 | 0x00000050; /* P0.2 and P0.3 as SCL0 and SDA0 */
	I2C0CONSET = 0x40; /* I2C Enable */
	I2C0SCLL = 0x32; /* I2C bit frequency 300 kHz with 50% duty cycle */
	I2C0SCLH = 0x32;       // Enable I2C
}

void i2c_start(void) {
    uart0_tx_string("inside i2c start\n");
    I2C0CONSET = 0x20; /* Set STA = 1 */
    delay_ms(1); // Optional delay
    uint32_t timeout = 10000;

    while ((I2C0CONSET & 0x08) == 0 && timeout--); // Wait for SI
    if (timeout == 0) {
        uart0_tx_string("ERROR: SI not set! I2C start failed.\n");
        return;
    }

    uart0_tx_string("I2C START SUCCESS\n");
    I2C0CONCLR = 0x28; // Clear STA and SI
}


void i2c_stop() {
    I2C0CONSET = 0x10;
    I2C0CONCLR = 0x20;
}

void i2c_write(unsigned char data) {
    I2C0DAT = data;
	I2C0CONSET = 0x40; /* I2C Enable */
	while( (I2C0CONSET & 0x08) == 0 ); /* Wait till SI = 1 */
	I2C0CONCLR = 0x08; /* Clear SI */
}

unsigned char i2c_read(unsigned char ack) {
    if (ack)
        I2C0CONSET = 0x04;
    else
        I2C0CONCLR = 0x04;

    I2C0CONCLR = 0x08;
    while (!(I2C0CONSET & 0x08));

    return I2C0DAT;
}
void i2c_multiread(char* arr, int bytes) {
    uint8_t i = 0;
	while( ( bytes - 1 ) != 0 )
	{
		I2C0CONSET = 0x44; /* I2C Enable with Acknowledge */
		while( (I2C0CONSET & 0x08) == 0 ); /* Wait till SI = 1 */
		I2C0CONCLR = 0x0C; /* Clear SI and Acknowledge */
		*( arr + i ) = I2C0DAT ;
		bytes--;
		i++;
	}
	I2C0CONSET = 0x40; /* I2C Enable */
	while( (I2C0CONSET & 0x08) == 0 ); /* Wait till SI = 1 */
	I2C0CONCLR = 0x08; /* Clear SI */
	*( arr + i ) = I2C0DAT ;
}