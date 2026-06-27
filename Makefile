CROSS_COMPILE := aarch64-linux-gnu-
CC            := $(CROSS_COMPILE)gcc
AR            := $(CROSS_COMPILE)ar
OBJDUMP       := $(CROSS_COMPILE)objdump

SRC_DIR       := src
ASM_DIR       := asm
INC_DIR       := include

BUILD_DIR     ?= build
OBJ_DIR       := $(BUILD_DIR)/obj

LIB_BASENAME  ?= axlib
LIB_NAME      := $(BUILD_DIR)/lib$(LIB_BASENAME).a

CSRCS         := $(wildcard $(SRC_DIR)/*.c)
ASRCS         := $(wildcard $(ASM_DIR)/*.S)

COBJS         := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(CSRCS))
AOBJS         := $(patsubst $(ASM_DIR)/%.S,$(OBJ_DIR)/%.o,$(ASRCS))

OBJS          := $(COBJS) $(AOBJS)

CPPFLAGS      += -I$(INC_DIR)
CFLAGS        += -march=armv8-a -ffreestanding -fno-builtin -Wall -Wextra -O2
ARFLAGS       ?= rcs

MKDIR_P       ?= mkdir -p
RM_RF         ?= rm -rf

.PHONY: all clean list help

all: $(LIB_NAME)

$(OBJ_DIR):
	$(MKDIR_P) $@

$(BUILD_DIR):
	$(MKDIR_P) $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	@echo "CC  $<"
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(ASM_DIR)/%.S | $(OBJ_DIR)
	@echo "AS  $<"
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(LIB_NAME): $(OBJS) | $(BUILD_DIR)
	@echo "AR  $@"
	@echo "---------------------------------------"
	@echo "axLib build"
	@echo "---------------------------------------"
	$(AR) $(ARFLAGS) $@ $(OBJS)

list: $(LIB_NAME)
	$(AR) -t $(LIB_NAME)
	$(OBJDUMP) -t $(LIB_NAME)

clean:
	$(RM_RF) $(BUILD_DIR)

help:
	@echo "Targets:"
	@echo "  all   - build the static library"
	@echo "  list  - show archive members and symbols"
	@echo "  clean - remove build artifacts"