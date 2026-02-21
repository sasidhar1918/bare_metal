# --- Project Configuration ---
TARGET      = firmware
CPU         = -mcpu=cortex-m4 -mthumb
BUILD_DIR   = build

# --- Toolchain ---
CC      = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy
SIZE    = arm-none-eabi-size

# --- Paths ---
CMSIS_DIR     = drivers/CMSIS
LINKER_SCRIPT = hw/src/linker.ld

INC_FLAGS = -Ihw/inc \
            -I$(CMSIS_DIR)/Core/Include \
            -I$(CMSIS_DIR)/Device/Vendor/Include

# --- Flags ---
# -fno-builtin: Fixes the memcpy/memset error
CFLAGS  = $(CPU) -Wall -Wextra -O2 $(INC_FLAGS) -DSTM32F401xC -g -fno-builtin

# -Wl,-Map: Generates the memory map file
LDFLAGS = -T$(LINKER_SCRIPT) -nostdlib -nostartfiles --specs=nano.specs \
          -Wl,-Map=$(BUILD_DIR)/$(TARGET).map

# --- Source Files ---
SRCS    = app/main.c \
          hw/src/systick.c \
          hw/src/startup.c \
          $(CMSIS_DIR)/Device/Vendor/Source/system_stm32f4xx.c

# --- Object File Logic ---
# This converts 'app/main.c' into 'build/app/main.o'
OBJS = $(addprefix $(BUILD_DIR)/, $(SRCS:.c=.o))

# --- Rules ---

.PHONY: all clean

all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).bin $(BUILD_DIR)/$(TARGET).hex

# Link: Creates ELF and Map file
$(BUILD_DIR)/$(TARGET).elf: $(OBJS)
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(OBJS) -o $@ $(LDFLAGS)
	@echo ""
	@echo "--- Memory Usage ---"
	$(SIZE) $@

# Binary file
$(BUILD_DIR)/$(TARGET).bin: $(BUILD_DIR)/$(TARGET).elf
	$(OBJCOPY) -O binary $< $@

# Hex file
$(BUILD_DIR)/$(TARGET).hex: $(BUILD_DIR)/$(TARGET).elf
	$(OBJCOPY) -O ihex $< $@

# Compile: Creates .o files in build subdirectories
$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@echo "Cleaning build directory..."
	rm -rf $(BUILD_DIR)
