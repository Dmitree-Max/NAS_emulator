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
../interior_formats.cpp 

OBJS += \
./Box.o \
./Coding.o \
./Disk.o \
./NAS_emulator.o \
./Net_stuff.o \
./Requesthandler.o \
./Snapshot.o \
./interior_formats.o 

CPP_DEPS += \
./Box.d \
./Coding.d \
./Disk.d \
./NAS_emulator.d \
./Net_stuff.d \
./Requesthandler.d \
./Snapshot.d \
./interior_formats.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -std=c++11 -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

