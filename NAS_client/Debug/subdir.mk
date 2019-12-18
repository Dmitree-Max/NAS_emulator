################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../NAS_client.cpp \
../client_messanger.cpp \
../client_messanger2.cpp 

OBJS += \
./NAS_client.o \
./client_messanger.o \
./client_messanger2.o 

CPP_DEPS += \
./NAS_client.d \
./client_messanger.d \
./client_messanger2.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


