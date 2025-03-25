#include <lpc214x.h>

void UART0_Init(void) {
    PINSEL0 |= 0x00000005; // Enable TxD0 and RxD0 (P0.0 & P0.1)
    U0LCR = 0x83;          // 8-bit data, 1 stop bit, no parity, DLAB=1
    U0DLL = 97;            // Baud rate 9600 with PCLK = 15MHz
    U0DLM = 0;
    U0LCR = 0x03;          // Disable DLAB
}

void UART0_SendChar(char ch) {
    while (!(U0LSR & 0x20)); // Wait until the transmitter is ready
    U0THR = ch;             // Send character
}

void delay() {
    for (volatile int i = 0; i < 500000; i++); // Basic delay
}

int main() {
    UART0_Init(); // Initialize UART0

    while (1) {
        UART0_SendChar('B'); // Send 'B' to ESP32
        delay();
    }
}
