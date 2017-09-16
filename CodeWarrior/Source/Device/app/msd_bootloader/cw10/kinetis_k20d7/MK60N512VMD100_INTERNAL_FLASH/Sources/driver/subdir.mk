################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"$(KINETIS)/usb_dci_kinetis.c" \
"$(SOURCE)/driver/usb_driver.c" \
"$(SOURCE)/common/usb_framework.c" \

C_SRCS += \
$(KINETIS)/usb_dci_kinetis.c \
$(SOURCE)/driver/usb_driver.c \
$(SOURCE)/common/usb_framework.c \

OBJS += \
./Sources/driver/usb_dci_kinetis_c.obj \
./Sources/driver/usb_driver_c.obj \
./Sources/driver/usb_framework_c.obj \

OBJS_QUOTED += \
"./Sources/driver/usb_dci_kinetis_c.obj" \
"./Sources/driver/usb_driver_c.obj" \
"./Sources/driver/usb_framework_c.obj" \

C_DEPS += \
./Sources/driver/usb_dci_kinetis_c.d \
./Sources/driver/usb_driver_c.d \
./Sources/driver/usb_framework_c.d \

C_DEPS_QUOTED += \
"./Sources/driver/usb_dci_kinetis_c.d" \
"./Sources/driver/usb_driver_c.d" \
"./Sources/driver/usb_framework_c.d" \

OBJS_OS_FORMAT += \
./Sources/driver/usb_dci_kinetis_c.obj \
./Sources/driver/usb_driver_c.obj \
./Sources/driver/usb_framework_c.obj \


# Each subdirectory must supply rules for building sources it contributes
Sources/driver/usb_dci_kinetis_c.obj: $(KINETIS)/usb_dci_kinetis.c
	@echo 'Building file: $<'
	@echo 'Executing target #1 $<'
	@echo 'Invoking: ARM Compiler'
	"$(ARM_ToolsDirEnv)/mwccarm" -gccinc @@"Sources/driver/usb_dci_kinetis.args" -o "Sources/driver/usb_dci_kinetis_c.obj" -c "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/driver/usb_dci_kinetis_c.d: $(KINETIS)/usb_dci_kinetis.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '

Sources/driver/usb_driver_c.obj: $(SOURCE)/driver/usb_driver.c
	@echo 'Building file: $<'
	@echo 'Executing target #2 $<'
	@echo 'Invoking: ARM Compiler'
	"$(ARM_ToolsDirEnv)/mwccarm" -gccinc @@"Sources/driver/usb_driver.args" -o "Sources/driver/usb_driver_c.obj" -c "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/driver/usb_driver_c.d: $(SOURCE)/driver/usb_driver.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '

Sources/driver/usb_framework_c.obj: $(SOURCE)/common/usb_framework.c
	@echo 'Building file: $<'
	@echo 'Executing target #3 $<'
	@echo 'Invoking: ARM Compiler'
	"$(ARM_ToolsDirEnv)/mwccarm" -gccinc @@"Sources/driver/usb_framework.args" -o "Sources/driver/usb_framework_c.obj" -c "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/driver/usb_framework_c.d: $(SOURCE)/common/usb_framework.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '


