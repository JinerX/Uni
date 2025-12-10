# Compiler settings
CXX := g++
CC  := gcc

CXXFLAGS := -std=c++17 -Iinclude -Wall -Wextra -O2
CFLAGS   := -Iinclude -Wall -Wextra -O2

# Sources
CPP_SRCS := $(wildcard src/*.cpp)
C_SRCS   := $(wildcard src/*.c)
MAIN     := main.cpp

# Objects
CPP_OBJS := $(CPP_SRCS:.cpp=.o)
C_OBJS   := $(C_SRCS:.c=.o)
MAIN_OBJ := $(MAIN:.cpp=.o)

OBJS := $(CPP_OBJS) $(C_OBJS) $(MAIN_OBJ)

# Output executable
TARGET := SHORTEST_PATH

# Default rule
all: $(TARGET)

# Link rule
$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET)

# Compilation rules
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Cleanup
clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
