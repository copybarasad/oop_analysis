ODIR = os
OS = main.o player.o enemy.o map.o enemy_barrack.o enemy_tower.o ally.o usables.o spells.o dummy.o hand.o game_master.o saver.o render.o command_proc.o
HS = player_game/map.h player_game/player.h player_game/game_master.h dummy/enemy.h dummy/enemy_barrack.h dummy/enemy_tower.h dummy/ally.h dummy/usables.h dummy/dummy.h spells/spells.h spells/hand.h player_game/render.h player_game/command_proc.h

all: game

game: $(OS)
	g++ -o game $(OS)

main.o: main.cpp $(HS)
	g++ -c main.cpp

game_master.o: player_game/game_master.cpp player_game/game_master.h
	g++ -c player_game/game_master.cpp

command_proc.o: player_game/command_proc.cpp player_game/command_proc.h
	g++ -c player_game/command_proc.cpp

render.o: player_game/render.cpp player_game/render.h
	g++ -c player_game/render.cpp

saver.o: player_game/saver.cpp player_game/saver.h
	g++ -c player_game/saver.cpp

map.o: player_game/map.cpp player_game/map.h
	g++ -c player_game/map.cpp

player.o: player_game/player.cpp player_game/player.h
	g++ -c player_game/player.cpp

dummy.o: dummy/dummy.cpp dummy/dummy.h
	g++ -c dummy/dummy.cpp

enemy.o: dummy/enemy.cpp dummy/enemy.h
	g++ -c dummy/enemy.cpp

ally.o: dummy/ally.cpp dummy/ally.h
	g++ -c dummy/ally.cpp

usables.o: dummy/usables.cpp dummy/usables.h
	g++ -c dummy/usables.cpp

enemy_barrack.o: dummy/enemy_barrack.cpp dummy/enemy_barrack.h
	g++ -c dummy/enemy_barrack.cpp

enemy_tower.o: dummy/enemy_tower.cpp dummy/enemy_tower.h
	g++ -c dummy/enemy_tower.cpp

hand.o: spells/hand.cpp spells/hand.h
	g++ -c spells/hand.cpp

spells.o: spells/spells.cpp spells/spells.h
	g++ -c spells/spells.cpp

clean:
	del *.o *.exe