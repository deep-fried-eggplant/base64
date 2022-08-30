CXX = g++
INCLUDE = -I../include
CXXFLAGS = -Wall -O2

all : 
	@echo "specify a program"

test : test.cpp
	$(CXX) $(INCLUDE) $^ $(CXXFLAGS) -o $@

encode : encode.cpp
	$(CXX) $(INCLUDE) $^ $(CXXFLAGS) -o $@

decode : decode.cpp
	$(CXX) $(INCLUDE) $^ $(CXXFLAGS) -o $@