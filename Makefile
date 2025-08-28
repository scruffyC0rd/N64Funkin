# Target ROM name
TARGET = test
BUILD_DIR = build
FSYSTEM_DIR = filesystem
ASSETS_DIR = assets

include $(N64_INST)/include/n64.mk

# Source files
src = main.c
OBJS = $(BUILD_DIR)/$(TARGET).o

# Asset conversion (PNG → SPRITE in filesystem/)
ASSETS_PNG  = $(wildcard $(ASSETS_DIR)/*.png)
ASSETS_SPR  = $(patsubst $(ASSETS_DIR)/%.png,$(FSYSTEM_DIR)/%.sprite,$(ASSETS_PNG))

MKSPRITE_FLAGS ?=

all: $(TARGET).z64

# Rule to convert PNG → SPRITE inside filesystem
$(FSYSTEM_DIR)/%.sprite: $(ASSETS_DIR)/%.png
	@mkdir -p $(FSYSTEM_DIR)
	@echo "    [SPRITE] $@"
	@$(N64_MKSPRITE) --format RGBA32 $(MKSPRITE_FLAGS) -o $(FSYSTEM_DIR) "$<"

# Example: custom flags for specific sprites
$(FSYSTEM_DIR)/BF.sprite: MKSPRITE_FLAGS=--format RGBA32 --tiles 32,32
$(FSYSTEM_DIR)/DAD.sprite: MKSPRITE_FLAGS=--format RGBA32 --tiles 32,32

# DFS rule: bundle converted sprites into filesystem
$(BUILD_DIR)/$(TARGET).dfs: $(ASSETS_SPR)

# ELF rule
$(BUILD_DIR)/$(TARGET).elf: $(OBJS)

# ROM rule
$(TARGET).z64: N64_ROM_TITLE = "N64Funkin"
$(TARGET).z64: $(BUILD_DIR)/$(TARGET).dfs

# Clean rule
clean:
	rm -rf $(BUILD_DIR) $(FSYSTEM_DIR) *.z64
.PHONY: clean
