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
DMA_HandleTypeDef hdma_i2c2_tx;
DMA_HandleTypeDef hdma_i2c2_rx;

QSPI_HandleTypeDef hqspi;

SPI_HandleTypeDef hspi3;

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart3;

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
/* USER CODE BEGIN PFP */

static void HST221_pwr_en(void);
static void do_who_am_i(void);
static void do_temp_polling(void);
static void do_temp_interrupt(void);
static void do_temp_dma(void);
static void do_temp_interrupt_EXTI(void);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

static void HST221_pwr_en(void)
{
    // Large-ish char buffer for strings sent over the console
    char buffer[100] = {0};

	// Configure control register 1 (CTRL_REG1, 0x20) bit 7 to enable one-shot
    uint8_t ctrlReg1 = 0x20;
    uint8_t ctrlData[] = {ctrlReg1, (1 << 7)};

    // Send the target register to the device and get status back
    HAL_StatusTypeDef status;
    status = HAL_I2C_Master_Transmit(&hi2c2, HST221_WRITE_ADDRESS, ctrlData, sizeof(ctrlData), 1000);

    // Print status results over UART1 to console session
    snprintf(buffer, sizeof(buffer), "\n(HST221 Power-Down Control) HAL_I2C_Master_Transmit: status %u\n", status);
    HAL_UART_Transmit(&huart1, (uint8_t*) buffer, strlen(buffer), 1000);

    // Read response back to get value of WHO_AM_I register
	uint8_t mode = 0xff;    // Default value should be 0 according to datasheet, target is 1 for enabled
	status = HAL_I2C_Master_Receive(&hi2c2, HST221_READ_ADDRESS, &mode, sizeof(mode), 1000);

	// Print status results and response value over UART1 to console session
	snprintf(buffer, sizeof(buffer), "(HST221 Power-Down Control) HAL_I2C_Master_Receive status: %u; mode: 0x%x\n", status, mode);
	HAL_UART_Transmit(&huart1, (uint8_t*) buffer, strlen(buffer), 1000);
}

static void do_who_am_i(void)
{
    // Declare address of the device's WHO_AM_I register
    uint8_t whoAmIReg = 0xf;

    // Send the target register to the device and get status back
    HAL_StatusTypeDef status;
    status = HAL_I2C_Master_Transmit(&hi2c2, HST221_WRITE_ADDRESS, &whoAmIReg, sizeof(whoAmIReg), 1000);

    // Print status results over UART1 to console session
    char buffer[100] = {0};
    snprintf(buffer, sizeof(buffer), "\tHAL_I2C_Master_Transmit status: %u\n", status);
    HAL_UART_Transmit(&huart1, (uint8_t*) buffer, strlen(buffer), 1000);

    // Read response back to get value of WHO_AM_I register
    uint8_t data = 0xff;    // Default value should be 0xbc according to datasheet
    status = HAL_I2C_Master_Receive(&hi2c2, HST221_READ_ADDRESS, &data, sizeof(data), 1000);

    // Print status results and response value over UART1 to console session
    snprintf(buffer, sizeof(buffer), "\tHAL_I2C_Master_Receive status: %u; data: 0x%x\n\n", status, data);
    HAL_UART_Transmit(&huart1, (uint8_t*) buffer, strlen(buffer), 1000);
}


