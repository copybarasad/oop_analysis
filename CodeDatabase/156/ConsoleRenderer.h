//
// Created by Artem on 20.11.2025.
//

#ifndef LABAOOP2_CONSOLERENDERER_H
#define LABAOOP2_CONSOLERENDERER_H


#include <vector>
#include <memory>
#include "../Entities/Entity.h"
#include "../GameField/Field.h"
#include "../Entities/Creatures/PlayerView.h"

class Player;

class ConsoleRenderer {
public:
    ConsoleRenderer(const Field &field, const std::vector<std::shared_ptr<Entity>> &entities,
                    PlayerView &player) : field(field), entities(entities), player(player) {}

    void draw();
    void prepareConsole();
    void clearDisplay();

private:
    const Field& field;
    const std::vector<std::shared_ptr<Entity>>& entities;
    PlayerView player;

    const std::string WALL = "■";        // Черный квадрат
    const std::string ENEMY = "●";       // Черный круг
    const std::string ENEMY_TOWER = "▲"; // Черный треугольник
    const std::string SPAWNER = "♦";     // Черный ромб
    const std::string ALLY = "A";        // Белый круг
    const std::string EXIT = "→";        // Стрелка вправо
    const std::string TRAP = "×";        // Крестик
    const std::string PLAYER = "◎";      // Круг в круге
    const std::map<EntityType, std::string> entityTypeToCharS = {
            {EntityType::PlayerEnt, PLAYER},
            {EntityType::EnemyDefenceTower, ENEMY_TOWER},
            {EntityType::EnemyEnt, ENEMY},
            {EntityType::Ally, ALLY},
            {EntityType::EnemySpawnerBuildingEnt, SPAWNER}
    };
    const std::map<CellEventType, std::string> cellEventTypeToCharS = {
            {CellEventType::TrapEvent, TRAP},
            {CellEventType::SlowingEvent, " "}
    };
    const std::map<EntityType, std::string> entityTypeToStr = {
            {EntityType::PlayerEnt, "Player"},
            {EntityType::EnemyDefenceTower, "Tower"},
            {EntityType::EnemyEnt, "Enemy"},
            {EntityType::Ally, "Ally"},
            {EntityType::EnemySpawnerBuildingEnt, "Spawner"}
    };

    std::string connectStrings(const std::string& grid, const std::string& entitiesInfo, const std::string& playerInfo, const std::string& legend, const std::string lastActions);


    std::string getStringLegend();
    std::string getStringWithGridRounded(const std::vector<std::vector<std::string>>& grid);
    std::string getStringWithEntitiesInfo();
    std::string getStringWithPlayerInfo();
    std::string getStringWithLastActions();
};


#endif //LABAOOP2_CONSOLERENDERER_H
