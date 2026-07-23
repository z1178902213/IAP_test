SET PATH=C:\Keil_v5\ARM\ARMCC\Bin;D:\Program Files\SiYuan\resources\kernel;C:\Program Files\Common Files\Oracle\Java\javapath;C:\Windows\system32;C:\Windows;C:\Windows\System32\Wbem;C:\Windows\System32\WindowsPowerShell\v1.0\;C:\Windows\System32\OpenSSH\;D:\Program Files\nvm;D:\ProgramData\nvm4w\nodejs;D:\Program Files\ffmpeg-8.0.1-essentials_build\bin;D:\Program Files\Git\cmd;D:\Program Files\cursor\resources\app\bin;D:\Program Files\apache-maven-3.9.16\bin;C:\Program Files\Java\jdk-17\bin;C:\Program Files\Microsoft VS Code\bin;C:\Users\witcher\AppData\Local\Microsoft\WindowsApps;D:\Program Files\nvm;D:\ProgramData\nvm4w\nodejs;C:\Users\witcher\AppData\Roaming\Python\Python313\Scripts;
SET CPU_TYPE=STM32F407ZGTx
SET CPU_VENDOR=STMicroelectronics
SET UV2_TARGET=STM32CubeMX_Proj
SET CPU_CLOCK=0x017D7840
"C:\Keil_v5\ARM\ARMCC\Bin\ArmAsm" --Via "stm32cubemx_proj\startup_stm32f407xx._ia"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "stm32cubemx_proj\user_task.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "stm32cubemx_proj\user_hal_callback.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "stm32cubemx_proj\main.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "stm32cubemx_proj\stm32f4xx_it.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "stm32cubemx_proj\stm32f4xx_hal_msp.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmLink" --Via "STM32CubeMX_Proj\STM32CubeMX_Proj.lnp"
"C:\Keil_v5\ARM\ARMCC\Bin\fromelf.exe" "STM32CubeMX_Proj\STM32CubeMX_Proj.axf" --i32combined --output "STM32CubeMX_Proj\STM32CubeMX_Proj.hex"