static void do_temp_polling(void)
{
    // Large-ish char buffer for strings sent over the console
    char buffer[100] = {0};

	// Configure control register 2 (CTRL_REG2, 0x21) bit 0 to enable one-shot
    uint8_t ctrlReg2 = 0x21;
    uint8_t ctrlData[] = {ctrlReg2, (1 << 0)};

    // Send the target register to the device and get status back
    HAL_StatusTypeDef status;
    status = HAL_I2C_Master_Transmit(&hi2c2, HST221_WRITE_ADDRESS, ctrlData, sizeof(ctrlData), 1000);

    // Print status results over UART1 to console session
    snprintf(buffer, sizeof(buffer), "\t(One-shot enabled): HAL_I2C_Master_Transmit: status %u\n", status);
    HAL_UART_Transmit(&huart1, (uint8_t*) buffer, strlen(buffer), 1000);

    // Define status register (STATUS_REG2, 0x27) bit 0 to monitor for new sample available
    uint8_t statusReg = 0x27;
    uint8_t sampleReady = 0;

    // Loiter for a bit to allow time for conversion to complete and be made available
    uint8_t count = 0;
    while (count < 10)  // 10 is an arbitrary "long enough" value, this wouldn't always be great real-world practice
    {
        // Send the address of the status register and report it over the console
        status = HAL_I2C_Master_Transmit(&hi2c2, HST221_WRITE_ADDRESS, &statusReg, sizeof(statusReg), 1000);
        snprintf(buffer, sizeof(buffer), "\t[%d] (status register): HAL_I2C_Master_Transmit: status %u\n", count, status);
        HAL_UART_Transmit(&huart1, (uint8_t*) buffer, strlen(buffer), 1000);

        // Read back the value of the status register and report it over the console
        status = HAL_I2C_Master_Receive(&hi2c2, HST221_READ_ADDRESS, (uint8_t *)&sampleReady, sizeof(sampleReady), 1000);
        snprintf(buffer, sizeof(buffer), "\tStatus register: 0x%02x\n", sampleReady);
        HAL_UART_Transmit(&huart1, (uint8_t*) buffer, strlen(buffer), 1000);

        // If the new sample is ready, report it to the console and break out of while-loop...
        if (sampleReady & 0x01)
        {
            snprintf(buffer, sizeof(buffer), "\tNew Temperature Sample Available!\n");
            HAL_UART_Transmit(&huart1, (uint8_t*) buffer, strlen(buffer), 1000);
            break;
        }

        // Else wait for a bit, increment the counter, and keep looping
        HAL_Delay(100);
        count++;
    }

    // With new sample ready, practice solutions implemented with reading sequentially from
    //  the LSB and MSB registers (0x2a and 0x2b) as well as via auto-increment
    static uint8_t toggle = 1;

    if (toggle)
    {
        toggle = 0;

        // Reading the lower half of the temperature register
        // Send target address
        uint8_t tempRegLSB = 0x2a;
        status = HAL_I2C_Master_Transmit(&hi2c2, HST221_WRITE_ADDRESS, &tempRegLSB, sizeof(tempRegLSB), 1000);
        snprintf(buffer, sizeof(buffer), "\t(LSB): HAL_I2C_Master_Transmit: status: %u\n", status);
        HAL_UART_Transmit(&huart1, (uint8_t*) buffer, strlen(buffer), 1000);

        // Read response back and print over console
        uint8_t tempDataLSB = 0xff; // Junk default value
        status = HAL_I2C_Master_Receive(&hi2c2, HST221_READ_ADDRESS, (uint8_t *)&tempDataLSB, sizeof(tempDataLSB), 1000);
        snprintf(buffer, sizeof(buffer), "\t(LSB): HAL_I2C_Master_Receive: status: %u; LSB data: 0x%02x\n", status, tempDataLSB);
        HAL_UART_Transmit(&huart1, (uint8_t*) buffer, strlen(buffer), 1000);

        // Reading the upper half of the temperature register
        // Send target address
        uint8_t tempRegMSB = 0x2b;
        status = HAL_I2C_Master_Transmit(&hi2c2, HST221_WRITE_ADDRESS, &tempRegMSB, sizeof(tempRegMSB), 1000);
        snprintf(buffer, sizeof(buffer), "\t(MSB): HAL_I2C_Master_Transmit: status: %u\n", status);
        HAL_UART_Transmit(&huart1, (uint8_t*) buffer, strlen(buffer), 1000);

        // Read response back and print over console
        uint8_t tempDataMSB = 0xff; // Junk default value
        status = HAL_I2C_Master_Receive(&hi2c2, HST221_READ_ADDRESS, (uint8_t *)&tempDataMSB, sizeof(tempDataMSB), 1000);
        snprintf(buffer, sizeof(buffer), "\t(MSB): HAL_I2C_Master_Receive: status: %u; MSB data: 0x%02x\n\n", status, tempDataMSB);
        HAL_UART_Transmit(&huart1, (uint8_t*) buffer, strlen(buffer), 1000);
    }
    else
    {
        toggle = 1;

        // Reading the lower half of the temperature register with auto-increment enabled
        // Send target address (OR'ing with 0x80 enables auto-inc)
        uint8_t tempRegLSB = 0x2a | 0x80;
        status = HAL_I2C_Master_Transmit(&hi2c2, HST221_WRITE_ADDRESS, &tempRegLSB, sizeof(tempRegLSB), 1000);
        snprintf(buffer, sizeof(buffer), "\t(Auto-increment): HAL_I2C_Master_Transmit: status: %u\n", status);
        HAL_UART_Transmit(&huart1, (uint8_t*) buffer, strlen(buffer), 1000);

        // Read response back for both registers and print over console
        uint16_t tempData = 0xbeef; // Junk default value (ALSO REALLY HOT!)
        status = HAL_I2C_Master_Receive(&hi2c2, HST221_READ_ADDRESS, (uint8_t *)&tempData, sizeof(tempData), 1000);
        snprintf(buffer, sizeof(buffer), "\t(Auto-increment): HAL_I2C_Master_Receive: status: %u; temperature data: 0x%04x\n\n", status, tempData);
        HAL_UART_Transmit(&huart1, (uint8_t*) buffer, strlen(buffer), 1000);
    }

}

