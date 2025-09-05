# ----------------------------
# Project settings
# ----------------------------
TARGET       = N64Funkin
BUILD_DIR    = build
FSYSTEM_DIR  = filesystem
ASSETS_DIR   = assets

# Select TV system (NTSC or PAL). Default = NTSC
TV ?= NTSC

# Import Libdragon rules
include $(N64_INST)/include/n64.mk

# ----------------------------
# Sources
# ----------------------------
SRC  = main.c

OBJS = $(SRC:%.c=$(BUILD_DIR)/%.o)

# ----------------------------
# Assets
# ----------------------------
# Recursive search for PNG and TTF
ASSETS_PNG  = $(shell find $(ASSETS_DIR) -type f -name '*.png')
ASSETS_TTF  = $(shell find $(ASSETS_DIR) -type f -name '*.ttf')

# Mirror directory structure into filesystem/
ASSETS_CONV = $(patsubst $(ASSETS_DIR)/%, $(FSYSTEM_DIR)/%,$(ASSETS_PNG:%.png=%.sprite)) \
              $(patsubst $(ASSETS_DIR)/%, $(FSYSTEM_DIR)/%,$(ASSETS_TTF:%.ttf=%.font64))

# Default sprite/font conversion flags
MKSPRITE_FLAGS ?= --format RGBA16
MKFONT_FLAGS   ?= --size 20

# ----------------------------
# Build rules
# ----------------------------
all: $(TARGET).z64

# Generic PNG → sprite
$(FSYSTEM_DIR)/%.sprite: $(ASSETS_DIR)/%.png
	@mkdir -p $(dir $@)
	@echo "    [SPRITE] $@"
	$(N64_MKSPRITE) $(MKSPRITE_FLAGS) -o $(dir $@) "$<"

# Special case: bg.png → tiled sprite
$(FSYSTEM_DIR)/bg.sprite: $(ASSETS_DIR)/bg.png
	@mkdir -p $(dir $@)
	@echo "    [SPRITE] $@ (tiled)"
	$(N64_MKSPRITE) --format RGBA16 --tiles 32,32 -o $(dir $@) "$<"

# TTF → font
$(FSYSTEM_DIR)/%.font64: $(ASSETS_DIR)/%.ttf
	@mkdir -p $(dir $@)
	@echo "    [FONT] $@"
	$(N64_MKFONT) $(MKFONT_FLAGS) -o $(dir $@) "$<"

# Ensure build dir exists
$(BUILD_DIR):
	@mkdir -p $@

# Depend on converted assets
$(BUILD_DIR)/$(TARGET).dfs: $(ASSETS_CONV)

# ELF links in object + DFS
$(BUILD_DIR)/$(TARGET).elf: $(OBJS)

# Final ROM (passes TV system variable)
$(TARGET).z64: N64_ROM_TITLE="Friday Night Funkin"
$(TARGET).z64: TV=$(TV)
$(TARGET).z64: $(BUILD_DIR)/$(TARGET).dfs

# ----------------------------
# Clean
# ----------------------------
clean:
	rm -rf $(BUILD_DIR) $(FSYSTEM_DIR) $(TARGET).z64

-include $(wildcard $(BUILD_DIR)/*.d)

.PHONY: all clean
