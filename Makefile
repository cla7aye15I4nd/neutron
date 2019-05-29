CXX = g++
CXXFLAGS = -std=c++14 -O3 src/main.cpp
make : code
code:
	$(CXX) -o $@ $(CXXFLAGS)
