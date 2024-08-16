#include "stm32f4xx.h"

void GPIO_Config(void) {
    // Enable clock for GPIOA
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

    // Set pin 5 of GPIOA as output
    GPIOA->MODER |= (1 << (5 * 2)); // Output mode
    GPIOA->OTYPER &= ~(1 << 5);     // Push-pull
    GPIOA->OSPEEDR |= (3 << (5 * 2)); // High speed
    GPIOA->PUPDR &= ~(3 << (5 * 2)); // No pull-up/pull-down
}

void Timer2_Config(void) {
    // Set the timer prescaler
    TIM2->PSC = 8399; // Prescaler value (Fck/PSC+1)

    // Set the auto-reload register (ARR)
    TIM2->ARR = 1999; // Auto-reload value

    // Set the counter to 0
    TIM2->CNT = 0;
}

void Timer2_Mode(void) {
    // Configure the timer to count up
    TIM2->CR1 |= TIM_CR1_CEN; // Enable the timer
}

void Timer2_Interrupt_Config(void) {
    // Enable the update interrupt (overflow)
    TIM2->DIER |= TIM_DIER_UIE;

    // Configure NVIC for TIM2
    NVIC_EnableIRQ(TIM2_IRQn);
    NVIC_SetPriority(TIM2_IRQn, 1);
}

int main(void) {
    // Enable the clock for TIM2
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

    // Configure GPIOA
    GPIO_Config();

    // Configure Timer2
    Timer2_Config();

    // Set Timer2 mode and start
    Timer2_Mode();

    // Configure Timer2 interrupts
    Timer2_Interrupt_Config();

    // Enable global interrupts
    __enable_irq();

    // Main loop
    while (1) ;   

}

void TIM2_IRQHandler(void) {
    // Check if update interrupt flag is set
    if (TIM2->SR & TIM_SR_UIF) {
        // Clear the interrupt flag
        TIM2->SR &= ~TIM_SR_UIF;

        // Toggle the LED
        GPIOA->ODR ^= (1 << 5);
    }
}
