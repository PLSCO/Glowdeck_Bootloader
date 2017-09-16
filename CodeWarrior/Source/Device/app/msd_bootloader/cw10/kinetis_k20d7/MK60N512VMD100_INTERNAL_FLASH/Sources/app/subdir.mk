################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"$(APP)/msd_bootloader/Boot_loader_task.c" \
"$(APP)/msd_bootloader/FAT16.c" \
"$(APP)/msd_bootloader/Loader.c" \
"$(APP)/common/kinetis/RealTimerCounter_kinetis.c" \
"$(APP)/msd_bootloader/bluetooth.c" \
"$(APP)/msd_bootloader/disk.c" \
"$(APP)/common/main_kinetis.c" \
"$(APP)/common/kinetis/printf.c" \
"$(APP)/common/kinetis/sci.c" \
"$(APP)/common/serial/serial_kinetis.c" \
"$(APP)/msd_bootloader/usb_descriptor.c" \
"$(APP)/common/kinetis/vectors.c" \
"$(APP)/common/kinetis/wdt_kinetis.c" \

C_SRCS += \
$(APP)/msd_bootloader/Boot_loader_task.c \
$(APP)/msd_bootloader/FAT16.c \
$(APP)/msd_bootloader/Loader.c \
$(APP)/common/kinetis/RealTimerCounter_kinetis.c \
$(APP)/msd_bootloader/bluetooth.c \
$(APP)/msd_bootloader/disk.c \
$(APP)/common/main_kinetis.c \
$(APP)/common/kinetis/printf.c \
$(APP)/common/kinetis/sci.c \
$(APP)/common/serial/serial_kinetis.c \
$(APP)/msd_bootloader/usb_descriptor.c \
$(APP)/common/kinetis/vectors.c \
$(APP)/common/kinetis/wdt_kinetis.c \

OBJS += \
./Sources/app/Boot_loader_task_c.obj \
./Sources/app/FAT16_c.obj \
./Sources/app/Loader_c.obj \
./Sources/app/RealTimerCounter_kinetis_c.obj \
./Sources/app/bluetooth_c.obj \
./Sources/app/disk_c.obj \
./Sources/app/main_kinetis_c.obj \
./Sources/app/printf_c.obj \
./Sources/app/sci_c.obj \
./Sources/app/serial_kinetis_c.obj \
./Sources/app/usb_descriptor_c.obj \
./Sources/app/vectors_c.obj \
./Sources/app/wdt_kinetis_c.obj \

OBJS_QUOTED += \
"./Sources/app/Boot_loader_task_c.obj" \
"./Sources/app/FAT16_c.obj" \
"./Sources/app/Loader_c.obj" \
"./Sources/app/RealTimerCounter_kinetis_c.obj" \
"./Sources/app/bluetooth_c.obj" \
"./Sources/app/disk_c.obj" \
"./Sources/app/main_kinetis_c.obj" \
"./Sources/app/printf_c.obj" \
"./Sources/app/sci_c.obj" \
"./Sources/app/serial_kinetis_c.obj" \
"./Sources/app/usb_descriptor_c.obj" \
"./Sources/app/vectors_c.obj" \
"./Sources/app/wdt_kinetis_c.obj" \

C_DEPS += \
./Sources/app/Boot_loader_task_c.d \
./Sources/app/FAT16_c.d \
./Sources/app/Loader_c.d \
./Sources/app/RealTimerCounter_kinetis_c.d \
./Sources/app/bluetooth_c.d \
./Sources/app/disk_c.d \
./Sources/app/main_kinetis_c.d \
./Sources/app/printf_c.d \
./Sources/app/sci_c.d \
./Sources/app/serial_kinetis_c.d \
./Sources/app/usb_descriptor_c.d \
./Sources/app/vectors_c.d \
./Sources/app/wdt_kinetis_c.d \

