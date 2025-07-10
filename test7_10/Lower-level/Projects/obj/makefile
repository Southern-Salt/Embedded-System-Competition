################################################################################
# MRS Version: 2.1.0
# Automatically-generated file. Do not edit!
################################################################################
-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include User/subdir.mk
-include Drivers/SYSTEM/usart/subdir.mk
-include Drivers/SYSTEM/sys/subdir.mk
-include Drivers/SYSTEM/delay/subdir.mk
-include Drivers/RISC_V/Startup/subdir.mk
-include Drivers/RISC_V/Core/subdir.mk
-include Drivers/CH32V30x_Driver/src/subdir.mk
-include Drivers/BSP/TaoJC/subdir.mk
-include Drivers/BSP/Motor/subdir.mk
-include Drivers/BSP/LED/subdir.mk
-include Drivers/BSP/ESP12F/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
SECONDARY_FLASH += \
CH32V307VCT6-USART.hex \

SECONDARY_LIST += \
CH32V307VCT6-USART.lst \

SECONDARY_SIZE += \
CH32V307VCT6-USART.siz \


# All Target
all: 
	$(MAKE) --no-print-directory main-build 

main-build: CH32V307VCT6-USART.elf secondary-outputs

# Tool invocations
CH32V307VCT6-USART.elf: $(OBJS) $(USER_OBJS)
	@	riscv-none-embed-gcc -march=rv32imacxw -mabi=ilp32 -msmall-data-limit=8 -msave-restore -fmax-errors=20 -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized -g -T "e:/CH32_Project/六月/串口屏实验 -六月二十九 - 副本/Projects/Ld/Link.ld" -nostartfiles -Xlinker --gc-sections -Wl,-Map,"CH32V307VCT6-USART.map" --specs=nano.specs --specs=nosys.specs -o "CH32V307VCT6-USART.elf" $(OBJS) $(USER_OBJS) $(LIBS) -lprintfloat
CH32V307VCT6-USART.hex: CH32V307VCT6-USART.elf
	@	riscv-none-embed-objcopy -O ihex "CH32V307VCT6-USART.elf" "CH32V307VCT6-USART.hex"
CH32V307VCT6-USART.lst: CH32V307VCT6-USART.elf
	@	riscv-none-embed-objdump --all-headers --demangle --disassemble -M xw "CH32V307VCT6-USART.elf" > "CH32V307VCT6-USART.lst"
CH32V307VCT6-USART.siz: CH32V307VCT6-USART.elf
	riscv-none-embed-size --format=berkeley "CH32V307VCT6-USART.elf"

# Other Targets
clean:
	-$(RM) $(OBJS)$(SECONDARY_FLASH)$(SECONDARY_LIST)$(SECONDARY_SIZE)$(S_DEPS)$(S_UPPER_DEPS)$(ASM_DEPS)$(ASM_UPPER_DEPS)$(C_DEPS) CH32V307VCT6-USART.elf

secondary-outputs: $(SECONDARY_FLASH) $(SECONDARY_LIST) $(SECONDARY_SIZE)

.PHONY: all clean dependents

-include ../makefile.targets