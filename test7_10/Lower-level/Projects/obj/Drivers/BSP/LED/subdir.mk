################################################################################
# MRS Version: 2.1.0
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
e:/CH32_Project/六月/串口屏实验\ -六月二十九\ -\ 副本/Drivers/BSP/LED/led.c 

C_DEPS += \
./Drivers/BSP/LED/led.d 

OBJS += \
./Drivers/BSP/LED/led.o 



# Each subdirectory must supply rules for building sources it contributes
Drivers/BSP/LED/led.o: e:/CH32_Project/六月/串口屏实验\ -六月二十九\ -\ 副本/Drivers/BSP/LED/led.c
	@	riscv-none-embed-gcc -march=rv32imacxw -mabi=ilp32 -msmall-data-limit=8 -msave-restore -fmax-errors=20 -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized -g -I"e:/CH32_Project/六月/串口屏实验 -六月二十九 - 副本/Drivers" -I"e:/CH32_Project/六月/串口屏实验 -六月二十九 - 副本/Drivers/CH32V30x_Driver/inc" -I"e:/CH32_Project/六月/串口屏实验 -六月二十九 - 副本/Drivers/RISC_V/Core" -I"e:/CH32_Project/六月/串口屏实验 -六月二十九 - 副本/Middlewares" -I"e:/CH32_Project/六月/串口屏实验 -六月二十九 - 副本/User" -I"e:/CH32_Project/六月/串口屏实验 -六月二十九 - 副本/Drivers/SYSTEM" -I"e:/CH32_Project/六月/串口屏实验 -六月二十九 - 副本/Drivers/BSP" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"

