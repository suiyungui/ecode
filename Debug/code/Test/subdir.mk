################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../code/Test/justfloat.c \
../code/Test/uart_app.c 

COMPILED_SRCS += \
code/Test/justfloat.src \
code/Test/uart_app.src 

C_DEPS += \
code/Test/justfloat.d \
code/Test/uart_app.d 

OBJS += \
code/Test/justfloat.o \
code/Test/uart_app.o 


# Each subdirectory must supply rules for building sources it contributes
code/Test/justfloat.src: ../code/Test/justfloat.c code/Test/subdir.mk
	cctc -cs --dep-file="$(*F).d" --misrac-version=2004 -D__CPU__=tc26xb "-fC:/Users/suiyungui/Desktop/wstcode/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc26xb -Y0 -N0 -Z0 -o "$@" "$<"
code/Test/justfloat.o: code/Test/justfloat.src code/Test/subdir.mk
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
code/Test/uart_app.src: ../code/Test/uart_app.c code/Test/subdir.mk
	cctc -cs --dep-file="$(*F).d" --misrac-version=2004 -D__CPU__=tc26xb "-fC:/Users/suiyungui/Desktop/wstcode/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc26xb -Y0 -N0 -Z0 -o "$@" "$<"
code/Test/uart_app.o: code/Test/uart_app.src code/Test/subdir.mk
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"

clean: clean-code-2f-Test

clean-code-2f-Test:
	-$(RM) code/Test/justfloat.d code/Test/justfloat.o code/Test/justfloat.src code/Test/uart_app.d code/Test/uart_app.o code/Test/uart_app.src

.PHONY: clean-code-2f-Test

