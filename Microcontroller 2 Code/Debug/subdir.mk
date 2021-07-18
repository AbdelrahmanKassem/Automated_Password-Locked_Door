################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../buzzer.c \
../external_eeprom.c \
../i2c.c \
../mc2_application.c \
../motor.c \
../timers.c \
../uart.c 

OBJS += \
./buzzer.o \
./external_eeprom.o \
./i2c.o \
./mc2_application.o \
./motor.o \
./timers.o \
./uart.o 

C_DEPS += \
./buzzer.d \
./external_eeprom.d \
./i2c.d \
./mc2_application.d \
./motor.d \
./timers.d \
./uart.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega16 -DF_CPU=8000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


