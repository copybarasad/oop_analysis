//
// Created by Mac on 10.11.2025.
//

#include "GameReader.h"

GameReader::GameReader(const std::string &filename)
    :reader(filename) {}

std::shared_ptr<Player> GameReader::readPlayer() {
    Player * player = new Player();
    player->load(reader);
    return std::shared_ptr<Player>(player);
}

std::shared_ptr<Field> GameReader::readField() {
    Field * field = new Field;
    field->load(reader);
    return std::shared_ptr<Field>(field);
}

LevelManager GameReader::readLevelManager() {
    LevelManager level_manager;
    level_manager.load(reader);
    return level_manager;
}


std::shared_ptr<Game> GameReader::readGame() {
    LevelManager level_manager = readLevelManager();
    std::shared_ptr<Field> field = readField();
    std::shared_ptr<Player> player = readPlayer();
    return std::make_shared<Game>(field, player, level_manager);
}
std::tm GameReader::getTimeSaved() {
    std::tm tm{};
    reader.read(tm);
    return tm;
}



