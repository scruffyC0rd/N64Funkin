# Target ROM name
TARGET = test
BUILD_DIR = build
FSYSTEM_DIR = filesystem

include $(N64_INST)/include/n64.mk

# Object files
OBJS = $(BUILD_DIR)/$(TARGET).o

# ROM build rule
$(TARGET).z64: N64_ROM_TITLE = "test"
$(TARGET).z64: $(BUILD_DIR)/$(TARGET).dfs

# DFS rule (filesystem support)
$(BUILD_DIR)/$(TARGET).dfs: $(wildcard $(FSYSTEM_DIR)/*)

# ELF rule
$(BUILD_DIR)/$(TARGET).elf: $(OBJS)

# Clean rule
clean:
	rm -rf $(BUILD_DIR) *.z64
.PHONY: clean
