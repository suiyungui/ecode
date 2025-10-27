################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../code/component/imu/ekf.c \
../code/component/imu/matrix.c 

COMPILED_SRCS += \
code/component/imu/ekf.src \
code/component/imu/matrix.src 

C_DEPS += \
code/component/imu/ekf.d \
code/component/imu/matrix.d 

OBJS += \
code/component/imu/ekf.o \
code/component/imu/matrix.o 


# Each subdirectory must supply rules for building sources it contributes
code/component/imu/ekf.src: ../code/component/imu/ekf.c code/component/imu/subdir.mk
	cctc -cs --dep-file="$(*F).d" --misrac-version=2004 -D__CPU__=tc26xb "-fC:/Users/suiyungui/Desktop/wstcode/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc26xb -Y0 -N0 -Z0 -o "$@" "$<"
code/component/imu/ekf.o: code/component/imu/ekf.src code/component/imu/subdir.mk
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
code/component/imu/matrix.src: ../code/component/imu/matrix.c code/component/imu/subdir.mk
	cctc -cs --dep-file="$(*F).d" --misrac-version=2004 -D__CPU__=tc26xb "-fC:/Users/suiyungui/Desktop/wstcode/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc26xb -Y0 -N0 -Z0 -o "$@" "$<"
code/component/imu/matrix.o: code/component/imu/matrix.src code/component/imu/subdir.mk
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"

clean: clean-code-2f-component-2f-imu

clean-code-2f-component-2f-imu:
	-$(RM) code/component/imu/ekf.d code/component/imu/ekf.o code/component/imu/ekf.src code/component/imu/matrix.d code/component/imu/matrix.o code/component/imu/matrix.src

.PHONY: clean-code-2f-component-2f-imu

