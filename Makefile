SAC2C ?= sac2c
TARGETS ?= "seq;seq_checks;mt_pth"
BUILD_DIR ?= build

.PHONY: all build clean

all: build

build:
	cmake -DSAC2C_EXEC=$(SAC2C) -DTARGETS=$(TARGETS) -B $(BUILD_DIR)
	cmake --build $(BUILD_DIR)

clean:
	$(RM) -r $(BUILD_DIR)
