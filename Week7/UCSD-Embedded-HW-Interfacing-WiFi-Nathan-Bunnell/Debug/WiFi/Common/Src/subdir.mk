################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../WiFi/Common/Src/es_wifi.c \
../WiFi/Common/Src/es_wifi_io.c \
../WiFi/Common/Src/wifi.c 

OBJS += \
./WiFi/Common/Src/es_wifi.o \
./WiFi/Common/Src/es_wifi_io.o \
./WiFi/Common/Src/wifi.o 

C_DEPS += \
./WiFi/Common/Src/es_wifi.d \
./WiFi/Common/Src/es_wifi_io.d \
./WiFi/Common/Src/wifi.d 


# Each subdirectory must supply rules for building sources it contributes
WiFi/Common/Src/es_wifi.o: ../WiFi/Common/Src/es_wifi.c WiFi/Common/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L475xx -c -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -I"C:/ST/STM32CubeIDE_1.6.1/STM32CubeIDE/Projects/UCSD-Embedded-HW-Interfacing-WiFi-Nathan-Bunnell/WiFi/WiFi_HTTP_Server/Inc" -I"C:/ST/STM32CubeIDE_1.6.1/STM32CubeIDE/Projects/UCSD-Embedded-HW-Interfacing-WiFi-Nathan-Bunnell/WiFi/WiFi_HTTP_Server/Src" -I"C:/ST/STM32CubeIDE_1.6.1/STM32CubeIDE/Projects/UCSD-Embedded-HW-Interfacing-WiFi-Nathan-Bunnell/Drivers/BSP/B-L475E-IOT01" -I"C:/ST/STM32CubeIDE_1.6.1/STM32CubeIDE/Projects/UCSD-Embedded-HW-Interfacing-WiFi-Nathan-Bunnell/Drivers/BSP/Components/Common" -I"C:/ST/STM32CubeIDE_1.6.1/STM32CubeIDE/Projects/UCSD-Embedded-HW-Interfacing-WiFi-Nathan-Bunnell/WiFi/Common/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"WiFi/Common/Src/es_wifi.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
WiFi/Common/Src/es_wifi_io.o: ../WiFi/Common/Src/es_wifi_io.c WiFi/Common/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L475xx -c -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -I"C:/ST/STM32CubeIDE_1.6.1/STM32CubeIDE/Projects/UCSD-Embedded-HW-Interfacing-WiFi-Nathan-Bunnell/WiFi/WiFi_HTTP_Server/Inc" -I"C:/ST/STM32CubeIDE_1.6.1/STM32CubeIDE/Projects/UCSD-Embedded-HW-Interfacing-WiFi-Nathan-Bunnell/WiFi/WiFi_HTTP_Server/Src" -I"C:/ST/STM32CubeIDE_1.6.1/STM32CubeIDE/Projects/UCSD-Embedded-HW-Interfacing-WiFi-Nathan-Bunnell/Drivers/BSP/B-L475E-IOT01" -I"C:/ST/STM32CubeIDE_1.6.1/STM32CubeIDE/Projects/UCSD-Embedded-HW-Interfacing-WiFi-Nathan-Bunnell/Drivers/BSP/Components/Common" -I"C:/ST/STM32CubeIDE_1.6.1/STM32CubeIDE/Projects/UCSD-Embedded-HW-Interfacing-WiFi-Nathan-Bunnell/WiFi/Common/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"WiFi/Common/Src/es_wifi_io.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
WiFi/Common/Src/wifi.o: ../WiFi/Common/Src/wifi.c WiFi/Common/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L475xx -c -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -I"C:/ST/STM32CubeIDE_1.6.1/STM32CubeIDE/Projects/UCSD-Embedded-HW-Interfacing-WiFi-Nathan-Bunnell/WiFi/WiFi_HTTP_Server/Inc" -I"C:/ST/STM32CubeIDE_1.6.1/STM32CubeIDE/Projects/UCSD-Embedded-HW-Interfacing-WiFi-Nathan-Bunnell/WiFi/WiFi_HTTP_Server/Src" -I"C:/ST/STM32CubeIDE_1.6.1/STM32CubeIDE/Projects/UCSD-Embedded-HW-Interfacing-WiFi-Nathan-Bunnell/Drivers/BSP/B-L475E-IOT01" -I"C:/ST/STM32CubeIDE_1.6.1/STM32CubeIDE/Projects/UCSD-Embedded-HW-Interfacing-WiFi-Nathan-Bunnell/Drivers/BSP/Components/Common" -I"C:/ST/STM32CubeIDE_1.6.1/STM32CubeIDE/Projects/UCSD-Embedded-HW-Interfacing-WiFi-Nathan-Bunnell/WiFi/Common/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"WiFi/Common/Src/wifi.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

