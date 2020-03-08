CXX = g++
CCFLAGS = -Wall -Wextra -pedantic -Werror --std=c++17 -g

.cpp.o:
	$(CXX) $(CCFLAGS) -c -O3 $<

all: cache_test clean

cache_test: cache.o test_cache_lib.o
	$(CXX) $(CCFLAGS) -o test_cache cache.o test_cache.o

cache.o:	cache.cc
	$(CXX) $(CCFLAGS) -c cache.cc

test_cache_lib.o: test_cache_lib.cc
	$(CXX) $(CCFLAGS) -c test_cache_lib.cc

clean:
	rm -f *.o