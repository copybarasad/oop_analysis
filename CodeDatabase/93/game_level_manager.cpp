#include "game_level_manager.h"
#include <random>
#include <algorithm>
#include "game_initializer.h"

void GameLevelManager::advanceToNextLevel(GameController& game) {
    int nextLevel = game.getCurrentLevel() + 1;
    int width = 10 + nextLevel * 5;
    int height = width;
    int enemyCount = 2 + nextLevel;

    auto& player = const_cast<Player&>(game.getPlayer());
    player.restoreFullHealth();

    auto& hand = player.getHand();
    size_t removeCount = hand.size() / 2;
    for (size_t i = 0; i < removeCount && hand.size() > 0; ++i) {
        static std::mt19937 gen(std::random_device{}());
        std::uniform_int_distribution<size_t> dis(0, hand.size() - 1);
        size_t idx = dis(gen);
        hand.removeSpell(idx);
    }

    game.getFieldForLoad() = GameField(width, height);

    std::vector<Enemy> newEnemies;
    GameInitializer::initializeEnemies(newEnemies, game.getField(), enemyCount);
    for (auto& e : newEnemies) {
        int health = 30 + (nextLevel - 1) * 8;
        int damage = 10 + (nextLevel - 1) * 3;
        e = Enemy(e.getPosition().x, e.getPosition().y, health, damage);
    }
    game.setEnemies(std::move(newEnemies));

    std::vector<EnemyTower> newTowers;
    GameInitializer::initializeTowers(newTowers, game.getField(), 1);
    for (auto& t : newTowers) {
        int damage = 12 + (nextLevel - 1) * 2;
        int range = 2 + (nextLevel >= 3 ? 1 : 0);
        t = EnemyTower(t.getPosition().x, t.getPosition().y, damage, range, 2);
    }
    game.setTowers(std::move(newTowers));

    player.setLevel(nextLevel);
    game.setGameRunning(true);
}