CC = g++

TARGET := final_program.exe

SRC_DIR := src
BUILD_DIR := build

SRCEXT := cpp

CFILES := $(shell find $(SRC_DIR) -type f -name *.c)
CPPFILES := $(shell find $(SRC_DIR) -type f -name *.cpp)

OBJS :=$(patsubst $(SRC_DIR)/%,$(BUILD_DIR)/%, $(CPPFILES:.cpp=.o) $(CFILES:.c=.o))

glad = ./external/glad

INC := -I./external/include -I./src/
LIB := -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl
CFLAGS := -std=c++17 -g

$(TARGET): $(OBJS)
	@echo "Linking"
	$(CC) -o $@ $^ $(LIB)
$(BUILD_DIR)/%.o : $(SRC_DIR)/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INC) -c -o $@ $^
$(BUILD_DIR)/%.o : $(SRC_DIR)/%.cpp
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INC) -c -o $@ $^
.PHONY : clean
clean:
	rm -f $(OBJS)
