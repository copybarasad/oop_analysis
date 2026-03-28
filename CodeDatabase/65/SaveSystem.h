#pragma once

#include <fstream>
#include <string>
#include <vector>
#include <memory>
#include <sys/stat.h>
#include "exceptions/GameException.h"
#include "Field.h"
#include "objects/Player.h"
#include "objects/Enemy.h"
#include "objects/Tower.h"
#include "objects/DamageTower.h"
#include "objects/SlowCell.h"
#include "objects/StaticObject.h"
#include "spells/SpellFactory.h"

class FileHandler {
private:
    std::fstream file;
    std::string filename;
    bool isOpen;

public:
    FileHandler(const std::string& fname, std::ios::openmode mode)
        : filename(fname), isOpen(false) {
        file.open(filename, mode);
        if (!file.is_open()) {
            if (mode & std::ios::in) {
                throw FileNotFoundException(filename);
            } else {
                throw FileAccessException(filename, "open");
            }
        }
        isOpen = true;
    }

    ~FileHandler() {
        if (isOpen && file.is_open()) {
            file.close();
        }
    }

    FileHandler(const FileHandler&) = delete;
    FileHandler& operator=(const FileHandler&) = delete;

    FileHandler(FileHandler&& other) noexcept
        : file(std::move(other.file)), filename(std::move(other.filename)),
          isOpen(other.isOpen) {
        other.isOpen = false;
    }

    std::fstream& get() {
        if (!isOpen || !file.is_open()) {
            throw FileAccessException(filename, "access");
        }
        return file;
    }

    bool good() const {
        return file.good();
    }

    void checkState() const {
        if (!good()) {
            throw CorruptedDataException("File stream is in bad state: " + filename);
        }
    }
};

class SaveSystem {
private:
    static constexpr const char* SAVE_FILE = "savegame.dat";
    static constexpr int MAGIC_NUMBER = 0x47414D45; // "GAME"
    static constexpr int VERSION = 1;
    static constexpr int MAX_STRING_LEN = 1000;

    template<typename T>
    void writeValue(std::fstream& file, const T& value) {
        file.write(reinterpret_cast<const char*>(&value), sizeof(T));
        if (!file.good()) {
            throw SaveException("Failed to write data to file");
        }
    }

    template<typename T>
    T readValue(std::fstream& file) {
        T value;
        file.read(reinterpret_cast<char*>(&value), sizeof(T));
        if (!file.good()) {
            throw CorruptedDataException("Failed to read data from file");
        }
        return value;
    }

    void writeString(std::fstream& file, const std::string& str) {
        size_t length = str.length();
        writeValue(file, length);
        file.write(str.c_str(), length);
        if (!file.good()) {
            throw SaveException("Failed to write string to file");
        }
    }

    std::string readString(std::fstream& file) {
        size_t length = readValue<size_t>(file);
        if (length > MAX_STRING_LEN) {
            throw CorruptedDataException("String length is too large: " + std::to_string(length));
        }
        std::string str(length, '\0');
        file.read(&str[0], length);
        if (!file.good()) {
            throw CorruptedDataException("Failed to read string from file");
        }
        return str;
    }

    void savePlayer(std::fstream& file, const Player* player) {
        writeValue(file, player->getX());
        writeValue(file, player->getY());
        
        writeValue(file, player->getHealth());
        writeValue(file, player->getMaxHealth());
        writeValue(file, player->getDamage());
        writeValue(file, player->getSpeed());
        writeValue(file, player->getRangeDamage());
        writeValue(file, player->getRange());
        
        writeValue(file, player->getXP());
        writeValue(file, player->getLevel());
        writeValue(file, player->getXPToNextLevel());
        
        writeValue(file, static_cast<int>(player->getAttackMode()));
        
        const Hand& hand = const_cast<Player*>(player)->getHand();
        writeValue(file, hand.getSize());
        writeValue(file, hand.getMaxSize());
        
        for (int i = 0; i < hand.getSize(); i++) {
            ISpell* spell = const_cast<Hand&>(hand).getSpell(i);
            if (spell) {
                writeValue(file, static_cast<int>(spell->getType()));
                writeValue(file, spell->getRange());
                writeValue(file, spell->getDamage());
                writeValue(file, spell->getAreaSize());
            }
        }
    }

