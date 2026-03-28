#ifndef SAVE_MANAGER_HPP
#define SAVE_MANAGER_HPP

#include "game_state.hpp"
#include <string>
#include <vector>
#include <filesystem>
#include <fstream>
#include <algorithm>
#include <iostream>

class SaveManager {
private:
    std::string saveDirectory;

    std::string getSavePath(const std::string& saveName) const;
    std::string sanitizeFilename(const std::string& name) const;

    void serializeGameState(const GameState& gameState, std::ostream& stream) const;
    void deserializeGameState(GameState& gameState, std::istream& stream);
    
    void serializeEntityManager(const eManager& manager, std::ostream& stream) const;
    void deserializeEntityManager(eManager& manager, std::istream& stream);
    
    void serializeEntity(const Entity* entity, std::ostream& stream) const;
    void deserializeEntity(Entity* entity, std::istream& stream);
    
    void serializeSpellHand(const SpellHand* hand, std::ostream& stream) const;
    void deserializeSpellHand(SpellHand* hand, std::istream& stream);
    
    void serializeSpellCard(const SpellCard* spell, std::ostream& stream) const;
    SpellCard* deserializeSpellCard(std::istream& stream) const;
    
    void serializeMap(const Map* map, std::ostream& stream) const;
    void deserializeMap(eManager& entityManager, std::istream& stream);

public:
    SaveManager(const std::string& directory = "saves/");

    bool saveGame(const GameState& gameState);
    bool loadGame(GameState& gameState, const std::string& saveName);

    std::vector<std::string> getSaveList() const;
    bool deleteSave(const std::string& saveName);
    bool saveExists(const std::string& saveName) const;
    
};

#endif