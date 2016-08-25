################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL := cmd.exe
RM := rm -rf

USER_OBJS :=

LIBS := 
PROJ := 

O_SRCS := 
C_SRCS := 
S_SRCS := 
S_UPPER_SRCS := 
OBJ_SRCS := 
ASM_SRCS := 
PREPROCESSING_SRCS := 
OBJS := 
OBJS_AS_ARGS := 
C_DEPS := 
C_DEPS_AS_ARGS := 
EXECUTABLES := 
OUTPUT_FILE_PATH :=
OUTPUT_FILE_PATH_AS_ARGS :=
AVR_APP_PATH :=$$$AVR_APP_PATH$$$
QUOTE := "
ADDITIONAL_DEPENDENCIES:=
OUTPUT_FILE_DEP:=
LIB_DEP:=
LINKER_SCRIPT_DEP:=

# Every subdirectory with source files must be described here
SUBDIRS := 


# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS +=  \
../avr_isr_vectors.c \
../can_application.c \
../can_driver.c \
../can_midleware.c \
../hardware_init.c \
../Indicator_Driver.c \
../main.c \
../print_functions.c \
../schedule_callbacks.c \
../system_callbacks.c \
../task_scheduler.c \
../timers_at90can128.c \
../twi_at90can128.c \
../twi_test.c \
../usart.c \
../watchdog.c


PREPROCESSING_SRCS += 


ASM_SRCS += 


OBJS +=  \
avr_isr_vectors.o \
can_application.o \
can_driver.o \
can_midleware.o \
hardware_init.o \
Indicator_Driver.o \
main.o \
print_functions.o \
schedule_callbacks.o \
system_callbacks.o \
task_scheduler.o \
timers_at90can128.o \
twi_at90can128.o \
twi_test.o \
usart.o \
watchdog.o

OBJS_AS_ARGS +=  \
avr_isr_vectors.o \
can_application.o \
can_driver.o \
can_midleware.o \
hardware_init.o \
Indicator_Driver.o \
main.o \
print_functions.o \
schedule_callbacks.o \
system_callbacks.o \
task_scheduler.o \
timers_at90can128.o \
twi_at90can128.o \
twi_test.o \
usart.o \
watchdog.o

C_DEPS +=  \
avr_isr_vectors.d \
can_application.d \
can_driver.d \
can_midleware.d \
hardware_init.d \
Indicator_Driver.d \
main.d \
print_functions.d \
schedule_callbacks.d \
system_callbacks.d \
task_scheduler.d \
timers_at90can128.d \
twi_at90can128.d \
twi_test.d \
usart.d \
watchdog.d

C_DEPS_AS_ARGS +=  \
avr_isr_vectors.d \
can_application.d \
can_driver.d \
can_midleware.d \
hardware_init.d \
Indicator_Driver.d \
main.d \
print_functions.d \
schedule_callbacks.d \
system_callbacks.d \
task_scheduler.d \
timers_at90can128.d \
twi_at90can128.d \
twi_test.d \
usart.d \
watchdog.d

OUTPUT_FILE_PATH +=avrcan.elf

OUTPUT_FILE_PATH_AS_ARGS +=avrcan.elf

ADDITIONAL_DEPENDENCIES:=

OUTPUT_FILE_DEP:= ./makedep.mk

LIB_DEP+= 

LINKER_SCRIPT_DEP+= 


# AVR32/GNU C Compiler

































./%.o: .././%.c
	@echo Building file: $<
	@echo Invoking: AVR/GNU C Compiler : 4.8.1
	$(QUOTE)C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.1061\avr8-gnu-toolchain\bin\avr-gcc.exe$(QUOTE)  -x c -funsigned-char -funsigned-bitfields -DDEBUG  -I"../../../../../../git/AVRCAN_DEV/Implementation/include"  -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -mrelax -g2 -Wall -mmcu=at90can128 -c -std=gnu99 -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
	@echo Finished building: $<
	



# AVR32/GNU Preprocessing Assembler



# AVR32/GNU Assembler




ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: $(OUTPUT_FILE_PATH) $(ADDITIONAL_DEPENDENCIES)

$(OUTPUT_FILE_PATH): $(OBJS) $(USER_OBJS) $(OUTPUT_FILE_DEP) $(LIB_DEP) $(LINKER_SCRIPT_DEP)
	@echo Building target: $@
	@echo Invoking: AVR/GNU Linker : 4.8.1
	$(QUOTE)C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.1061\avr8-gnu-toolchain\bin\avr-gcc.exe$(QUOTE) -o$(OUTPUT_FILE_PATH_AS_ARGS) $(OBJS_AS_ARGS) $(USER_OBJS) $(LIBS) -Wl,-Map="avrcan.map" -Wl,--start-group -Wl,-lm  -Wl,--end-group -Wl,--gc-sections -mrelax -mmcu=at90can128  
	@echo Finished building target: $@
	"C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.1061\avr8-gnu-toolchain\bin\avr-objcopy.exe" -O ihex -R .eeprom -R .fuse -R .lock -R .signature -R .user_signatures  "avrcan.elf" "avrcan.hex"
	"C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.1061\avr8-gnu-toolchain\bin\avr-objcopy.exe" -j .eeprom  --set-section-flags=.eeprom=alloc,load --change-section-lma .eeprom=0  --no-change-warnings -O ihex "avrcan.elf" "avrcan.eep" || exit 0
	"C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.1061\avr8-gnu-toolchain\bin\avr-objdump.exe" -h -S "avrcan.elf" > "avrcan.lss"
	"C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.1061\avr8-gnu-toolchain\bin\avr-objcopy.exe" -O srec -R .eeprom -R .fuse -R .lock -R .signature -R .user_signatures "avrcan.elf" "avrcan.srec"
	"C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.1061\avr8-gnu-toolchain\bin\avr-size.exe" "avrcan.elf"
	
	





# Other Targets
clean:
	-$(RM) $(OBJS_AS_ARGS) $(EXECUTABLES)  
	-$(RM) $(C_DEPS_AS_ARGS)   
	rm -rf "avrcan.elf" "avrcan.a" "avrcan.hex" "avrcan.lss" "avrcan.eep" "avrcan.map" "avrcan.srec" "avrcan.usersignatures"
	