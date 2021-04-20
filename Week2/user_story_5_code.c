/************************************
  * User Story 5 implementation begin:
  *
  * 	Connect ARD-0 to analog voltage
  * 	source and read using ADC polling
  * 	mode. Collect 12 readings, drop
  * 	the highest and lowest value,
  * 	then pipe the average of the
  * 	remaining ten out the serial port.
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

	  // Storage array for polled values
	  uint16_t valueRawArray[12];

	  // Collect 12 readings, added a short delay to get a little variety in readings
	  for (uint8_t i = 0; i < 12; i++)
	  {
		  HAL_ADC_Start(&hadc1);
		  HAL_ADC_PollForConversion(&hadc1, 10);
		  valueRawArray[i] = HAL_ADC_GetValue(&hadc1);
		  HAL_Delay(500);
	  }

	  // Find and nullify the lowest and highest readings
	  uint8_t lowestIndex = 0;
	  uint16_t lowestValue = valueRawArray[0];
	  uint8_t highestIndex = 0;
	  uint16_t highestValue = valueRawArray[0];

	  for (uint8_t i = 0; i < 12; i++)
	  {
		  if (valueRawArray[i] <= lowestValue)
		  {
			  lowestValue = valueRawArray[i];
			  lowestIndex= i;
		  }

		  if (valueRawArray[i] >= highestValue)
		  {
			  highestValue = valueRawArray[i];
			  highestIndex= i;
		  }
	  }

	  valueRawArray[lowestIndex] = 0;
	  valueRawArray[highestIndex] = 0;

	  // Average the remaining ten readings
	  uint16_t valueRawAverage = 0;
	  uint16_t valueRawSum = 0;

	  for (uint8_t i = 0; i < 12; i++)
	  {
		  valueRawSum += valueRawArray[i];
	  }

	  valueRawAverage = valueRawSum / 10;

	  // Define formatting strings to provide a clean serial output
	  char printString[255] = "\nAverage of 10 readings on ARD-A1: raw: ";
	  char voltsString[20] = ", volts: ";

	  // Store value into a buffer
	  char bufferRaw[20];
	  snprintf(bufferRaw, sizeof(bufferRaw), "%u", valueRawAverage);

	  // Convert raw counts into voltage
	  char bufferVoltage[20];
	  float valueVoltage = (valueRawAverage * (3.3/4096));
	  snprintf(bufferVoltage, sizeof(bufferVoltage), "%5.3f", valueVoltage);

	  // Concat the formatting strings and data to an output
	  // to send out over the UART1 serial port/ USB-micro
	  // port to PC
	  strcat(printString, bufferRaw);
	  strcat(printString, voltsString);
	  strcat(printString, bufferVoltage);

	  HAL_UART_Transmit(&huart1, (uint8_t *) printString, strlen(printString), 1000);

	  /************************************
	  * User Story 5 implementation end
	  *
	  ***********************************/

  }