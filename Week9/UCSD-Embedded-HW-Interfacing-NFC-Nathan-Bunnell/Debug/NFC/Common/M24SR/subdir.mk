################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../NFC/Common/M24SR/m24sr_wrapper.c 

OBJS += \
./NFC/Common/M24SR/m24sr_wrapper.o 

C_DEPS += \
./NFC/Common/M24SR/m24sr_wrapper.d 


# Each subdirectory must supply rules for building sources it contributes
NFC/Common/M24SR/m24sr_wrapper.o: ../NFC/Common/M24SR/m24sr_wrapper.c NFC/Common/M24SR/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L475xx -c -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I"C:/ST/STM32CubeIDE_1.6.1/STM32CubeIDE/Projects/UCSD-Embedded-HW-Interfacing-NFC-Nathan-Bunnell/NFC/Common/M24SR" -I"C:/ST/STM32CubeIDE_1.6.1/STM32CubeIDE/Projects/UCSD-Embedded-HW-Interfacing-NFC-Nathan-Bunnell/NFC/Common/NDEF_TagType4_lib" -I"C:/ST/STM32CubeIDE_1.6.1/STM32CubeIDE/Projects/UCSD-Embedded-HW-Interfacing-NFC-Nathan-Bunnell/NFC/WriteTag/Inc" -I"C:/ST/STM32CubeIDE_1.6.1/STM32CubeIDE/Projects/UCSD-Embedded-HW-Interfacing-NFC-Nathan-Bunnell/BSP/B-L475E-IOT01" -I"C:/ST/STM32CubeIDE_1.6.1/STM32CubeIDE/Projects/UCSD-Embedded-HW-Interfacing-NFC-Nathan-Bunnell/BSP/Components/m24sr" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"NFC/Common/M24SR/m24sr_wrapper.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

