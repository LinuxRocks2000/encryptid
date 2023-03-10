SRC_DIR := src
OBJ_DIR := build
BIN_DIR := lib

EXE := $(BIN_DIR)/libencryptid.so
SRC := $(wildcard $(SRC_DIR)/*.c)
OBJ := $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

CPPFLAGS := -Iinclude
CFLAGS   := -Wall -g
LDFLAGS  := -shared
LDLIBS   := -lm

.PHONY: all clean

all: $(EXE) test

$(EXE): $(OBJ) | $(BIN_DIR)
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(BIN_DIR) $(OBJ_DIR):
	mkdir -p $@

clean:
	@$(RM) -rv $(OBJ_DIR)

-include $(OBJ:.o=.d)

test: tests/test.c
	$(CC) -Llib -lencryptid -Iinclude -Wl,-rpath=$(PWD)/lib $< -o $@
