################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/gameInfra/GameFlow.cpp \
../src/gameInfra/IPlayer.cpp 

OBJS += \
./src/gameInfra/GameFlow.o \
./src/gameInfra/IPlayer.o 

CPP_DEPS += \
./src/gameInfra/GameFlow.d \
./src/gameInfra/IPlayer.d 


# Each subdirectory must supply rules for building sources it contributes
src/gameInfra/%.o: ../src/gameInfra/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


