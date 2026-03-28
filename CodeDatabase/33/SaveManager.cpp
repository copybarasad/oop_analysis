#include "SaveManager.h"
#include "Exceptions.h"
#include "Game.h"
#include <fstream>
#include <string>

SaveManager::SaveManager(std::string default_path) : path_(std::move(default_path)) {}

void SaveManager::SaveToFile(const Game& game, const std::string& path) const {
    std::ofstream ofs(path, std::ios::trunc);
    if (!ofs) throw FileOpenError(path);
    ofs << kMagic() << "\n";
    game.SaveToStream(ofs);
    if (!ofs.good()) throw FileWriteError(path);
}

void SaveManager::LoadFromFile(Game& game, const std::string& path) const {
    std::ifstream ifs(path);
    if (!ifs) throw FileOpenError(path);
    std::string magic;
    std::getline(ifs, magic);
    if (magic != kMagic()) throw BadHeaderError("bad header in " + path);
    game.LoadFromStream(ifs);
    if (ifs.bad()) throw FileReadError(path);
}

void SaveManager::Save(const Game& game) const { SaveToFile(game, path_); }
void SaveManager::Load(Game& game) const { LoadFromFile(game, path_); }
