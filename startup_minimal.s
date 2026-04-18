.syntax unified

.section .isr_vector, "a"

    // System Exceptions 
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

    // IRQs (Interrupt Requests) 
    .word 0                     @ 16: Window Watchdog interrupt                                0x0000 0040
    .word 0                     @ 17: PVD through EXTI line detection interrupt                0x0000 0044
    .word 0                     @ 18: Tamper and TimeStamp interrupts through the EXTI line    0x0000 0048
    .word 0                     @ 19: RTC Wakeup interrupt through the EXTI line               0x0000 004C
    .word 0                     @ 20: Flash global interrupt                                   0x0000 0050
    .word 0                     @ 21: RCC global interrupt                                     0x0000 0054
    .word 0                     @ 22: EXTI Line0 interrupt                                     0x0000 0058
    .word 0                     @ 23: EXTI Line1 interrupt                                     0x0000 005C
    .word 0                     @ 24: EXTI Line2 interrupt                                     0x0000 0060
    .word 0                     @ 25: EXTI Line3 interrupt                                     0x0000 0064
    .word 0                     @ 26: EXTI Line4 interrupt                                     0x0000 0068
    .word 0                     @ 27: DMA1 Stream0 global interrupt                            0x0000 006C
    .word 0                     @ 28: DMA1 Stream1 global interrupt                            0x0000 0070
    .word 0                     @ 29: DMA1 Stream2 global interrupt                            0x0000 0074
    .word 0                     @ 30: DMA1 Stream3 global interrupt                            0x0000 0078
    .word 0                     @ 31: DMA1 Stream4 global interrupt                            0x0000 007C
    .word 0                     @ 32: DMA1 Stream5 global interrupt                            0x0000 0080
    .word 0                     @ 33: DMA1 Stream6 global interrupt                            0x0000 0084
    .word 0                     @ 34: ADC1, ADC2 and ADC3 global interrupts                    0x0000 0088
    .word 0                     @ 35: CAN1 TX interrupts                                       0x0000 008C
    .word 0                     @ 36: CAN1 RX0 interrupts                                      0x0000 0090
    .word 0                     @ 37: CAN1 RX1 interrupt                                       0x0000 0094
    .word 0                     @ 38: CAN1 SCE interrupt                                       0x0000 0098
    .word 0                     @ 39: EXTI Line[9:5] interrupts                                0x0000 009C
    .word 0                     @ 40: TIM1 Break interrupt and TIM9 global interrupt           0x0000 00A0
    .word 0                     @ 41: TIM1 Update interrupt and TIM10 global interrupt         0x0000 00A4
    .word 0                     @ 42: TIM1 Trigger and Commutation interrupts and TIM11 global interrupt 0x0000 00A8
    .word 0                     @ 43: TIM1 Capture compare interrupt                           0x0000 00AC
    .word 0                     @ 44: TIM2 global interrupt                                    0x0000 00B0
    .word 0                     @ 45: TIM3 global interrupt                                    0x0000 00B4
    .word 0                     @ 46: TIM4 global interrupt                                    0x0000 00B8
    .word 0                     @ 47: I2C1 event interrupt                                     0x0000 00BC
    .word 0                     @ 48: I2C1 error interrupt                                     0x0000 00C0
    .word 0                     @ 49: I2C2 event interrupt                                     0x0000 00C4
    .word 0                     @ 50: I2C2 error interrupt                                     0x0000 00C8
    .word 0                     @ 51: SPI1 global interrupt                                    0x0000 00CC
    .word 0                     @ 52: SPI2 global interrupt                                    0x0000 00D0
    .word 0                     @ 53: USART1 global interrupt                                  0x0000 00D4
    .word USART2_IRQHandler     @ 54: USART2 global interrupt                                  0x0000 00D8
    .word 0                     @ 55: USART3 global interrupt                                  0x0000 00DC
    .word EXTI15_10_IRQHandler  @ 56: EXTI15_10 EXTI Line[15:10] interrupts                    0x0000 00E0

.section .text
    .type  Reset_Handler, %function

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

/* Non-zero data */
ldr r0, =_sdata     /* RAM start */
ldr r1, =_edata     /* RAM end */
ldr r2, =_sidata  /* FLASH start */
movs r3, #0
b LoopCopyDataInit

CopyDataInit:
    ldr r4, [r2, r3]
    str r4, [r0, r3]
    adds r3, r3, #4

LoopCopyDataInit:
    adds r4, r0, r3
    cmp r4, r1
    bcc CopyDataInit

b main
