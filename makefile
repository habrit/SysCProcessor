# Makefile
SYSTEMC_HOME = $(CONDA_PREFIX)
SYSTEMC_LIBDIR = $(SYSTEMC_HOME)/lib
SYSTEMC_INCDIR = $(SYSTEMC_HOME)/include

CXX = g++
CXXFLAGS = -std=c++17 -I. -I$(SYSTEMC_INCDIR) -L$(SYSTEMC_LIBDIR) -lsystemc -lm
TARGET = sim.out

# List of object files

OBJS = ALU.o main.o ALUtb.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(CXXFLAGS)

ALU.o: ALU.cpp
	$(CXX) -c ALU.cpp $(CXXFLAGS) $(CPPFLAGS)


ALUtb.o: ALUtb.cpp
	$(CXX) -c ALUtb.cpp $(CXXFLAGS) $(CPPFLAGS)

main.o: main.cpp
	$(CXX) -c main.cpp $(CXXFLAGS) $(CPPFLAGS)

#4bitadder.o: 4bitadder.cpp
#	$(CXX) -c 4bitadder.cpp $(CXXFLAGS) $(CPPFLAGS)

#4bitaddertb.o: 4bitaddertb.cpp
#	$(CXX) -c 4bitaddertb.cpp $(CXXFLAGS) $(CPPFLAGS)


clean:
	rm -f *.o $(TARGET)

