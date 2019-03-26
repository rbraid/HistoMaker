CPPFLAGS= -g -O3 -Wl,--no-as-needed $(shell grsi-config --cflags)
LDFLAGS= -g $(shell root-config --ldflags) $(shell grsi-config --root)
LDLIBS= $(shell grsi-config --all-libs)

EXE = RunMe

SRC_DIR = src
OBJ_DIR = obj

SRC = $(wildcard $(SRC_DIR)/*.cxx)
OBJ = $(SRC:$(SRC_DIR)/%.cxx=$(OBJ_DIR)/%.o)
# $(info OBJ is ${OBJ})

CPPFLAGS += -Iinclude
CFLAGS += -Wall
LDFLAGS += -Llib
LDLIBS += -lm

.PHONY: all clean

all: $(EXE)

$(EXE): $(OBJ)
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cxx
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

# clean:
# 	$(RM) $(OBJ)