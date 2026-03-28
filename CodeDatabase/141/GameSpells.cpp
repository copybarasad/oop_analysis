#include "Game.h"
#include "SpellSystem.h"

void Game::addRandomSpellToPlayer() {
    if (player.getHand().isFull()) {
        addMessage("Рука полна, нельзя добавить заклинание");
        return;
    }

    Spell *newSpell = spellSystem.createRandomSpell();

    if (player.getHand().addSpell(newSpell)) {
        addMessage("Получено заклинание: " + player.getHand().getSpell(player.getHand().getSize() - 1)->getName());
    } else {
        delete newSpell;
    }
}

bool Game::findSafeSpawnPositionNearPlayer(int &x, int &y) const {
    int playerX, playerY;
    player.getPosition(playerX, playerY);

    std::vector<std::pair<int, int> > directions = {
        {0, 1}, {1, 0}, {0, -1}, {-1, 0}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1}
    };

    for (const auto &dir: directions) {
        int checkX = playerX + dir.first;
        int checkY = playerY + dir.second;

        if (field.isValidPosition(checkX, checkY) &&
            field.isPassable(checkX, checkY) &&
            movementSystem.isPositionFree(checkX, checkY, field, player, enemies, buildings, towers, allies)) {
            x = checkX;
            y = checkY;
            return true;
        }
    }
    return false;
}

void Game::castTowerSpell(int sourceX, int sourceY, int targetX, int targetY, int range, int damage) {
    spellSystem.castTowerSpell(sourceX, sourceY, targetX, targetY, range, damage,
                               field, player, enemies, buildings,
                               [this](const std::string &msg) { addMessage(msg); });
}

void Game::addEnhancement(const Enhancement &enhancement) {
    spellSystem.addEnhancement(enhancement);
}

void Game::applyEnhancements(Spell &spell) {
    spellSystem.applyEnhancements(spell, [this](const std::string &msg) { addMessage(msg); });
}

std::vector<Enhancement> Game::getPendingEnhancements() const {
    return spellSystem.getPendingEnhancements();
}

void Game::clearPendingEnhancements() {
    spellSystem.clearEnhancements();
}

void Game::addPendingEnhancement(const Enhancement &enhancement) {
    spellSystem.addEnhancement(enhancement);
}
