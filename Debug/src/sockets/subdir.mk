################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/sockets/Strings.cpp \
../src/sockets/TCPClient.cpp \
../src/sockets/TCPServer.cpp \
../src/sockets/TCPSocket.cpp \
../src/sockets/UDPSocket.cpp 

OBJS += \
./src/sockets/Strings.o \
./src/sockets/TCPClient.o \
./src/sockets/TCPServer.o \
./src/sockets/TCPSocket.o \
./src/sockets/UDPSocket.o 

CPP_DEPS += \
./src/sockets/Strings.d \
./src/sockets/TCPClient.d \
./src/sockets/TCPServer.d \
./src/sockets/TCPSocket.d \
./src/sockets/UDPSocket.d 


# Each subdirectory must supply rules for building sources it contributes
src/sockets/%.o: ../src/sockets/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


