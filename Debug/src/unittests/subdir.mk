################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/unittests/Assert.cpp \
../src/unittests/UnitTest.cpp 

OBJS += \
./src/unittests/Assert.o \
./src/unittests/UnitTest.o 

CPP_DEPS += \
./src/unittests/Assert.d \
./src/unittests/UnitTest.d 


# Each subdirectory must supply rules for building sources it contributes
src/unittests/%.o: ../src/unittests/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


