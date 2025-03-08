V=1
SOURCE_DIR=src
BUILD_DIR=build
include $(N64_INST)/include/n64.mk

CFLAGS += -I$(MODELS_DIR)

all: BattleSeas.z64
.PHONY: all

SOURCES := $(wildcard $(SOURCE_DIR)/*.c )

OBJS := $(SOURCES:$(SOURCE_DIR)/%.c=$(BUILD_DIR)/%.o)

BattleSeas.z64: N64_ROM_TITLE="BattleSeas"

$(BUILD_DIR)/BattleSeas.elf: $(OBJS)

clean:
	rm -f $(BUILD_DIR)/* *.z64
.PHONY: clean

-include $(wildcard $(BUILD_DIR)/*.d)