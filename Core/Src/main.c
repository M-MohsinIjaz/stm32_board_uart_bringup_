#include "main.h"
#include "uart_driver_custom.h"
#include <string.h>
void SystemClock_Config(void);
extern void Reset_Handler(void);
static void MX_GPIO_Init(void)
{
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

int primary_bootloader(void)
{
    // HAL Initialization
    HAL_Init();

    // System Clock configuration
    SystemClock_Config();

    // GPIO and UART initialization
    MX_GPIO_Init();
    UART_Init_CUSTOM();  // For debugging output
    UART_Transmit_Data("Primary BootLoader is up \n\rEnter QUIT to exit or RESTART to restart the BootLoader\r");
    char input_cmd[8]=""; // Adding 1 byte extra of Null terminator
    // Main loop
    while (1)
    {
    	UART_Receive_Data(input_cmd,7);
    	UART_Transmit_Data(input_cmd);
    	if (strcmp(input_cmd, "QUIT") == 0)
    	{
    		UART_Transmit_Data("\rPrimary BootLoader is Exiting");
    		break;
    	}
    	else if (strcmp(input_cmd, "RESTART") == 0)
    	{
    		UART_Transmit_Data("\r Restarting .....");
    		HAL_Delay(1000);
    		//UART_DeInit_CUSTOM();
    		return RESTART;
    	}
    	else
		{
    		UART_Transmit_Data("\rPrimary BootLoader is up \n\rEnter QUIT to exit or RESTART to restart the BootLoader\r");
		}
    }
    return 0;
}

int main(void)
{
    // HAL Initialization
    HAL_Init();

    // System Clock configuration
    SystemClock_Config();

    // GPIO and UART initialization
    MX_GPIO_Init();
    UART_Init_CUSTOM();  // For debugging output

    // Main loop
    while (1)
    {
        HAL_Delay(1000);  // 1 second delay for the main loop
    }
}

// Error Handler
void Error_Handler(void)
{
    __disable_irq();
    while (1)
    {
        // Stay here in case of error
    }
}

// System Clock Configuration (assuming HSI oscillator)
void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
    	UART_Transmit_Data("System Clock Error.\n");
        Error_Handler();
    }

    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
    {
    	UART_Transmit_Data("Clock Configuration Error.\n");
        Error_Handler();
    }
}

