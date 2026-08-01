# 1. DETECT HOST ENVIRONMENT
ifeq ($(OS),Windows_NT)
    HOST_OS  := Windows
    HOST_ARCH:= x86_64
else
    HOST_OS  := $(shell uname -s)
    HOST_ARCH:= $(shell uname -m)
endif

# 2. CHOOSE TARGET CONFIGURATION (Defaults to host architecture)
# Options: x86_64, x86, rpi3
TARGET_ARCH ?= $(HOST_ARCH)

CFLAGS = -I./include -I./external/pffft/include/pffft -Wall -Wextra -O3

# 3. SET COMPILER AND FLAGS BASED ON ARCHITECTURE
ifeq ($(TARGET_ARCH),rpi3)
    # Raspberry Pi 3 Target (ARMv7 Hard Float)
    # If we are already on an ARMv7/ARMv8 Pi, use native GCC. Otherwise, use cross-compiler.
    ifneq ($(HOST_ARCH),armv7l)
        ifneq ($(HOST_ARCH),aarch64)
            CROSS_COMPILE := arm-linux-gnueabihf-
        endif
    endif

    CC       := $(CROSS_COMPILE)gcc
    CXX      := $(CROSS_COMPILE)g++
    # Optimization flags specific to Raspberry Pi 3 Cortex-A53
    CFLAGS   += -march=armv7-a -mfpu=neon-vfpv4 -mfloat-abi=hard
    EXE_EXT  :=

else ifeq ($(TARGET_ARCH),x86)
    # 32-bit x86 Target
    CC       := gcc
    CXX      := g++
    CFLAGS   += -m32
    EXE_EXT  :=

else ifeq ($(TARGET_ARCH),x86_64)
    # 64-bit x86 Target
    CC       := gcc
    CXX      := g++
    CFLAGS   += -m64
    EXE_EXT  :=
endif

# 4. SYSTEM METADATA AND CLEANUP VARIATION
ifeq ($(HOST_OS),Windows)
    EXE_EXT := .exe
    RM      := del /Q
else
    RM      := rm -rf
endif


# 5. BUILD RULES
TARGET := build/program_$(TARGET_ARCH)$(EXE_EXT)
SRCS   := src/main.c src/synth.c src/voice.c src/presets.c external/pffft/src/pffft.c external/pffft/src/pffft_common.c
OBJS   := $(addprefix build/,$(SRCS:.c=.o))

.PHONY: all clean configure

all: $(TARGET)

$(TARGET): $(OBJS)
	@mkdir -p $(@D)
	$(CC) $(OBJS) -o $@ -lasound -pthread -lm

build/%.o: %.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

configure:
	sudo apt-get install libasound2-dev

run: all
	./$(TARGET)

clean:
	$(RM) build/**/*.o $(TARGET)