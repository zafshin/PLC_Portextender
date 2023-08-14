################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/adc.c \
../Core/Src/crc.c \
../Core/Src/gpio.c \
../Core/Src/i2c.c \
../Core/Src/socket.c \
../Core/Src/spi.c \
../Core/Src/stm32f1xx_hal_msp.c \
../Core/Src/stm32f1xx_it.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c \
../Core/Src/system_stm32f1xx.c \
../Core/Src/usart.c \
../Core/Src/w5500.c \
../Core/Src/wizchip_conf.c 

CPP_SRCS += \
../Core/Src/app.cpp \
../Core/Src/eflash.cpp \
../Core/Src/httpServer.cpp \
../Core/Src/main.cpp \
../Core/Src/modbus.cpp \
../Core/Src/mosbus2serial.cpp \
../Core/Src/tcpsocket.cpp \
../Core/Src/uservice.cpp 

C_DEPS += \
./Core/Src/adc.d \
./Core/Src/crc.d \
./Core/Src/gpio.d \
./Core/Src/i2c.d \
./Core/Src/socket.d \
./Core/Src/spi.d \
./Core/Src/stm32f1xx_hal_msp.d \
./Core/Src/stm32f1xx_it.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d \
./Core/Src/system_stm32f1xx.d \
./Core/Src/usart.d \
./Core/Src/w5500.d \
./Core/Src/wizchip_conf.d 

OBJS += \
./Core/Src/adc.o \
./Core/Src/app.o \
./Core/Src/crc.o \
./Core/Src/eflash.o \
./Core/Src/gpio.o \
./Core/Src/httpServer.o \
./Core/Src/i2c.o \
./Core/Src/main.o \
./Core/Src/modbus.o \
./Core/Src/mosbus2serial.o \
./Core/Src/socket.o \
./Core/Src/spi.o \
./Core/Src/stm32f1xx_hal_msp.o \
./Core/Src/stm32f1xx_it.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/system_stm32f1xx.o \
./Core/Src/tcpsocket.o \
./Core/Src/usart.o \
./Core/Src/uservice.o \
./Core/Src/w5500.o \
./Core/Src/wizchip_conf.o 

CPP_DEPS += \
./Core/Src/app.d \
./Core/Src/eflash.d \
./Core/Src/httpServer.d \
./Core/Src/main.d \
./Core/Src/modbus.d \
./Core/Src/mosbus2serial.d \
./Core/Src/tcpsocket.d \
./Core/Src/uservice.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o Core/Src/%.su Core/Src/%.cyclo: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xE -DSTM32_THREAD_SAFE_STRATEGY=2 -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../Core/ThreadSafe -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
Core/Src/%.o Core/Src/%.su Core/Src/%.cyclo: ../Core/Src/%.cpp Core/Src/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m3 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xE -DSTM32_THREAD_SAFE_STRATEGY=2 -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../Core/ThreadSafe -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/adc.cyclo ./Core/Src/adc.d ./Core/Src/adc.o ./Core/Src/adc.su ./Core/Src/app.cyclo ./Core/Src/app.d ./Core/Src/app.o ./Core/Src/app.su ./Core/Src/crc.cyclo ./Core/Src/crc.d ./Core/Src/crc.o ./Core/Src/crc.su ./Core/Src/eflash.cyclo ./Core/Src/eflash.d ./Core/Src/eflash.o ./Core/Src/eflash.su ./Core/Src/gpio.cyclo ./Core/Src/gpio.d ./Core/Src/gpio.o ./Core/Src/gpio.su ./Core/Src/httpServer.cyclo ./Core/Src/httpServer.d ./Core/Src/httpServer.o ./Core/Src/httpServer.su ./Core/Src/i2c.cyclo ./Core/Src/i2c.d ./Core/Src/i2c.o ./Core/Src/i2c.su ./Core/Src/main.cyclo ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/main.su ./Core/Src/modbus.cyclo ./Core/Src/modbus.d ./Core/Src/modbus.o ./Core/Src/modbus.su ./Core/Src/mosbus2serial.cyclo ./Core/Src/mosbus2serial.d ./Core/Src/mosbus2serial.o ./Core/Src/mosbus2serial.su ./Core/Src/socket.cyclo ./Core/Src/socket.d ./Core/Src/socket.o ./Core/Src/socket.su ./Core/Src/spi.cyclo ./Core/Src/spi.d ./Core/Src/spi.o ./Core/Src/spi.su ./Core/Src/stm32f1xx_hal_msp.cyclo ./Core/Src/stm32f1xx_hal_msp.d ./Core/Src/stm32f1xx_hal_msp.o ./Core/Src/stm32f1xx_hal_msp.su ./Core/Src/stm32f1xx_it.cyclo ./Core/Src/stm32f1xx_it.d ./Core/Src/stm32f1xx_it.o ./Core/Src/stm32f1xx_it.su ./Core/Src/syscalls.cyclo ./Core/Src/syscalls.d ./Core/Src/syscalls.o ./Core/Src/syscalls.su ./Core/Src/sysmem.cyclo ./Core/Src/sysmem.d ./Core/Src/sysmem.o ./Core/Src/sysmem.su ./Core/Src/system_stm32f1xx.cyclo ./Core/Src/system_stm32f1xx.d ./Core/Src/system_stm32f1xx.o ./Core/Src/system_stm32f1xx.su ./Core/Src/tcpsocket.cyclo ./Core/Src/tcpsocket.d ./Core/Src/tcpsocket.o ./Core/Src/tcpsocket.su ./Core/Src/usart.cyclo ./Core/Src/usart.d ./Core/Src/usart.o ./Core/Src/usart.su ./Core/Src/uservice.cyclo ./Core/Src/uservice.d ./Core/Src/uservice.o ./Core/Src/uservice.su ./Core/Src/w5500.cyclo ./Core/Src/w5500.d ./Core/Src/w5500.o ./Core/Src/w5500.su ./Core/Src/wizchip_conf.cyclo ./Core/Src/wizchip_conf.d ./Core/Src/wizchip_conf.o ./Core/Src/wizchip_conf.su

.PHONY: clean-Core-2f-Src

