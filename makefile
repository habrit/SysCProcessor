# Makefile
SYSTEMC_HOME = $(CONDA_PREFIX)
SYSTEMC_LIBDIR = $(SYSTEMC_HOME)/lib
SYSTEMC_INCDIR = $(SYSTEMC_HOME)/include

CXX = g++
CXXFLAGS = -std=c++17 -I. -I$(SYSTEMC_INCDIR) -L$(SYSTEMC_LIBDIR) -lsystemc -lm
TARGET = sim.out



all: $(TARGET)

$(TARGET): ALU.o ALUtb.o
	$(CXX) -o $(TARGET) ALU.o ALUtb.o $(CXXFLAGS)

ALU.o: ALU.cpp
	$(CXX) -c ALU.cpp $(CXXFLAGS) $(CPPFLAGS)

ALUtb.o: ALUtb.cpp
	$(CXX) -c ALUtb.cpp $(CXXFLAGS) $(CPPFLAGS)

clean:
	rm -f *.o $(TARGET)

