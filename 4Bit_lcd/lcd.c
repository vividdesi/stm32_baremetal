#include "stm32f4xx.h"

void delay() {
    for (volatile int i = 0; i < 10000; i++);
}

void enable_fun() {
    GPIOA->ODR |= (1 << 10);  // Set EN
    delay();
    GPIOA->ODR &= ~(1 << 10); // Clear EN
    delay();
}

void send_command(char c) {
    char temp;
    GPIOA->ODR &= ~(1 << 8);  // RS = 0 for command
    temp = (c & 0xF0) >> 4;//shifting MSB DATA to LSB
    GPIOB->ODR &= ~(0xF << 6);//clearing the bits from the 6 position to 9 bits
    GPIOB->ODR |= (temp << 6);//sending the temp data from the 6 pin
    enable_fun();
    temp = c & 0x0F;//storing the LSB side binary values in temp variable
    GPIOB->ODR &= ~(0xF << 6);
    GPIOB->ODR |= (temp << 6);//Sending the LSB data from the 6 pin
    enable_fun();
}

void send_data(char f) {
    char temp;
    GPIOA->ODR |= (1 << 8);  // RS = 1 for data
    temp = (f & 0xF0) >> 4;//shifting MSB DATA to LSB
    GPIOB->ODR &= ~(0xF << 6);//clearing the bits from the 6 position to 9 bits
    GPIOB->ODR |= (temp << 6);//sending the temp data from the 6 pin
    enable_fun();
    temp = f & 0x0F;//storing the LSB side binary values in temp variable
    GPIOB->ODR &= ~(0xF << 6);
    GPIOB->ODR |= (temp << 6);//Sending the LSB data from the 6 pin
    enable_fun();
}

int main(void) {
    // Enable clock for GPIOA and GPIOB
    RCC->AHB1ENR |= (3<<0);

    // Configure PA8 and PA10 as output (RS and EN)
    GPIOA->MODER |= (1<<16) | (1<<20);


    // Configure PB6-PB9 as output (D4-D7)
    GPIOB->MODER |= (1<<12) | (1<<14) | (1<<16) | (1<<18);


    char commands[] = {0x28, 0x80, 0x01, 0x0E};
    char data[] = "Embedded Systems";

    // Initialize LCD
    for (int i = 0; i < 4; i++) {
        send_command(commands[i]);
    }

    // Display data on LCD
    for (int j = 0; data[j] != '\0'; j++) {
        send_data(data[j]);
        delay();
    }

    while (1) {
        // Infinite loop to keep the program running
    }
}
