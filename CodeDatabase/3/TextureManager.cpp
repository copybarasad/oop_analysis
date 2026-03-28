#include "ViewController/Managers/TextureManager.hpp"
#include "types.hpp"

TextureManager::TextureManager() {
    fieldTextures[RenderFieldType::ENEMY] = std::make_unique<sf::Texture>();
    fieldTextures[RenderFieldType::ENEMY]->loadFromFile(Paths::ENEMY_PATH);

    fieldTextures[RenderFieldType::ENEMY_WAITING] = std::make_unique<sf::Texture>();
    fieldTextures[RenderFieldType::ENEMY_WAITING]->loadFromFile(Paths::ENEMY_WAITING_PATH);

    fieldTextures[RenderFieldType::FIELD_TRAP] = std::make_unique<sf::Texture>();
    fieldTextures[RenderFieldType::FIELD_TRAP]->loadFromFile(Paths::FIELD_TRAP_PATH);

    fieldTextures[RenderFieldType::FIELD_BLOCK] = std::make_unique<sf::Texture>();
    fieldTextures[RenderFieldType::FIELD_BLOCK]->loadFromFile(Paths::FIELD_BLOCK_PATH);

    fieldTextures[RenderFieldType::FIELD_DEFAULT] = std::make_unique<sf::Texture>();
    fieldTextures[RenderFieldType::FIELD_DEFAULT]->loadFromFile(Paths::FIELD_DEFAULT_PATH);

    fieldTextures[RenderFieldType::FIELD_SLOW] = std::make_unique<sf::Texture>();
    fieldTextures[RenderFieldType::FIELD_SLOW]->loadFromFile(Paths::FIELD_SLOW_PATH);

    fieldTextures[RenderFieldType::FIELD_TOWER] = std::make_unique<sf::Texture>();
    fieldTextures[RenderFieldType::FIELD_TOWER]->loadFromFile(Paths::FIELD_TOWER_PATH);

    fieldTextures[RenderFieldType::PLAYER_MELEE] = std::make_unique<sf::Texture>();
    fieldTextures[RenderFieldType::PLAYER_MELEE]->loadFromFile(Paths::PLAYER_MELEE_PATH);

    fieldTextures[RenderFieldType::PLAYER_RANGE] = std::make_unique<sf::Texture>();
    fieldTextures[RenderFieldType::PLAYER_RANGE]->loadFromFile(Paths::PLAYER_RANGE_PATH);

    fieldTextures[RenderFieldType::PLAYER_WAITING] = std::make_unique<sf::Texture>();
    fieldTextures[RenderFieldType::PLAYER_WAITING]->loadFromFile(Paths::PLAYER_WAITING_PATH);

    fieldTextures[RenderFieldType::FIELD_TOWER_ATTACK] = std::make_unique<sf::Texture>();
    fieldTextures[RenderFieldType::FIELD_TOWER_ATTACK]->loadFromFile(Paths::FIELD_TOWER_ATTACK_PATH);

    fieldTextures[RenderFieldType::BULLET] = std::make_unique<sf::Texture>();
    fieldTextures[RenderFieldType::BULLET]->loadFromFile(Paths::BULLET_PATH);
}

sf::Texture& TextureManager::getTexture(RenderFieldType t) {
    auto it = fieldTextures.find(t);
    return *it->second;
}