// Define a pair of IRQ status flags and the I2C interrupt callback functions
static uint8_t irqCompleteTX = 0;
static uint8_t irqCompleteRX = 0;

// Used for IT/DMA access
uint16_t tempData = 0xffff; // Junk default value (ALSO REALLY HOT!)

void HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef *hi2c)
{
    // Indicate something on the board
    HAL_GPIO_TogglePin(LED2_GPIO_Port, LED2_Pin);

    // Set status flag
    irqCompleteTX = 1;
}

void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef *hi2c)
{
    // Indicate something on the board
    HAL_GPIO_TogglePin(LED2_GPIO_Port, LED2_Pin);

    // Set status flag
    irqCompleteRX = 1;
}

static void do_temp_interrupt(void)
{
	// Clear flags
	irqCompleteTX = 0;
	irqCompleteRX = 0;

    // Large-ish char buffer for strings sent over the console
    char buffer[100] = {0};

	// Configure control register 2 (CTRL_REG2, 0x21) bit 0 to enable one-shot
    uint8_t ctrlReg2 = 0x21;
    uint8_t ctrlData[] = {ctrlReg2, 0x01};

    // Send the target register to the device and get status back using the *_IT function
    HAL_StatusTypeDef status;
    status = HAL_I2C_Master_Transmit_IT(&hi2c2, HST221_WRITE_ADDRESS, ctrlData, sizeof(ctrlData));

    // Print status results over UART1 to console session
    snprintf(buffer, sizeof(buffer), "\t(One-shot enabled): HAL_I2C_Master_Transmit_IT: status %u\n", status);
    HAL_UART_Transmit(&huart1, (uint8_t*) buffer, strlen(buffer), 1000);

    // Wait for interrupt to complete
	while (irqCompleteTX == 0)
	{
		HAL_Delay(1000);
	}

	// Clear flag
	irqCompleteTX = 0;

    // Reading the lower half of the temperature register with auto-increment enabled
	// Send target address (OR'ing with 0x80 enables auto-inc)
	uint8_t tempRegLSB = 0x2a | 0x80;
	status = HAL_I2C_Master_Transmit_IT(&hi2c2, HST221_WRITE_ADDRESS, &tempRegLSB, sizeof(tempRegLSB));
	snprintf(buffer, sizeof(buffer), "\t(Auto-increment): HAL_I2C_Master_Transmit_IT: status: %u\n", status);
	HAL_UART_Transmit(&huart1, (uint8_t*) buffer, strlen(buffer), 1000);

    // Wait for interrupt to complete
	while (irqCompleteTX == 0)
	{
		HAL_Delay(1000);
	}

	// Read response back for both registers and print over console
	status = HAL_I2C_Master_Receive_IT(&hi2c2, HST221_READ_ADDRESS, (uint8_t *)&tempData, sizeof(tempData));
	snprintf(buffer, sizeof(buffer), "\t(Auto-increment): HAL_I2C_Master_Receive_IT: status: %u; temperature data: 0x%04x\n\n", status, tempData);
	HAL_UART_Transmit(&huart1, (uint8_t*) buffer, strlen(buffer), 1000);
}

