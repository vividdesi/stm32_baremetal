#include "stm32f4xx.h"
#define DELAY 2500000

void UART2_Init(void);
void UART2_Transmit(char *string);
char UART2_Receive(void);

void GPIO_Init(void);
volatile long i;

void led_on(void) {
	UART2_Transmit("Led on \r\n");
	UART2_Transmit("\r\n");
	GPIOA->ODR = (1U << 5);
	//for (i = 0; i < DELAY; i++)
		;

}

void led_off(void) {
	UART2_Transmit("Led off \r\n");
	UART2_Transmit("\r\n");
	GPIOA->ODR &= ~(1U << 5);
	//for (i = 0; i < DELAY; i++)
		;
}

void status(void) {
	UART2_Transmit("Status ok \r\n");
	UART2_Transmit("\r\n");
}

typedef void (*operation)(void);

int main(void) {
	UART2_Init();
	GPIO_Init();

	UART2_Transmit("UART Ready. Type something:\r\n");

	operation op[3] = { status, led_on, led_off };

	while (1) {

		UART2_Transmit("Choose:\r\n");
		UART2_Transmit("1. Status\r\n");
		UART2_Transmit("2. LED On\r\n");
		UART2_Transmit("3. LED Off\r\n");
		char received = UART2_Receive();

		int input = received - '0';

		if (input >= 1 && input <= 3) {

			op[input - 1](); // Call the right function
		}

		else {
			UART2_Transmit("Invalid\r\n");
			UART2_Transmit("\r\n");
		}

		//GPIOA->ODR &= ~(1U << 5);

	}
}

void GPIO_Init(void) {
	RCC->AHB1ENR |= (1U << 0);

	// 2. Configure PA5 as output
	GPIOA->MODER &= ~(3U << (5 * 2));   // clear mode bits for PA5
	GPIOA->MODER |= (1U << (5 * 2));   // set PA5 as output (01)
}

void UART2_Init(void) {
	// 1. Enable clocks for GPIOA and USART2
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	RCC->APB1ENR |= RCC_APB1ENR_USART2EN;

	// 2. Configure PA2 (TX) and PA3 (RX) as Alternate Function
	GPIOA->MODER &= ~((0x3 << (2 * 2)) | (0x3 << (3 * 2)));      // Clear mode
	GPIOA->MODER |= ((0x2 << (2 * 2)) | (0x2 << (3 * 2)));      // Set AF mode

	GPIOA->AFR[0] &= ~((0xF << (4 * 2)) | (0xF << (4 * 3)));     // Clear AFR
	GPIOA->AFR[0] |= ((0x7 << (4 * 2)) | (0x7 << (4 * 3)));  // Set AF7 (USART2)

	GPIOA->OSPEEDR |= ((0x3 << (2 * 2)) | (0x3 << (3 * 2)));     // High speed
	GPIOA->PUPDR &= ~((0x3 << (2 * 2)) | (0x3 << (3 * 2)));    // No pull
	GPIOA->PUPDR |= ((0x1 << (2 * 2)) | (0x1 << (3 * 2)));   // Optional pull-up

	// 3. Configure USART2
	USART2->BRR = 16000000 / 9600;   // Assuming 16 MHz clock -> 9600 baud
	USART2->CR1 |= USART_CR1_TE | USART_CR1_RE;  // Enable TX and RX
	USART2->CR1 |= USART_CR1_UE;                  // Enable USART
}

void UART2_Transmit(char *string) {
	while (*string) {
		while (!(USART2->SR & USART_SR_TXE))
			; // Wait until TXE is set
		USART2->DR = (*string++ & 0xFF);       // Send data
	}
}

char UART2_Receive(void) {
	while (!(USART2->SR & USART_SR_RXNE))
		;     // Wait until RXNE is set
	return (char) (USART2->DR & 0xFF);          // Read data
}

