################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/BSP/B-L475E-IOT01/stm32l475e_iot01.c \
../Drivers/BSP/B-L475E-IOT01/stm32l475e_iot01_accelero.c \
../Drivers/BSP/B-L475E-IOT01/stm32l475e_iot01_gyro.c \
../Drivers/BSP/B-L475E-IOT01/stm32l475e_iot01_hsensor.c \
../Drivers/BSP/B-L475E-IOT01/stm32l475e_iot01_magneto.c \
../Drivers/BSP/B-L475E-IOT01/stm32l475e_iot01_psensor.c \
../Drivers/BSP/B-L475E-IOT01/stm32l475e_iot01_qspi.c \
../Drivers/BSP/B-L475E-IOT01/stm32l475e_iot01_tsensor.c 

OBJS += \
./Drivers/BSP/B-L475E-IOT01/stm32l475e_iot01.o \
./Drivers/BSP/B-L475E-IOT01/stm32l475e_iot01_accelero.o \
./Drivers/BSP/B-L475E-IOT01/stm32l475e_iot01_gyro.o \
./Drivers/BSP/B-L475E-IOT01/stm32l475e_iot01_hsensor.o \
./Drivers/BSP/B-L475E-IOT01/stm32l475e_iot01_magneto.o \
./Drivers/BSP/B-L475E-IOT01/stm32l475e_iot01_psensor.o \
./Drivers/BSP/B-L475E-IOT01/stm32l475e_iot01_qspi.o \
./Drivers/BSP/B-L475E-IOT01/stm32l475e_iot01_tsensor.o 

