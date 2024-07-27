#include "stm32f401xe.h"
#include<stdint.h>

void delay() {
    for (volatile int i = 0; i < 10000; i++);
}
void Printdata ( char data) // data = 8 bit hexadecimal data
{
// do = PA10
// dl = PB3
// d2 = PB5
// d3 PB4
// d4 = PB10
// d5 = PA8
// d6 = PA9
// d7 = PC7
// en = PA6
// rw = PA7
// rs = PB6
// Bit 0 => a // do = PA10
if ((data&0x01) == 0x01) { GPIOA->ODR |= (1<<10);}
else { GPIOA->ODR &= (~(1<<10));}
// Bit 1 => b // d1 = PB3
if ((data&0x02) == 0x02) { GPIOB->ODR |= (1<<3); }
else                     { GPIOB->ODR &= (~(1<<3)); }
// Bit 2 => c // PB5
if ((data& 0x04) == 0x04) { GPIOB->ODR |= (1<<5); }
else { GPIOB->ODR &= (~(1<<5)); }
// Bit 3 => d // d = PB4
if ((data&0x08) == 0x08) { GPIOB->ODR |= (1<<4); }
else { GPIOB->ODR &= (~(1<<4)); }
// Bit 4 => e // d4 = PB10
if ((data&0x10) == 0x10) { GPIOB->ODR |= (1<<10); }
else { GPIOB->ODR &= (~(1<<10)); }
// Bit 5 => f // d5 PA8
if ((data&0x20) == 0x20) { GPIOA->ODR |= (1<<8); }
else { GPIOA->ODR &= (~(1<<8)); }
// Bit 6 => g // g = PA9
if ((data&0x40) == 0x40) { GPIOA->ODR |= (1<<9); }
else { GPIOA->ODR &= (~(1<<9)); }
// Bit 7 => h // dp= PC7
if ((data&0x80) == 0x80) { GPIOC->ODR |= (1<<7); }
else { GPIOC->ODR &= (~(1<<7)); }
}
void Lcd_Data (char data)
{
Printdata (data); // Pass the 8Bit data to data line's of LCD
GPIOA->ODR = GPIOA->ODR & (~(1<<7)); // PA7 Drive R/W Line of LCD to be LOW
GPIOB->ODR = GPIOB->ODR | (1<<6); // PB6 Drive RS Line of LCD to be HIGH
GPIOA->ODR = GPIOA->ODR | (1<<6); // PA6 Drive EN Line of LCD to be HIGH
delay ( ); // Delay
GPIOA->ODR = GPIOA->ODR & (~(1<<6)); // Drive EN Line of LCD to be LOW
}
/***** Passing Data to Instruction register of LCD *******/
/***** Pass commands to LCD ********/
void Lcd_Command ( char command)
{
Printdata (command); // Pass the 8Bit data to data line's of LCD
GPIOA->ODR = GPIOA->ODR & (~(1<<7)); // Drive R/W Line of LCD to be LOW
GPIOB->ODR = GPIOB->ODR & (~(1<<6)); // Drive RS Line of LCD to be LOW
GPIOA->ODR = GPIOA->ODR | (1<<6); // Drive EN Line of LCD to be HIGH
delay (); // Delay
GPIOA->ODR = GPIOA->ODR & (~(1<<6)); // Drive EN Line of LCD to be LOW
}
void Lcd_String ( char *str, char length)
{
 // LCD Data function in Loop for printing string or word of data to LCD
unsigned char i=0;
for (i=0;i<length; i++)
 {
   Lcd_Data (str[i]);
}
}
void Lcd_Initialise (void)
{
Lcd_Command (0x38); // configuring lcd in 16column and 2row format
Lcd_Command (0x06); // display on cursor off
Lcd_Command (0x0C); // autoincrement cursor
Lcd_Command (0x01); // clearscreen
}


int main(void)
{
RCC->AHB1ENR = RCC->AHB1ENR | (1<<0) | (1<<1) | (1<<2); // Clock of GPIO's Enabled GPIOA, GPIOB, GPIOC
// all the pins 7 segment display as output pins
GPIOA->MODER = GPIOA->MODER | (1<<16) | (1<<18) | (1<<20);
GPIOA->MODER = GPIOA->MODER & (~(1<<17)) & (~(1<<19)) & (~(1<<21)); // pa8, pa9, pa10 as output
GPIOB->MODER = GPIOB->MODER | (1<<6) | (1<<8) | (1<<10) | (1<<20);
GPIOB->MODER = GPIOB->MODER & (~(1<<7)) & (~(1<<9)) & (~(1<<11)) & (~(1<<21)); // pb3,pb4,pb5,pb10 as outp
GPIOC->MODER = GPIOC->MODER | (1<<14);
GPIOC->MODER = GPIOC->MODER & (~(1<<15)); // PC7 as output
// en = PA6
// rw = PA7
// rs = PB6
GPIOA->MODER = GPIOA->MODER | (1<<12) | (1<<14);
GPIOA->MODER = GPIOA->MODER & (~(1<<13)) & (~(1<<15)); // pa7, pa6 as output
GPIOB->MODER = GPIOB->MODER | (1<<12);
GPIOB->MODER = GPIOB->MODER & (~(1<<13)); // pb6 as output
Lcd_Initialise();
while (1)
{
Lcd_Command (0x80);
Lcd_String("Embedded",8);
Lcd_Command (0xc0);
Lcd_String("systems", 7);
}
}



