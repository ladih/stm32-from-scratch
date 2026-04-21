CC = arm-none-eabi-gcc

FREERTOS_DIR = third_party/FreeRTOS-Kernel

CFLAGS = -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard -nostdlib \
         -I$(FREERTOS_DIR)/include \
         -I$(FREERTOS_DIR)/portable/GCC/ARM_CM4F \
         -I.

LDFLAGS = -T linker_minimal.ld

LDFLAGS += -Wl,--gc-sections,--print-gc-sections

SRCS = startup_minimal.s \
       main.c uart.c led.c button.c adc.c tim2.c utils.c commands.c \
       $(FREERTOS_DIR)/tasks.c \
       $(FREERTOS_DIR)/queue.c \
       $(FREERTOS_DIR)/list.c \
       $(FREERTOS_DIR)/timers.c \
       $(FREERTOS_DIR)/event_groups.c \
       $(FREERTOS_DIR)/stream_buffer.c \
       $(FREERTOS_DIR)/portable/GCC/ARM_CM4F/port.c \
       $(FREERTOS_DIR)/portable/MemMang/heap_4.c


all: blink.bin

blink.elf: $(SRCS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(SRCS) -lgcc -o blink.elf

blink.bin: blink.elf
	arm-none-eabi-objcopy -O binary blink.elf blink.bin

flash: blink.bin
	st-flash write blink.bin 0x08004000

clean:
	rm -f blink.bin blink.elf