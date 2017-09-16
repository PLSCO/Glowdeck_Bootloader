################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"$(SOURCE)/common/usb_class.c" \
"$(SOURCE)/class/usb_msc.c" \
"$(SOURCE)/class/usb_msc_scsi.c" \

C_SRCS += \
$(SOURCE)/common/usb_class.c \
$(SOURCE)/class/usb_msc.c \
$(SOURCE)/class/usb_msc_scsi.c \

OBJS += \
./Sources/class/usb_class_c.obj \
./Sources/class/usb_msc_c.obj \
./Sources/class/usb_msc_scsi_c.obj \

OBJS_QUOTED += \
"./Sources/class/usb_class_c.obj" \
"./Sources/class/usb_msc_c.obj" \
"./Sources/class/usb_msc_scsi_c.obj" \

C_DEPS += \
./Sources/class/usb_class_c.d \
./Sources/class/usb_msc_c.d \
./Sources/class/usb_msc_scsi_c.d \

C_DEPS_QUOTED += \
"./Sources/class/usb_class_c.d" \
"./Sources/class/usb_msc_c.d" \
"./Sources/class/usb_msc_scsi_c.d" \

OBJS_OS_FORMAT += \
./Sources/class/usb_class_c.obj \
./Sources/class/usb_msc_c.obj \
./Sources/class/usb_msc_scsi_c.obj \


# Each subdirectory must supply rules for building sources it contributes
Sources/class/usb_class_c.obj: $(SOURCE)/common/usb_class.c
	@echo 'Building file: $<'
	@echo 'Executing target #4 $<'
	@echo 'Invoking: ARM Compiler'
	"$(ARM_ToolsDirEnv)/mwccarm" -gccinc @@"Sources/class/usb_class.args" -o "Sources/class/usb_class_c.obj" -c "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/class/usb_class_c.d: $(SOURCE)/common/usb_class.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '

Sources/class/usb_msc_c.obj: $(SOURCE)/class/usb_msc.c
	@echo 'Building file: $<'
	@echo 'Executing target #5 $<'
	@echo 'Invoking: ARM Compiler'
	"$(ARM_ToolsDirEnv)/mwccarm" -gccinc @@"Sources/class/usb_msc.args" -o "Sources/class/usb_msc_c.obj" -c "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/class/usb_msc_c.d: $(SOURCE)/class/usb_msc.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '

Sources/class/usb_msc_scsi_c.obj: $(SOURCE)/class/usb_msc_scsi.c
	@echo 'Building file: $<'
	@echo 'Executing target #6 $<'
	@echo 'Invoking: ARM Compiler'
	"$(ARM_ToolsDirEnv)/mwccarm" -gccinc @@"Sources/class/usb_msc_scsi.args" -o "Sources/class/usb_msc_scsi_c.obj" -c "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/class/usb_msc_scsi_c.d: $(SOURCE)/class/usb_msc_scsi.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '


