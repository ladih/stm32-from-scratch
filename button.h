// button.h

extern volatile int button_pressed;

void EXTI15_10_IRQHandler(void);

void button_init(void);