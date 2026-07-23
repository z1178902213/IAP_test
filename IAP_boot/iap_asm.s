    PRESERVE8
    THUMB

    AREA    |.text|, CODE, READONLY

; void SetMSPAndJump(uint32_t sp, uint32_t entry)
SetMSPAndJump PROC
    MSR     MSP, R0
    BX      R1
    ENDP

    END