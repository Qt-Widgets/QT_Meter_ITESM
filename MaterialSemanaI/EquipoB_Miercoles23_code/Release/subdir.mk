################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../raspi_can_queue.c \
../raspi_candrv.c \
../raspi_gui_ipc.c 

OBJS += \
./raspi_can_queue.o \
./raspi_candrv.o \
./raspi_gui_ipc.o 

C_DEPS += \
./raspi_can_queue.d \
./raspi_candrv.d \
./raspi_gui_ipc.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-linux-gnueabihf-gcc -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


