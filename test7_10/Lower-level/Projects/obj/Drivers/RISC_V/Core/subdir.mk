################################################################################
# MRS Version: 2.1.0
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
e:/CH32_Project/����/������ʵ��\ -���¶�ʮ��\ -\ ����/Drivers/RISC_V/Core/core_riscv.c 

C_DEPS += \
./Drivers/RISC_V/Core/core_riscv.d 

OBJS += \
./Drivers/RISC_V/Core/core_riscv.o 



# Each subdirectory must supply rules for building sources it contributes
Drivers/RISC_V/Core/core_riscv.o: e:/CH32_Project/����/������ʵ��\ -���¶�ʮ��\ -\ ����/Drivers/RISC_V/Core/core_riscv.c
	@	riscv-none-embed-gcc -march=rv32imacxw -mabi=ilp32 -msmall-data-limit=8 -msave-restore -fmax-errors=20 -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized -g -I"e:/CH32_Project/����/������ʵ�� -���¶�ʮ�� - ����/Drivers" -I"e:/CH32_Project/����/������ʵ�� -���¶�ʮ�� - ����/Drivers/CH32V30x_Driver/inc" -I"e:/CH32_Project/����/������ʵ�� -���¶�ʮ�� - ����/Drivers/RISC_V/Core" -I"e:/CH32_Project/����/������ʵ�� -���¶�ʮ�� - ����/Middlewares" -I"e:/CH32_Project/����/������ʵ�� -���¶�ʮ�� - ����/User" -I"e:/CH32_Project/����/������ʵ�� -���¶�ʮ�� - ����/Drivers/SYSTEM" -I"e:/CH32_Project/����/������ʵ�� -���¶�ʮ�� - ����/Drivers/BSP" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"

