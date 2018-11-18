################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/gamexo/XOBoardViewer.cpp \
../src/gamexo/XOGame.cpp \
../src/gamexo/XOMove.cpp \
../src/gamexo/XOPlayer.cpp 

OBJS += \
./src/gamexo/XOBoardViewer.o \
./src/gamexo/XOGame.o \
./src/gamexo/XOMove.o \
./src/gamexo/XOPlayer.o 

CPP_DEPS += \
./src/gamexo/XOBoardViewer.d \
./src/gamexo/XOGame.d \
./src/gamexo/XOMove.d \
./src/gamexo/XOPlayer.d 


# Each subdirectory must supply rules for building sources it contributes
src/gamexo/%.o: ../src/gamexo/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


