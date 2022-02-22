CXX = clang++
CXXFLAGS = -std=c++17 -Wall

all: test

test: Settings_tests.exe
	./Settings_tests.exe

Settings_tests.exe: Settings_tests.cpp unit_test_framework.cpp
	$(CXX) $(CXXFLAGS) $< unit_test_framework.cpp -o $@

# disable built-in rules
.SUFFIXES:

# these targets do not create any files
.PHONY: clean
clean:
	rm -vrf *.o *.exe *.d *.gch *.dSYM *.stackdump *.out
