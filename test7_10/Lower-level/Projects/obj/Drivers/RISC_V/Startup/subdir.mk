################################################################################
# MRS Version: 2.1.0
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_UPPER_SRCS += \
e:/CH32_Project/六月/串口屏实验\ -六月二十九\ -\ 副本/Drivers/RISC_V/Startup/startup_ch32v30x_D8C.S 

S_UPPER_DEPS += \
./Drivers/RISC_V/Startup/startup_ch32v30x_D8C.d 

OBJS += \
./Drivers/RISC_V/Startup/startup_ch32v30x_D8C.o 



# Each subdirectory must supply rules for building sources it contributes
Drivers/RISC_V/Startup/startup_ch32v30x_D8C.o: e:/CH32_Project/六月/串口屏实验\ -六月二十九\ -\ 副本/Drivers/RISC_V/Startup/startup_ch32v30x_D8C.S
	@	riscv-none-embed-gcc -march=rv32imacxw -mabi=ilp32 -msmall-data-limit=8 -msave-restore -fmax-errors=20 -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized -g -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"

