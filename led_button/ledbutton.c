#include <stdint.h>

#define RCC_BASE        0x40023800
#define GPIOA_BASE      0x40020000
#define GPIOC_BASE      0x40020800

#define RCC_AHB1ENR     *((volatile unsigned int *)(RCC_BASE + 0x30))
#define GPIOA_MODER     *((volatile unsigned int *)(GPIOA_BASE + 0x00))
#define GPIOA_ODR       *((volatile unsigned int *)(GPIOA_BASE + 0x14))
#define GPIOC_MODER     *((volatile unsigned int *)(GPIOC_BASE + 0x00))
#define GPIOC_IDR       *((volatile unsigned int *)(GPIOC_BASE + 0x10))

#define GPIOAEN         (1<<0)
#define GPIOCEN         (1<<2)
#define BUTTON_PIN      13

void delay(volatile uint32_t s) {
    for(; s>0; s--);
}

int main(void) {
    // Enable clock to GPIOA and GPIOC
    RCC_AHB1ENR |= GPIOAEN;  // Enable clock to GPIOA
    RCC_AHB1ENR |= GPIOCEN;  // Enable clock to GPIOC
   
    GPIOA_MODER &= ~(0xA03FF << (2 * 0));  // Clear mode bits Port A 0 1 4 9
    GPIOA_MODER |= (0x40105 << (2 * 0)); // Set mode to general purpose output Port A 0 1 4 9
   

    // Set GPIOC PIN13 as input (Button)
    GPIOC_MODER &= ~(0x3 << (2 * BUTTON_PIN));  // Clear mode bits (default is input)

    while(1) {
        // Check if button is pressed
        if ((GPIOC_IDR & (1 << BUTTON_PIN))==0) {
            GPIOA_ODR |= (1 << 0);
            delay(400000);
            GPIOA_ODR |= (1 << 1);
            delay(400000);
            GPIOA_ODR |= (1 << 4);
            delay(400000);
            GPIOA_ODR |= (1 << 9);
            delay(400000);
        }
        else {
            // Button is not pressed
        	 GPIOA_ODR &= ~((3 << 0)|(1<<4)|(1<<9));
        }

        delay(100000);  // Simple delay
    }

    return 0;
}

