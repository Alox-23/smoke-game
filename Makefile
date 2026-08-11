CC        := gcc
CSTD      := -std=c23
WARN      := -Wall -Wextra -Wpedantic
OPT       := -O2
DBG       := -g

SDL_CFLAGS := -I/usr/include/SDL2 -D_REENTRANT
SDL_LIBS := -lSDL2 -lSDL2_image -lm

SRC_DIR   := src
BUILD_DIR := build
BIN       := $(BUILD_DIR)/game

SRCS := $(shell find $(SRC_DIR) -name '*.c')
OBJS := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRCS))
DEPS := $(OBJS:.o=.d)

CFLAGS  := $(CSTD) $(WARN) $(OPT) $(SDL_CFLAGS) -MMD -MP -I$(SRC_DIR)
LDFLAGS :=
LDLIBS  := $(SDL_LIBS)

.PHONY: all debug clean run

all: $(BIN)

debug: CFLAGS += $(DBG) -fsanitize=address,undefined -O0
debug: LDFLAGS += -fsanitize=address,undefined
debug: clean $(BIN)

$(BIN): $(OBJS)
	@mkdir -p $(dir $@)
	$(CC) $(LDFLAGS) $(OBJS) -o $@ $(LDLIBS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

-include $(DEPS)

run: $(BIN)
	./$(BIN)

clean:
	rm -rf $(BUILD_DIR)
