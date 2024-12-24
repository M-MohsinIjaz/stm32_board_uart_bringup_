# STM32 Custom UART Driver and Startup Implementation

This repository contains custom implementations of a UART driver and a startup file for STM32 microcontrollers, specifically for the STM32F103 series. These implementations provide basic UART initialization, data transmission, reception, and system startup handling.

## Features

### `custom_startup.c`
- **Interrupt Vector Table**: Includes handlers for exceptions like HardFault, SysTick, and others.
- **Reset Handler**:
  - Initializes the `.data` section by copying data from Flash to SRAM.
  - Zeroes out the `.bss` section in SRAM.
  - Calls static constructors.
  - Executes the primary bootloader in a loop until a hard reset is required.
- **Default Handler**: Provides an infinite loop for debugging unimplemented interrupts.

### `uart_driver_custom.c`
- **UART Initialization**:
  - Enables clocks for GPIOA and USART1.
  - Configures GPIO pins PA9 (TX) and PA10 (RX).
  - Sets a default baud rate of 9600 bps.
  - Enables USART1 transmitter and receiver.
- **UART Deinitialization**:
  - Resets USART1 and GPIOA clocks.
  - Restores GPIO configurations for PA9 and PA10 to default.
- **UART Data Transmission**:
  - Transmits strings character-by-character.
  - Waits for the transmission to complete using the TC flag.
- **UART Data Reception**:
  - Receives characters into a buffer until a newline (`\n`) or carriage return (`\r`) is detected.
  - Supports echoing of received characters (configurable via the `ECHO` macro).
  - Handles backspace (`\b`) for input correction.
- **Baud Rate Configuration**:
  - Allows setting a custom baud rate with `UART_Configure_BaudRate(uint32_t baud_rate)`.

## File Descriptions

### 1. `custom_startup.c`
- Implements startup routines, including system initialization, bootloader entry, and interrupt vector handling.

### 2. `uart_driver_custom.c`
- Provides a comprehensive UART driver, including initialization, data transmission, reception, deinitialization, and baud rate configuration.

## Getting Started

### Prerequisites
- STM32CubeIDE or any ARM GCC-compatible toolchain.
- An STM32 board (e.g., STM32F103).

### Adding to Your Project
1. Copy `custom_startup.c` to your project's source folder.
2. Update your linker settings to use `custom_startup.c` instead of the default startup file.
3. Include `uart_driver_custom.c` in your project.

### Example Usage
```c
#include "stm32f103x6.h"
#include "uart_driver_custom.h"

int main(void) {
    char buffer[128];

    UART_Init_CUSTOM();
    UART_Transmit_Data("Welcome to STM32 UART Driver!\n\r");

    while (1) {
        UART_Receive_Data(buffer, sizeof(buffer));
        UART_Transmit_Data("You entered: ");
        UART_Transmit_Data(buffer);
        UART_Transmit_Data("\n\r");

        if (strcmp(buffer, "QUIT") == 0) {
            UART_Transmit_Data("Exiting...\n\r");
            break;
        }
    }

    UART_DeInit_CUSTOM();
    return 0;
}
```

## Contributing
Pull requests are welcome. For major changes, please open an issue first to discuss what you would like to change.

## Contact
For any questions or issues, please open an issue on this repository or contact the repository maintainer.

