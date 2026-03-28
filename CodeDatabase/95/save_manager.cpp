#include "save_manager.hpp"
#include "game_objects/player.hpp"
#include "game_objects/enemy.hpp"
#include "game_objects/tower.hpp"
#include "game_objects/spells/aoe_spell.hpp"
#include "game_objects/spells/damage_spell.hpp"
#include "game_objects/spells/trap_spell.hpp"

SaveManager::SaveManager(const std::string& directory) : saveDirectory(directory) {
    std::filesystem::create_directories(saveDirectory);
}

bool SaveManager::saveGame(const GameState& gameState) {
    std::string saveName;
    std::cin >> saveName; 
    std::string cleanName = sanitizeFilename(saveName);
    std::string filename = getSavePath(cleanName);
    
    try {
        std::ofstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Ошибка: не могу создать файл " << filename << std::endl;
            return false;
        }
        
        serializeGameState(gameState, file);
        file.close();
        
        std::cout << "Игра сохранена: " << filename << std::endl;
        return true;
        
    } catch (const std::exception& e) {
        std::cerr << "Ошибка сохранения: " << e.what() << std::endl;
        return false;
    }
}

bool SaveManager::loadGame(GameState& gameState, const std::string& saveName) {
    std::string cleanName = sanitizeFilename(saveName);
    std::string filename = getSavePath(cleanName);
    
    try {
        if (!std::filesystem::exists(filename)) {
            std::cerr << "Файл сохранения не найден: " << filename << std::endl;
            return false;
        }
        
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Ошибка: не могу открыть файл " << filename << std::endl;
            return false;
        }
        
        deserializeGameState(gameState, file);
        file.close();

        std::cout << "Игра загружена: " << filename << std::endl;
        return true;
        
    } catch (const std::exception& e) {
        std::cerr << "Ошибка загрузки: " << e.what() << std::endl;
        return false;
    }
}

std::vector<std::string> SaveManager::getSaveList() const {
    std::vector<std::string> saves;
    
    try {
        if (!std::filesystem::exists(saveDirectory)) {
            return saves;
        }
        
        for (const auto& entry : std::filesystem::directory_iterator(saveDirectory)) {
            if (entry.is_regular_file() && entry.path().extension() == ".txt") {
                saves.push_back(entry.path().stem().string());
            }
        }
        
        std::sort(saves.begin(), saves.end());
        
    } catch (const std::exception& e) {
        std::cerr << "Ошибка чтения списка сохранений: " << e.what() << std::endl;
    }
    
    return saves;
}

bool SaveManager::deleteSave(const std::string& saveName) {
    std::string cleanName = sanitizeFilename(saveName);
    std::string filename = getSavePath(cleanName);
    
    try {
        if (!std::filesystem::exists(filename)) {
            std::cerr << "Файл сохранения не найден: " << filename << std::endl;
            return false;
        }
        
        bool result = std::filesystem::remove(filename);
        if (result) {
            std::cout << "Сохранение удалено: " << cleanName << std::endl;
        }
        return result;
        
    } catch (const std::exception& e) {
        std::cerr << "Ошибка удаления сохранения: " << e.what() << std::endl;
        return false;
    }
}

bool SaveManager::saveExists(const std::string& saveName) const {
    std::string cleanName = sanitizeFilename(saveName);
    return std::filesystem::exists(getSavePath(cleanName));
}

std::string SaveManager::getSavePath(const std::string& saveName) const {
    return saveDirectory + saveName + ".txt";
}

std::string SaveManager::sanitizeFilename(const std::string& name) const {
    if (name.empty()) return "unnamed_save";
    
    std::string result = name;
    std::replace_if(result.begin(), result.end(), 
        [](char c) { 
            return c == '/' || c == '\\' || c == ':' || c == '*' || 
                    c == '?' || c == '"' || c == '<' || c == '>' || c == '|';
        }, '_');
    return result;
}

