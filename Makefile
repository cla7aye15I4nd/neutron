CXX = g++
CXXFLAGS = -std=c++14 -O3 src/main.cpp
make : clean code
clean:
	rm code
code:
	$(CXX) -o $@ $(CXXFLAGS)
