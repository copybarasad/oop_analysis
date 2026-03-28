//
// Created by Mac on 10.11.2025.
//

#include "GameSaver.h"

#include <sstream>

#include "../field_objects/Field.h"
#include "../Game.h"

GameSaver::GameSaver(const std::string &filename)
    :saver(filename) {

}

void GameSaver::savePlayer(std::shared_ptr<Player> player) {
    player->save(saver);
}

void GameSaver::saveField(std::shared_ptr<Field> field) {
    field->save(saver);
}

void GameSaver::saveLevelManager(LevelManager level_manager) {
    level_manager.save(saver);
}


void GameSaver::saveGame(std::shared_ptr<Game> game) {

    auto field = game->getField();
    auto player = field->getPlayer();

    saveLevelManager(game->getLevelManager());
    saveField(field);
    savePlayer(player);
}

