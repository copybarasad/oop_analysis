#ifndef TXT_LOAD_S_H
#define TXT_LOAD_S_H

#include <fstream>
#include <filesystem>

#include "../BaseLoadService/LoadService.hpp"
#include "../../GameStateDeserializers/BaseDeserializer/BaseDeserializer.hpp"
#include "modules/Services/SaveLoadServices/GameStateDeserializers/FieldDeserializer/FieldDeserializer.hpp"
#include "modules/Services/SaveLoadServices/GameStateDeserializers/EntityManagerDeserializer/EntityManagerDeserializer.hpp"
#include "modules/Services/SaveLoadServices/GameStateDeserializers/SpellHandDeserializer/SpellHandDeserializer.hpp"
#include "modules/Services/SaveLoadServices/GameStateDeserializers/SpellStoreDeserializer/SpellStoreDeserializer.hpp"

class TxtLoadService : public LoadService {
    private:
    std::string savesDirectory;
    BaseDeserializer deserializer;

    public:
    TxtLoadService (const std::string& directory = "GameSaves");

    GameCycleService::GameState* load (const std::string& saveName) override;
    bool saveExists (const std::string& saveName) const override;

    private:
    std::string getSavePath (const std::string& saveName) const;
    void validateSaveFile (std::ifstream& file, const std::string& saveName) const;
    std::string readLine (std::ifstream& file) const;
    void parseGameState (std::ifstream& file, size_t& currentLevel, GameProgress& progress);
    Field* parseFieldData (std::ifstream& file);
    EntityManager* parseEntitiesData (std::ifstream& file, Field* field);
    SpellHand* parseSpellData (std::ifstream& file);
    SpellStore* parseSpellStoreData (std::ifstream& file);
    void parsePlayerData (std::ifstream& file);
};

#endif