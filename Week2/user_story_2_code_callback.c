void HAL_ADC_ConvCplt_Callback(ADC_HandleTypeDef *hadc)
{

	uint16_t adcValue = HAL_ADC_GetValue(&Hadc1);

	// Define formatting strings to provide a clean serial output
	char printString[255] = "\nARD-A1: raw: ";
	char voltsString[20] = ", volts: ";

	// Store value into a buffer
	char bufferRaw[20];
	snprintf(bufferRaw, sizeof(bufferRaw), "%u", adcValue);

	// Convert raw counts into voltage
	char bufferVoltage[20];
	float valueVoltage= (adcValue * (3.3/4096));
	snprintf(bufferVoltage, sizeof(bufferVoltage), "%5.3f", valueVoltage);

	// Concat the formatting strings and data to an output
	// to send out over the UART1 serial port/ USB-micro
	// port to PC
	strcat(printString, bufferRaw);
	strcat(printString, voltsString);
	strcat(printString, bufferVoltage);

	HAL_UART_Transmit(&huart1, (uint8_t *) printString, strlen(printString), 1000);
}
