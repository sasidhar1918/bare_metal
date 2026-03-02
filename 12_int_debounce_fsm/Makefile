# --- Project Configuration ---
PROJECT_NAME = stm32f4_debounce
BUILD_DIR    = build
SRC_DIR      = src
INC_DIR      = include

# --- Toolchain Selection ---
CC      = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy
SIZE    = arm-none-eabi-size

# --- CPU Architecture Flags ---
# Targeting Cortex-M4 as defined in .repl 
CPU_FLAGS = -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16

# --- Compilation Flags ---
# Includes paths for the layered architecture
CFLAGS  = $(CPU_FLAGS) -I$(INC_DIR) -I$(SRC_DIR)/app -I$(SRC_DIR)/bsp -I$(SRC_DIR)/hw_base
CFLAGS += -Wall -Wextra -O0 -g3 -std=c11 -ffunction-sections -fdata-sections

# --- Linker Flags ---
# Points to the linker script in the /linker directory
LDFLAGS  = $(CPU_FLAGS) -T linker/stm32f4.ld -Wl,-Map=$(BUILD_DIR)/$(PROJECT_NAME).map,--gc-sections

# --- Source Files ---
SRCS  = $(SRC_DIR)/hw_base/startup.c
SRCS += $(SRC_DIR)/bsp/gpio.c
SRCS += $(SRC_DIR)/app/debounce_fsm.c
SRCS += $(SRC_DIR)/app/main.c

# --- Object Files ---
OBJS = $(addprefix $(BUILD_DIR)/, $(notdir $(SRCS:.c=.o)))
vpath %.c $(SRC_DIR)/hw_base $(SRC_DIR)/bsp $(SRC_DIR)/app

# --- Build Rules ---

all: $(BUILD_DIR)/$(PROJECT_NAME).elf $(BUILD_DIR)/$(PROJECT_NAME).bin

$(BUILD_DIR)/%.o: %.c | $(BUILD_DIR)
	@echo "Compiling: $<"
	@$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/$(PROJECT_NAME).elf: $(OBJS)
	@echo "Linking: $@"
	@$(CC) $(OBJS) $(LDFLAGS) -o $@
	@$(SIZE) $@

$(BUILD_DIR)/$(PROJECT_NAME).bin: $(BUILD_DIR)/$(PROJECT_NAME).elf
	@$(OBJCOPY) -O binary $< $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean
