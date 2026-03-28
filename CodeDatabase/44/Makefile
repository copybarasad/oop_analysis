CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2

SRC = main.cpp Entity.cpp Player.cpp Enemy.cpp Cell.cpp Field.cpp EnemyBuilding.cpp \
      Game.cpp SpellHand.cpp DirectDamageSpell.cpp AreaDamageSpell.cpp TrapSpell.cpp EnemyTower.cpp \
      InputHandler.cpp GameRenderer.cpp KeyBindings.cpp EventLogger.cpp EventDispatcher.cpp
OBJ = $(SRC:.cpp=.o)

TARGET = game

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	del /F /Q $(OBJ) $(TARGET)

rebuild: clean all

.PHONY: all clean rebuild