@echo off
set "AXF=./STM32CubeMX_Proj/STM32CubeMX_Proj.axf"
fromelf -c "%AXF%" -o "./STM32CubeMX_Proj/STM32CubeMX_Proj_asm.txt"
fromelf -v "%AXF%" -o "./STM32CubeMX_Proj/STM32CubeMX_Proj_detail.txt"