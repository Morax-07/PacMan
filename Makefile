# Compilatore e flag
CXX = compc++
CXXFLAGS = -Wall -Wextra




# Librerie e include
LDFLAGS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
INCLUDE = -Iinclude




# Directory e file
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin
SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRCS))
TARGET = $(BIN_DIR)/PacMan




# Versione standard (passabile da linea di comando: make STD_VERSION=26)
export STD_VERSION ?= 23




# Regole per la build
all: $(TARGET)
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $@ $(LDFLAGS)
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $< -o $@



clean:
	rm -rf $(OBJ_DIR)/*.o $(TARGET) gcm.cache



.PHONY: all clean%