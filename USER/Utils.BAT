SET PATH=C:\Keil_v5\ARM\ARMCC\Bin;C:\Program Files (x86)\STMicroelectronics\st_toolset\asm;C:\Windows\system32;C:\Windows;C:\Windows\System32\Wbem;C:\Windows\System32\WindowsPowerShell\v1.0\;C:\Program Files (x86)\COSMIC\CXSTM8
SET CPU_TYPE=STM32F103C8
SET CPU_VENDOR=STMicroelectronics
SET UV2_TARGET=Utils
SET CPU_CLOCK=0x007A1200
"C:\Keil_v5\ARM\ARMCC\Bin\ArmAsm" --Via "..\output\startup_stm32f10x_hd._ia"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "..\output\main.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "..\output\stm32f10x_it.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "..\output\timx.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "..\output\gpio.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "..\output\flash_led.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "..\output\linked_list.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "..\output\linked_list_data.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "..\output\pwm.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "..\output\pwm_led.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "..\output\usart1_test.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "..\output\i2c_eeprom.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "..\output\i2c.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "..\output\usart_data.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "..\output\usart.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "..\output\i2c_eeprom_test.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "..\output\stm32f10x_gpio.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "..\output\stm32f10x_rcc.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "..\output\stm32f10x_usart.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "..\output\stm32f10x_tim.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "..\output\misc.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "..\output\stm32f10x_i2c.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "..\output\stm32f10x_exti.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "..\output\core_cm3.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "..\output\system_stm32f10x.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmLink" --Via "..\Output\F103-Utils.lnp"
"C:\Keil_v5\ARM\ARMCC\Bin\fromelf.exe" "..\Output\F103-Utils.axf" --i32combined --output "..\Output\F103-Utils.hex"
