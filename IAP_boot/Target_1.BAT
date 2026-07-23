SET PATH=C:\Keil_v5\ARM\ARMCC\Bin;D:\Program Files\SiYuan\resources\kernel;C:\Program Files\Common Files\Oracle\Java\javapath;C:\Windows\system32;C:\Windows;C:\Windows\System32\Wbem;C:\Windows\System32\WindowsPowerShell\v1.0\;C:\Windows\System32\OpenSSH\;D:\Program Files\nvm;D:\ProgramData\nvm4w\nodejs;D:\Program Files\ffmpeg-8.0.1-essentials_build\bin;D:\Program Files\Git\cmd;D:\Program Files\cursor\resources\app\bin;D:\Program Files\apache-maven-3.9.16\bin;C:\Program Files\Java\jdk-17\bin;C:\Program Files\Microsoft VS Code\bin;C:\Keil_v5\ARM\ARMCC\bin;C:\Users\witcher\AppData\Local\Microsoft\WindowsApps;D:\Program Files\nvm;D:\ProgramData\nvm4w\nodejs;C:\Users\witcher\AppData\Roaming\Python\Python313\Scripts;
SET CPU_TYPE=STM32F407ZGTx
SET CPU_VENDOR=STMicroelectronics
SET UV2_TARGET=Target_1
SET CPU_CLOCK=0x00B71B00
"C:\Keil_v5\ARM\ARMCC\Bin\ArmAsm" --Via ".\objects\startup_stm32f40_41xxx._ia"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\objects\main.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\objects\bsp_led.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\objects\bsp_uart.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\objects\bsp_spi.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\objects\app.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\objects\iflash.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\objects\user_rcc.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\objects\user_gpio.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\objects\user_uart.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\objects\user_spi.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmLink" --Via ".\Objects\layer2.lnp"
"C:\Keil_v5\ARM\ARMCC\Bin\fromelf.exe" ".\Objects\layer2.axf" --i32combined --output ".\Objects\layer2.hex"
