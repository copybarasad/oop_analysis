#ifndef MOVEMENT_HPP
#define MOVEMENT_HPP

#include "../map/gamemap.hpp"
#include "../entities/character.hpp"
#include "../entities/player.hpp"
#include <iostream>
#include <string>

class MagicHand;

class Movement {
private:
    GameMap& gameMap;
    MagicHand* magicHand = nullptr;
public:
    Movement(GameMap& map);
    
    bool moveCharacter(Character* character, const std::string& direction);
    bool isValidMove(Character* character, int newX, int newY) const;
    void updateCharacterPosition(Character* character, int newX, int newY);
    void handleSlowingCell(Player* player, int x, int y);
    void handleChests(Player* player, int x, int y);
    void getDirectionOffset(const std::string& direction, int& dx, int& dy);
    void setMagicHand(MagicHand* m) { magicHand = m; }
};

#endif