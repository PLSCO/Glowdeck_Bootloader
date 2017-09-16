################################################################################
# Automatically-generated file. Do not edit!
################################################################################

first : all
-include ../makefile.init

# This file contains definitions of environment variables used in the makefiles and .args files if exist.
-include makefile.local

RM := "$(Gnu_Make_Install_DirEnv)/rm" -f

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include Startup_Code/subdir.mk
-include Sources/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
PTC0_twrmcf52259.elf \

EXECUTABLES_QUOTED += \
"PTC0_twrmcf52259.elf" \

EXECUTABLES_OS_FORMAT += \
PTC0_twrmcf52259.elf \

_OUTPUTTYPE_NAME_BURNER_OUTPUT_OUTPUTS += \
PTC0_twrmcf52259.elf.s19 \

_OUTPUTTYPE_NAME_BURNER_OUTPUT_OUTPUTS_QUOTED += \
"PTC0_twrmcf52259.elf.s19" \

_OUTPUTTYPE_NAME_BURNER_OUTPUT_OUTPUTS_OS_FORMAT += \
PTC0_twrmcf52259.elf.s19 \


# All Target
call-burner := 0
ifneq ($(strip $(EXECUTABLES)),)
ifneq ($(strip $(BBL_SRCS_QUOTED)),)
call-burner := 1
endif
endif
ifeq ($(call-burner),1)
all: PTC0_twrmcf52259.elf PTC0_twrmcf52259.elf.s19
else
all: PTC0_twrmcf52259.elf
endif

# Tool invocations
PTC0_twrmcf52259.elf: $(OBJS) $(USER_OBJS) ../Project_Settings/Linker_Files/MCF52259_Internal_Flash.lcf
	@echo 'Building target: $@'
	@echo 'Invoking: ColdFire Linker'
	"$(CF_ToolsDirEnv)/mwldmcf" @@"PTC0_twrmcf52259.args" -o "PTC0_twrmcf52259.elf"  $(USER_OBJS_QUOTED)
	@echo 'Finished building target: $@'
	@echo ' '

PTC0_twrmcf52259.elf.s19: $(BBL_SRCS) $(EXECUTABLES)
	@echo 'Invoking: Burner'
	"$(HC08ToolsEnv)/burner.exe"  -f=$(strip $(BBL_SRCS_QUOTED)) -env"ABS_FILE=$(strip $(EXECUTABLES_OS_FORMAT))"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(_OUTPUTTYPE_NAME_BURNER_OUTPUT_OUTPUTS_OS_FORMAT)$(EXECUTABLES_OS_FORMAT)$(CC_DEPS)$(OBJS_QUOTED)$(CPP_DEPS_QUOTED)$(CC_DEPS_QUOTED)$(_OUTPUTTYPE_NAME_BURNER_OUTPUT_OUTPUTS_QUOTED)$(CPP_DEPS)$(EXECUTABLES)$(C++_DEPS_OS_FORMAT)$(CXX_DEPS_QUOTED)$(C_DEPS)$(_OUTPUTTYPE_NAME_BURNER_OUTPUT_OUTPUTS)$(CXX_DEPS_OS_FORMAT)$(C_UPPER_DEPS_OS_FORMAT)$(CC_DEPS_OS_FORMAT)$(CXX_DEPS)$(C++_DEPS)$(C_UPPER_DEPS_QUOTED)$(CPP_DEPS_OS_FORMAT)$(C++_DEPS_QUOTED)$(C_DEPS_OS_FORMAT)$(EXECUTABLES_QUOTED)$(C_UPPER_DEPS)$(OBJS_OS_FORMAT)$(C_DEPS_QUOTED) PTC0_twrmcf52259.elf
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
