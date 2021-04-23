/************************************
  * User Story 2 implementation begin:
  *
  * 	Connect ARD-1 to analog voltage
  * 	source and read using ADC interrupt
  * 	mode. Pipe output to console.
  *
  ***********************************/

  // Startup string to show that we're working
  char startupString[255] = "Starting ADC IT mode...\n";
  HAL_UART_Transmit(&huart1, (uint8_t *) startupString, strlen(startupString), 1000);

  // Calibrate ADC1
  HAL_ADCEx_Calibration_Start(&hadc1, ADC_SINGLE_ENDED);

  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

	  // Blink & delay to give indication of *something* happening
	  HAL_GPIO_TogglePin(LED3_WIFI__LED4_BLE_GPIO_Port, LED3_WIFI__LED4_BLE_Pin);
	  HAL_Delay(1000);

	  // Enable ADC1 in IT mode
	  HAL_ADC_Start_IT(&hadc1);

	  // Status string to show that we're working
	  char statusString[255] = "Waiting for ADC IT...\n";
	  HAL_UART_Transmit(&huart1, (uint8_t *) statusString, strlen(statusString), 1000);

	  /************************************
	  * User Story 2 implementation end
	  *
	  ***********************************/

  }