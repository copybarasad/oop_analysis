#include "GameSaver.h"
#include "Game.h"
#include "Player.h"
#include "Exceptions.h"
#include <fstream>
#include <memory>

namespace Game {

void GameSaver::save(const Game& gameState, const Player& player, const std::string& filename) {
    std::ofstream ofs(filename, std::ios::binary);
    if (!ofs) throw FileOpenException(filename);

    ofs.write(reinterpret_cast<const char*>(&GameSaver::MAGIC_NUMBER), sizeof(GameSaver::MAGIC_NUMBER));
    ofs.write(reinterpret_cast<const char*>(&GameSaver::SAVE_VERSION), sizeof(GameSaver::SAVE_VERSION));
    
    player.serialize(ofs);
    gameState.serialize(ofs);

    if (ofs.fail()) throw FileWriteException("An error occurred during file write operation.");
}

std::unique_ptr<Game> GameSaver::load(Player& player, const std::string& filename) {
    std::ifstream ifs(filename, std::ios::binary);
    if (!ifs) throw FileOpenException(filename);

    uint32_t magic, version;
    ifs.read(reinterpret_cast<char*>(&magic), sizeof(magic));
    ifs.read(reinterpret_cast<char*>(&version), sizeof(version));

    if (ifs.fail()) throw FileReadException("Could not read save file header.");
    
    if (magic != GameSaver::MAGIC_NUMBER) throw InvalidDataException("Not a valid save file.");
    if (version != GameSaver::SAVE_VERSION) throw InvalidDataException("Save file version mismatch.");
    
    player.deserialize(ifs);
    auto loadedGame = std::make_unique<Game>(ifs, player);
    
    if (ifs.fail() && !ifs.eof()) throw FileReadException("An error occurred while reading game state.");

    return loadedGame;
}

}