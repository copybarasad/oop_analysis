#pragma once
#include <vector>
#include "types.hpp"
#include <random>
#include "GameObjects/Spells/Hand.hpp"
#include "LevelManager/LMtypes.hpp"
#include "InputController/commands.hpp"
#include "LevelManager/LevelManager.hpp"
#include "GameObjects/Entities/Player.hpp"
#include "GameClass/EnemyController.hpp"
#include "GameClass/MapController.hpp"
#include "GameClass/TowerController.hpp"


class CastController {
public:
    CastController(int level);
    CastDTO toDTO();

    void executeContracts(MapController& map, EnemyController& enemies, TowerController& towers, Player& player);

    void confirm();

    void startCast(Motion cmd, Coordinates playerPos, int mapH, int mapW);

    void moveTarget(Motion cmd);

    int handSize();
    std::vector<Coordinates> getAllowedSteps();
    Coordinates getTarget();
    void goToNextLevel();
    void addSpell();
    void upgradeSpell(SpellType spell);
    bool canUseHere(FieldType ft);
private:
    LevelManager LM;
    Hand hand;
    std::vector<SpellContract> contracts;
    int spellIndex;
    CastDTO dto;

    void startSpell(Coordinates playerPos, int mapH, int mapW);
};
