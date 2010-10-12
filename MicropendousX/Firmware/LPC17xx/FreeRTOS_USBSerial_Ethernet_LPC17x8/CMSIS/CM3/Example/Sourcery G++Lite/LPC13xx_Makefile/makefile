

all:
	arm-none-eabi-gcc ../../../CoreSupport/core_cm3.c -c -mcpu=cortex-m3 -mthumb -gdwarf-2 -MD -Wall -O0 -mapcs-frame -mthumb-interwork -I../../../CoreSupport -I../../../DeviceSupport/NXP/LPC13xx -I../../../INC/NXP/LPC13xx/ -Wa,-alhms="./Lst/core_cm3.lst" -o ./Obj/core_cm3.o
	arm-none-eabi-as ./startup_LPC13xx.s -mcpu=cortex-m3 -mthumb --gdwarf-2 -mthumb-interwork --MD ./Obj/startup_LPC13xx.d -I../../../INC/NXP/LPC13xx/ -alhms="./Lst/startup_LPC13xx.lst" -o ./Obj/startup_LPC13xx.o
	arm-none-eabi-gcc ./system_LPC13xx.c -c -mcpu=cortex-m3 -mthumb -gdwarf-2 -MD -Wall -O0 -mapcs-frame -mthumb-interwork -I../../../CoreSupport -I../../../DeviceSupport/NXP/LPC13xx -I../../../INC/NXP/LPC13xx/ -Wa,-alhms="./Lst/system_lpc13xx.lst" -o ./Obj/system_lpc13xx.o
	arm-none-eabi-gcc ./main_LPC13xx.c -c -mcpu=cortex-m3 -mthumb -gdwarf-2 -MD -Wall -O0 -mapcs-frame -mthumb-interwork -I../../../CoreSupport -I../../../DeviceSupport/NXP/LPC13xx -I../../../INC/NXP/LPC13xx/ -Wa,-alhms="./Lst/main_lpc13xx.lst" -o ./Obj/main_lpc13xx.o
	arm-none-eabi-gcc -T ./LPC13xx.ld -mcpu=cortex-m3 -mthumb -mthumb-interwork -Wl,-Map="./Lst/CMSIS_Example.map" --gc-sections -o ./Obj/CMSIS_Example.elf "./obj/core_cm3.o" "./obj/startup_lpc13xx.o" "./obj/system_lpc13xx.o" "./obj/main_lpc13xx.o"
	arm-none-eabi-objcopy -O ihex ./Obj/CMSIS_Example.elf ./CMSIS_Example.hex
	arm-none-eabi-objdump -h -z -S ./Obj/CMSIS_Example.elf > ./Lst/CMSIS_Example.lss
	arm-none-eabi-nm -n ./Obj/CMSIS_Example.elf > ./Lst/CMSIS_Example.sym
	arm-none-eabi-size ./Obj/CMSIS_Example.elf -A -d  --target=elf32-littlearm
