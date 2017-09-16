################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"D:/Temp/USB_MSD_Device_bootloader/Application_Examples/DEMOJM_PTE2/exceptions.c" \
"D:/Temp/USB_MSD_Device_bootloader/Application_Examples/DEMOJM_PTE2/main.c" \
"D:/Temp/USB_MSD_Device_bootloader/Application_Examples/DEMOJM_PTE2/mcf51jm128.c" \
"D:/Temp/USB_MSD_Device_bootloader/Application_Examples/DEMOJM_PTE2/startcf.c" \

C_SRCS += \
D:/Temp/USB_MSD_Device_bootloader/Application_Examples/DEMOJM_PTE2/exceptions.c \
D:/Temp/USB_MSD_Device_bootloader/Application_Examples/DEMOJM_PTE2/main.c \
D:/Temp/USB_MSD_Device_bootloader/Application_Examples/DEMOJM_PTE2/mcf51jm128.c \
D:/Temp/USB_MSD_Device_bootloader/Application_Examples/DEMOJM_PTE2/startcf.c \

OBJS += \
./exceptions_c.obj \
./main_c.obj \
./mcf51jm128_c.obj \
./startcf_c.obj \

OBJS_QUOTED += \
"./exceptions_c.obj" \
"./main_c.obj" \
"./mcf51jm128_c.obj" \
"./startcf_c.obj" \

C_DEPS += \
./exceptions_c.d \
./main_c.d \
./mcf51jm128_c.d \
./startcf_c.d \

OBJS_OS_FORMAT += \
./exceptions_c.obj \
./main_c.obj \
./mcf51jm128_c.obj \
./startcf_c.obj \


# Each subdirectory must supply rules for building sources it contributes
exceptions_c.obj: D:/Temp/USB_MSD_Device_bootloader/Application_Examples/DEMOJM_PTE2/exceptions.c
	@echo 'Building file: $<'
	@echo 'Invoking: ColdFire Compiler'
	"$(CF_ToolsDirEnv)/mwccmcf" @@"exceptions.args" -o "./exceptions_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

exceptions_c.d: D:/Temp/USB_MSD_Device_bootloader/Application_Examples/DEMOJM_PTE2/exceptions.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '

main_c.obj: D:/Temp/USB_MSD_Device_bootloader/Application_Examples/DEMOJM_PTE2/main.c
	@echo 'Building file: $<'
	@echo 'Invoking: ColdFire Compiler'
	"$(CF_ToolsDirEnv)/mwccmcf" @@"main.args" -o "./main_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

main_c.d: D:/Temp/USB_MSD_Device_bootloader/Application_Examples/DEMOJM_PTE2/main.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '

mcf51jm128_c.obj: D:/Temp/USB_MSD_Device_bootloader/Application_Examples/DEMOJM_PTE2/mcf51jm128.c
	@echo 'Building file: $<'
	@echo 'Invoking: ColdFire Compiler'
	"$(CF_ToolsDirEnv)/mwccmcf" @@"mcf51jm128.args" -o "./mcf51jm128_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

mcf51jm128_c.d: D:/Temp/USB_MSD_Device_bootloader/Application_Examples/DEMOJM_PTE2/mcf51jm128.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '

startcf_c.obj: D:/Temp/USB_MSD_Device_bootloader/Application_Examples/DEMOJM_PTE2/startcf.c
	@echo 'Building file: $<'
	@echo 'Invoking: ColdFire Compiler'
	"$(CF_ToolsDirEnv)/mwccmcf" @@"startcf.args" -o "./startcf_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

startcf_c.d: D:/Temp/USB_MSD_Device_bootloader/Application_Examples/DEMOJM_PTE2/startcf.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '


