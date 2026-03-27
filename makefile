CC = arm-none-eabi-gcc
CFLAGS = -mcpu=cortex-m4 -mthumb -nostdlib
LDFLAGS = -T linker_minimal.ld
SRCS = startup_minimal.s main.c uart.c led.c systick.c button.c adc.c tim2.c

BUILD_DIR = build

all: $(BUILD_DIR)/blink.bin

$(BUILD_DIR)/blink.elf: $(SRCS)
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(LDFLAGS) $(SRCS) -o $(BUILD_DIR)/blink.elf

$(BUILD_DIR)/blink.bin: $(BUILD_DIR)/blink.elf
	arm-none-eabi-objcopy -O binary $(BUILD_DIR)/blink.elf $(BUILD_DIR)/blink.bin

flash: $(BUILD_DIR)/blink.bin
	st-flash write $(BUILD_DIR)/blink.bin 0x08000000

clean:
	rm -rf $(BUILD_DIR)