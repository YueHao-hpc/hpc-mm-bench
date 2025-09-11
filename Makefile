# === Compiler & flags ===
CXX      := g++
CXXFLAGS := -O3 -march=native -std=c++17 -Wall -Wextra
LDFLAGS  :=

# Dirs
BIN_DIR  := bin
SRC_DIR  := src

# Targets
SERIAL   := $(BIN_DIR)/mm_serial

.PHONY: all clean dirs run run512 run1024

all: dirs $(SERIAL)

dirs:
	@mkdir -p $(BIN_DIR) data

# Serial version
$(SERIAL): $(SRC_DIR)/mm_serial.cpp
	$(CXX) $(CXXFLAGS) $< -o $@ $(LDFLAGS)

# Quick runs
run: all
	$(SERIAL) 512

run512: all
	$(SERIAL) 512

run1024: all
	$(SERIAL) 1024

clean:
	rm -rf $(BIN_DIR) *.o