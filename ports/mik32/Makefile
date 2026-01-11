include ../../py/mkenv.mk

# Include py core make definitions.
include $(TOP)/py/py.mk

# Set makefile-level MicroPython feature configurations.
MICROPY_ROM_TEXT_COMPRESSION ?= 0

CROSS_COMPILE ?= riscv-none-elf-

INC += -I.
INC += -I$(TOP)
INC += -I$(BUILD)
INC += -I$(TOP)/lib/mik32v2_shared/include
INC += -I$(TOP)/lib/mik32v2_shared/periphery
INC += -I$(TOP)/lib/mik32v2_shared/libs

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
	shared/libc/string0.c \
	shared/libc/printf.c \
    shared/runtime/pyexec.c \
	shared/readline/readline.c \
	shared/runtime/stdout_helpers.c \
	shared/runtime/gchelper_native.c

SRC_QSTR += shared/readline/readline.c shared/runtime/pyexec.c

SRC_S += lib/mik32v2_shared/runtime/crt0.S
SRC_s += shared/runtime/gchelper_rv32i.s

# Define the required object files.
OBJ += $(PY_CORE_O)
OBJ += $(addprefix $(BUILD)/, $(SRC_C:.c=.o))
OBJ += $(addprefix $(BUILD)/, $(SRC_S:.S=.o))
OBJ += $(addprefix $(BUILD)/, $(SRC_s:.s=.o))

all: $(BUILD)/firmware.hex

$(BUILD)/firmware.elf: $(OBJ)
	$(ECHO) "LINK $@"
	$(Q)$(LD) $(LDFLAGS) -o $@ $^
	$(Q)$(SIZE) $@

$(BUILD)/firmware.hex: $(BUILD)/firmware.elf
	$(ECHO) "Create $@"
	$(Q)$(OBJCOPY) -O ihex -j .text -j .data -j .startup -j .ram_text \
		-j .preinit_array -j .init_array -j .fini_array \
		-j .tdata -j .srodata $^ $@

# Include remaining core make rules.
include $(TOP)/py/mkrules.mk
