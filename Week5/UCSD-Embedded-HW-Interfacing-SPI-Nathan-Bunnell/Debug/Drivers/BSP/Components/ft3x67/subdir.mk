################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/BSP/Components/ft3x67/ft3x67.c 

OBJS += \
./Drivers/BSP/Components/ft3x67/ft3x67.o 

C_DEPS += \
./Drivers/BSP/Components/ft3x67/ft3x67.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/BSP/Components/ft3x67/ft3x67.o: ../Drivers/BSP/Components/ft3x67/ft3x67.c Drivers/BSP/Components/ft3x67/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L475xx -c -I../X-CUBE-MEMS1/Target -I../Drivers/BSP/B-L475E-IOT01A1 -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -I../Drivers/BSP/Components/hts221 -I"C:/ST/STM32CubeIDE_1.6.1/STM32CubeIDE/Projects/UCSD-Embedded-HW-Interfacing-SPI-Nathan-Bunnell/Drivers/BSP/B-L475E-IOT01" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Drivers/BSP/Components/ft3x67/ft3x67.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