C_DEPS_QUOTED += \
"./Sources/app/Boot_loader_task_c.d" \
"./Sources/app/FAT16_c.d" \
"./Sources/app/Loader_c.d" \
"./Sources/app/RealTimerCounter_kinetis_c.d" \
"./Sources/app/bluetooth_c.d" \
"./Sources/app/disk_c.d" \
"./Sources/app/main_kinetis_c.d" \
"./Sources/app/printf_c.d" \
"./Sources/app/sci_c.d" \
"./Sources/app/serial_kinetis_c.d" \
"./Sources/app/usb_descriptor_c.d" \
"./Sources/app/vectors_c.d" \
"./Sources/app/wdt_kinetis_c.d" \

OBJS_OS_FORMAT += \
./Sources/app/Boot_loader_task_c.obj \
./Sources/app/FAT16_c.obj \
./Sources/app/Loader_c.obj \
./Sources/app/RealTimerCounter_kinetis_c.obj \
./Sources/app/bluetooth_c.obj \
./Sources/app/disk_c.obj \
./Sources/app/main_kinetis_c.obj \
./Sources/app/printf_c.obj \
./Sources/app/sci_c.obj \
./Sources/app/serial_kinetis_c.obj \
./Sources/app/usb_descriptor_c.obj \
./Sources/app/vectors_c.obj \
./Sources/app/wdt_kinetis_c.obj \


# Each subdirectory must supply rules for building sources it contributes
Sources/app/Boot_loader_task_c.obj: $(APP)/msd_bootloader/Boot_loader_task.c
	@echo 'Building file: $<'
	@echo 'Executing target #7 $<'
	@echo 'Invoking: ARM Compiler'
	"$(ARM_ToolsDirEnv)/mwccarm" -gccinc @@"Sources/app/Boot_loader_task.args" -o "Sources/app/Boot_loader_task_c.obj" -c "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/app/Boot_loader_task_c.d: $(APP)/msd_bootloader/Boot_loader_task.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '

Sources/app/FAT16_c.obj: $(APP)/msd_bootloader/FAT16.c
	@echo 'Building file: $<'
	@echo 'Executing target #8 $<'
	@echo 'Invoking: ARM Compiler'
	"$(ARM_ToolsDirEnv)/mwccarm" -gccinc @@"Sources/app/FAT16.args" -o "Sources/app/FAT16_c.obj" -c "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/app/FAT16_c.d: $(APP)/msd_bootloader/FAT16.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '

Sources/app/Loader_c.obj: $(APP)/msd_bootloader/Loader.c
	@echo 'Building file: $<'
	@echo 'Executing target #9 $<'
	@echo 'Invoking: ARM Compiler'
	"$(ARM_ToolsDirEnv)/mwccarm" -gccinc @@"Sources/app/Loader.args" -o "Sources/app/Loader_c.obj" -c "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/app/Loader_c.d: $(APP)/msd_bootloader/Loader.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '

Sources/app/RealTimerCounter_kinetis_c.obj: $(APP)/common/kinetis/RealTimerCounter_kinetis.c
	@echo 'Building file: $<'
	@echo 'Executing target #10 $<'
	@echo 'Invoking: ARM Compiler'
	"$(ARM_ToolsDirEnv)/mwccarm" -gccinc @@"Sources/app/RealTimerCounter_kinetis.args" -o "Sources/app/RealTimerCounter_kinetis_c.obj" -c "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/app/RealTimerCounter_kinetis_c.d: $(APP)/common/kinetis/RealTimerCounter_kinetis.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '

Sources/app/bluetooth_c.obj: $(APP)/msd_bootloader/bluetooth.c
	@echo 'Building file: $<'
	@echo 'Executing target #11 $<'
	@echo 'Invoking: ARM Compiler'
	"$(ARM_ToolsDirEnv)/mwccarm" -gccinc @@"Sources/app/bluetooth.args" -o "Sources/app/bluetooth_c.obj" -c "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/app/bluetooth_c.d: $(APP)/msd_bootloader/bluetooth.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '

