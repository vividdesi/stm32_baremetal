#include "stm32f4xx.h"

void delay() {
    for (volatile long i = 0; i < 75000; i++);
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
    temp = (c & 0xF0) >> 4;
    GPIOB->ODR &= ~(0xF << 6);
    GPIOB->ODR |= (temp << 6);
    enable_fun();
    temp = c & 0x0F;
    GPIOB->ODR &= ~(0xF << 6);
    GPIOB->ODR |= (temp << 6);
    enable_fun();
}

void send_data(unsigned char f) {
    unsigned char temp;
    GPIOA->ODR |= (1 << 8);  // RS = 1 for data
    temp = (f & 0xF0) >> 4;
    GPIOB->ODR &= ~(0xF << 6);
    GPIOB->ODR |= (temp << 6);
    enable_fun();
    temp = f & 0x0F;
    GPIOB->ODR &= ~(0xF << 6);
    GPIOB->ODR |= (temp << 6);
    enable_fun();
}

int main(void) {
    int i = 0;
    char s[] = "KEYPAD";

    // Enable clock for GPIOA, GPIOB, and GPIOC
    RCC->AHB1ENR |= (7 << 0);

    // Clear and configure PA8 and PA10 as output (RS and EN)
    GPIOA->MODER &= ~((3 << 20) | (3 << 16));
    GPIOA->MODER |= (1 << 16) | (1 << 20);

    // Clear and configure PB6-PB9 as output (D4-D7)
    GPIOB->MODER &= ~(0xFF << 12);
    GPIOB->MODER |= (1 << 12) | (1 << 14) | (1 << 16) | (1 << 18);

    // Clear and configure PC0-PC3 as input
    GPIOC->MODER &= ~(0xFF << 0);
    // Enable pull-up resistors for PC0-PC3
    GPIOC->PUPDR |= (1 << 0) | (1 << 2) | (1 << 4) | (1 << 6);

    // Clear and configure PC4-PC7 as output
    GPIOC->MODER &= ~(0xFF << 8);
    GPIOC->MODER |= (1 << 8) | (1 << 10) | (1 << 12) | (1 << 14);

    // Initialize LCD
    char commands[] = {0x33, 0x32, 0x28, 0x0E, 0x01, 0x85};
    for (int i = 0; i < 6; i++) {
        send_command(commands[i]);
    }

    for (i = 0; s[i] != '\0'; i++) {
        send_data(s[i]);
        delay();
    }
    send_command(0xC0);

    while (1) {
        // Scan each row
        for (int row = 0; row < 4; row++) {
            // Set all rows high
            GPIOC->ODR |= (0xF << 4);
            // Set current row low
            GPIOC->ODR &= ~(1 << (4 + row));
            delay();  // Small delay to allow signals to settle

            // Check columns
            uint16_t columns = GPIOC->IDR & 0x0F;
            if (columns != 0x0F) {  // If any key is pressed
                delay();  // Debounce delay
                columns = GPIOC->IDR & 0x0F;
                    switch (row) {
                        case 0:
                            if (columns == 0x0E) send_data('1');
                            else if (columns == 0x0D) send_data('2');
                            else if (columns == 0x0B) send_data('3');
                            else if (columns == 0x07) send_data('A');
                            break;
                        case 1:
                            if (columns == 0x0E) send_data('4');
                            else if (columns == 0x0D) send_data('5');
                            else if (columns == 0x0B) send_data('6');
                            else if (columns == 0x07) send_data('B');
                            break;
                        case 2:
                            if (columns == 0x0E) send_data('7');
                            else if (columns == 0x0D) send_data('8');
                            else if (columns == 0x0B) send_data('9');
                            else if (columns == 0x07) send_data('C');
                            break;
                        case 3:
                            if (columns == 0x0E) send_data('*');
                            else if (columns == 0x0D) send_data('0');
                            else if (columns == 0x0B) send_data('#');
                            else if (columns == 0x07) send_data('D');
                            break;

                    }

            }
        }
    }
}


/*
 * PINS FOR ROWS
 *    PC4   R1- 1   2   3   A
 *    PC5   R2- 4   5   6   B
 *    PC6   R3- 7   8   9   C
 *    PC7   R4- *   0   #   D
               C1  C2  C3  C4
PINS FOR COLUMN-  PC0 PC1 PC2 PC3
 */
