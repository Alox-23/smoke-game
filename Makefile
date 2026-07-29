CC := gcc
CSTD := -std=c23
WARN := -Wall -Wextra -Wpedantic
OPT := -O2
DBG := -g

SDL_CFLAGS := $(shell pkg-config --cflags sdl2 SDL2_image SDL2_mixer SDL2_ttf)
SDL_LIBS   := $(shell pkg-config --libs sdl2 SDL2_image SDL2_mixer SDL2_ttf)

SRC_DIR := src
BUILD_DIR := build
BIN := $(BUILD_DIR)/game

SRCS := $(shell find $(SRC_DIR) -name '*.c')
OBJS := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRCS))
DEPS := $(OBJS:.o=.d)

CFLAGS := $(CSTD) $(WARN) $(OPT) $(SDL_CFLAGS) -MMD -MP -I$(SRC_DIR)
LDFLAGS := $(SDL_LIBS) -lm

.PHONY: all debug clean run

all: $(BIN)

debug: CFLAGS += $(DBG) -fsanitize=address,undefined -O0
debug: LDFLAGS += -fsanitize=address,undefined
debug: clean $(BIN)

$(BIN): $(OBJS)
	@mkdir -p $(dir $@)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

-include $(DEPS)

run: $(BIN)
	./$(BIN)

clean:
	rm -rf $(BUILD_DIR)
