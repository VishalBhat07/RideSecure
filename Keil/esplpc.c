#include<lpc214x.h>
#define FOSC          15000000        
#define PLL_M         4              
#define CCLK          (FOSC * PLL_M)  
#define PCLK          (CCLK / 4)      
char buffer[128];
int idx = 0;
void init_pll(void) {
    PLL0CFG = ((PLL_M - 1) & 0x1F) | (1 << 5);  
	
    PLL0CON = 0x01;                     
    PLL0FEED = 0xAA;                     
    PLL0FEED = 0x55;
    while(!(PLL0STAT & (1 << 10)));      
    PLL0CON = 0x03;                     
    PLL0FEED = 0xAA;
    PLL0FEED = 0x55;
}
void delay_ms(unsigned int ms) {
    for (unsigned int i = 0; i < ms *4000; i++);
}
void uart0_init(void) {
    PINSEL0 |= 0x00000005;             
    U0LCR = 0x83;                       
    unsigned int divisor = PCLK/(16 * 4800);
    U0DLL = divisor & 0xFF;             
    U0DLM = (divisor >> 8) & 0xFF;     
    U0LCR = 0x03;                       
    U0FCR = 0x01;                       
    delay_ms(10);                       
    U0FCR = 0x87;                     
}
void uart0_sendchar(char c) {
    while (!(U0LSR & 0x20));            
    U0THR = c;
    delay_ms(1);                        
}
int main() {
    init_pll();
    uart0_init();
    delay_ms(500);

    while (1) {
        uart0_sendchar('b'); // 'b'
        delay_ms(500);
        uart0_sendchar('O'); // 'O'
        delay_ms(500);
    }
}