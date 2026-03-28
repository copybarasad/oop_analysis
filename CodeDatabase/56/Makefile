
CXX = g++
CXXFLAGS = -std=c++17 -Wall
TARGET = game
SOURCES = main.cpp game_objects.cpp console_input.cpp Game.cpp player.cpp enemy.cpp building.cpp combat_system.cpp place.cpp field.cpp  spawn_system.cpp freezy_system.cpp movement_system.cpp AIEnemy.cpp trap_spell.cpp spell_tower.cpp Spell.cpp spell_cast.cpp area_spell.cpp directed_spell.cpp hand_spells.cpp 
OBJECTS = $(SOURCES:.cpp=.o)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)

.PHONY: clean