void SaveManager::serializeGameState(const GameState& gameState, std::ostream& stream) const {
    stream << static_cast<int>(gameState.status) << " " << gameState.turn_counter << "\n";
    if (stream.fail()) {
        throw std::runtime_error("Ошибка записи базовых данных игры");
    }
        
    serializeEntityManager(gameState.entityManager, stream);
}

void SaveManager::deserializeGameState(GameState& gameState, std::istream& stream) {    
    int status_value;
    stream >> status_value >> gameState.turn_counter;
    if (stream.fail()) {
        throw std::runtime_error("Ошибка чтения базовых данных игры");
    }
    
    if (status_value < 0 || status_value > 3) {
        throw std::runtime_error("Некорректное значение статуса игры");
    }
    gameState.status = static_cast<GameStatus>(status_value);
    
    deserializeEntityManager(gameState.entityManager, stream);
}

void SaveManager::serializeEntityManager(const eManager& entityManager, std::ostream& stream) const {
    auto player = entityManager.getPlayer();
    if (player) {
        stream << "PLAYER\n";
        serializeEntity(player, stream);
        stream << "\n";
        
        auto hand = entityManager.getPlayerHand();
        if (hand) {
            stream << "HAND\n";
            serializeSpellHand(hand, stream);
            stream << "\n";
        } else {
            stream << "NO_HAND\n";
        }
    } else {
        stream << "NO_PLAYER\n";
    }

    if (stream.fail()) {
        throw std::runtime_error("Ошибка записи данных игрока");
    }
    
    auto enemies = entityManager.getEnemies();
    stream << "ENEMIES " << enemies.size() << "\n";
    for (auto enemy : enemies) {
        serializeEntity(enemy, stream);
        stream << "\n";
    }
    
    if (stream.fail()) {
        throw std::runtime_error("Ошибка записи данных врагов");
    }

    auto tower = entityManager.getTower();
    if (tower) {
        stream << "TOWER\n";
        serializeEntity(tower, stream);
        stream << "\n";
    } else {
        stream << "NO_TOWER\n";
    }

    if (stream.fail()) {
        throw std::runtime_error("Ошибка записи данных башни");
    }
    
    stream << "MAP\n";
    serializeMap(entityManager.getMap(), stream);

    if (stream.fail()) {
        throw std::runtime_error("Ошибка записи данных карты");
    }
}

void SaveManager::deserializeEntityManager(eManager& entityManager, std::istream& stream) {
    std::string section;
       
    stream >> section;
    if (stream.fail()) {
        throw std::runtime_error("Ошибка чтения секции данных: файл поврежден");
    }

    if (section == "PLAYER") {
        Entity* playerEntity = entityManager.getPlayer();
        if (!playerEntity) {
            throw std::runtime_error("Игрок не создан");
        }
        deserializeEntity(playerEntity, stream);

        stream >> section;  
        if (stream.fail()) {
            throw std::runtime_error("Ошибка чтения секции руки заклинаний");
        }
        
        if (section == "HAND") {
            auto hand = entityManager.getPlayerHand();
            if (hand) {
                deserializeSpellHand(hand, stream);
            }
            stream >> section;
        }
    } else if (section != "NO_PLAYER") {
        throw std::runtime_error("Неверный формат секции игрока");
    }

    if (section == "ENEMIES") {
        int enemy_count;
        stream >> enemy_count;

        if (stream.fail() || enemy_count < 0) {
            throw std::runtime_error("Некорректное количество врагов в сохранении");
        }
       
        entityManager.clearEnemies();
        for (int i = 0; i < enemy_count; ++i) {
            Entity* enemy = new Enemy(0, 0);
            deserializeEntity(enemy, stream);
            entityManager.setEnemy(enemy);
        }
        stream >> section;
    } else {
        throw std::runtime_error("Неверный формат секции врагов");
    }
    
    if (section == "TOWER") {
        auto tower = entityManager.getTower();
        if (!tower) {
            tower = new Tower(0, 0);
            entityManager.setTower(tower);
        }
        deserializeEntity(tower, stream);
        stream >> section;
    } else if (section != "NO_TOWER") {
        throw std::runtime_error("Неверный формат секции башни");
    }

    if (section == "MAP") {
        deserializeMap(entityManager, stream);
    } else {
        throw std::runtime_error("Неверный формат секции карты");
    }
}

