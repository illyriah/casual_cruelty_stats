# Makefile for Web_Pong project.

# Compiler Variables
CXX=g++
CXXFLAGS=-fPIC -shared -lcurl -lxml2 -I/usr/include/libxml2 --std=c++11 -Wall

# Temperary Directory
TMPDIR=tmp

# Special Rules
.PHONY : all
.SECONDEXPANSION :

# List of Objects
OBJECTNAMES= get_player_names.cpp initialize_curl.cpp parse_XLR_stats.cpp \
	utils.cpp get_webpage.cpp get_map_name.cpp 

# Auto Created Varibles
OBJECTFILES=${OBJECTNAMES:%.cpp=${TMPDIR}/%.o}

all : tmp libxlr.so

libxlr.so : ${OBJECTFILES}
	$(CXX) $^ -o libxlr.so $(CXXFLAGS)

# Genaric Object Creation Rule
${OBJECTFILES} : $$(patsubst ${TMPDIR}/%.o,%.cpp,$$@) | ${TMPDIR}
	${CXX} ${CXXFLAGS} -c $< -o $@

#Web_Pong.o : Web_Pong.cpp Web_Pong.hpp
#$(CXX) $(CXXFLAGS) -c Web_Pong.cpp -o Web_Pong.o


# Create the temporary directory.
$(TMPDIR) :
	mkdir $(TMPDIR)

#clean:
#	rm -rf *o hello
