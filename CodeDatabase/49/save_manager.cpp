#include "save_manager.h"

static const char FILE_MAGIC[4] = {'G','S','V','1'};
static const uint32_t FILE_VERSION = 1u;

void SaveManager::save_game(const Game& game, const std::string& path)
{
    const std::string tmp = path + ".tmp";

    std::ofstream ofs;
    ofs.exceptions(std::ios::failbit | std::ios::badbit);
    try
    {
        ofs.open(tmp, std::ios::binary | std::ios::trunc);

        ofs.write(FILE_MAGIC, sizeof(FILE_MAGIC));
        ofs.write(reinterpret_cast<const char*>(&FILE_VERSION), sizeof(FILE_VERSION));

        game.save_binary(ofs);

        ofs.close();

        if (std::rename(tmp.c_str(), path.c_str()) != 0)
        {
            std::remove(tmp.c_str());
            throw FileIOException("Failed to rename temp save file to target path: " + path);
        }
    }
    catch (const std::ios_base::failure& e)
    {
        try { ofs.close(); } catch(...) {}
        std::remove(tmp.c_str());
        throw FileIOException(std::string("File I/O error during save: ") + e.what());
    }
    catch (const std::exception& e)
    {
        try { ofs.close(); } catch(...) {}
        std::remove(tmp.c_str());
        throw;
    }
}

void SaveManager::load_game(Game& game, const std::string& path)
{
    std::ifstream ifs;
    ifs.exceptions(std::ios::failbit | std::ios::badbit);
    try
    {
        ifs.open(path, std::ios::binary);

        char magic[4];
        ifs.read(magic, sizeof(magic));
        if (std::memcmp(magic, FILE_MAGIC, sizeof(magic)) != 0)
        {
            throw LoadException("Invalid save file magic (not a recognized game save).");
        }

        uint32_t version = 0;
        ifs.read(reinterpret_cast<char*>(&version), sizeof(version));
        if (version != FILE_VERSION)
        {
            throw LoadException("Unsupported save file version: " + std::to_string(version));
        }

        game.load_binary(ifs);

        ifs.close();
    }
    catch (const std::ios_base::failure& e)
    {
        try { ifs.close(); } catch(...) {}
        throw FileIOException(std::string("File I/O error during load: ") + e.what());
    }
}