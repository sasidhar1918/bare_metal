# --- Project Configuration ---
PROJECT_NAME = stm32f4_uart_driver
BUILD_DIR = build
SRC_DIR = src
SCRIPTS_DIR = scripts

# --- Toolchain Selection ---
CC = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy
SIZE = arm-none-eabi-size

# --- CPU Architecture Flags ---
# STM32F407 is a Cortex-M4 with FPU
CPU = -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16

# --- Compilation Flags ---
# -Wall: Show all warnings
# -O0: No optimization (best for debugging in Renode)
# -g: Include debug symbols
# -I: Include directories for headers
CFLAGS = $(CPU) -Wall -O0 -g \
         -Iinclude \
         -I$(SRC_DIR)/hal \
         -I$(SRC_DIR)/driver \
         -I$(SRC_DIR)/app \
         -I$(SRC_DIR)/system

# --- Linker Flags ---
# -T: Point to your specific linker script
LDFLAGS = $(CPU) -T $(SCRIPTS_DIR)/linker_script.ld \
          --specs=nosys.specs -nostartfiles -Wl,-Map=$(BUILD_DIR)/$(PROJECT_NAME).map

# --- Source Files Discovery ---
# This finds all .c files in your subdirectories
SRCS = $(shell find $(SRC_DIR) -name "*.c")
OBJS = $(SRCS:%.c=$(BUILD_DIR)/%.o)

# --- Build Rules ---

all: $(BUILD_DIR)/$(PROJECT_NAME).elf $(BUILD_DIR)/$(PROJECT_NAME).bin

# Link the objects into the ELF file
$(BUILD_DIR)/$(PROJECT_NAME).elf: $(OBJS)
	@mkdir -p $(@D)
	$(CC) $(OBJS) $(LDFLAGS) -o $@
	$(SIZE) $@

# Convert ELF to BIN for Renode
$(BUILD_DIR)/$(PROJECT_NAME).bin: $(BUILD_DIR)/$(PROJECT_NAME).elf
	$(OBJCOPY) -O binary $< $@

# Compile C files into Object files
$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)

# --- Debugging Help ---
print_srcs:
	@echo "Source files found:"
	@echo $(SRCS)

.PHONY: all clean print_srcs