static void do_temp_dma(void)
{
	// Clear flags
    irqCompleteTX = 0;
    irqCompleteRX = 0;

    // Large-ish char buffer for strings sent over the console
    char buffer[100] = {0};

	// Configure control register 2 (CTRL_REG2, 0x21) bit 0 to enable one-shot
    uint8_t ctrlReg2 = 0x21;
    uint8_t ctrlData[] = {ctrlReg2, 0x01};

    // Send the target register to the device and get status back using the *_DMA function
    HAL_StatusTypeDef status;
    status = HAL_I2C_Master_Transmit_DMA(&hi2c2, HST221_WRITE_ADDRESS, ctrlData, sizeof(ctrlData));

    // Print status results over UART1 to console session
    snprintf(buffer, sizeof(buffer), "\t(One-shot enabled): HAL_I2C_Master_Transmit_DMA: status %u\n", status);
    HAL_UART_Transmit(&huart1, (uint8_t*) buffer, strlen(buffer), 1000);

    // Wait for interrupt to complete
	while (irqCompleteTX == 0)
	{
		HAL_Delay(100);
	}

	// Clear flag
	irqCompleteTX = 0;

    // Reading the lower half of the temperature register with auto-increment enabled
	// Send target address (OR'ing with 0x80 enables auto-inc)
	uint8_t tempRegLSB = 0x2a | 0x80;
	status = HAL_I2C_Master_Transmit_DMA(&hi2c2, HST221_WRITE_ADDRESS, &tempRegLSB, sizeof(tempRegLSB));
	snprintf(buffer, sizeof(buffer), "\t(Auto-increment): HAL_I2C_Master_Transmit_DMA: status: %u\n", status);
	HAL_UART_Transmit(&huart1, (uint8_t*) buffer, strlen(buffer), 1000);

    // Wait for interrupt to complete
    while (irqCompleteTX == 0)
    {
        HAL_Delay(100);
    }

    // Read response back for both registers and print over console
	status = HAL_I2C_Master_Receive_DMA(&hi2c2, HST221_READ_ADDRESS, (uint8_t *)&tempData, sizeof(tempData));
	snprintf(buffer, sizeof(buffer), "\t(Auto-increment): HAL_I2C_Master_Receive_DMA: status: %u; temperature data: 0x%04x\n\n", status, tempData);
	HAL_UART_Transmit(&huart1, (uint8_t*) buffer, strlen(buffer), 1000);
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
  /* USER CODE BEGIN 2 */

  	// Enable the HST221 PD bit in CR1 to allow collection of samples
    HST221_pwr_en();

    // Header info for CLI
    char* cliHeader = "\nsimpleCLI Interface v0.2\n------------------------------\n";
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
  	  char* cliPrompt = "Options:\n\t1: \"WHO AM I\" read\n\t2: Temperature read (polling mode)\n\t3: Temperature read (interrupt mode)\n\t4: Temperature read (DMA mode)\n\t5: *TODO* Temperature read (EXTI mode) *TODO*\r\n$> ";
  	  char* cliResponse = "Invalid input!\r\n";

  	  // Issue prompt
  	  HAL_UART_Transmit(&huart1, (uint8_t*) cliPrompt, strlen(cliPrompt), 1000);

  	  // Get the user selection and echo it on the terminal
  	  char cliInput;
  	  HAL_UART_Receive(&huart1, (uint8_t*) &cliInput, 1, HAL_MAX_DELAY);
  	  HAL_UART_Transmit(&huart1, (uint8_t*) &cliInput, 1, 1000);

  	  // Evaluate input
  	  switch (cliInput)
  	  {
          case '1':
              cliResponse = "\r\n\"WHO AM I\" request:\n";
              HAL_UART_Transmit(&huart1, (uint8_t*) cliResponse, strlen(cliResponse), 1000);
              do_who_am_i();
              break;

          case '2':
              cliResponse = "\r\nTemperature read (poll) request:\n";
              HAL_UART_Transmit(&huart1, (uint8_t*) cliResponse, strlen(cliResponse), 1000);
              do_temp_polling();
              break;

          case '3':
              cliResponse = "\r\nTemperature read (IT) request:\n";
              HAL_UART_Transmit(&huart1, (uint8_t*) cliResponse, strlen(cliResponse), 1000);
              do_temp_interrupt();
              break;

          case '4':
              cliResponse = "\r\nTemperature read (DMA) request:\n";
              HAL_UART_Transmit(&huart1, (uint8_t*) cliResponse, strlen(cliResponse), 1000);
              do_temp_dma();
              break;

          case '5':
              cliResponse = "\r\nTemperature read (EXTI) request:\n";
              HAL_UART_Transmit(&huart1, (uint8_t*) cliResponse, strlen(cliResponse), 1000);
              do_temp_interrupt_EXTI();
              break;

          default:
              HAL_UART_Transmit(&huart1, (uint8_t*) cliResponse, strlen(cliResponse), 1000);
              break;
  	  }

    }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Configure LSE Drive Capability
  */
  HAL_PWR_EnableBkUpAccess();
  __HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSE|RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 40;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1|RCC_PERIPHCLK_USART3
                              |RCC_PERIPHCLK_I2C2|RCC_PERIPHCLK_DFSDM1
                              |RCC_PERIPHCLK_USB;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
  PeriphClkInit.Usart3ClockSelection = RCC_USART3CLKSOURCE_PCLK1;
  PeriphClkInit.I2c2ClockSelection = RCC_I2C2CLKSOURCE_PCLK1;
  PeriphClkInit.Dfsdm1ClockSelection = RCC_DFSDM1CLKSOURCE_PCLK;
  PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_PLLSAI1;
  PeriphClkInit.PLLSAI1.PLLSAI1Source = RCC_PLLSOURCE_MSI;
  PeriphClkInit.PLLSAI1.PLLSAI1M = 1;
  PeriphClkInit.PLLSAI1.PLLSAI1N = 24;
  PeriphClkInit.PLLSAI1.PLLSAI1P = RCC_PLLP_DIV7;
  PeriphClkInit.PLLSAI1.PLLSAI1Q = RCC_PLLQ_DIV2;
  PeriphClkInit.PLLSAI1.PLLSAI1R = RCC_PLLR_DIV2;
  PeriphClkInit.PLLSAI1.PLLSAI1ClockOut = RCC_PLLSAI1_48M2CLK;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Enable MSI Auto calibration
  */
  HAL_RCCEx_EnableMSIPLLMode();
}

