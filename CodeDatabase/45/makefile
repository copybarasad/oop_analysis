CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Wpedantic
TARGET = game
SOURCES = main.cpp entity.cpp player.cpp monster.cpp cell.cpp map.cpp hand.cpp spell.cpp direct_spell.cpp area_spell.cpp trap_spell.cpp guard_tower.cpp game_controller.cpp save_loader.cpp command.cpp input_master.cpp keyboard_input.cpp visual_master.cpp visual_terminal.cpp input_handler.cpp game_drawer.cpp
OBJECTS = $(SOURCES:.cpp=.o)

all: clear $(TARGET)
	./$(TARGET)

clear:
	clear

$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)

.PHONY: all clean clear