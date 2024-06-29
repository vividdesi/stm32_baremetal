#include <stdint.h>
#define delay  250000
int main(void)
{
	 volatile uint32_t  *GPIOA_MODER=(uint32_t*)  0x40020000 ;//Base address of Gpioa
	 volatile uint32_t  *RCCAHB1=(uint32_t*) 0x40023830 ;//Base Address of rcc + offset address of gpioa
	 volatile uint32_t  *GPIOA_STATE=(uint32_t*)  0x40020014;//Base address of gpio + off set address of gpioa port output
	 uint32_t  i=0;//
	*RCCAHB1=(1U<<0);//enabling clock to GPIOA
	*GPIOA_MODER &= 0XFFFFF3FF;//Ensuring mode register are set 000
	*GPIOA_MODER |= 0x01 << 10U;//Making PA5 as output
	//*GPIOA_STATE |= 0x01 << 5U;
   	while(1)
   	{
   		*GPIOA_STATE = 0x000;//Clearing all pins
   		for(i=0;i<delay;i++);
   		*GPIOA_STATE |=(1U<<5);//set the 5th pin as high
   		for(i=0;i<delay;i++);
   	}
}
