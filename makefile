PROJECT_NAME = TicTacToe

# Compiler
CXX = g++
CXXFLAGS = -W -std=c++17

# Directories
SRC_DIR = src
RES_DIR = res
BIN_DIR = bin

# Files
EXEC = $(BIN_DIR)/$(PROJECT_NAME)
SRCS = $(wildcard $(SRC_DIR)/*.cpp)
# TODO add object file variable and rules
# to allow rebuilding updated files only

# SFML library (edit paths as necessary)
SFML_DIR = C:/SFML-2.5.1
SFML_BIN = $(SFML_DIR)/bin
SFML_LIB = -L $(SFML_DIR)/lib
SFML_INCLUDE = -I $(SFML_DIR)/include
SFML_FLAGS = -lsfml-graphics -lsfml-window -lsfml-system -lsfml-network

.DEFAULT_GOAL = build

# Build
.PHONY: build
build:
	mkdir -p $(BIN_DIR)
	cp $(SFML_BIN)/sfml-graphics-2.dll $(BIN_DIR)/
	cp $(SFML_BIN)/sfml-window-2.dll $(BIN_DIR)/
	cp $(SFML_BIN)/sfml-system-2.dll $(BIN_DIR)/
	cp $(SFML_BIN)/sfml-network-2.dll $(BIN_DIR)/
	cp -r $(RES_DIR)/ $(BIN_DIR)/
	$(CXX) $(CXXFLAGS) -o $(EXEC) $(SRCS) $(SFML_INCLUDE) $(SFML_LIB) $(SFML_FLAGS)

# Run
.PHONY: run
run:
	./$(BIN_DIR)/$(PROJECT_NAME)

# Clean
.PHONY: clean
clean:
	rm -r $(BIN_DIR)