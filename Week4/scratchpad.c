/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include <stdio.h>
#include <string.h>

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

// Define R/W addresses for temp sensor
#define HST221_READ_ADDRESS 0xbf
#define HST221_WRITE_ADDRESS 0xbe

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
DFSDM_Channel_HandleTypeDef hdfsdm1_channel1;

I2C_HandleTypeDef hi2c2;

QSPI_HandleTypeDef hqspi;

SPI_HandleTypeDef hspi3;

UART_HandleTypeDef huart4;
UART_HandleTypeDef huart1;
UART_HandleTypeDef huart3;
DMA_HandleTypeDef hdma_uart4_rx;
DMA_HandleTypeDef hdma_uart4_tx;

PCD_HandleTypeDef hpcd_USB_OTG_FS;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_DFSDM1_Init(void);
static void MX_I2C2_Init(void);
static void MX_QUADSPI_Init(void);
static void MX_SPI3_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_USART3_UART_Init(void);
static void MX_USB_OTG_FS_PCD_Init(void);
static void MX_UART4_Init(void);
/* USER CODE BEGIN PFP */

static void do_who_am_i(void);
static void do_temp_polling(void);
static void do_temp_interrupt(void);
static void do_temp_dma(void);
static void do_temp_interrupt_EXTI(void);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

static void do_who_am_i(void)
{
    // Declare address of the device's WHO_AM_I register
    uint8_t whoAmIReg = 0x0f;

    // Send the target register to the device and get status back
    HAL_StatusTypeDef status;
    status = HAL_I2C_Master_Transmit(&hi2c2, HST221_WRITE_ADDRESS, &who_am_i, sizeof(who_am_i), 1000);

    // Print status results over UART1 to console session
    char buffer[100];
    snprintf(buffer, sizeof(buffer), "\r\nHAL_I2C_Master_Transmit: status %u\r\n", status);
    HAL_UART_Transmit(&huart1, (uint8_t*) buffer, sizeof(buffer), 1000);

    // Read response back to get value of WHO_AM_I register
    uint8_t data = 0xff;    // Default value should be 0xbc according to datasheet
    status = HAL_I2C_Master_Receive(&hi2c2, HST221_READ_ADDRESS, &data, sizeof(data), 1000);

    // Print status results and response value over UART1 to console session
    snprintf(buffer, sizeof(buffer), "\r\nHAL_I2C_Master_Receive: status %u; data: 0x%x\r\n", status, data);
    HAL_UART_Transmit(&huart1, (uint8_t*) buffer, sizeof(buffer), 1000);

}


