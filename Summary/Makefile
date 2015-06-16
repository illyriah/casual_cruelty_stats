CXX=g++
CXXFLAGS=-lcurl -lxml2 -I/usr/include/libxml2 -I../XLR_Parser -std=c++11
objects=../XLR_Parser/tmp/*.o

all: server_summary

server_summary: server_summary.cpp
	$(MAKE) -C ../XLR_Parser
	$(CXX) $^ -o $@ $(objects) $(CXXFLAGS)
