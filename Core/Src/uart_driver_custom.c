#include "stm32f103x6.h"
#define ECHO 1

extern uint32_t HAL_RCC_GetPCLK2Freq(void);
void UART_Init_CUSTOM(void) {
    // Step 1: Enable clocks
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;    // Enable GPIOA clock
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;  // Enable USART1 clock

    // Step 2: Configure GPIO pins for UART (PA9 -> TX, PA10 -> RX)
    GPIOA->CRH &= ~(GPIO_CRH_CNF9 | GPIO_CRH_MODE9);  // Clear configuration
    GPIOA->CRH |= (GPIO_CRH_CNF9_1 | GPIO_CRH_MODE9_1); // PA9: Alternate Function Push-Pull, 2 MHz

    GPIOA->CRH &= ~(GPIO_CRH_CNF10 | GPIO_CRH_MODE10); // Clear configuration for PA10
    GPIOA->CRH |= GPIO_CRH_CNF10_0;                   // PA10: Input floating

    // Step 3: Configure USART1
    uint32_t baud_rate = 9600;
     uint32_t pclk2 = HAL_RCC_GetPCLK2Freq();
    USART1->BRR = (pclk2 + (baud_rate / 2)) / baud_rate;
    //USART1->BRR = HAL_RCC_GetPCLK2Freq() /(uint32_t)(16*9600); // Set baud rate to 9600 (assuming 72 MHz PCLK2)
    USART1->CR1 |= USART_CR1_TE;    // Enable transmitter
    USART1->CR1 |= USART_CR1_RE;    // Enable receiver
    USART1->CR1 |= USART_CR1_UE;    // Enable USART1

}
void UART_DeInit_CUSTOM(void) {
    // Step 1: Disable USART1
    USART1->CR1 &= ~USART_CR1_UE; // Disable USART1

    // Step 2: Reset GPIO pins (PA9 -> TX, PA10 -> RX)
    GPIOA->CRH &= ~(GPIO_CRH_CNF9 | GPIO_CRH_MODE9);   // Clear PA9 configuration
    GPIOA->CRH &= ~(GPIO_CRH_CNF10 | GPIO_CRH_MODE10); // Clear PA10 configuration

    // Optionally set pins to their default state (Input Floating)
    GPIOA->CRH |= (GPIO_CRH_CNF9_0);  // Set PA9 as Input Floating
    GPIOA->CRH |= (GPIO_CRH_CNF10_0); // Set PA10 as Input Floating

    // Step 3: Disable clocks for USART1 and GPIOA
    RCC->APB2ENR &= ~RCC_APB2ENR_USART1EN; // Disable USART1 clock
    RCC->APB2ENR &= ~RCC_APB2ENR_IOPAEN;   // Disable GPIOA clock
}

void UART_Configure_BaudRate(uint32_t baud_rate)
{
     uint32_t pclk2 = HAL_RCC_GetPCLK2Freq();
     USART1->BRR = (pclk2 + (baud_rate / 2)) / baud_rate;

}
void UART_Transmit_Data(char *data) {
    while (*data) {
        while (!(USART1->SR & USART_SR_TXE)); // Wait until TXE (Transmit Data Register Empty) is set
        USART1->DR = *data & 0xFF;           // Write data to transmit
        data++;
    }
    while (!(USART1->SR & USART_SR_TC));     // Wait until TC (Transmission Complete) is set
}
void UART_Receive_Data(char *buffer, uint16_t buffer_size) {
    uint16_t index = 0;

    while (1) {
        // Wait until RXNE (Read Data Register Not Empty) is set
        while (!(USART1->SR & USART_SR_RXNE));

        // Read the received data
        char received_char = (char)(USART1->DR & 0xFF);
#if defined(ECHO) && (ECHO != 0)
        UART_Transmit_Data(&received_char);
#endif
        // Check for 'Enter' (newline or carriage return)
        if (received_char == '\n' || received_char == '\r') {
            buffer[index] = '\0'; // Null-terminate the string
            break;
        }
        // Store the character in the buffer if there's space
        if (index < buffer_size) {
        	if (received_char == '\b')
        	{
        		buffer[--index] = '\0';
        	}
        	else
        	{
        		buffer[index++] = received_char;
        	}
        }
        else
        {
        	index=0;
        }
    }
}


