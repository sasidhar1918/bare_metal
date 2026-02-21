# --- Toolchain Definitions ---
CC      = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy
SIZE    = arm-none-eabi-size
BUILD_DIR = build

# --- Compilation Flags ---
# -mcpu=cortex-m4: Target the M4 core
# -mthumb: Use the Thumb instruction set (required for Cortex-M)
# -O0: No optimization (easiest for debugging)
# -g3: Maximum debug information
CPU_FLAGS = -mcpu=cortex-m4 -mthumb
CFLAGS    = $(CPU_FLAGS) -Wall -O0 -g3 -Iapp -Ihw
LDFLAGS   = -T hw/linker.ld -Wl,-Map=$(BUILD_DIR)/final.map -nostdlib

# --- Files ---
SRCS = app/main.c hw/startup.c
# This converts app/main.c -> build/main.o and hw/startup.c -> build/startup.o
OBJS = $(addprefix $(BUILD_DIR)/, $(notdir $(SRCS:.c=.o)))

# --- Build Rules ---

all: $(BUILD_DIR)/final.elf $(BUILD_DIR)/final.hex
	@echo "Build Successful!"
	@$(SIZE) $(BUILD_DIR)/final.elf

# Rule for main.o
$(BUILD_DIR)/main.o: app/main.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Rule for startup.o
$(BUILD_DIR)/startup.o: hw/startup.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Linking
$(BUILD_DIR)/final.elf: $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o $@

# Convert ELF to HEX
$(BUILD_DIR)/final.hex: $(BUILD_DIR)/final.elf
	$(OBJCOPY) -O ihex $< $@

# --- Simulation & Utility ---

# Run QEMU in wait mode (-S stops at reset, -s starts GDB server on 1234)
sim: all
	qemu-system-arm -M netduinoplus2 -kernel $(BUILD_DIR)/final.elf -nographic -S -s

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean sim
