PP = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -I.

SOURCE = main.cpp game.cpp basic_info.cpp enemy.cpp movement.cpp place.cpp player.cpp cell.cpp tower.cpp area_spell.cpp default_spell.cpp spell_hand.cpp trap_spell.cpp magic_tower.cpp weak_default_spell.cpp commands.cpp console_input.cpp render.cpp
OBJECTS = $(SOURCE:.cpp=.o)

all: lb

lb: $(OBJECTS)
	$(PP) $(CXXFLAGS) -o lb $(OBJECTS) -lm

main.o: main.cpp basic_info.h imagic_spell_card.h enemy.h player.h movement.h place.h cell.h tower.h  area_spell.h default_spell.h spell_hand.h trap_spell.h magic_tower.h weak_default_spell.h game.h commands.h console_input.h game_controller.h render.h render_controller.h
	$(PP) $(CXXFLAGS) -c main.cpp

game.o: game.cpp game.h
	$(PP) $(CXXFLAGS) -c game.cpp

basic_info.o: basic_info.cpp 
	$(PP) $(CXXFLAGS) -c basic_info.cpp

enemy.o: enemy.cpp basic_info.h player.h
	$(PP) $(CXXFLAGS) -c enemy.cpp

player.o: player.cpp basic_info.h enemy.h spell_hand.h
	$(PP) $(CXXFLAGS) -c player.cpp

movement.o: movement.cpp place.h player.h
	$(PP) $(CXXFLAGS) -c movement.cpp 

place.o: place.cpp player.h cell.h
	$(PP) $(CXXFLAGS) -c place.cpp

cell.o: cell.cpp
	$(PP) $(CXXFLAGS) -c cell.cpp

tower.o: tower.cpp basic_info.h enemy.h place.h
	$(PP) $(CXXFLAGS) -c tower.cpp

area_spell.o: area_spell.cpp imagic_spell_card.h
	$(PP) $(CXXFLAGS) -c area_spell.cpp

default_spell.o: default_spell.cpp imagic_spell_card.h
	$(PP) $(CXXFLAGS) -c default_spell.cpp

trap_spell.o: trap_spell.cpp imagic_spell_card.h
	$(PP) $(CXXFLAGS) -c trap_spell.cpp

spell_hand.o: spell_hand.cpp imagic_spell_card.h default_spell.h area_spell.h trap_spell.h
	$(PP) $(CXXFLAGS) -c spell_hand.cpp

magic_towerr.o: magic_tower.cpp basic_info.h player.h place.h weak_default_spell.h
	$(PP) $(CXXFLAGS) -c magic_towerr.cpp

weak_default_spell.o: weak_default_spell.cpp imagic_spell_card.h
	$(PP) $(CXXFLAGS) -c weak_default_spell.cpp

commands.o: commands.cpp game.h
	$(PP) $(CXXFLAGS) -c commands.cpp

console_input.o: console_input.cpp commands.h
	$(PP) $(CXXFLAGS) -c console_input.cpp

render.o: render.cpp 
	$(PP) $(CXXFLAGS) -c render.cpp

clean:
	rm -f $(OBJECTS)