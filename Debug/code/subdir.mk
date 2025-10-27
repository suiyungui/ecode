################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../code/Scheduler.c \
../code/imu.c \
../code/mode_control.c \
../code/motor_app.c \
../code/pid_app.c \
"../code/remote control.c" \
../code/servo_app.c 

COMPILED_SRCS += \
code/Scheduler.src \
code/imu.src \
code/mode_control.src \
code/motor_app.src \
code/pid_app.src \
"code/remote control.src" \
code/servo_app.src 

C_DEPS += \
code/Scheduler.d \
code/imu.d \
code/mode_control.d \
code/motor_app.d \
code/pid_app.d \
"code/remote control.d" \
code/servo_app.d 

OBJS += \
code/Scheduler.o \
code/imu.o \
code/mode_control.o \
code/motor_app.o \
code/pid_app.o \
"code/remote control.o" \
code/servo_app.o 


# Each subdirectory must supply rules for building sources it contributes
code/Scheduler.src: ../code/Scheduler.c code/subdir.mk
	cctc -cs --dep-file="$(*F).d" --misrac-version=2004 -D__CPU__=tc26xb "-fC:/Users/suiyungui/Desktop/wstcode/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc26xb -Y0 -N0 -Z0 -o "$@" "$<"
code/Scheduler.o: code/Scheduler.src code/subdir.mk
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
code/imu.src: ../code/imu.c code/subdir.mk
	cctc -cs --dep-file="$(*F).d" --misrac-version=2004 -D__CPU__=tc26xb "-fC:/Users/suiyungui/Desktop/wstcode/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc26xb -Y0 -N0 -Z0 -o "$@" "$<"
code/imu.o: code/imu.src code/subdir.mk
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
code/mode_control.src: ../code/mode_control.c code/subdir.mk
	cctc -cs --dep-file="$(*F).d" --misrac-version=2004 -D__CPU__=tc26xb "-fC:/Users/suiyungui/Desktop/wstcode/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc26xb -Y0 -N0 -Z0 -o "$@" "$<"
code/mode_control.o: code/mode_control.src code/subdir.mk
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
code/motor_app.src: ../code/motor_app.c code/subdir.mk
	cctc -cs --dep-file="$(*F).d" --misrac-version=2004 -D__CPU__=tc26xb "-fC:/Users/suiyungui/Desktop/wstcode/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc26xb -Y0 -N0 -Z0 -o "$@" "$<"
code/motor_app.o: code/motor_app.src code/subdir.mk
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
code/pid_app.src: ../code/pid_app.c code/subdir.mk
	cctc -cs --dep-file="$(*F).d" --misrac-version=2004 -D__CPU__=tc26xb "-fC:/Users/suiyungui/Desktop/wstcode/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc26xb -Y0 -N0 -Z0 -o "$@" "$<"
code/pid_app.o: code/pid_app.src code/subdir.mk
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
"code/remote control.src": "../code/remote control.c" code/subdir.mk
	cctc -cs --dep-file="$(*F).d" --misrac-version=2004 -D__CPU__=tc26xb "-fC:/Users/suiyungui/Desktop/wstcode/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc26xb -Y0 -N0 -Z0 -o "$@" "$<"
"code/remote control.o": "code/remote control.src" code/subdir.mk
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
code/servo_app.src: ../code/servo_app.c code/subdir.mk
	cctc -cs --dep-file="$(*F).d" --misrac-version=2004 -D__CPU__=tc26xb "-fC:/Users/suiyungui/Desktop/wstcode/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc26xb -Y0 -N0 -Z0 -o "$@" "$<"
code/servo_app.o: code/servo_app.src code/subdir.mk
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"

clean: clean-code

clean-code:
	-$(RM) code/Scheduler.d code/Scheduler.o code/Scheduler.src code/imu.d code/imu.o code/imu.src code/mode_control.d code/mode_control.o code/mode_control.src code/motor_app.d code/motor_app.o code/motor_app.src code/pid_app.d code/pid_app.o code/pid_app.src "code/remote control.d" "code/remote control.o" "code/remote control.src" code/servo_app.d code/servo_app.o code/servo_app.src

.PHONY: clean-code

