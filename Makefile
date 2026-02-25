SAC2C ?= sac2c
# Uniqueness violation bug in seq_checks
#TARGETS ?= "seq;seq_checks;mt_pth"
TARGETS ?= "seq;mt_pth"
BUILD_DIR ?= build

.PHONY: all build clean

all: build

build:
	cmake -DSAC2C_EXEC=$(SAC2C) -DTARGETS=$(TARGETS) -B $(BUILD_DIR)
	cmake --build $(BUILD_DIR)

clean:
	$(RM) -r $(BUILD_DIR)
