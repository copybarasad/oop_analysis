#pragma once

#include <iostream>
#include "player.h"
#include "enemy.h"
class CommandMapper;
void displayMenu(const CommandMapper& mapper);
void showStatsPlayer(Player& player);
void showStatsEnemy(Enemy& enemy);
void renderSpellShop(int playerPoints);