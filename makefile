CXX = g++
CCFLAGS = --std=c++17 -g

.cpp.o:
	$(CXX) $(CCFLAGS) -c -O3 $<

all: cache_test clean

cache_test: cache.o test_cache_lib.o fifo_evictor.o
	$(CXX) $(CCFLAGS) -o test_cache_lib cache.o test_cache_lib.o fifo_evictor.o

cache.o:	cache.cc
	$(CXX) $(CCFLAGS) -c cache.cc

test_cache_lib.o: test_cache_lib.cc
	$(CXX) $(CCFLAGS) -c test_cache_lib.cc

fifo_evictor.o: fifo_evictor.cc
	$(CXX) $(CCFLAGS) -c fifo_evictor.cc

clean:
	rm -f *.o