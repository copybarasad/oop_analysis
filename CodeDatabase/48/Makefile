CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2 -I.
TARGET = rpg_game

SOURCES = main.cpp \
          GameField.cpp \
          Player.cpp \
          Enemy.cpp \
          Cell.cpp \
          EnemyTower.cpp \
          DirectDamageSpell.cpp \
          AreaDamageSpell.cpp \
          TrapSpell.cpp \
          SpellHand.cpp \
          GameController.cpp \
          GameInstructions.cpp \
          Game.cpp \
          Level.cpp \
          GameSession.cpp \
          SaveSystem.cpp

OBJECTS = $(SOURCES:.cpp=.o)

$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)

# Создание папки для сохранений при сборке
setup: $(TARGET)
	mkdir -p saves

# Полная пересборка
rebuild: clean $(TARGET) setup

.PHONY: clean setup rebuild