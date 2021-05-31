################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../BSP/Components/hx8347i/hx8347i.c 

OBJS += \
./BSP/Components/hx8347i/hx8347i.o 

C_DEPS += \
./BSP/Components/hx8347i/hx8347i.d 


# Each subdirectory must supply rules for building sources it contributes
BSP/Components/hx8347i/hx8347i.o: ../BSP/Components/hx8347i/hx8347i.c BSP/Components/hx8347i/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DBLUENRG_MS -DUSE_HAL_DRIVER -DSTM32L475xx -c -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -I"C:/ST/STM32CubeIDE_1.6.1/STM32CubeIDE/Projects/UCSD-Embedded-HW-Interfacing-BLE-Nathan-Bunnell/BLE/Common" -I"C:/ST/STM32CubeIDE_1.6.1/STM32CubeIDE/Projects/UCSD-Embedded-HW-Interfacing-BLE-Nathan-Bunnell/BLE/Common/ble_core" -I"C:/ST/STM32CubeIDE_1.6.1/STM32CubeIDE/Projects/UCSD-Embedded-HW-Interfacing-BLE-Nathan-Bunnell/BLE/Common/ble_services" -I"C:/ST/STM32CubeIDE_1.6.1/STM32CubeIDE/Projects/UCSD-Embedded-HW-Interfacing-BLE-Nathan-Bunnell/BLE/Common/debug" -I"C:/ST/STM32CubeIDE_1.6.1/STM32CubeIDE/Projects/UCSD-Embedded-HW-Interfacing-BLE-Nathan-Bunnell/BLE/Common/hw" -I"C:/ST/STM32CubeIDE_1.6.1/STM32CubeIDE/Projects/UCSD-Embedded-HW-Interfacing-BLE-Nathan-Bunnell/BLE/Common/tl" -I"C:/ST/STM32CubeIDE_1.6.1/STM32CubeIDE/Projects/UCSD-Embedded-HW-Interfacing-BLE-Nathan-Bunnell/BLE/Common/utilities" -I"C:/ST/STM32CubeIDE_1.6.1/STM32CubeIDE/Projects/UCSD-Embedded-HW-Interfacing-BLE-Nathan-Bunnell/BLE/P2P_LedButton/Inc" -I"C:/ST/STM32CubeIDE_1.6.1/STM32CubeIDE/Projects/UCSD-Embedded-HW-Interfacing-BLE-Nathan-Bunnell/BSP/B-L475E-IOT01" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"BSP/Components/hx8347i/hx8347i.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