/**
  * @brief DFSDM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_DFSDM1_Init(void)
{

  /* USER CODE BEGIN DFSDM1_Init 0 */

  /* USER CODE END DFSDM1_Init 0 */

  /* USER CODE BEGIN DFSDM1_Init 1 */

  /* USER CODE END DFSDM1_Init 1 */
  hdfsdm1_channel1.Instance = DFSDM1_Channel1;
  hdfsdm1_channel1.Init.OutputClock.Activation = ENABLE;
  hdfsdm1_channel1.Init.OutputClock.Selection = DFSDM_CHANNEL_OUTPUT_CLOCK_SYSTEM;
  hdfsdm1_channel1.Init.OutputClock.Divider = 2;
  hdfsdm1_channel1.Init.Input.Multiplexer = DFSDM_CHANNEL_EXTERNAL_INPUTS;
  hdfsdm1_channel1.Init.Input.DataPacking = DFSDM_CHANNEL_STANDARD_MODE;
  hdfsdm1_channel1.Init.Input.Pins = DFSDM_CHANNEL_FOLLOWING_CHANNEL_PINS;
  hdfsdm1_channel1.Init.SerialInterface.Type = DFSDM_CHANNEL_SPI_RISING;
  hdfsdm1_channel1.Init.SerialInterface.SpiClock = DFSDM_CHANNEL_SPI_CLOCK_INTERNAL;
  hdfsdm1_channel1.Init.Awd.FilterOrder = DFSDM_CHANNEL_FASTSINC_ORDER;
  hdfsdm1_channel1.Init.Awd.Oversampling = 1;
  hdfsdm1_channel1.Init.Offset = 0;
  hdfsdm1_channel1.Init.RightBitShift = 0x00;
  if (HAL_DFSDM_ChannelInit(&hdfsdm1_channel1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN DFSDM1_Init 2 */

  /* USER CODE END DFSDM1_Init 2 */

}

/**
  * @brief I2C2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C2_Init(void)
{

  /* USER CODE BEGIN I2C2_Init 0 */

  /* USER CODE END I2C2_Init 0 */

  /* USER CODE BEGIN I2C2_Init 1 */

  /* USER CODE END I2C2_Init 1 */
  hi2c2.Instance = I2C2;
  hi2c2.Init.Timing = 0x10909CEC;
  hi2c2.Init.OwnAddress1 = 0;
  hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c2.Init.OwnAddress2 = 0;
  hi2c2.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c2) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c2, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c2, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C2_Init 2 */

  /* USER CODE END I2C2_Init 2 */

}

