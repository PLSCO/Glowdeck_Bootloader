################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"D:/Temp/USB_MSD_Device_bootloader/Application_Examples/TWR-K60N512_PTA10/Startup_Code/kinetis_sysinit.c" \

C_SRCS += \
D:/Temp/USB_MSD_Device_bootloader/Application_Examples/TWR-K60N512_PTA10/Startup_Code/kinetis_sysinit.c \

OBJS += \
./Startup_Code/kinetis_sysinit.obj \

C_DEPS += \
./Startup_Code/kinetis_sysinit.d \

OBJS_QUOTED += \
"./Startup_Code/kinetis_sysinit.obj" \

OBJS_OS_FORMAT += \
./Startup_Code/kinetis_sysinit.obj \


# Each subdirectory must supply rules for building sources it contributes
Startup_Code/kinetis_sysinit.obj: D:/Temp/USB_MSD_Device_bootloader/Application_Examples/TWR-K60N512_PTA10/Startup_Code/kinetis_sysinit.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"$(ARM_ToolsDirEnv)/mwccarm" -gccinc @@"Startup_Code/kinetis_sysinit.args" -o "Startup_Code/kinetis_sysinit.obj" -c "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Startup_Code/kinetis_sysinit.d: D:/Temp/USB_MSD_Device_bootloader/Application_Examples/TWR-K60N512_PTA10/Startup_Code/kinetis_sysinit.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '


