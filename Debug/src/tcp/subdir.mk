################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/tcp/ClientTCP.cpp \
../src/tcp/IHandler.cpp \
../src/tcp/ITCPServerHandler.cpp \
../src/tcp/ServerTCP.cpp \
../src/tcp/Socket.cpp \
../src/tcp/TCP.cpp 

OBJS += \
./src/tcp/ClientTCP.o \
./src/tcp/IHandler.o \
./src/tcp/ITCPServerHandler.o \
./src/tcp/ServerTCP.o \
./src/tcp/Socket.o \
./src/tcp/TCP.o 

CPP_DEPS += \
./src/tcp/ClientTCP.d \
./src/tcp/IHandler.d \
./src/tcp/ITCPServerHandler.d \
./src/tcp/ServerTCP.d \
./src/tcp/Socket.d \
./src/tcp/TCP.d 


# Each subdirectory must supply rules for building sources it contributes
src/tcp/%.o: ../src/tcp/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


