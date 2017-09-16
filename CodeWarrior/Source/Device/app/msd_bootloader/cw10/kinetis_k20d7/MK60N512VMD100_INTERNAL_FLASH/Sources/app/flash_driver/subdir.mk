################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"$(APP)/msd_bootloader/flash_driver/flash_FTFL.c" \

C_SRCS += \
$(APP)/msd_bootloader/flash_driver/flash_FTFL.c \

OBJS += \
./Sources/app/flash_driver/flash_FTFL_c.obj \

OBJS_QUOTED += \
"./Sources/app/flash_driver/flash_FTFL_c.obj" \

C_DEPS += \
./Sources/app/flash_driver/flash_FTFL_c.d \

C_DEPS_QUOTED += \
"./Sources/app/flash_driver/flash_FTFL_c.d" \

OBJS_OS_FORMAT += \
./Sources/app/flash_driver/flash_FTFL_c.obj \


# Each subdirectory must supply rules for building sources it contributes
Sources/app/flash_driver/flash_FTFL_c.obj: $(APP)/msd_bootloader/flash_driver/flash_FTFL.c
	@echo 'Building file: $<'
	@echo 'Executing target #18 $<'
	@echo 'Invoking: ARM Compiler'
	"$(ARM_ToolsDirEnv)/mwccarm" -gccinc @@"Sources/app/flash_driver/flash_FTFL.args" -o "Sources/app/flash_driver/flash_FTFL_c.obj" -c "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/app/flash_driver/flash_FTFL_c.d: $(APP)/msd_bootloader/flash_driver/flash_FTFL.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '


