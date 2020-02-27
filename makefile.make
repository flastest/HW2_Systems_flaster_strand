CXX = g++
CCFLAGS = -Wall -Wextra -pedantic -Werror --std=c++17 -g

.cpp.o:
	$(CXX) $(CCFLAGS) -c -O3 $<

all: cache_test 

cache_test: cache.o test_cache.o
	$(CXX) $(CCFLAGS) -o test_cache cache.o test_cache.o

cache.o:	cache.cc
	$(CXX) $(CCFLAGS) -c cache.cc

test_cache.o: test_cache.cc
	$(CXX) $(CCFLAGS) -c test_cache.cc

clean:
	rm -f *.o