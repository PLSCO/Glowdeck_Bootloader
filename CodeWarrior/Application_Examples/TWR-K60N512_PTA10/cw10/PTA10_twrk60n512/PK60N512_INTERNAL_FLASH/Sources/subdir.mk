################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"D:/Temp/USB_MSD_Device_bootloader/Application_Examples/TWR-K60N512_PTA10/Sources/main.c" \

C_SRCS += \
D:/Temp/USB_MSD_Device_bootloader/Application_Examples/TWR-K60N512_PTA10/Sources/main.c \

OBJS += \
./Sources/main.obj \

C_DEPS += \
./Sources/main.d \

OBJS_QUOTED += \
"./Sources/main.obj" \

OBJS_OS_FORMAT += \
./Sources/main.obj \


# Each subdirectory must supply rules for building sources it contributes
Sources/main.obj: D:/Temp/USB_MSD_Device_bootloader/Application_Examples/TWR-K60N512_PTA10/Sources/main.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"$(ARM_ToolsDirEnv)/mwccarm" -gccinc @@"Sources/main.args" -o "Sources/main.obj" -c "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/main.d: D:/Temp/USB_MSD_Device_bootloader/Application_Examples/TWR-K60N512_PTA10/Sources/main.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '


