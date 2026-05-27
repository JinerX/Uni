CXX      ?= g++
CXXFLAGS  = -std=c++17 -O2 -fopenmp
TARGET    = ga

$(TARGET): main.cpp
	$(CXX) $(CXXFLAGS) -o $(TARGET) main.cpp

clean:
	rm -f $(TARGET)

.PHONY: clean
