################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"D:/Temp/USB_MSD_Device_bootloader/Application_Examples/TWR-MCF5225X_PTC0/Startup_Code/startcf.c" \

C_SRCS += \
D:/Temp/USB_MSD_Device_bootloader/Application_Examples/TWR-MCF5225X_PTC0/Startup_Code/startcf.c \

OBJS += \
./Startup_Code/startcf_c.obj \

OBJS_QUOTED += \
"./Startup_Code/startcf_c.obj" \

C_DEPS += \
./Startup_Code/startcf_c.d \

OBJS_OS_FORMAT += \
./Startup_Code/startcf_c.obj \


# Each subdirectory must supply rules for building sources it contributes
Startup_Code/startcf_c.obj: D:/Temp/USB_MSD_Device_bootloader/Application_Examples/TWR-MCF5225X_PTC0/Startup_Code/startcf.c
	@echo 'Building file: $<'
	@echo 'Invoking: ColdFire Compiler'
	"$(CF_ToolsDirEnv)/mwccmcf" @@"Startup_Code/startcf.args" -o "Startup_Code/startcf_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Startup_Code/startcf_c.d: D:/Temp/USB_MSD_Device_bootloader/Application_Examples/TWR-MCF5225X_PTC0/Startup_Code/startcf.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '


