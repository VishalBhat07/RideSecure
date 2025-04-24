#include<lpc214x.h>

void delay(void);

int main(void){
	IODIR0=0X0000000F;
	PINSEL0=0X00000000;
	while(1){
		IOPIN0=(1<<0);
		delay();
		IOPIN0=(1<<1);
		delay();
		IOPIN0=(1<<2);
		delay();
		IOPIN0=(1<<3);
		delay();
	}
}

void delay(){
	unsigned int i;
	for(i=0;i<=3599;i++);
}