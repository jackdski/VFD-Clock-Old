################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
circbuf.obj: ../circbuf.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"/home/jack/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.2.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me -O2 --opt_for_speed=0 --include_path="/home/jack/ti/ccsv8/ccs_base/arm/include" --include_path="/home/jack/ti/ccsv8/ccs_base/arm/include/CMSIS" --include_path="/home/jack/workspace_v8/VFD-Clock" --include_path="/home/jack/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.2.LTS/include" --advice:power=all --define=__MSP432P401R__ --define=ccs -g --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="circbuf.d_raw" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: "$<"'
	@echo ' '

main.obj: ../main.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"/home/jack/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.2.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me -O2 --opt_for_speed=0 --include_path="/home/jack/ti/ccsv8/ccs_base/arm/include" --include_path="/home/jack/ti/ccsv8/ccs_base/arm/include/CMSIS" --include_path="/home/jack/workspace_v8/VFD-Clock" --include_path="/home/jack/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.2.LTS/include" --advice:power=all --define=__MSP432P401R__ --define=ccs -g --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="main.d_raw" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: "$<"'
	@echo ' '

peripherals.obj: ../peripherals.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"/home/jack/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.2.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me -O2 --opt_for_speed=0 --include_path="/home/jack/ti/ccsv8/ccs_base/arm/include" --include_path="/home/jack/ti/ccsv8/ccs_base/arm/include/CMSIS" --include_path="/home/jack/workspace_v8/VFD-Clock" --include_path="/home/jack/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.2.LTS/include" --advice:power=all --define=__MSP432P401R__ --define=ccs -g --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="peripherals.d_raw" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: "$<"'
	@echo ' '

rtc.obj: ../rtc.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"/home/jack/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.2.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me -O2 --opt_for_speed=0 --include_path="/home/jack/ti/ccsv8/ccs_base/arm/include" --include_path="/home/jack/ti/ccsv8/ccs_base/arm/include/CMSIS" --include_path="/home/jack/workspace_v8/VFD-Clock" --include_path="/home/jack/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.2.LTS/include" --advice:power=all --define=__MSP432P401R__ --define=ccs -g --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="rtc.d_raw" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: "$<"'
	@echo ' '

startup_msp432p401r_ccs.obj: ../startup_msp432p401r_ccs.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"/home/jack/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.2.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me -O2 --opt_for_speed=0 --include_path="/home/jack/ti/ccsv8/ccs_base/arm/include" --include_path="/home/jack/ti/ccsv8/ccs_base/arm/include/CMSIS" --include_path="/home/jack/workspace_v8/VFD-Clock" --include_path="/home/jack/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.2.LTS/include" --advice:power=all --define=__MSP432P401R__ --define=ccs -g --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="startup_msp432p401r_ccs.d_raw" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: "$<"'
	@echo ' '

system_msp432p401r.obj: ../system_msp432p401r.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"/home/jack/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.2.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me -O2 --opt_for_speed=0 --include_path="/home/jack/ti/ccsv8/ccs_base/arm/include" --include_path="/home/jack/ti/ccsv8/ccs_base/arm/include/CMSIS" --include_path="/home/jack/workspace_v8/VFD-Clock" --include_path="/home/jack/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.2.LTS/include" --advice:power=all --define=__MSP432P401R__ --define=ccs -g --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="system_msp432p401r.d_raw" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: "$<"'
	@echo ' '

timer.obj: ../timer.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"/home/jack/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.2.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me -O2 --opt_for_speed=0 --include_path="/home/jack/ti/ccsv8/ccs_base/arm/include" --include_path="/home/jack/ti/ccsv8/ccs_base/arm/include/CMSIS" --include_path="/home/jack/workspace_v8/VFD-Clock" --include_path="/home/jack/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.2.LTS/include" --advice:power=all --define=__MSP432P401R__ --define=ccs -g --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="timer.d_raw" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: "$<"'
	@echo ' '

tubes.obj: ../tubes.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"/home/jack/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.2.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me -O2 --opt_for_speed=0 --include_path="/home/jack/ti/ccsv8/ccs_base/arm/include" --include_path="/home/jack/ti/ccsv8/ccs_base/arm/include/CMSIS" --include_path="/home/jack/workspace_v8/VFD-Clock" --include_path="/home/jack/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.2.LTS/include" --advice:power=all --define=__MSP432P401R__ --define=ccs -g --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="tubes.d_raw" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: "$<"'
	@echo ' '


