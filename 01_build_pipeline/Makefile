# Toolchain & CPU
CC      = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy
CPU     = -mcpu=cortex-m4 -mthumb

# Flags
CFLAGS  = $(CPU) -Wall -O0 -g3
LDFLAGS = -T hw/linker.ld -Wl,-Map=build/final.map -nostdlib

# Files
SRCS = app/main.c hw/startup.c
OBJS = $(addprefix build/, $(notdir $(SRCS:.c=.o)))

# Build Rules
all: build/final.elf build/final.hex

build/%.o: app/%.c
	@mkdir -p build
	$(CC) $(CFLAGS) -c $< -o $@

build/%.o: hw/%.c
	@mkdir -p build
	$(CC) $(CFLAGS) -c $< -o $@

build/final.elf: $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o $@

build/final.hex: build/final.elf
	$(OBJCOPY) -O ihex $< $@

# Simulation Command
sim: all
	qemu-system-arm -M netduinoplus2 -kernel build/final.elf -nographic -S -s

clean:
	rm -rf build/
