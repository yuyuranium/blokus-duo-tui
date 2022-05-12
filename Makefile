CC := gcc

INC_DIR := inc
SRC_DIR := src
BUILD_DIR := build

TARGETS := blokus  # Add targets here
SOURCES := $(addprefix $(SRC_DIR)/, $(TARGETS:=.c))
OBJECTS := $(addprefix $(BUILD_DIR)/, $(TARGETS:=.o))
HEADERS := $(wildcard $(INC_DIR)/*.h)

__dir := $(shell mkdir -p $(BUILD_DIR))

DEBUG = -g
CFLAGS = -Wall -I$(INC_DIR) $(DEBUG)
LDFLAGS = -lpthread

all: $(OBJECTS)

$(OBJECTS): $(SOURCES) $(HEADERS)
	@$(CC) -o $@ -c $< $(CFLAGS)
	@echo [CXX] $< "-> .o"

clean:
	rm -rf $(BUILD_DIR)
