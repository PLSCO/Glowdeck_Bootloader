################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"$(APP)/common/kinetis/kinetis_sysinit.c" \

C_SRCS += \
$(APP)/common/kinetis/kinetis_sysinit.c \

OBJS += \
./Project\ Settings/Startup/kinetis_sysinit_c.obj \

OBJS_QUOTED += \
"./Project Settings/Startup/kinetis_sysinit_c.obj" \

C_DEPS += \
./Project\ Settings/Startup/kinetis_sysinit_c.d \

C_DEPS_QUOTED += \
"./Project Settings/Startup/kinetis_sysinit_c.d" \

OBJS_OS_FORMAT += \
./Project\ Settings/Startup/kinetis_sysinit_c.obj \


# Each subdirectory must supply rules for building sources it contributes
Project\ Settings/Startup/kinetis_sysinit_c.obj: $(APP)/common/kinetis/kinetis_sysinit.c
	@echo 'Building file: $<'
	@echo 'Executing target #19 $<'
	@echo 'Invoking: ARM Compiler'
	"$(ARM_ToolsDirEnv)/mwccarm" -gccinc @@"Project Settings/Startup/kinetis_sysinit.args" -o "Project Settings/Startup/kinetis_sysinit_c.obj" -c "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Project\ Settings/Startup/kinetis_sysinit_c.d: $(APP)/common/kinetis/kinetis_sysinit.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '


