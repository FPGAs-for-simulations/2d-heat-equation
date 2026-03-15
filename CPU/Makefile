CXX = g++
CXXFLAGS = -Wall -O3 -std=c++17
TARGET = solver
SRC = solver.cpp

all: $(TARGET)

$(TARGET): $(SRC) config.h
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET) heat_history.csv analysis.png evolution.gif performance.csv

.PHONY: all clean
