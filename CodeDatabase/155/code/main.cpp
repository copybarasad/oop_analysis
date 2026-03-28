#include <iostream>
#include <windows.h>
#include <random>
#include <vector>
#include <cmath>
#include "player_game/game_master.h"
#include "player_game/player.h"
#include "player_game/map.h"
#include "dummy/enemy.h"
#include "dummy/enemy_barrack.h"
#include "dummy/ally.h"
#include "spells/hand.h"

int main()
{
    SetConsoleOutputCP(65001);
    game_master* game = new game_master();
    game->menu();
}