#include "GameStateSerializer.h"
#include "GameState.h"

void GameStateSerializer::serialize(const GameState& gameState, std::ostream& stream) const {
    stream << "GAME_SAVE_V1" << std::endl;

    const auto& field = gameState.getField();
    auto playerPos = field.getPlayerPosition();
    stream << field.getWidth() << " " << field.getHeight() << " "
        << playerPos.first << " " << playerPos.second << std::endl;

    for (int y = 0; y < field.getHeight(); y++) {
        for (int x = 0; x < field.getWidth(); x++) {
            stream << field.getCellContent(x, y) << " ";
        }
        stream << std::endl;
    }

    const auto& player = gameState.getPlayer();
    stream << player.getHealth() << " " << player.getScore() << " "
        << player.getMana() << " " << player.getMaxMana() << std::endl;

    stream << player.getSpellHand().getSpellCount() << std::endl;

    const auto& enemies = gameState.getEnemies();
    stream << enemies.size() << std::endl;
    for (const auto& enemy : enemies) {
        stream << enemy.getX() << " " << enemy.getY() << " "
            << enemy.getHealth() << " " << enemy.getDamage() << std::endl;
    }
}

void GameStateSerializer::deserialize(GameState& gameState, std::istream& stream) const {
    std::string version;
    stream >> version;
    if (version != "GAME_SAVE_V1") {
        throw std::runtime_error("Invalid save file version");
    }

    int width, height, playerX, playerY;
    stream >> width >> height >> playerX >> playerY;

    auto& field = gameState.getField();
    field = GameField(width, height);
    field.setPlayerPosition(playerX, playerY);

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            char content;
            stream >> content;
            field.setCellContent(x, y, content);
        }
    }

    int health, score, mana, maxMana;
    stream >> health >> score >> mana >> maxMana;

    auto& player = gameState.getPlayer();
    player.setHealth(health);
    player.setScore(score);
    player.setMana(mana);
    player.setMaxMana(maxMana);

    int spellCount;
    stream >> spellCount;
    player.clearSpellHand();

    int enemyCount;
    stream >> enemyCount;
    auto& enemies = gameState.getEnemies();
    enemies.clear();
    for (int i = 0; i < enemyCount; i++) {
        int x, y, enemyHealth, enemyDamage;
        stream >> x >> y >> enemyHealth >> enemyDamage;
        Enemy enemy(x, y);
        enemy.setHealth(enemyHealth);
        enemies.push_back(enemy);
    }
}
