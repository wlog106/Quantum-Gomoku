CXX = g++
CXXFLAGS = -Wall -Wextra -O2 -pthread -MMD -MP

SRC = $(shell find ../lib -name "*.cpp") $(shell find ../client -name "*.cpp")

OBJ = $(SRC:.cpp=.o)
DEP = $(SRC:.cpp=.d)

TARGET = main

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $@ $(CXXFLAGS)

%.o: %.cpp
	$(CXX) -c $< -o $@ $(CXXFLAGS)

-include $(DEP)

clean:
	rm -f $(OBJ) $(DEP) $(TARGET)