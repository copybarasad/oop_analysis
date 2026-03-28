#ifndef TXT_SAVE_S_H
#define TXT_SAVE_S_H

#include <fstream>
#include <filesystem>

#include "../BaseSaveService/SaveService.hpp"

class TxtSaveService : public SaveService {
    private:
    const GameCycleService::GameState& gameState;
    std::string savesDirectory;

    public:
    TxtSaveService (const GameCycleService::GameState& state, const std::string& directory="GameSaves");

    void save (const std::string& saveName) override;

    private:
    void ensureSavesDirectory () const;
    std::string getSavePath (const std::string& saveName) const;
    void validateSaveName (const std::string& saveName) const;
    void saveGameState (std::ofstream& file) const;
    void saveFieldData (std::ofstream& file) const;
    void saveEntitiesData (std::ofstream& file) const;
    void saveSpellData (std::ofstream& file) const;
    void saveSpellStoreData (std::ofstream& file) const;
    void savePlayerData (std::ofstream& file) const;
};

#endif