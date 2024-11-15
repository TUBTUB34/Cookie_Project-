################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
Headers/%.obj: ../Headers/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs1040/ccs/tools/compiler/ti-cgt-c2000_20.2.5.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla1 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 --include_path="C:/ti/c2000/C2000Ware_5_03_00_00/device_support/f2837xd/headers/include" --include_path="C:/ti/c2000/C2000Ware_5_03_00_00/device_support/f2837xd/common/include" --include_path="C:/ti/c2000/C2000Ware_5_03_00_00/libraries" --include_path="C:/7820/CookieProject" --include_path="C:/ti/bios_6_83_00_18/packages/ti/posix/ccs" --include_path="C:/ti/ccs1040/ccs/tools/compiler/ti-cgt-c2000_20.2.5.LTS/include" --define=CPU1 -g --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --gen_func_subsections=on --abi=coffabi --preproc_with_compile --preproc_dependency="Headers/$(basename $(<F)).d_raw" --obj_directory="Headers" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


