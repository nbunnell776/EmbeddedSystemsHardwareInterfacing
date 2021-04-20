  /************************************
  * User Story 2 implementation begin:
  *
  * 	Connect ARD-1 to analog voltage
  * 	source and read using ADC interrupt
  * 	mode. Pipe output to console.
  *
  ***********************************/

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

	  /************************************
	  * User Story 2 implementation end
	  *
	  ***********************************/

  }