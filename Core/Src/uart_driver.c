#include "uart_driver.h"

void UART_SendString(UART_HandleTypeDef *huart, const char *str) {
    while (*str) {
        HAL_UART_Transmit(huart, (uint8_t *)str, 1, 100);
        str++;
    }
}

void UART_ReceiveString(UART_HandleTypeDef *huart, char *buffer, uint16_t length) {
    for (uint16_t i = 0; i < length; i++) {
        HAL_UART_Receive(huart, (uint8_t *)&buffer[i], 1, HAL_MAX_DELAY);
        if (buffer[i] == '\n' || buffer[i] == '\r') { // Stop on newline or carriage return
            buffer[i] = '\0';
            break;
        }
    }
}


