# ===== Compiler: use clang++ on macOS =====
CXX := clang++
CXXFLAGS := -O3 -std=c++17 -Wall -Wextra
LDFLAGS :=

# Homebrew libomp paths
OMP_INC := $(shell brew --prefix libomp)/include
OMP_LIB := $(shell brew --prefix libomp)/lib

# ===== Dirs & Targets =====
SRC_DIR := src
BIN_DIR := bin
SERIAL  := $(BIN_DIR)/mm_serial
OPENMP  := $(BIN_DIR)/mm_openmp

.PHONY: all clean dirs run512 run1024 runomp512 runomp1024

all: dirs $(SERIAL) $(OPENMP)

dirs:
	@mkdir -p $(BIN_DIR) data

# Serial (no OpenMP flags needed)
$(SERIAL): $(SRC_DIR)/mm_serial.cpp
	$(CXX) $(CXXFLAGS) $< -o $@ $(LDFLAGS)

# OpenMP with clang++ + libomp
$(OPENMP): $(SRC_DIR)/mm_openmp.cpp
	$(CXX) $(CXXFLAGS) -Xpreprocessor -fopenmp -I$(OMP_INC) $< -L$(OMP_LIB) -lomp -o $@ $(LDFLAGS)

# Quick runs
run512: $(SERIAL)
	$(SERIAL) 512
run1024: $(SERIAL)
	$(SERIAL) 1024

runomp512: $(OPENMP)
	OMP_NUM_THREADS=4 $(OPENMP) 512
runomp1024: $(OPENMP)
	OMP_NUM_THREADS=4 $(OPENMP) 1024

clean:
	rm -rf $(BIN_DIR) *.o