/**
  * @brief QUADSPI Initialization Function
  * @param None
  * @retval None
  */
static void MX_QUADSPI_Init(void)
{

  /* USER CODE BEGIN QUADSPI_Init 0 */

  /* USER CODE END QUADSPI_Init 0 */

  /* USER CODE BEGIN QUADSPI_Init 1 */

  /* USER CODE END QUADSPI_Init 1 */
  /* QUADSPI parameter configuration*/
  hqspi.Instance = QUADSPI;
  hqspi.Init.ClockPrescaler = 2;
  hqspi.Init.FifoThreshold = 4;
  hqspi.Init.SampleShifting = QSPI_SAMPLE_SHIFTING_HALFCYCLE;
  hqspi.Init.FlashSize = 23;
  hqspi.Init.ChipSelectHighTime = QSPI_CS_HIGH_TIME_1_CYCLE;
  hqspi.Init.ClockMode = QSPI_CLOCK_MODE_0;
  if (HAL_QSPI_Init(&hqspi) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN QUADSPI_Init 2 */

  /* USER CODE END QUADSPI_Init 2 */

}

/**
  * @brief SPI3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI3_Init(void)
{

  /* USER CODE BEGIN SPI3_Init 0 */

  /* USER CODE END SPI3_Init 0 */

  /* USER CODE BEGIN SPI3_Init 1 */

  /* USER CODE END SPI3_Init 1 */
  /* SPI3 parameter configuration*/
  hspi3.Instance = SPI3;
  hspi3.Init.Mode = SPI_MODE_MASTER;
  hspi3.Init.Direction = SPI_DIRECTION_2LINES;
  hspi3.Init.DataSize = SPI_DATASIZE_4BIT;
  hspi3.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi3.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi3.Init.NSS = SPI_NSS_SOFT;
  hspi3.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi3.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi3.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi3.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi3.Init.CRCPolynomial = 7;
  hspi3.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
  hspi3.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
  if (HAL_SPI_Init(&hspi3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI3_Init 2 */

  /* USER CODE END SPI3_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief USART3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 115200;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  huart3.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart3.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

}

/**
  * @brief USB_OTG_FS Initialization Function
  * @param None
  * @retval None
  */
static void MX_USB_OTG_FS_PCD_Init(void)
{

  /* USER CODE BEGIN USB_OTG_FS_Init 0 */

  /* USER CODE END USB_OTG_FS_Init 0 */

  /* USER CODE BEGIN USB_OTG_FS_Init 1 */

  /* USER CODE END USB_OTG_FS_Init 1 */
  hpcd_USB_OTG_FS.Instance = USB_OTG_FS;
  hpcd_USB_OTG_FS.Init.dev_endpoints = 6;
  hpcd_USB_OTG_FS.Init.speed = PCD_SPEED_FULL;
  hpcd_USB_OTG_FS.Init.phy_itface = PCD_PHY_EMBEDDED;
  hpcd_USB_OTG_FS.Init.Sof_enable = DISABLE;
  hpcd_USB_OTG_FS.Init.low_power_enable = DISABLE;
  hpcd_USB_OTG_FS.Init.lpm_enable = DISABLE;
  hpcd_USB_OTG_FS.Init.battery_charging_enable = DISABLE;
  hpcd_USB_OTG_FS.Init.use_dedicated_ep1 = DISABLE;
  hpcd_USB_OTG_FS.Init.vbus_sensing_enable = DISABLE;
  if (HAL_PCD_Init(&hpcd_USB_OTG_FS) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USB_OTG_FS_Init 2 */

  /* USER CODE END USB_OTG_FS_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Channel4_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel4_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel4_IRQn);
  /* DMA1_Channel5_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel5_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, M24SR64_Y_RF_DISABLE_Pin|M24SR64_Y_GPO_Pin|ISM43362_RST_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, ARD_D10_Pin|SPBTLE_RF_RST_Pin|ARD_D9_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, ARD_D8_Pin|ISM43362_BOOT0_Pin|ISM43362_WAKEUP_Pin|LED2_Pin
                          |SPSGRF_915_SDN_Pin|ARD_D5_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, USB_OTG_FS_PWR_EN_Pin|PMOD_RESET_Pin|STSAFE_A100_RESET_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(SPBTLE_RF_SPI3_CSN_GPIO_Port, SPBTLE_RF_SPI3_CSN_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, VL53L0X_XSHUT_Pin|LED3_WIFI__LED4_BLE_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(SPSGRF_915_SPI3_CSN_GPIO_Port, SPSGRF_915_SPI3_CSN_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(ISM43362_SPI3_CSN_GPIO_Port, ISM43362_SPI3_CSN_Pin, GPIO_PIN_SET);

  /*Configure GPIO pins : M24SR64_Y_RF_DISABLE_Pin M24SR64_Y_GPO_Pin ISM43362_RST_Pin ISM43362_SPI3_CSN_Pin */
  GPIO_InitStruct.Pin = M24SR64_Y_RF_DISABLE_Pin|M24SR64_Y_GPO_Pin|ISM43362_RST_Pin|ISM43362_SPI3_CSN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : USB_OTG_FS_OVRCR_EXTI3_Pin SPSGRF_915_GPIO3_EXTI5_Pin SPBTLE_RF_IRQ_EXTI6_Pin ISM43362_DRDY_EXTI1_Pin */
  GPIO_InitStruct.Pin = USB_OTG_FS_OVRCR_EXTI3_Pin|SPSGRF_915_GPIO3_EXTI5_Pin|SPBTLE_RF_IRQ_EXTI6_Pin|ISM43362_DRDY_EXTI1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pin : BUTTON_EXTI13_Pin */
  GPIO_InitStruct.Pin = BUTTON_EXTI13_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(BUTTON_EXTI13_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : ARD_A5_Pin ARD_A4_Pin ARD_A3_Pin ARD_A2_Pin
                           ARD_A1_Pin ARD_A0_Pin */
  GPIO_InitStruct.Pin = ARD_A5_Pin|ARD_A4_Pin|ARD_A3_Pin|ARD_A2_Pin
                          |ARD_A1_Pin|ARD_A0_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG_ADC_CONTROL;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : ARD_D1_Pin ARD_D0_Pin */
  GPIO_InitStruct.Pin = ARD_D1_Pin|ARD_D0_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF8_UART4;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : ARD_D10_Pin SPBTLE_RF_RST_Pin ARD_D9_Pin */
  GPIO_InitStruct.Pin = ARD_D10_Pin|SPBTLE_RF_RST_Pin|ARD_D9_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : ARD_D4_Pin */
  GPIO_InitStruct.Pin = ARD_D4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF1_TIM2;
  HAL_GPIO_Init(ARD_D4_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : ARD_D7_Pin */
  GPIO_InitStruct.Pin = ARD_D7_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG_ADC_CONTROL;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(ARD_D7_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : ARD_D13_Pin ARD_D12_Pin ARD_D11_Pin */
  GPIO_InitStruct.Pin = ARD_D13_Pin|ARD_D12_Pin|ARD_D11_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : ARD_D3_Pin */
  GPIO_InitStruct.Pin = ARD_D3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(ARD_D3_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : ARD_D6_Pin */
  GPIO_InitStruct.Pin = ARD_D6_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG_ADC_CONTROL;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(ARD_D6_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : ARD_D8_Pin ISM43362_BOOT0_Pin ISM43362_WAKEUP_Pin LED2_Pin
                           SPSGRF_915_SDN_Pin ARD_D5_Pin SPSGRF_915_SPI3_CSN_Pin */
  GPIO_InitStruct.Pin = ARD_D8_Pin|ISM43362_BOOT0_Pin|ISM43362_WAKEUP_Pin|LED2_Pin
                          |SPSGRF_915_SDN_Pin|ARD_D5_Pin|SPSGRF_915_SPI3_CSN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : LPS22HB_INT_DRDY_EXTI0_Pin LSM6DSL_INT1_EXTI11_Pin ARD_D2_Pin HTS221_DRDY_EXTI15_Pin
                           PMOD_IRQ_EXTI12_Pin */
  GPIO_InitStruct.Pin = LPS22HB_INT_DRDY_EXTI0_Pin|LSM6DSL_INT1_EXTI11_Pin|ARD_D2_Pin|HTS221_DRDY_EXTI15_Pin
                          |PMOD_IRQ_EXTI12_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : USB_OTG_FS_PWR_EN_Pin SPBTLE_RF_SPI3_CSN_Pin PMOD_RESET_Pin STSAFE_A100_RESET_Pin */
  GPIO_InitStruct.Pin = USB_OTG_FS_PWR_EN_Pin|SPBTLE_RF_SPI3_CSN_Pin|PMOD_RESET_Pin|STSAFE_A100_RESET_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : VL53L0X_XSHUT_Pin LED3_WIFI__LED4_BLE_Pin */
  GPIO_InitStruct.Pin = VL53L0X_XSHUT_Pin|LED3_WIFI__LED4_BLE_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : VL53L0X_GPIO1_EXTI7_Pin LSM3MDL_DRDY_EXTI8_Pin */
  GPIO_InitStruct.Pin = VL53L0X_GPIO1_EXTI7_Pin|LSM3MDL_DRDY_EXTI8_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : PMOD_SPI2_SCK_Pin */
  GPIO_InitStruct.Pin = PMOD_SPI2_SCK_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
  HAL_GPIO_Init(PMOD_SPI2_SCK_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : PMOD_UART2_CTS_Pin PMOD_UART2_RTS_Pin PMOD_UART2_TX_Pin PMOD_UART2_RX_Pin */
  GPIO_InitStruct.Pin = PMOD_UART2_CTS_Pin|PMOD_UART2_RTS_Pin|PMOD_UART2_TX_Pin|PMOD_UART2_RX_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : ARD_D15_Pin ARD_D14_Pin */
  GPIO_InitStruct.Pin = ARD_D15_Pin|ARD_D14_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