void SaveManager::serializeEntity(const Entity* entity, std::ostream& stream) const {
    if (!entity) {
        throw std::runtime_error("Попытка сериализации nullptr entity");
    }
    
    if (dynamic_cast<const Player*>(entity)) {
        stream << "PLAYER_TYPE ";
    } else if (dynamic_cast<const Enemy*>(entity)) {
        stream << "ENEMY_TYPE ";
    } else if (dynamic_cast<const Tower*>(entity)) {
        stream << "TOWER_TYPE ";
    } else {
        throw std::runtime_error("Неизвестный тип entity для сериализации");
    }
    
    stream << entity->getPos().x << " " << entity->getPos().y << " ";
    stream << static_cast<int>(entity->getFaction()) << " ";
    stream << entity->getHP() << " " << entity->getDamage();
    
    if (const Player* player = dynamic_cast<const Player*>(entity)) {
        stream << " " << player->getStars();
    }

    if (stream.fail()) {
        throw std::runtime_error("Ошибка записи данных entity");
    }
}

void SaveManager::deserializeEntity(Entity* entity, std::istream& stream) { 
    if (!entity) {
        throw std::runtime_error("Попытка десериализации в nullptr entity");
    }
    
    std::string entityType;
    if (!(stream >> entityType)) {
        throw std::runtime_error("Ошибка чтения типа entity");
    }
    
    int x, y, factionValue, hp, damage;
    if (!(stream >> x >> y >> factionValue >> hp >> damage)) {
        throw std::runtime_error("Ошибка чтения основных параметров entity");
    }

    if (hp < 0 || damage < 0 || x < 0 || y < 0) {
        throw std::runtime_error("Некорректные значения параметров entity");
    }

    if (entityType == "PLAYER_TYPE") {
        Player* player = dynamic_cast<Player*>(entity);
        if (player) {
            int stars;
            if (stream >> stars) {
                if (stars < 0) {
                    throw std::runtime_error("Некорректное количество звезд");
                }
                std::cout << "Stars: " << stars << "\n";
                player->setStars(stars);
            }
        }
    }
    
    entity->setPos(x, y);
    entity->setFaction(static_cast<Faction>(factionValue));
    entity->setHP(hp);
    entity->setDamage(damage);  
}

void SaveManager::serializeSpellHand(const SpellHand* hand, std::ostream& stream) const {
    if (!hand) {
        throw std::runtime_error("Попытка сериализации nullptr SpellHand");
    }
   
    stream << hand->getSize() << " " << hand->getMaxSize();
    
    for (size_t i = 0; i < hand->getSize(); ++i) {
        const SpellCard* spell = hand->getSpell(i);
        if (spell) {
            stream << " ";
            serializeSpellCard(spell, stream);
        }
    }

    if (stream.fail()) {
        throw std::runtime_error("Ошибка записи данных руки заклинаний");
    }
}

void SaveManager::deserializeSpellHand(SpellHand* hand, std::istream& stream) {
    if (!hand) {
        throw std::runtime_error("Попытка десериализации в nullptr SpellHand");
    }
    
    size_t spellCount, maxSize;
    if (!(stream >> spellCount >> maxSize)) {
        throw std::runtime_error("Ошибка чтения параметров руки заклинаний");
    }

    if (spellCount > maxSize) {
        throw std::runtime_error("Некорректные данные руки заклинаний: количество превышает максимальный размер");
    }

    while (hand->getSize() > 0) {
        hand->removeSpell(0);
    }
    
    for (size_t i = 0; i < spellCount; ++i) {
        SpellCard* spell = deserializeSpellCard(stream);
        if (spell) {
            hand->addSpell(spell);
        } else {
            throw std::runtime_error("Ошибка создания заклинания");
        }
    }
}

