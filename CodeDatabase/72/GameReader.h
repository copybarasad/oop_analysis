//
// Created by Mac on 10.11.2025.
//

#ifndef GAME_TERM_GAMEREADER_H
#define GAME_TERM_GAMEREADER_H
#include "Reader.h"
#include "../field_objects/Player.h"
#include "../field_objects/Field.h"
#include "../Game.h"

template<typename, typename>
class TemplateGame;


class Field;

class GameReader {

    Reader reader;

    std::shared_ptr<Player> readPlayer();

    std::shared_ptr<Field> readField();

    LevelManager  readLevelManager();

public:

    explicit GameReader(const std::string & filename);

    std::shared_ptr<Game> readGame();

    std::tm getTimeSaved();
};


#endif //GAME_TERM_GAMEREADER_H