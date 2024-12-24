#ifndef UART_DRIVER_H
#define UART_DRIVER_H

#include "stm32f1xx_hal.h"

void UART_SendString(UART_HandleTypeDef *huart, const char *str);
void UART_ReceiveString(UART_HandleTypeDef *huart, char *buffer, uint16_t length);

#endif // UART_DRIVER_H