Sources/app/disk_c.obj: $(APP)/msd_bootloader/disk.c
	@echo 'Building file: $<'
	@echo 'Executing target #12 $<'
	@echo 'Invoking: ARM Compiler'
	"$(ARM_ToolsDirEnv)/mwccarm" -gccinc @@"Sources/app/disk.args" -o "Sources/app/disk_c.obj" -c "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/app/disk_c.d: $(APP)/msd_bootloader/disk.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '

Sources/app/main_kinetis_c.obj: $(APP)/common/main_kinetis.c
	@echo 'Building file: $<'
	@echo 'Executing target #13 $<'
	@echo 'Invoking: ARM Compiler'
	"$(ARM_ToolsDirEnv)/mwccarm" -gccinc @@"Sources/app/main_kinetis.args" -o "Sources/app/main_kinetis_c.obj" -c "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/app/main_kinetis_c.d: $(APP)/common/main_kinetis.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '

Sources/app/printf_c.obj: $(APP)/common/kinetis/printf.c
	@echo 'Building file: $<'
	@echo 'Executing target #14 $<'
	@echo 'Invoking: ARM Compiler'
	"$(ARM_ToolsDirEnv)/mwccarm" -gccinc @@"Sources/app/printf.args" -o "Sources/app/printf_c.obj" -c "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/app/printf_c.d: $(APP)/common/kinetis/printf.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '

Sources/app/sci_c.obj: $(APP)/common/kinetis/sci.c
	@echo 'Building file: $<'
	@echo 'Executing target #15 $<'
	@echo 'Invoking: ARM Compiler'
	"$(ARM_ToolsDirEnv)/mwccarm" -gccinc @@"Sources/app/sci.args" -o "Sources/app/sci_c.obj" -c "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/app/sci_c.d: $(APP)/common/kinetis/sci.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '

Sources/app/serial_kinetis_c.obj: $(APP)/common/serial/serial_kinetis.c
	@echo 'Building file: $<'
	@echo 'Executing target #16 $<'
	@echo 'Invoking: ARM Compiler'
	"$(ARM_ToolsDirEnv)/mwccarm" -gccinc @@"Sources/app/serial_kinetis.args" -o "Sources/app/serial_kinetis_c.obj" -c "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/app/serial_kinetis_c.d: $(APP)/common/serial/serial_kinetis.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '

Sources/app/usb_descriptor_c.obj: $(APP)/msd_bootloader/usb_descriptor.c
	@echo 'Building file: $<'
	@echo 'Executing target #17 $<'
	@echo 'Invoking: ARM Compiler'
	"$(ARM_ToolsDirEnv)/mwccarm" -gccinc @@"Sources/app/usb_descriptor.args" -o "Sources/app/usb_descriptor_c.obj" -c "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/app/usb_descriptor_c.d: $(APP)/msd_bootloader/usb_descriptor.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '

Sources/app/vectors_c.obj: $(APP)/common/kinetis/vectors.c
	@echo 'Building file: $<'
	@echo 'Executing target #18 $<'
	@echo 'Invoking: ARM Compiler'
	"$(ARM_ToolsDirEnv)/mwccarm" -gccinc @@"Sources/app/vectors.args" -o "Sources/app/vectors_c.obj" -c "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/app/vectors_c.d: $(APP)/common/kinetis/vectors.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '

Sources/app/wdt_kinetis_c.obj: $(APP)/common/kinetis/wdt_kinetis.c
	@echo 'Building file: $<'
	@echo 'Executing target #19 $<'
	@echo 'Invoking: ARM Compiler'
	"$(ARM_ToolsDirEnv)/mwccarm" -gccinc @@"Sources/app/wdt_kinetis.args" -o "Sources/app/wdt_kinetis_c.obj" -c "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/app/wdt_kinetis_c.d: $(APP)/common/kinetis/wdt_kinetis.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '


