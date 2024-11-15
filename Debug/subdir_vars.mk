################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Add inputs and outputs from these tool invocations to the build variables 
CFG_SRCS += \
../CookieProject.cfg 

CPP_SRCS += \
../Messages.cpp 

CMD_SRCS += \
../F2837xD_Headers_BIOS_cpu1.cmd \
../TMS320F28379D.cmd 

ASM_SRCS += \
../DelayUs.asm 

C_SRCS += \
../CookieProject_DevInit.c \
../F2837xD_GlobalVariableDefs.c \
../SCI_Fxns.c \
../main.c 

GEN_CMDS += \
./configPkg/linker.cmd 

GEN_FILES += \
./configPkg/linker.cmd \
./configPkg/compiler.opt 

GEN_MISC_DIRS += \
./configPkg/ 

C_DEPS += \
./CookieProject_DevInit.d \
./F2837xD_GlobalVariableDefs.d \
./SCI_Fxns.d \
./main.d 

GEN_OPTS += \
./configPkg/compiler.opt 

OBJS += \
./CookieProject_DevInit.obj \
./DelayUs.obj \
./F2837xD_GlobalVariableDefs.obj \
./Messages.obj \
./SCI_Fxns.obj \
./main.obj 

ASM_DEPS += \
./DelayUs.d 

CPP_DEPS += \
./Messages.d 

GEN_MISC_DIRS__QUOTED += \
"configPkg\" 

OBJS__QUOTED += \
"CookieProject_DevInit.obj" \
"DelayUs.obj" \
"F2837xD_GlobalVariableDefs.obj" \
"Messages.obj" \
"SCI_Fxns.obj" \
"main.obj" 

C_DEPS__QUOTED += \
"CookieProject_DevInit.d" \
"F2837xD_GlobalVariableDefs.d" \
"SCI_Fxns.d" \
"main.d" 

CPP_DEPS__QUOTED += \
"Messages.d" 

GEN_FILES__QUOTED += \
"configPkg\linker.cmd" \
"configPkg\compiler.opt" 

ASM_DEPS__QUOTED += \
"DelayUs.d" 

C_SRCS__QUOTED += \
"../CookieProject_DevInit.c" \
"../F2837xD_GlobalVariableDefs.c" \
"../SCI_Fxns.c" \
"../main.c" 

ASM_SRCS__QUOTED += \
"../DelayUs.asm" 

CPP_SRCS__QUOTED += \
"../Messages.cpp" 


