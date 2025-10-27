################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../code/component/motor/small_driver_uart_control.c 

COMPILED_SRCS += \
code/component/motor/small_driver_uart_control.src 

C_DEPS += \
code/component/motor/small_driver_uart_control.d 

OBJS += \
code/component/motor/small_driver_uart_control.o 


# Each subdirectory must supply rules for building sources it contributes
code/component/motor/small_driver_uart_control.src: ../code/component/motor/small_driver_uart_control.c code/component/motor/subdir.mk
	cctc -cs --dep-file="$(*F).d" --misrac-version=2004 -D__CPU__=tc26xb "-fC:/Users/suiyungui/Desktop/wstcode/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc26xb -Y0 -N0 -Z0 -o "$@" "$<"
code/component/motor/small_driver_uart_control.o: code/component/motor/small_driver_uart_control.src code/component/motor/subdir.mk
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"

clean: clean-code-2f-component-2f-motor

clean-code-2f-component-2f-motor:
	-$(RM) code/component/motor/small_driver_uart_control.d code/component/motor/small_driver_uart_control.o code/component/motor/small_driver_uart_control.src

.PHONY: clean-code-2f-component-2f-motor

