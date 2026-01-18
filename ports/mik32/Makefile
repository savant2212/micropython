BOARD ?= ELBEAR-ACE-UNO
BOARD_DIR ?= boards/$(BOARD)

ifeq ($(wildcard $(BOARD_DIR)/.),)
$(error Invalid BOARD specified: $(BOARD_DIR))
endif

# Make the build directory reflect the board.
BUILD ?= build-$(BOARD)
GIT_SUBMODULES += lib/mik32v2_shared

include ../../py/mkenv.mk

# Include board specific .mk file.
include $(BOARD_DIR)/mpconfigboard.mk

# Include py core make definitions.
include $(TOP)/py/py.mk

# Set makefile-level MicroPython feature configurations.
MICROPY_ROM_TEXT_COMPRESSION ?= 0

CROSS_COMPILE ?= riscv-none-elf-

INC += -I.
INC += -I./boards/$(BOARD)
INC += -I$(TOP)
INC += -I$(BUILD)
INC += -I$(TOP)/lib/mik32v2_shared/include
INC += -I$(TOP)/lib/mik32v2_shared/periphery
INC += -I$(TOP)/lib/mik32v2_shared/libs

GEN_PIN_MKPINS = make-pins.py
GEN_PIN_PREFIX = pins_prefix.c
GEN_PINS_BOARD_CSV = $(BOARD_DIR)/pins.csv
GEN_PINS_MCU_CSV = ./mik32-pin-alt.csv
GEN_PINS_SRC = $(BUILD)/pins_board.c
GEN_PINS_HDR = $(HEADER_BUILD)/pins_board.h

# Set CFLAGS.
CFLAGS += $(INC) -Wall -Werror -std=c99 -nostdlib
CFLAGS += -march=rv32imc_zicsr_zifencei -mabi=ilp32
CFLAGS += -msmall-data-limit=8 -mstrict-align -mno-save-restore
CFLAGS += -fdata-sections -ffunction-sections
CSUPEROPT = -Os # save some code space for performance-critical code

AFLAGS += -march=rv32imc_zicsr_zifencei -mabi=ilp32

# Select debugging or optimisation build.
ifeq ($(DEBUG), 1)
CFLAGS += -Og -ggdb3
else
CFLAGS += -Os -DNDEBUG
endif

# Set linker flags.
LDFLAGS += -nostdlib -T k1948.ld --gc-sections

# Define the required source files.
SRC_C += lib.c system.c main.c uart_core.c \
	lib/mik32v2_shared/libs/uart_lib.c \
	$(wildcard $(BOARD_DIR)/*.c)

SHARED_SRC_C +=	shared/libc/string0.c \
	shared/libc/printf.c \
    shared/runtime/pyexec.c \
	shared/readline/readline.c \
	shared/runtime/stdout_helpers.c \
	shared/runtime/gchelper_native.c

SRC_S += lib/mik32v2_shared/runtime/crt0.S
SRC_s += shared/runtime/gchelper_rv32i.s

SRC_QSTR += shared/readline/readline.c shared/runtime/pyexec.c
SRC_QSTR += $(SHARED_SRC_C) $(SRC_C) $(GEN_PINS_SRC)

# Define the required object files.
OBJ += $(PY_CORE_O)
OBJ += $(addprefix $(BUILD)/, $(SRC_C:.c=.o))
OBJ += $(addprefix $(BUILD)/, $(GEN_PINS_SRC:.c=.o))
OBJ += $(addprefix $(BUILD)/, $(SHARED_SRC_C:.c=.o))
OBJ += $(addprefix $(BUILD)/, $(SRC_S:.S=.o))
OBJ += $(addprefix $(BUILD)/, $(SRC_s:.s=.o))


all: $(BUILD)/firmware.hex

$(BUILD)/firmware.elf: $(OBJ)
	$(ECHO) "LINK $@"
	$(Q)$(LD) $(LDFLAGS) -o $@ $^
	$(Q)$(SIZE) $@

$(BUILD)/%_board.c $(HEADER_BUILD)/%_board.h: $(BOARD_DIR)/%.csv $(GEN_PIN_MKPINS) $(GEN_PIN_PREFIX) | $(HEADER_BUILD)
	$(ECHO) "GEN $@"
	$(Q)$(PYTHON) $(GEN_PIN_MKPINS) \
	    --af-csv $(GEN_PINS_MCU_CSV) \
	    --board-csv $(GEN_PINS_BOARD_CSV) \
	    --prefix $(GEN_PIN_PREFIX) \
	    --output-source $(GEN_PINS_SRC) \
	    --output-header $(GEN_PINS_HDR)

$(BUILD)/firmware.hex: $(BUILD)/firmware.elf
	$(ECHO) "Create $@"
	$(Q)$(OBJCOPY) -O ihex -j .text -j .data -j .startup -j .ram_text \
		-j .preinit_array -j .init_array -j .fini_array \
		-j .tdata -j .srodata $^ $@

# Include remaining core make rules.
include $(TOP)/py/mkrules.mk
