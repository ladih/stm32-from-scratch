// uart.h

#define BUF_SIZE 128

extern volatile char rx_buf[BUF_SIZE];
extern volatile int rx_idx;
extern volatile int line_ready;

void uart_init(void);
void uart_print(const char *s);
void uart_print_int(int n);