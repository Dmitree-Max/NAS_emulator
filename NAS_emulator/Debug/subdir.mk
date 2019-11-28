################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Box.cpp \
../Disk.cpp \
../NAS_emulator.cpp \
../Net_stuff.cpp \
../Snapshot.cpp \
../server_messanger.cpp 

OBJS += \
./Box.o \
./Disk.o \
./NAS_emulator.o \
./Net_stuff.o \
./Snapshot.o \
./server_messanger.o 

CPP_DEPS += \
./Box.d \
./Disk.d \
./NAS_emulator.d \
./Net_stuff.d \
./Snapshot.d \
./server_messanger.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -std=c++11 -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


