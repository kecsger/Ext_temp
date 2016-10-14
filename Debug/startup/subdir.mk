################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../startup/startup_stm32.s 

C_SRCS += \
../startup/sysmem.c 

OBJS += \
./startup/startup_stm32.o \
./startup/sysmem.o 

C_DEPS += \
./startup/sysmem.d 


# Each subdirectory must supply rules for building sources it contributes
startup/%.o: ../startup/%.s
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Assembler'
	@echo %cd%
	arm-none-eabi-as -mcpu=cortex-m3 -mthumb -mfloat-abi=soft -I"C:/Users/kecsger12/workspace/Ext_temp/inc" -I"C:/Users/kecsger12/workspace/Ext_temp/CMSIS/core" -I"C:/Users/kecsger12/workspace/Ext_temp/CMSIS/device" -I"C:/Users/kecsger12/workspace/Ext_temp/StdPeriph_Driver/inc" -g -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

startup/%.o: ../startup/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo %cd%
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -mfloat-abi=soft -DSTM32L151C6TxA -DSTM32L1 -DSTM32 -DDEBUG -DUSE_STDPERIPH_DRIVER -I"C:/Users/kecsger12/workspace/Ext_temp/inc" -I"C:/Users/kecsger12/workspace/Ext_temp/CMSIS/core" -I"C:/Users/kecsger12/workspace/Ext_temp/CMSIS/device" -I"C:/Users/kecsger12/workspace/Ext_temp/StdPeriph_Driver/inc" -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


