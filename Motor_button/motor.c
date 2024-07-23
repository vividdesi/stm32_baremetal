// push button with DC motor Pins(dc->PA0,PA1. switch->PA4,PB0)

#include "stm32f401xe.h"  // CMSIS Cortex-M Peripheral Access Layer
void delay(void)
{
    for (int i = 0; i < 2500; i++) 
    for (int j = 0; j < 500; j++);
}
int main(void)
{
    // Enable GPIOA and GPIOC clock
    RCC->AHB1ENR |= (3<<0);

    // Configure PA5 as output mode (LED)
    GPIOA->MODER |= (5<<0);
    // Configure PC13 as input mode with pull-up
        GPIOA->MODER &= ~(3<<8);
        GPIOB->MODER &= ~ (3<<0);
        GPIOA->PUPDR |= (1<<8);//PA4 AS PULL-UP
        GPIOB->PUPDR |= (1<<0);//PB0 AS PULL-UP
        GPIOA->ODR &= ~(1<<1);//SETTING PA0 AND PA1 AS LOW
        GPIOA->ODR &= ~(1<<0);

    while (1)
    {
        // Check if button is pressed (active low)
           
    	if ((GPIOA->IDR & (1<<4)) == 0)
    	{
            GPIOA->ODR = (1<<1);
            delay();
            GPIOA->ODR = ~(1<<0);
            delay();
    	}

    	else if ((GPIOB->IDR & (1<<0)) == 0)
    	{
            GPIOA->ODR  = (1<<0);
            delay();
            GPIOA->ODR  = ~(1<<1);
            delay();
    	}
    	else
    	{
    		GPIOA->ODR &= ~(1<<1);
    		GPIOA->ODR &= ~(1<<0);
    	}
    	}
    }

