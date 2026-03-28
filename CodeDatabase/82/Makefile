CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -Wpedantic -O2
TARGET := game

SOURCES := $(filter-out test_%, $(wildcard *.cpp))
OBJECTS := $(SOURCES:.cpp=.o)

$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET)
	@echo "Сборка завершена: $(TARGET)"

%.o: %.cpp %.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c main.cpp -o main.o

clean:
	rm -f $(OBJECTS) $(TARGET)

run: $(TARGET)
	./$(TARGET)

rebuild: clean $(TARGET)

.PHONY: clean run rebuild