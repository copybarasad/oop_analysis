#ifndef GAME_IO_HPP
#define GAME_IO_HPP

#include <string>

class Game;

void GameSave(Game& game, const std::string& filename);
void GameLoad(Game& game, const std::string& filename);

#endif