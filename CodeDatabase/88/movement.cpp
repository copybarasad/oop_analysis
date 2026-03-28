#include "movement.hpp"
#include "magic.hpp"

Movement::Movement(GameMap& map) : gameMap(map){}

bool Movement::moveCharacter(Character* character, const std::string& direction) {
    if (character == nullptr) {
        return false;
    }
    
    Player* player = dynamic_cast<Player*>(character);
    if (player && player->isSlowed()) {
        player->clearSlow();
        return false; 
    }
    
    int currentX = character->getX();
    int currentY = character->getY();
    int dx = 0, dy = 0;
    
    getDirectionOffset(direction, dx, dy);
    
    int newX = currentX + dx;
    int newY = currentY + dy;
    
    if (isValidMove(character, newX, newY)) {
        updateCharacterPosition(character, newX, newY);
        
        if (player) {
            handleSlowingCell(player, newX, newY);
            handleChests(player, newX, newY);
        }
        
        return true;
    }
    
    return false;
}

bool Movement::isValidMove(Character* character, int newX, int newY) const {
    if (!gameMap.isValidPosition(newX, newY)) {
        return false;
    }
    
    Plot& plot = gameMap.getPlot(newX, newY);
    
    if (plot.isNotPassable()) {
        return false;
    }
    
    Character* target = gameMap.getCharacterAt(newX, newY);
    if (target && target != character) {
        return false;
    }
    
    return true;
}

void Movement::updateCharacterPosition(Character* character, int newX, int newY) {
    if (character == nullptr) {
        return;
    }
    int oldX = character->getX();
    int oldY = character->getY();
    
    gameMap.removeCharacter(oldX, oldY);
    character->setPosition(newX, newY);
    gameMap.placeCharacter(character, newX, newY);
}

void Movement::getDirectionOffset(const std::string& direction, int& dx, int& dy) {
    if (direction == "d") dx = 1;
    else if (direction == "a") dx = -1;
    else if (direction == "s") dy = 1;
    else if (direction == "w") dy = -1;
}

void Movement::handleSlowingCell(Player* player, int x, int y) {
    if (gameMap.getPlot(x, y).getOriginalType() == '~') {
        player->setSlowed(true);
        std::cout << "Player stepped on slowing terrain! Cannot move next turn.\n";
    }
}

void Movement::handleChests(Player* player, int x, int y) {
    if (gameMap.getPlot(x, y).getOriginalType() == 'G') {
        if (magicHand) {
            magicHand->getRandSpell();
        }
        gameMap.getPlot(x, y).consumeChest();
        std::cout << "You opened a chest and found a spell!\n";
    }
}
