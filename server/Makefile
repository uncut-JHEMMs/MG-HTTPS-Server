CXX = clang++
CXXFLAGS = -std=c++17 -Wall -g -lhttpserver

all: service latency

service: server.cpp
	$(CXX) $(CXXFLAGS) server.cpp -o service 

latency: latencyCalc.cpp
	clang++ -std=c++17 -Wall latencyCalc.cpp -o latencyCalc 

# disable built-in rules
.SUFFIXES:

# these targets do not create any files
.PHONY: clean
clean:
	rm -vrf *.o *.exe *.d
	rm -vf service latencyCalc