C_DEPS += \
./Drivers/BSP/B-L475E-IOT01/stm32l475e_iot01.d \
./Drivers/BSP/B-L475E-IOT01/stm32l475e_iot01_accelero.d \
./Drivers/BSP/B-L475E-IOT01/stm32l475e_iot01_gyro.d \
./Drivers/BSP/B-L475E-IOT01/stm32l475e_iot01_hsensor.d \
./Drivers/BSP/B-L475E-IOT01/stm32l475e_iot01_magneto.d \
./Drivers/BSP/B-L475E-IOT01/stm32l475e_iot01_psensor.d \
./Drivers/BSP/B-L475E-IOT01/stm32l475e_iot01_qspi.d \
./Drivers/BSP/B-L475E-IOT01/stm32l475e_iot01_tsensor.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/BSP/B-L475E-IOT01/stm32l475e_iot01.o: ../Drivers/BSP/B-L475E-IOT01/stm32l475e_iot01.c Drivers/BSP/B-L475E-IOT01/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L475xx -c -I../X-CUBE-MEMS1/Target -I../Drivers/BSP/B-L475E-IOT01A1 -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -I../Drivers/BSP/Components/hts221 -I"C:/ST/STM32CubeIDE_1.6.1/STM32CubeIDE/Projects/UCSD-Embedded-HW-Interfacing-SPI-Nathan-Bunnell/Drivers/BSP/B-L475E-IOT01" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Drivers/BSP/B-L475E-IOT01/stm32l475e_iot01.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Drivers/BSP/B-L475E-IOT01/stm32l475e_iot01_accelero.o: ../Drivers/BSP/B-L475E-IOT01/stm32l475e_iot01_accelero.c Drivers/BSP/B-L475E-IOT01/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L475xx -c -I../X-CUBE-MEMS1/Target -I../Drivers/BSP/B-L475E-IOT01A1 -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -I../Drivers/BSP/Components/hts221 -I"C:/ST/STM32CubeIDE_1.6.1/STM32CubeIDE/Projects/UCSD-Embedded-HW-Interfacing-SPI-Nathan-Bunnell/Drivers/BSP/B-L475E-IOT01" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Drivers/BSP/B-L475E-IOT01/stm32l475e_iot01_accelero.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Drivers/BSP/B-L475E-IOT01/stm32l475e_iot01_gyro.o: ../Drivers/BSP/B-L475E-IOT01/stm32l475e_iot01_gyro.c Drivers/BSP/B-L475E-IOT01/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L475xx -c -I../X-CUBE-MEMS1/Target -I../Drivers/BSP/B-L475E-IOT01A1 -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -I../Drivers/BSP/Components/hts221 -I"C:/ST/STM32CubeIDE_1.6.1/STM32CubeIDE/Projects/UCSD-Embedded-HW-Interfacing-SPI-Nathan-Bunnell/Drivers/BSP/B-L475E-IOT01" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Drivers/BSP/B-L475E-IOT01/stm32l475e_iot01_gyro.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Drivers/BSP/B-L475E-IOT01/stm32l475e_iot01_hsensor.o: ../Drivers/BSP/B-L475E-IOT01/stm32l475e_iot01_hsensor.c Drivers/BSP/B-L475E-IOT01/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L475xx -c -I../X-CUBE-MEMS1/Target -I../Drivers/BSP/B-L475E-IOT01A1 -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -I../Drivers/BSP/Components/hts221 -I"C:/ST/STM32CubeIDE_1.6.1/STM32CubeIDE/Projects/UCSD-Embedded-HW-Interfacing-SPI-Nathan-Bunnell/Drivers/BSP/B-L475E-IOT01" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Drivers/BSP/B-L475E-IOT01/stm32l475e_iot01_hsensor.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Drivers/BSP/B-L475E-IOT01/stm32l475e_iot01_magneto.o: ../Drivers/BSP/B-L475E-IOT01/stm32l475e_iot01_magneto.c Drivers/BSP/B-L475E-IOT01/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L475xx -c -I../X-CUBE-MEMS1/Target -I../Drivers/BSP/B-L475E-IOT01A1 -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -I../Drivers/BSP/Components/hts221 -I"C:/ST/STM32CubeIDE_1.6.1/STM32CubeIDE/Projects/UCSD-Embedded-HW-Interfacing-SPI-Nathan-Bunnell/Drivers/BSP/B-L475E-IOT01" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Drivers/BSP/B-L475E-IOT01/stm32l475e_iot01_magneto.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Drivers/BSP/B-L475E-IOT01/stm32l475e_iot01_psensor.o: ../Drivers/BSP/B-L475E-IOT01/stm32l475e_iot01_psensor.c Drivers/BSP/B-L475E-IOT01/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L475xx -c -I../X-CUBE-MEMS1/Target -I../Drivers/BSP/B-L475E-IOT01A1 -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -I../Drivers/BSP/Components/hts221 -I"C:/ST/STM32CubeIDE_1.6.1/STM32CubeIDE/Projects/UCSD-Embedded-HW-Interfacing-SPI-Nathan-Bunnell/Drivers/BSP/B-L475E-IOT01" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Drivers/BSP/B-L475E-IOT01/stm32l475e_iot01_psensor.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Drivers/BSP/B-L475E-IOT01/stm32l475e_iot01_qspi.o: ../Drivers/BSP/B-L475E-IOT01/stm32l475e_iot01_qspi.c Drivers/BSP/B-L475E-IOT01/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L475xx -c -I../X-CUBE-MEMS1/Target -I../Drivers/BSP/B-L475E-IOT01A1 -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -I../Drivers/BSP/Components/hts221 -I"C:/ST/STM32CubeIDE_1.6.1/STM32CubeIDE/Projects/UCSD-Embedded-HW-Interfacing-SPI-Nathan-Bunnell/Drivers/BSP/B-L475E-IOT01" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Drivers/BSP/B-L475E-IOT01/stm32l475e_iot01_qspi.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Drivers/BSP/B-L475E-IOT01/stm32l475e_iot01_tsensor.o: ../Drivers/BSP/B-L475E-IOT01/stm32l475e_iot01_tsensor.c Drivers/BSP/B-L475E-IOT01/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L475xx -c -I../X-CUBE-MEMS1/Target -I../Drivers/BSP/B-L475E-IOT01A1 -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -I../Drivers/BSP/Components/hts221 -I"C:/ST/STM32CubeIDE_1.6.1/STM32CubeIDE/Projects/UCSD-Embedded-HW-Interfacing-SPI-Nathan-Bunnell/Drivers/BSP/B-L475E-IOT01" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Drivers/BSP/B-L475E-IOT01/stm32l475e_iot01_tsensor.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
