SET PATH=C:\Keil_v5\ARM\ARMCC\Bin;C:\Program Files (x86)\STMicroelectronics\st_toolset\asm;C:\Windows\system32;C:\Windows;C:\Windows\System32\Wbem;C:\Windows\System32\WindowsPowerShell\v1.0\;C:\Program Files (x86)\COSMIC\CXSTM8
SET CPU_TYPE=STM32F103C8
SET CPU_VENDOR=STMicroelectronics
SET UV2_TARGET=Utils
SET CPU_CLOCK=0x007A1200
"C:\Keil_v5\ARM\ARMCC\Bin\ArmAsm" --Via ".\startup_stm32f10x_hd._ia"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\main.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\stm32f10x_it.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\usart1.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\timx.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\gpio.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\usart1_util.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\flash_led.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\usart1_data.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\linked_list.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\linked_list_data.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\pwm.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\pwm_led.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\stm32f10x_gpio.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\stm32f10x_rcc.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\stm32f10x_usart.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\stm32f10x_tim.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\misc.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\core_cm3.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\system_stm32f10x.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmLink" --Via ".\F103-Utils.lnp"
"C:\Keil_v5\ARM\ARMCC\Bin\fromelf.exe" ".\F103-Utils.axf" --i32combined --output ".\F103-Utils.hex"
