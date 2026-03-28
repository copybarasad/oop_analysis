#ifndef GAME_LEVEL_H
#define GAME_LEVEL_H

#include <list>
#include <map>

#include "../../Field/Field/Field.hpp"
#include "../../Managers/EntityManager/EntityManager.hpp"
#include "modules/GameBlock/TurnProcessor/PlayerTurnProcessor/PlayerTurnProcessor.hpp"
#include "modules/GameBlock/TurnProcessor/EnemyTurnProcessor/EnemyTurnProcessor.hpp"
#include "modules/GameBlock/TurnProcessor/AllyTurnProcessor/AllyTurnProcessor.hpp"
#include "modules/GameBlock/TurnProcessor/BuildingTurnProcessor/BuildingTurnProcessor.hpp"
#include "modules/GameBlock/TurnProcessor/TowerTurnProcessor/TowerTurnProcessor.hpp"

class GameLevel {
    private:
    Field field;
    SpellHand hand;
    EntityManager manager;
    SpellStore store;
    PlayerTurnProcessor playerTurn;
    AllyTurnProcessor allyTurn;
    EnemyTurnProcessor enemyTurn;
    BuildingTurnProcessor buildingTurn;
    TowerTurnProcessor towerTurn;
    size_t currentTurnID;
    bool levelCompleted;
    bool playerAlive;
    std::string pendingPlayerAction;
    ChangeableFields* pendingChangeableFields;

    public:
    GameLevel (std::pair<size_t, size_t> fieldSize, std::pair<double, double> fieldInit, size_t spellNumber, std::string baseSpell,
        std::list<std::pair<Entity*, std::pair<size_t, size_t>>> entitiesList, std::vector<std::string> storeSpells, size_t currentTurnID);
    ~GameLevel ();

    void updateState ();
    bool isLevelCompleted () const;
    bool isPlayerAlive () const;
    size_t getCurrentTurnID () const;
    bool isWaitingForPlayerInput () const;
    bool queuePlayerAction(const std::string& action, ChangeableFields* changeableFields);
    Field& getField ();
    SpellHand& getHand ();
    EntityManager& getEntityManager();
    SpellStore& getSpellStore ();

    private:
    void setEntities (std::list<std::pair<Entity*, std::pair<size_t, size_t>>> entitiesList);
    void processCurrentTurn ();
    void advanceToNextTurn ();
    void resetTurnStates ();
    void checkLevelCompletion ();
    void checkPlayer ();
};

#endif