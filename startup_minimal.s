.syntax unified

.section .isr_vector,"a",%progbits
    .word _estack            @ 0: Stack pointer                                             0x0000 0000
    .word Reset_Handler      @ 1: Reset                                                     0x0000 0004
    .word 0                  @ 2: Non maskable interrupt, Clock Security System (NMI)       0x0000 0008
    .word 0                  @ 3: All class of fault (HardFault)                            0x0000 000C
    .word 0                  @ 4: Memory management (MemManage)                             0x0000 0010
    .word 0                  @ 5: Pre-fetch fault, memory access fault (BusFault)           0x0000 0014
    .word 0                  @ 6: Undefined instruction or illegal state (UsageFault)       0x0000 0018
    .word 0                  @ 7: Reserved                                                  0x0000 001C
    .word 0                  @ 8: Reserved                                                  0x0000 0020
    .word 0                  @ 9: Reserved                                                  0x0000 0024
    .word 0                  @ 10: Reserved                                                 0x0000 0028
    .word 0                  @ 11: System Service call via SWI instruction (SVCall)         0x0000 002C
    .word 0                  @ 12: Debug Monitor (Debug Monitor)                            0x0000 0030                    
    .word 0                  @ 13: Reserved                                                 0x0000 0034
    .word 0                  @ 14: Pendable request for system service (PendSV)             0x0000 0038
    .word SysTick_Handler    @ 15: System tick timer (SysTick)                              0x0000 003C

/*
    *** Currently not used ***
    .word 0                  @ 16: Window Watchdog interrupt
    .word 0                  @ 17: PVD through EXTI line detection interrupt
    .word 0                  @ 18: Tamper and TimeStamp interrupts through the EXTI line
    .word 0                  @ 19: RTC Wakeup interrupt through the EXTI line
    .word 0                  @ 20: Flash global interrupt
    .word 0                  @ 21: RCC global interrupt
    .word button_press       @ 22: EXTI Line0 interrupt
*/

.section .text
    .weak SysTick_Handler
    .type  Reset_Handler, %function

    /*
    *** Currently not used ***
    .weak button_press
    */

Reset_Handler:

/* Zero-initialize the .bss section in RAM */
    ldr r2, =_sbss
    ldr r4, =_ebss
    movs r3, #0
    b LoopFillZerobss

FillZerobss:
    str r3, [r2]
    adds r2, r2, #4

LoopFillZerobss:
    cmp r2, r4
    bcc FillZerobss

    b main
