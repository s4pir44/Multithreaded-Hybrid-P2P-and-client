################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/server/Broker.cpp \
../src/server/GameServer.cpp \
../src/server/SingleClient.cpp \
../src/server/UDPClient.cpp \
../src/server/UDPServer.cpp \
../src/server/XOHandler.cpp 

OBJS += \
./src/server/Broker.o \
./src/server/GameServer.o \
./src/server/SingleClient.o \
./src/server/UDPClient.o \
./src/server/UDPServer.o \
./src/server/XOHandler.o 

CPP_DEPS += \
./src/server/Broker.d \
./src/server/GameServer.d \
./src/server/SingleClient.d \
./src/server/UDPClient.d \
./src/server/UDPServer.d \
./src/server/XOHandler.d 


# Each subdirectory must supply rules for building sources it contributes
src/server/%.o: ../src/server/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


