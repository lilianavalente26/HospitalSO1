CC = gcc
CFLAGS = -Wall -Wextra -std=c99
LDFLAGS = -lm
SRC_DIR = src
INCLUDE_DIR = include
OBJ_DIR = obj
BIN_DIR = bin

SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))
TARGET = $(BIN_DIR)/HospitalSO1

$(TARGET): $(OBJS)
    $(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
    $(CC) $(CFLAGS) -c $< -o $@ -I$(INCLUDE_DIR)

clean:
    rm -f $(OBJ_DIR)/*.o $(TARGET)

.PHONY: clean
