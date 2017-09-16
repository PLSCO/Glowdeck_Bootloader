################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"D:/Temp/USB_MSD_Device_bootloader/Application_Examples/TWR-MCF5225X_PTC0/Sources/MCF52259_sysinit.c" \
"D:/Temp/USB_MSD_Device_bootloader/Application_Examples/TWR-MCF5225X_PTC0/Sources/cfm.c" \
"D:/Temp/USB_MSD_Device_bootloader/Application_Examples/TWR-MCF5225X_PTC0/Sources/exceptions.c" \
"D:/Temp/USB_MSD_Device_bootloader/Application_Examples/TWR-MCF5225X_PTC0/Sources/main.c" \
"D:/Temp/USB_MSD_Device_bootloader/Application_Examples/TWR-MCF5225X_PTC0/Sources/uart_support.c" \

C_SRCS += \
D:/Temp/USB_MSD_Device_bootloader/Application_Examples/TWR-MCF5225X_PTC0/Sources/MCF52259_sysinit.c \
D:/Temp/USB_MSD_Device_bootloader/Application_Examples/TWR-MCF5225X_PTC0/Sources/cfm.c \
D:/Temp/USB_MSD_Device_bootloader/Application_Examples/TWR-MCF5225X_PTC0/Sources/exceptions.c \
D:/Temp/USB_MSD_Device_bootloader/Application_Examples/TWR-MCF5225X_PTC0/Sources/main.c \
D:/Temp/USB_MSD_Device_bootloader/Application_Examples/TWR-MCF5225X_PTC0/Sources/uart_support.c \

OBJS += \
./Sources/MCF52259_sysinit_c.obj \
./Sources/cfm_c.obj \
./Sources/exceptions_c.obj \
./Sources/main_c.obj \
./Sources/uart_support_c.obj \

OBJS_QUOTED += \
"./Sources/MCF52259_sysinit_c.obj" \
"./Sources/cfm_c.obj" \
"./Sources/exceptions_c.obj" \
"./Sources/main_c.obj" \
"./Sources/uart_support_c.obj" \

C_DEPS += \
./Sources/MCF52259_sysinit_c.d \
./Sources/cfm_c.d \
./Sources/exceptions_c.d \
./Sources/main_c.d \
./Sources/uart_support_c.d \

OBJS_OS_FORMAT += \
./Sources/MCF52259_sysinit_c.obj \
./Sources/cfm_c.obj \
./Sources/exceptions_c.obj \
./Sources/main_c.obj \
./Sources/uart_support_c.obj \


# Each subdirectory must supply rules for building sources it contributes
Sources/MCF52259_sysinit_c.obj: D:/Temp/USB_MSD_Device_bootloader/Application_Examples/TWR-MCF5225X_PTC0/Sources/MCF52259_sysinit.c
	@echo 'Building file: $<'
	@echo 'Invoking: ColdFire Compiler'
	"$(CF_ToolsDirEnv)/mwccmcf" @@"Sources/MCF52259_sysinit.args" -o "Sources/MCF52259_sysinit_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/MCF52259_sysinit_c.d: D:/Temp/USB_MSD_Device_bootloader/Application_Examples/TWR-MCF5225X_PTC0/Sources/MCF52259_sysinit.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '

Sources/cfm_c.obj: D:/Temp/USB_MSD_Device_bootloader/Application_Examples/TWR-MCF5225X_PTC0/Sources/cfm.c
	@echo 'Building file: $<'
	@echo 'Invoking: ColdFire Compiler'
	"$(CF_ToolsDirEnv)/mwccmcf" @@"Sources/cfm.args" -o "Sources/cfm_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/cfm_c.d: D:/Temp/USB_MSD_Device_bootloader/Application_Examples/TWR-MCF5225X_PTC0/Sources/cfm.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '

Sources/exceptions_c.obj: D:/Temp/USB_MSD_Device_bootloader/Application_Examples/TWR-MCF5225X_PTC0/Sources/exceptions.c
	@echo 'Building file: $<'
	@echo 'Invoking: ColdFire Compiler'
	"$(CF_ToolsDirEnv)/mwccmcf" @@"Sources/exceptions.args" -o "Sources/exceptions_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/exceptions_c.d: D:/Temp/USB_MSD_Device_bootloader/Application_Examples/TWR-MCF5225X_PTC0/Sources/exceptions.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '

Sources/main_c.obj: D:/Temp/USB_MSD_Device_bootloader/Application_Examples/TWR-MCF5225X_PTC0/Sources/main.c
	@echo 'Building file: $<'
	@echo 'Invoking: ColdFire Compiler'
	"$(CF_ToolsDirEnv)/mwccmcf" @@"Sources/main.args" -o "Sources/main_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/main_c.d: D:/Temp/USB_MSD_Device_bootloader/Application_Examples/TWR-MCF5225X_PTC0/Sources/main.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '

Sources/uart_support_c.obj: D:/Temp/USB_MSD_Device_bootloader/Application_Examples/TWR-MCF5225X_PTC0/Sources/uart_support.c
	@echo 'Building file: $<'
	@echo 'Invoking: ColdFire Compiler'
	"$(CF_ToolsDirEnv)/mwccmcf" @@"Sources/uart_support.args" -o "Sources/uart_support_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/uart_support_c.d: D:/Temp/USB_MSD_Device_bootloader/Application_Examples/TWR-MCF5225X_PTC0/Sources/uart_support.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '


