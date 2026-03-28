CXX = g++
CXXFLAGS = -std=c++17 -O2 -Wall
SOURCES = main.cpp Player.cpp Enemy.cpp Cell.cpp GameField.cpp GameManager.cpp
TARGET = game

$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SOURCES)

clean:
	rm -f $(TARGET)

run: $(TARGET)
	./$(TARGET)

.PHONY: clean run
