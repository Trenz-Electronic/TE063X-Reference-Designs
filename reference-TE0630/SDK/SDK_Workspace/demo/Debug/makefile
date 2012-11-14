################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include src/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
ELFSIZE += \
demo.elf.size \

ELFCHECK += \
demo.elf.elfcheck \


# All Target
all: demo.elf secondary-outputs

# Tool invocations
demo.elf: $(OBJS) ../src/lscript.ld $(USER_OBJS)
	@echo Building target: $@
	@echo Invoking: MicroBlaze gcc linker
	mb-gcc -Wl,-T -Wl,../src/lscript.ld -L../../standalone_bsp_0/microblaze_0/lib -mxl-pattern-compare -mcpu=v7.30.b -mno-xl-soft-mul -o"demo.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo Finished building target: $@
	@echo ' '

demo.elf.size: demo.elf
	@echo Invoking: MicroBlaze Print Size
	mb-size demo.elf  |tee "demo.elf.size"
	@echo Finished building: $@
	@echo ' '

demo.elf.elfcheck: demo.elf
	@echo Invoking: Xilinx ELF Check
	elfcheck demo.elf -hw ../../reference-TE0300_hw_platform/system.xml -pe microblaze_0  |tee "demo.elf.elfcheck"
	@echo Finished building: $@
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(EXECUTABLES)$(ELFSIZE)$(ELFCHECK)$(S_UPPER_DEPS) demo.elf
	-@echo ' '

secondary-outputs: $(ELFSIZE) $(ELFCHECK)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