    void loadPlayer(std::fstream& file, Player* player) {
        int x = readValue<int>(file);
        int y = readValue<int>(file);
        player->setX(x);
        player->setY(y);
        
        int health = readValue<int>(file);
        int maxHealth = readValue<int>(file);
        int damage = readValue<int>(file);
        int speed = readValue<int>(file);
        int rangeDamage = readValue<int>(file);
        int range = readValue<int>(file);
        
        player->setHealth(health);
        player->setMaxHealth(maxHealth);
        player->setDamage(damage);
        player->setSpeed(speed);
        player->setRangeDamage(rangeDamage);
        player->setRange(range);
        
        int xp = readValue<int>(file);
        int level = readValue<int>(file);
        int xpToNext = readValue<int>(file);
        
        int attackMode = readValue<int>(file);
        
        int handSize = readValue<int>(file);
        int maxSize = readValue<int>(file);
        
        Hand& hand = player->getHand();
        
        while (hand.getSize() > 0) {
            hand.removeSpell(0);
        }
        
        player->setXP(xp);
        player->setLevel(level);
        player->setXPToNextLevel(xpToNext);
        player->setAttackMode(static_cast<AttackMode>(attackMode));

        for (int i = 0; i < handSize; i++) {
            SpellType type = static_cast<SpellType>(readValue<int>(file));
            int spellRange = readValue<int>(file);
            int spellDamage = readValue<int>(file);
            int areaSize = readValue<int>(file);
            
            auto spell = SpellFactory::createSpell(type, spellRange, spellDamage, areaSize);
            if (spell) {
                hand.addSpell(std::move(spell));
            }
        }
    }

    void saveField(std::fstream& file, const Field* field) {
        writeValue(file, field->getWidth());
        writeValue(file, field->getHeight());
        
        const auto& objects = field->getObjects();
        writeValue(file, static_cast<int>(objects.size()));
        
        for (const auto& obj : objects) {
            ObjectTag tag = obj->getTags();
            writeValue(file, static_cast<int>(tag));
            
            writeValue(file, obj->getX());
            writeValue(file, obj->getY());
            writeString(file, std::string(1, obj->getSprite()));
            writeString(file, obj->getColor());
            
            if (tag & ObjectTag::TAG_ENEMY) {
                Enemy* enemy = dynamic_cast<Enemy*>(obj.get());
                if (enemy) {
                    writeValue(file, enemy->getHealth());
                    writeValue(file, enemy->getMaxHealth());
                    writeValue(file, enemy->getDamage());
                    writeValue(file, enemy->getSpeed());
                    writeValue(file, enemy->getAggroRange());
                }
            } else if (tag & ObjectTag::TAG_TOWER) {
                Entity* tower = dynamic_cast<Entity*>(obj.get());
                if (tower) {
                    writeValue(file, tower->getHealth());
                    writeValue(file, tower->getMaxHealth());
                    writeValue(file, tower->getDamage());
                    writeValue(file, tower->getSpeed());
                }
            }
        }
    }

    void loadField(std::fstream& file, Field* field) {
        int width = readValue<int>(file);
        int height = readValue<int>(file);
        
        if (width != field->getWidth() || height != field->getHeight()) {
            throw CorruptedDataException(
                "Field dimensions mismatch. This should not happen. Expected: " + 
                std::to_string(field->getWidth()) + "x" + std::to_string(field->getHeight()) +
                ", got: " + std::to_string(width) + "x" + std::to_string(height)
            );
        }
        
        int numObjects = readValue<int>(file);
        
        if (numObjects < 0 || numObjects > 10000) {
            throw CorruptedDataException("Invalid number of objects: " + std::to_string(numObjects));
        }
        
        for (int i = 0; i < numObjects; i++) {
            ObjectTag tag = static_cast<ObjectTag>(readValue<int>(file));
            
            int x = readValue<int>(file);
            int y = readValue<int>(file);
            std::string spriteStr = readString(file);
            std::string color = readString(file);
            char sprite = spriteStr.empty() ? '?' : spriteStr[0];
            
            if (tag & ObjectTag::TAG_ENEMY) {
                int health = readValue<int>(file);
                int maxHealth = readValue<int>(file);
                int damage = readValue<int>(file);
                int speed = readValue<int>(file);
                int aggroRange = readValue<int>(file);
                
                auto enemy = std::make_unique<Enemy>(x, y, sprite, color, health, damage, speed, aggroRange, tag);
                enemy->setMaxHealth(maxHealth);
                enemy->setHealth(health);
                field->addObject(std::move(enemy));
            } else if (tag & ObjectTag::TAG_TOWER) {
                int health = readValue<int>(file);
                int maxHealth = readValue<int>(file);
                int damage = readValue<int>(file);
                int speed = readValue<int>(file);
                
                std::unique_ptr<GameObject> tower;
                if (tag & ObjectTag::TAG_DAMAGE_TOWER) {
                    tower = std::make_unique<DamageTower>(x, y);
                } else {
                    tower = std::make_unique<Tower>(x, y);
                }
                
                if (Entity* e = dynamic_cast<Entity*>(tower.get())) {
                    e->setHealth(health);
                    e->setMaxHealth(maxHealth);
                    e->setDamage(damage);
                }
                field->addObject(std::move(tower));
            } else if (tag & ObjectTag::TAG_SLOW_CELL) {
                field->addObject(std::make_unique<SlowCell>(x, y));
            } else if (tag & ObjectTag::TAG_WALL) {
                field->addObject(std::make_unique<Wall>(x, y));
            }
        }
    }

public:
    struct SaveInfo {
        int level;
        int fieldWidth;
        int fieldHeight;
    };

