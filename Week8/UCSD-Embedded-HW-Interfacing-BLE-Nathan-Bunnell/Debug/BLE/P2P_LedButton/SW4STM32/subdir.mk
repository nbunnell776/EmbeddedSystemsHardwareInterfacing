################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../BLE/P2P_LedButton/SW4STM32/startup_stm32l475xx.s 

OBJS += \
./BLE/P2P_LedButton/SW4STM32/startup_stm32l475xx.o 

S_DEPS += \
./BLE/P2P_LedButton/SW4STM32/startup_stm32l475xx.d 


# Each subdirectory must supply rules for building sources it contributes
BLE/P2P_LedButton/SW4STM32/startup_stm32l475xx.o: ../BLE/P2P_LedButton/SW4STM32/startup_stm32l475xx.s BLE/P2P_LedButton/SW4STM32/subdir.mk
	arm-none-eabi-gcc -mcpu=cortex-m4 -g3 -c -x assembler-with-cpp -MMD -MP -MF"BLE/P2P_LedButton/SW4STM32/startup_stm32l475xx.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@" "$<"

