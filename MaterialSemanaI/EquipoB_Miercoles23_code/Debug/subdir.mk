################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../gauge.c \
../main.c \
../raspi_can_queue.c \
../raspi_candrv.c \
../raspi_gui_ipc.c \
../raspi_timer.c 

OBJS += \
./gauge.o \
./main.o \
./raspi_can_queue.o \
./raspi_candrv.o \
./raspi_gui_ipc.o \
./raspi_timer.o 

C_DEPS += \
./gauge.d \
./main.d \
./raspi_can_queue.d \
./raspi_candrv.d \
./raspi_gui_ipc.d \
./raspi_timer.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-linux-gnueabihf-gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