static void do_polling(void)
{
    // Large-ish char buffer for strings sent over the console
    char buffer[100];

	// Configure control register 2 (CTRL_REG2, 0x21) bit 0 to enable one-shot
    uint8_t ctrlReg2 = 0x21;
    uint8_t ctrlData[] = {ctrlReg2, 0x01};

    // Send the target register to the device and get status back
    HAL_StatusTypeDef status;
    status = HAL_I2C_Master_Transmit(&hi2c2, HST221_WRITE_ADDRESS, ctrlData, sizeof(ctrlData), 1000);

    // Print status results over UART1 to console session
    snprintf(buffer, sizeof(buffer), "\r\n(One-shot enabled): HAL_I2C_Master_Transmit: status %u\r\n", status);
    HAL_UART_Transmit(&huart1, (uint8_t*) buffer, sizeof(buffer), 1000);

    // Define status register (STATUS_REG2, 0x27) bit 0 to monitor for new sample available
    uint8_t statusReg = 0x27;
    uint8_t sampleReady = 0;

    // Loiter for a bit to allow time for converison to complete and be made available
    uint8_t count = 0;
    while (count < 10)  // 10 is an arbitrary "long enough" value, this wouldnt always be great real-world practice
    {
        // Send the address of the status register and report it over the console
        status = HAL_I2C_Master_Transmit(&hi2c2, HST221_WRITE_ADDRESS, &statusReg, sizeof(statusReg), 1000);
        snprintf(buffer, sizeof(buffer), "\r\n[%d] (status register): HAL_I2C_Master_Transmit: status %u\r\n", count, status);
        HAL_UART_Transmit(&huart1, (uint8_t*) buffer, sizeof(buffer), 1000);

        // Read back the value of the status register and report it over the console
        status = HAL_I2C_Master_Receive(&hi2c2, HST221_READ_ADDRESS, (uint8_t *)&sampleReady, sizeof(sampleReady), 1000);
        snprintf(buffer, sizeof(buffer), "\r\nStatus register 0x%02x\r\n", count, sampleReady);
        HAL_UART_Transmit(&huart1, (uint8_t*) buffer, sizeof(buffer), 1000);

        // If the new sample is ready, report it to the console and break out of while-loop...
        if (sampleReady & 0x01)
        {
            snprintf(buffer, sizeof(buffer), "\r\nNew Temperature Sample Available\r\n");
            HAL_UART_Transmit(&huart1, (uint8_t*) buffer, sizeof(buffer), 1000);
        }

        // Else wait for a bit, increment the counter, and keep looping
        HAL_Delay(1000);
        count++;
    }

    // With new sample ready, practice solutions implemented with reading sequentially from 
    //  the LSB and MSB registers (0x2A and 0x2B) as well as via auto-increment
    static bool toggle = 1;

    if (toggle)
    {
        toggle = 0;

        // Reading the lower half of the temperature register
        // Send target address
        uint8_t tempRegLSB = 0x2a;
        status = HAL_I2C_Master_Transmit(&hi2c2, HST221_WRITE_ADDRESS, &tempRegLSB, sizeof(tempRegLSB), 1000);
        snprintf(buffer, sizeof(buffer), "\r\n(LSB): HAL_I2C_Master_Transmit: status: %u)\r\n", status);
        HAL_UART_Transmit(&huart1, (uint8_t*) buffer, sizeof(buffer), 1000);

        // Read response back and print over console
        uint8_t tempDataLSB = 0xff; // Junk default value
        status = HAL_I2C_Master_Receive(&hi2c2, HST221_READ_ADDRESS, (uint8_t *)&tempDataLSB, sizeof(tempDataLSB), 1000);
        snprintf(buffer, sizeof(buffer), "\r\n(LSB): HAL_I2C_Master_Receive: status: %u; LSB data: 0x%02x)\r\n", status, tempDataLSB);
        HAL_UART_Transmit(&huart1, (uint8_t*) buffer, sizeof(buffer), 1000);

        // Reading the upper half of the temperature register
        // Send target address
        uint8_t tempRegMSB = 0x2b;
        status = HAL_I2C_Master_Transmit(&hi2c2, HST221_WRITE_ADDRESS, &tempRegMSB, sizeof(tempRegMSB), 1000);
        snprintf(buffer, sizeof(buffer), "\r\n(MSB): HAL_I2C_Master_Transmit: status: %u)\r\n", status);
        HAL_UART_Transmit(&huart1, (uint8_t*) buffer, sizeof(buffer), 1000);

        // Read response back and print over console
        uint8_t tempDataMSB = 0xff; // Junk default value
        status = HAL_I2C_Master_Receive(&hi2c2, HST221_READ_ADDRESS, (uint8_t *)&tempDataMSB, sizeof(tempDataMSB), 1000);
        snprintf(buffer, sizeof(buffer), "\r\n(MSB): HAL_I2C_Master_Receive: status: %u; MSB data: 0x%02x)\r\n", status, tempDataMSB);
        HAL_UART_Transmit(&huart1, (uint8_t*) buffer, sizeof(buffer), 1000);
    }
    else
    {
        toggle = 1;

        // Reading the lower half of the temperature register with auto-increment enabled
        // Send target address (OR'ing with 0x80 enables auto-inc)
        uint8_t tempRegLSB = 0x2a | 0x80;
        status = HAL_I2C_Master_Transmit(&hi2c2, HST221_WRITE_ADDRESS, &tempRegLSB, sizeof(tempRegLSB), 1000);
        snprintf(buffer, sizeof(buffer), "\r\n(Auto-increment): HAL_I2C_Master_Transmit: status: %u)\r\n", status);
        HAL_UART_Transmit(&huart1, (uint8_t*) buffer, sizeof(buffer), 1000);

        // Read response back for both registers and print over console
        uint16_t tempData = 0xbeef; // Junk default value (ALSO REALLY HOT!)
        status = HAL_I2C_Master_Receive(&hi2c2, HST221_READ_ADDRESS, (uint8_t *)&tempData, sizeof(tempData), 1000);
        snprintf(buffer, sizeof(buffer), "\r\n(Auto-increment): HAL_I2C_Master_Receive: status: %u; temperature data: 0x%04x)\r\n", status, tempData);
        HAL_UART_Transmit(&huart1, (uint8_t*) buffer, sizeof(buffer), 1000);
    }

}

// Define an IRQ status flag and the I2C interrupt callback function
static bool irqComplete = 0;

void HAL_I2C_masterTxCpltCallback(I2C_HandldeTypeDef *hi2c)
{
    // Indicate something on the board
    HAL_GPIO_TogglePin(LED2_GPIO_Port, LED2_Pin);

    // Set status flag
    irqComplete = 1;
}

static void do_interrupt(void)
{
    // Clear flag
    irqComplete = 0;

    // Large-ish char buffer for strings sent over the console
    char buffer[100];

	// Configure control register 2 (CTRL_REG2, 0x21) bit 0 to enable one-shot
    uint8_t ctrlReg2 = 0x21;
    uint8_t ctrlData[] = {ctrlReg2, 0x01};

    // Send the target register to the device and get status back using the *_IT function
    HAL_StatusTypeDef status;
    status = HAL_I2C_Master_Transmit_IT(&hi2c2, HST221_WRITE_ADDRESS, ctrlData, sizeof(ctrlData));

    // Print status results over UART1 to console session
    snprintf(buffer, sizeof(buffer), "\r\n(One-shot enabled): HAL_I2C_Master_Transmit_IT: status %u\r\n", status);
    HAL_UART_Transmit(&huart1, (uint8_t*) buffer, sizeof(buffer), 1000);

    // Wait for interrupt to complete
    while (irqComplete == 0)
    {
        HAL_Delay(1000);
    }

}

