CXX = g++
CXXFLAGS = -Wall -pthread
TARGET = linuxmanager
Source = $(wildcard ./Network/*.cpp ./Process/*.cpp ./System/*.cpp) main.cpp

$(TARGET): 
	$(CXX) $(CXXFLAGS) $(Source) -o $(TARGET) 
