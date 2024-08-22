#include "stm32f4xx.h"

void Timer2_Config(void) {
    // Set the timer prescaler
    TIM2->PSC = 15999; // Prescaler value (Fck/PSC+1) 16mhz/15999=1khz

    // Set the auto-reload register (ARR)
    TIM2->ARR = 1000; // Auto-reload value

    // Set the counter to 0
    TIM2->CNT = 0;
}

void Timer2_Mode(void) {
    // Configure the timer to count up
    TIM2->CR1 |= TIM_CR1_CEN; // Enable the timer
}

int main(void) {
    // Enable the clock for TIM2
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

    // Configure GPIOA
    // Enable clock for GPIOA
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

        // Set pin 5 of GPIOA as output
        GPIOA->MODER |= (1 << (5 * 2)); // Output mode
    // Configure Timer2
    Timer2_Config();

    // Set Timer2 mode and start
    Timer2_Mode();

    // Main loop
    while (1) {
        // Check if update interrupt flag is set (polling)
        if ((TIM2->SR & TIM_SR_UIF) == 1) {
            // Clear the interrupt flag
            TIM2->SR &= ~TIM_SR_UIF;

            // Toggle the LED
            GPIOA->ODR ^= (1 << 5);
        }
    }
}
