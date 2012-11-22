################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/control_fifo.c \
../src/func.c \
../src/interrupts.c \
../src/main.c 

LD_SRCS += \
../src/lscript.ld 

OBJS += \
./src/control_fifo.o \
./src/func.o \
./src/interrupts.o \
./src/main.o 

C_DEPS += \
./src/control_fifo.d \
./src/func.d \
./src/interrupts.d \
./src/main.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo Building file: $<
	@echo Invoking: MicroBlaze gcc compiler
	mb-gcc -Wall -O0 -g3 -c -fmessage-length=0 -I../../standalone_bsp_0/microblaze_0/include -mxl-pattern-compare -mcpu=v7.30.b -mno-xl-soft-mul -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo Finished building: $<
	@echo ' '


