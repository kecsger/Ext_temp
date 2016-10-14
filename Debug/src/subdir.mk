################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/BatSense.c \
../src/MCP9804.c \
../src/MIDAS.c \
../src/RFM73.c \
../src/i2c.c \
../src/led.c \
../src/main.c \
../src/spi.c \
../src/system_stm32l1xx.c \
../src/timer.c 

OBJS += \
./src/BatSense.o \
./src/MCP9804.o \
./src/MIDAS.o \
./src/RFM73.o \
./src/i2c.o \
./src/led.o \
./src/main.o \
./src/spi.o \
./src/system_stm32l1xx.o \
./src/timer.o 

C_DEPS += \
./src/BatSense.d \
./src/MCP9804.d \
./src/MIDAS.d \
./src/RFM73.d \
./src/i2c.d \
./src/led.d \
./src/main.d \
./src/spi.d \
./src/system_stm32l1xx.d \
./src/timer.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo %cd%
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -mfloat-abi=soft -DSTM32L151C6TxA -DSTM32L1 -DSTM32 -DDEBUG -DUSE_STDPERIPH_DRIVER -I"C:/Users/kecsger12/workspace/Ext_temp/inc" -I"C:/Users/kecsger12/workspace/Ext_temp/CMSIS/core" -I"C:/Users/kecsger12/workspace/Ext_temp/CMSIS/device" -I"C:/Users/kecsger12/workspace/Ext_temp/StdPeriph_Driver/inc" -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


