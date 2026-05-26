CXX      = g++
CXXFLAGS = -O2 -std=c++17

all: main main2

main: main.cpp tsp_sa.h
	$(CXX) $(CXXFLAGS) -o main main.cpp

main2: main2.cpp tsp_sa.h
	$(CXX) $(CXXFLAGS) -o main2 main2.cpp

clean:
	rm -f main main2
