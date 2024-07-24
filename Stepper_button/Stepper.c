//stepper motor 360

#include "stm32f401xe.h"  // CMSIS Cortex-M Peripheral Access Layer
void delay(void)
{
    for (int i = 0; i < 100; i++) // Adjust delay length for your clock speed
    for (int j = 0; j < 255; j++);
}
int main(void)
{
    // Enable GPIOA and GPIOB clock
    RCC->AHB1ENR |= (3<<0);//

    // Configure PA2,PA3 as input mode (switch)
    GPIOA->MODER &= ~(15<<26);
    //GPIOA->MODER &= ~ (3<<6);

    //Configuring PA2,PA3 as PULLUP
    GPIOA->PUPDR |= (1<<26);
    GPIOA->PUPDR |= (1<<28);


    // Configure PA0,PA1,PA4,PB0 as output mode (motor)
    GPIOA->MODER &= ~((15<<0)|(3<<8));
    GPIOB->MODER &= ~ (3<<0);
    GPIOA->MODER |= (5<<0)|(1<<8);//Configuring pin as output at portA
    GPIOB->MODER |=(1<<0);//Configuring pin as output at portB
    GPIOA->ODR &=~(1<<0);//PA0
    GPIOA->ODR &=~(1<<1);//PA1
    GPIOA->ODR &=~(1<<4);//PA4
    GPIOB->ODR &=~(1<<0);//PB0
while(1)
{
    if((GPIOA->IDR & (1<<13)) == 0)
    {
    	for(int i=0;i<(360/7.2);i++)
    	{
            GPIOA->ODR = (1<<0);
            delay();
            GPIOA->ODR &=~(1<<0);
            delay();
            GPIOA->ODR = (1<<1);
            delay();
            GPIOA->ODR &=~(1<<1);
            delay();
            GPIOA->ODR = (1<<4);
            delay();
            GPIOA->ODR &=~(1<<4);
            delay();
            GPIOB->ODR = (1<<0);
            delay();
            GPIOB->ODR &=~(1<<0);
            delay();
    	}
    }

    else if((GPIOA->IDR & (1<<14)) == 0)
        {
        	for(int i=0;i<(360/7.2);i++)
        	{
        		GPIOB->ODR = (1<<0);
        		delay();
        		GPIOB->ODR &=~(1<<0);
        		delay();
        	    GPIOA->ODR = (1<<4);
        	    delay();
        		GPIOA->ODR &=~(1<<4);
        		delay();
        		GPIOA->ODR = (1<<1);
        		delay();
        		GPIOA->ODR &=~(1<<1);
        		delay();
        		GPIOA->ODR = (1<<0);
        		delay();
        		GPIOA->ODR &=~(1<<0);
        		delay();
        	}
        }
    else
    {
    	GPIOA->ODR &=~(1<<0);//PA0
    	GPIOA->ODR &=~(1<<1);//PA1
    	GPIOA->ODR &=~(1<<4);//PA4
    	GPIOB->ODR &=~(1<<0);//PB0
    }
    }

   }
