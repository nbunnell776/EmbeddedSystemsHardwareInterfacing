################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include NFC/WriteTag/Src/subdir.mk
-include NFC/WriteTag/SW4STM32/subdir.mk
-include NFC/Common/NDEF_TagType4_lib/subdir.mk
-include NFC/Common/M24SR/subdir.mk
-include Drivers/STM32L4xx_HAL_Driver/Src/subdir.mk
-include BSP/Components/wm8994/subdir.mk
-include BSP/Components/stmpe811/subdir.mk
-include BSP/Components/stmpe1600/subdir.mk
-include BSP/Components/st7789h2/subdir.mk
-include BSP/Components/st7735/subdir.mk
-include BSP/Components/st25dv/subdir.mk
-include BSP/Components/ov9655/subdir.mk
-include BSP/Components/mfxstm32l152/subdir.mk
-include BSP/Components/m24sr/subdir.mk
-include BSP/Components/lsm6dsl/subdir.mk
-include BSP/Components/lsm303dlhc/subdir.mk
-include BSP/Components/lsm303c/subdir.mk
-include BSP/Components/ls016b8uy/subdir.mk
-include BSP/Components/lps22hb/subdir.mk
-include BSP/Components/lis3mdl/subdir.mk
-include BSP/Components/l3gd20/subdir.mk
-include BSP/Components/hx8347i/subdir.mk
-include BSP/Components/hx8347g/subdir.mk
-include BSP/Components/hts221/subdir.mk
-include BSP/Components/ft6x06/subdir.mk
-include BSP/Components/ft5336/subdir.mk
-include BSP/Components/ft3x67/subdir.mk
-include BSP/Components/cy8c4014lqi/subdir.mk
-include BSP/Components/cs43l22/subdir.mk
-include BSP/Components/cs42l51/subdir.mk
-include BSP/B-L475E-IOT01/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := UCSD-Embedded-HW-Interfacing-NFC-Nathan-Bunnell
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX := 
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME).$(BUILD_ARTIFACT_EXTENSION)

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
UCSD-Embedded-HW-Interfacing-NFC-Nathan-Bunnell.elf \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
UCSD-Embedded-HW-Interfacing-NFC-Nathan-Bunnell.list \

OBJCOPY_BIN += \
UCSD-Embedded-HW-Interfacing-NFC-Nathan-Bunnell.bin \


# All Target
all: main-build

# Main-build Target
main-build: UCSD-Embedded-HW-Interfacing-NFC-Nathan-Bunnell.elf secondary-outputs

# Tool invocations
UCSD-Embedded-HW-Interfacing-NFC-Nathan-Bunnell.elf: $(OBJS) $(USER_OBJS) C:\ST\STM32CubeIDE_1.6.1\STM32CubeIDE\Projects\UCSD-Embedded-HW-Interfacing-NFC-Nathan-Bunnell\STM32L475VGTX_FLASH.ld makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-gcc -o "UCSD-Embedded-HW-Interfacing-NFC-Nathan-Bunnell.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m4 -T"C:\ST\STM32CubeIDE_1.6.1\STM32CubeIDE\Projects\UCSD-Embedded-HW-Interfacing-NFC-Nathan-Bunnell\STM32L475VGTX_FLASH.ld" --specs=nosys.specs -Wl,-Map="UCSD-Embedded-HW-Interfacing-NFC-Nathan-Bunnell.map" -Wl,--gc-sections -static --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

UCSD-Embedded-HW-Interfacing-NFC-Nathan-Bunnell.list: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "UCSD-Embedded-HW-Interfacing-NFC-Nathan-Bunnell.list"
	@echo 'Finished building: $@'
	@echo ' '

UCSD-Embedded-HW-Interfacing-NFC-Nathan-Bunnell.bin: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objcopy  -O binary $(EXECUTABLES) "UCSD-Embedded-HW-Interfacing-NFC-Nathan-Bunnell.bin"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) *
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_BIN)

fail-specified-linker-script-missing:
	@echo 'Error: Cannot find the specified linker script. Check the linker settings in the build configuration.'
	@exit 2

warn-no-linker-script-specified:
	@echo 'Warning: No linker script specified. Check the linker settings in the build configuration.'

.PHONY: all clean dependents fail-specified-linker-script-missing warn-no-linker-script-specified
.SECONDARY:

-include ../makefile.targets
