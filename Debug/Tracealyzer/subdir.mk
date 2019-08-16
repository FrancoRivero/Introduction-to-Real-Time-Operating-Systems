################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Tracealyzer/SEGGER_RTT.c \
../Tracealyzer/SEGGER_RTT_Printf.c \
../Tracealyzer/trcKernelPort.c \
../Tracealyzer/trcSnapshotRecorder.c \
../Tracealyzer/trcStreamingRecorder.c 

OBJS += \
./Tracealyzer/SEGGER_RTT.o \
./Tracealyzer/SEGGER_RTT_Printf.o \
./Tracealyzer/trcKernelPort.o \
./Tracealyzer/trcSnapshotRecorder.o \
./Tracealyzer/trcStreamingRecorder.o 

C_DEPS += \
./Tracealyzer/SEGGER_RTT.d \
./Tracealyzer/SEGGER_RTT_Printf.d \
./Tracealyzer/trcKernelPort.d \
./Tracealyzer/trcSnapshotRecorder.d \
./Tracealyzer/trcStreamingRecorder.d 


# Each subdirectory must supply rules for building sources it contributes
Tracealyzer/%.o: ../Tracealyzer/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wall  -g3 -DCPU_MK64FN1M0VDC12 -DCPU_MK64FN1M0VDC12_cm4 -DFSL_RTOS_FREE_RTOS -DSDK_OS_FREE_RTOS -I../CMSIS -I../board -I../drivers -I../freertos -I../utilities -I"D:\Facultad\SISTEMAS OPERATIVOS 2\TP4\Tracealyzer" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


