//
// Created by Mac on 10.11.2025.
//

#ifndef GAME_TERM_GAMESAVER_H
#define GAME_TERM_GAMESAVER_H
#include "Saver.h"
#include "../LevelManager.h"
#include "../field_objects/Player.h"


class Game;

class GameSaver {

    Saver saver;

    void savePlayer(std::shared_ptr<Player> player);

    void saveField(std::shared_ptr<Field> field);

    void saveLevelManager(LevelManager level_manager);

public:

    explicit GameSaver(const std::string & filename);

    void saveGame(std::shared_ptr<Game> game);
};


#endif //GAME_TERM_GAMESAVER_H