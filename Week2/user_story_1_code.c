/************************************
  * User Story 1 implementation begin:
  *
  * 	Connect ARD-0 to analog voltage
  * 	source and read using ADC polling
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

	  // Enable ADC1
	  HAL_ADC_Start(&hadc1);

	  // Poll ADC1 for value
	  HAL_ADC_PollForConversion(&hadc1, 10);

	  uint16_t valueRaw = HAL_ADC_GetValue(&hadc1);

	  // Define formatting strings to provide a clean serial output
	  char printString[255] = "\nARD-A1: raw: ";
	  char voltsString[20] = ", volts: ";

	  // Store value into a buffer
	  char bufferRaw[20];
	  snprintf(bufferRaw, sizeof(bufferRaw), "%u", valueRaw);

	  // Convert raw counts into voltage
	  char bufferVoltage[20];
	  float valueVoltage= (valueRaw * (3.3/4096));
	  snprintf(bufferVoltage, sizeof(bufferVoltage), "%5.3f", valueVoltage);

	  // Concat the formatting strings and data to an output
	  // to send out over the UART1 serial port/ USB-micro
	  // port to PC
	  strcat(printString, bufferRaw);
	  strcat(printString, voltsString);
	  strcat(printString, bufferVoltage);

	  HAL_UART_Transmit(&huart1, (uint8_t *) printString, strlen(printString), 1000);

	  /************************************
	  * User Story 1 implementation end
	  *
	  ***********************************/

  }