    SaveInfo getSaveInfo() {
        try {
            FileHandler handler(SAVE_FILE, std::ios::in | std::ios::binary);
            std::fstream& file = handler.get();
            
            int magic = readValue<int>(file);
            if (magic != MAGIC_NUMBER) {
                throw CorruptedDataException("Invalid file format (magic number mismatch)");
            }
            
            int version = readValue<int>(file);
            if (version != VERSION) {
                throw CorruptedDataException(
                    "Incompatible save file version. Expected: " + 
                    std::to_string(VERSION) + ", got: " + std::to_string(version)
                );
            }
            
            SaveInfo info;
            info.level = readValue<int>(file);
            info.fieldWidth = readValue<int>(file);
            info.fieldHeight = readValue<int>(file);
            
            return info;
            
        } catch (const GameException& e) {
            throw;
        } catch (const std::exception& e) {
            throw LoadException(std::string("Failed to read save info: ") + e.what());
        }
    }

    void save(const Field* field, const Player* player, int currentLevel) {
        try {
            FileHandler handler(SAVE_FILE, std::ios::out | std::ios::binary | std::ios::trunc);
            std::fstream& file = handler.get();
            
            writeValue(file, MAGIC_NUMBER);
            writeValue(file, VERSION);
            writeValue(file, currentLevel);
            
            writeValue(file, field->getWidth());
            writeValue(file, field->getHeight());
            
            savePlayer(file, player);
            saveField(file, field);
            
            handler.checkState();
            
        } catch (const GameException& e) {
            throw;
        } catch (const std::exception& e) {
            throw SaveException(std::string("Unexpected error: ") + e.what());
        }
    }

    void load(Field* field, Player* player, int& currentLevel) {
        try {
            FileHandler handler(SAVE_FILE, std::ios::in | std::ios::binary);
            std::fstream& file = handler.get();
            
            int magic = readValue<int>(file);
            if (magic != MAGIC_NUMBER) {
                throw CorruptedDataException("Invalid file format (magic number mismatch)");
            }
            
            int version = readValue<int>(file);
            if (version != VERSION) {
                throw CorruptedDataException(
                    "Incompatible save file version. Expected: " + 
                    std::to_string(VERSION) + ", got: " + std::to_string(version)
                );
            }
            
            currentLevel = readValue<int>(file);
            
            int savedWidth = readValue<int>(file);
            int savedHeight = readValue<int>(file);
            
            if (savedWidth != field->getWidth() || savedHeight != field->getHeight()) {
                throw CorruptedDataException(
                    "Field dimensions mismatch. Expected: " + 
                    std::to_string(field->getWidth()) + "x" + std::to_string(field->getHeight()) +
                    ", got: " + std::to_string(savedWidth) + "x" + std::to_string(savedHeight)
                );
            }
            
            loadPlayer(file, player);
            loadField(file, field);
            
            handler.checkState();
        } catch (const GameException& e) {
            throw;
        } catch (const std::exception& e) {
            throw LoadException(std::string("Unexpected error: ") + e.what());
        }
    }

    bool saveExists() const {
        struct stat buffer;
        return (stat(SAVE_FILE, &buffer) == 0);
    }

    void deleteSave() {
        if (saveExists()) {
            if (remove(SAVE_FILE) != 0) {
                throw SaveException("Failed to delete save file");
            }
        }
    }
};
