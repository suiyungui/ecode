################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../code/component/pid/pid.c 

COMPILED_SRCS += \
code/component/pid/pid.src 

C_DEPS += \
code/component/pid/pid.d 

OBJS += \
code/component/pid/pid.o 


# Each subdirectory must supply rules for building sources it contributes
code/component/pid/pid.src: ../code/component/pid/pid.c code/component/pid/subdir.mk
	cctc -cs --dep-file="$(*F).d" --misrac-version=2004 -D__CPU__=tc26xb "-fC:/Users/suiyungui/Desktop/wstcode/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc26xb -Y0 -N0 -Z0 -o "$@" "$<"
code/component/pid/pid.o: code/component/pid/pid.src code/component/pid/subdir.mk
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"

clean: clean-code-2f-component-2f-pid

clean-code-2f-component-2f-pid:
	-$(RM) code/component/pid/pid.d code/component/pid/pid.o code/component/pid/pid.src

.PHONY: clean-code-2f-component-2f-pid

