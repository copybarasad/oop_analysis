#pragma once
#include "./Entities/Hero.hpp"
#include "./Entities/Enemy.hpp"
#include "./Field/Field.hpp"
#include "./Entities/Portal.hpp"
#include "./Entities/Slime.hpp"
#include "./Entities/Demon.hpp"
#include "./Entities/Shadow.hpp"  
#include "./Entities/MagicStatue.hpp"
#include <vector>
#include "./Entities/Knight.hpp"
#include "./Systems/InterfaceGameLogger.hpp"
#include "./Spells/InterfaceSpell.hpp"

class Entity_Manager {
private:
    Field* field;
    Portal* portal;
    Hero* hero;
    MagicStatue* statue;
    std::vector<Enemy*> enemies;
    std::vector<Knight*> allies;

    int static constexpr NUMBER_OF_ENEMIES = 3;
    int static constexpr NUMBER_OF_PORTALS = 1;
    int static constexpr NUMBER_OF_HEROS = 1;
    int static constexpr NUMBER_OF_STATUE = 1;
    float static constexpr CHANCE_TO_GO_TO_THE_HERO = 0.7;
    int static constexpr MAX_COUNT_ENEMIES_ON_FIELD = 8;

    
public:
    Entity_Manager(Field* field, int level = 1, bool from_file = false);
    ~Entity_Manager(); 

    void moveAllEnemies(Field& field, InterfaceGameLogger& logger_consol);
    void moveAllAllies(Field& field, InterfaceGameLogger& logger_consol);
    bool allEnemiesDead();
    void removeDeadEnemies();
    void removeDeadAllies();
    void spawn_enemis_by_portal(std::vector<std::vector<Cell*>>& cells, Field& field);
    void destroyPortal();
    bool isPortalAlive() { return portal != nullptr; }
    
    std::vector<Enemy*>& get_enemies() { return enemies; }
    std::vector<Knight*>& get_allies() { return allies; }
    
    void addAlly(Knight* knight) { allies.push_back(knight); std::cout << "Проверяем после добавления массив Allies in manager: " << allies.size() << std::endl; }

    Hero* get_hero() { return hero; }
    Portal* get_portal() { return portal; }
    MagicStatue* get_statue() { return statue; }
    void clearAllEntities(bool flag_delete_hero);
    void spawnEnemy(const std::string& type, int x, int y, int hp);
    void set_portal(Portal* new_portal) { portal = new_portal; }
    void set_statue(MagicStatue* new_statue) { statue = new_statue; }
    void set_hero(Hero* newHero);
    
    void updateField(Field* newField, Hero* hero, int level = 1, bool fromSave = false);
    void initializeEntities(int level = 1, bool fromSave = false);
};