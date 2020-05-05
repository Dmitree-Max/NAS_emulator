################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Box.cpp \
../Coding.cpp \
../Disk.cpp \
../NAS_emulator.cpp \
../Net_stuff.cpp \
../Requesthandler.cpp \
../Snapshot.cpp \
../Socketinteractions.cpp \
../global_functions.cpp \
../interior_formats.cpp \
../system_initialize.cpp 

OBJS += \
./Box.o \
./Coding.o \
./Disk.o \
./NAS_emulator.o \
./Net_stuff.o \
./Requesthandler.o \
./Snapshot.o \
./Socketinteractions.o \
./global_functions.o \
./interior_formats.o \
./system_initialize.o 

CPP_DEPS += \
./Box.d \
./Coding.d \
./Disk.d \
./NAS_emulator.d \
./Net_stuff.d \
./Requesthandler.d \
./Snapshot.d \
./Socketinteractions.d \
./global_functions.d \
./interior_formats.d \
./system_initialize.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -std=c++11 -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