static void do_dma(void)
{
	// Clear flag
    irqComplete = 0;

    // Large-ish char buffer for strings sent over the console
    char buffer[100];

	// Configure control register 2 (CTRL_REG2, 0x21) bit 0 to enable one-shot
    uint8_t ctrlReg2 = 0x21;
    uint8_t ctrlData[] = {ctrlReg2, 0x01};

    // Send the target register to the device and get status back using the *_DMA function
    HAL_StatusTypeDef status;
    status = HAL_I2C_Master_Transmit_DMA(&hi2c2, HST221_WRITE_ADDRESS, ctrlData, sizeof(ctrlData));

    // Print status results over UART1 to console session
    snprintf(buffer, sizeof(buffer), "\r\n(One-shot enabled): HAL_I2C_Master_Transmit_DMA: status %u\r\n", status);
    HAL_UART_Transmit(&huart1, (uint8_t*) buffer, sizeof(buffer), 1000);

    // Wait for interrupt to complete
    while (irqComplete == 0)
    {
        HAL_Delay(1000);
    }
}

static void do_temp_interrupt_EXTI(void)
{
    ; // TO DO
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_DFSDM1_Init();
  MX_I2C2_Init();
  MX_QUADSPI_Init();
  MX_SPI3_Init();
  MX_USART1_UART_Init();
  MX_USART3_UART_Init();
  MX_USB_OTG_FS_PCD_Init();
  MX_UART4_Init();
  /* USER CODE BEGIN 2 */

  /*
   * TODO:
   *    IMPLEMENT A POWER DOWN CONTROL INIT FUNCTION FOR CTRL_REG1 (0x20) BIT 7
   * 
   */

  // Header info for CLI
  char* cliHeader = "\r\nsimpleCLI Interface v0.2\r\n";
  HAL_UART_Transmit(&huart1, (uint8_t*) cliHeader, strlen(cliHeader), 1000);


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

	   /************************************
	    * User Story 1 implementation begin:
	    *
	    * 	Define a simple CLI with options
	    * 	for a who am i request, or a 
        *   temperature read via polling, 
        *   interrupt, or DMA read from the 
        *   sensor over the I2C bus
	    *
	    ***********************************/

	  // Define strings to structure prompt around
	  char* cliPrompt = "Options:\r\n\t1: WHO AM I\r\n\t2: Temperature read (polling mode)\r\n\t3: Temperature read (interrupt mode)\r\n\t4: Temperature read (DMA mode)\r\n\t5: *TODO* Temperature read (EXTI mode) *TODO*\r\n$> ";
	  char* cliResponse = "Invalid input!\r\n";

	  // Issue prompt
	  HAL_UART_Transmit(&huart1, (uint8_t*) cliPrompt, strlen(cliPrompt), 1000);

	  // Get the user selection
	  char cliInput;
	  HAL_UART_Receive(&huart1, (uint8_t*) &cliInput, 1, HAL_MAX_DELAY);

	  // Evaluate input
	  switch (cliInput)
	  {
        case '1':
            cliResponse = "\r\nWHO AM I request:\r\n";
            HAL_UART_Transmit(&huart1, (uint8_t*) cliResponse, strlen(cliResponse), 1000);
            do_who_am_i();
            break;

        case '2':
            cliResponse = "\r\nTemperature read (poll) request:\r\n";
            HAL_UART_Transmit(&huart1, (uint8_t*) cliResponse, strlen(cliResponse), 1000);
            do_temp_polling();
            break;

        case '3':
            cliResponse = "\r\nTemperature read (IT) request:\r\n";
            HAL_UART_Transmit(&huart1, (uint8_t*) cliResponse, strlen(cliResponse), 1000);
            do_temp_interrupt();
            break;

        case '4':
            cliResponse = "\r\nTemperature read (DMA) request:\r\n";
            HAL_UART_Transmit(&huart1, (uint8_t*) cliResponse, strlen(cliResponse), 1000);
            do_temp_dma();
            break;

        /*
        TODO:
        case '5':
            cliResponse = "\r\nTemperature read (EXTI) request:\r\n";
            HAL_UART_Transmit(&huart1, (uint8_t*) cliResponse, strlen(cliResponse), 1000);
            do_temp_interrupt_EXTI();
            break;
        */

        default:
            HAL_UART_Transmit(&huart1, (uint8_t*) cliResponse, strlen(cliResponse), 1000);
            break;
	  }

  }
  /* USER CODE END 3 */
}

