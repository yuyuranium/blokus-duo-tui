CC := gcc

INC_DIR := inc
SRC_DIR := src
BUILD_DIR := build

HEADERS := $(wildcard $(INC_DIR)/*.h)
SOURCES := $(wildcard $(SRC_DIR)/*.c)
OBJECTS := $(SOURCES:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

__dir := $(shell mkdir -p $(BUILD_DIR))

DEBUG = -g
CFLAGS = -Wall -I$(INC_DIR) $(DEBUG)
LDFLAGS = -lpthread

all: $(OBJECTS)

$(OBJECTS): $(BUILD_DIR)/%.o: $(SRC_DIR)/%.c $(HEADERS)
	@$(CC) -o $@ -c $< $(CFLAGS)
	@echo [CXX] $< "-> .o"

clean:
	rm -rf $(BUILD_DIR)
