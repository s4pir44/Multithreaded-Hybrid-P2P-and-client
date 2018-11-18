################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/centralDirectory/AcceptAll.cpp \
../src/centralDirectory/Games.cpp \
../src/centralDirectory/RejectAll.cpp \
../src/centralDirectory/Score.cpp \
../src/centralDirectory/Users.cpp 

OBJS += \
./src/centralDirectory/AcceptAll.o \
./src/centralDirectory/Games.o \
./src/centralDirectory/RejectAll.o \
./src/centralDirectory/Score.o \
./src/centralDirectory/Users.o 

CPP_DEPS += \
./src/centralDirectory/AcceptAll.d \
./src/centralDirectory/Games.d \
./src/centralDirectory/RejectAll.d \
./src/centralDirectory/Score.d \
./src/centralDirectory/Users.d 


# Each subdirectory must supply rules for building sources it contributes
src/centralDirectory/%.o: ../src/centralDirectory/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