void SaveManager::serializeSpellCard(const SpellCard* spell, std::ostream& stream) const {
    if (!spell) {
        throw std::runtime_error("Попытка сериализации nullptr SpellCard");
    }
    
    if (dynamic_cast<const AoeDamageSpell*>(spell)) {
        stream << "AOE_SPELL";
    } else if (dynamic_cast<const DirectDamageSpell*>(spell)) {
        stream << "DIRECT_DAMAGE_SPELL";
    } else if (dynamic_cast<const TrapSpell*>(spell)) {
        stream << "TRAP_SPELL";
    } else {
        stream << "UNKNOWN_SPELL";
    }
    
    stream << " " << spell->getRange();
    
    if (const AoeDamageSpell* aoe = dynamic_cast<const AoeDamageSpell*>(spell)) {
        stream << " " << aoe->getDamage();
    } else if (const DirectDamageSpell* direct = dynamic_cast<const DirectDamageSpell*>(spell)) {
        stream << " " << direct->getDamage();
    } else if (const TrapSpell* trap = dynamic_cast<const TrapSpell*>(spell)) {
        stream << " " << trap->getDamage();
    }

    if (stream.fail()) {
        throw std::runtime_error("Ошибка записи данных заклинания");
    }
}

SpellCard* SaveManager::deserializeSpellCard(std::istream& stream) const {
    std::string spellType;
    if (!(stream >> spellType)) {
        throw std::runtime_error("Ошибка чтения типа заклинания");
    }
    
    int range;
    if (!(stream >> range)) {
        throw std::runtime_error("Ошибка чтения радиуса заклинания");
    }
    
    if (range < 0) {
        throw std::runtime_error("Некорректный радиус заклинания");
    }

    int damage;
    if (!(stream >> damage)) {
        throw std::runtime_error("Ошибка чтения урона заклинания");
    }

    if (damage < 0) {
        throw std::runtime_error("Некорректный урон заклинания");
    }

    if (spellType == "AOE_SPELL") {
        return new AoeDamageSpell(range, damage);     
    } else if (spellType == "DIRECT_DAMAGE_SPELL") {
        return new DirectDamageSpell(range, damage);
    } else if (spellType == "TRAP_SPELL") {
        return new TrapSpell(range, damage);
    } else {
        throw std::runtime_error("Некорректный тип заклинания");
    }
}

void SaveManager::serializeMap(const Map* map, std::ostream& stream) const {
    if (!map) {
        throw std::runtime_error("Попытка сериализации nullptr Map");
    }
    
    stream << map->getWidth() << " " << map->getHeight() << "\n";

    if (stream.fail()) {
        throw std::runtime_error("Ошибка записи размеров карты");
    }

    for (int y = 0; y < map->getHeight(); ++y) {
        for (int x = 0; x < map->getWidth(); ++x) {
            const Tile& tile = map->getTile(x, y);
            stream << static_cast<int>(tile.getType()) << " " << tile.isWalkable() << " ";
        }
        stream << "\n";
    }

    if (stream.fail()) {
        throw std::runtime_error("Ошибка записи данных тайлов карты");
    }
}

void SaveManager::deserializeMap(eManager& entityManager, std::istream& stream) {
    int width, height;
    if (!(stream >> width >> height)) {
        throw std::runtime_error("Ошибка чтения размеров карты");
    }

    if (width <= 0 || height <= 0) {
        throw std::runtime_error("Некорректные размеры карты");
    }

    Map* newMap = new Map(width, height);
    
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int typeValue;
            bool walkable;
            if (!(stream >> typeValue >> walkable)) {
                delete newMap;
                throw std::runtime_error("Ошибка чтения тайла");
            }
            newMap->setTile(x, y, static_cast<TileType>(typeValue));
        }
    }

    entityManager.setMap(newMap);
    
    std::cout << "Карта загружена: " << width << "x" << height << std::endl;
}
