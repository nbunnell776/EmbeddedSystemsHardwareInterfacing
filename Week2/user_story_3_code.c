/************************************
    * User Story 3 implementation begin:
    *
    * 	Connect ARD-2 to analog voltage
    * 	source and read using ADC DMA
    * 	mode. Pipe output to console.
    *
    ***********************************/

    // Startup string to show that we're working
    char startupString[255] = "Starting ADC DMA mode...\n";
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
  	  HAL_ADC_Start_DMA(&hadc1, &adcValue, 1);

  	  /************************************
  	  * User Story 3 implementation end
  	  *
  	  ***********************************/

    }