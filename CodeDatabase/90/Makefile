CXX := g++
CXXFLAGS := -std=c++20 -Wall -Wextra -pedantic -O2
TARGET := game
SOURCES := $(wildcard *.cpp)
HEADERS := $(wildcard *.h)
OBJECTS := $(SOURCES:.cpp=.o)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJECTS)

%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJECTS)

.PHONY: clean
