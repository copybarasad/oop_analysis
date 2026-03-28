#ifndef PLAYER_TURN_PROC_H
#define PLAYER_TURN_PROC_H

#include <map>

#include "../BaseTurnProcessor/TurnProcessor.hpp"
#include "../../../Services/MovementService/PlayerMovementService/PlayerMovementService.hpp"
#include "../../../Services/CombatService/CombatService.hpp"
#include "../../../Services/CastService/CastService/CastService.hpp"
#include "../../../Services/BuySpellService/BuySpellService.hpp"
#include "../../../Factories/SpellFactories/DirectSpellFactory/DirectSpellFactory.hpp"
#include "../../../Factories/SpellFactories/AreaSpellFactory/AreaSpellFactory.hpp"
#include "../../../Factories/SpellFactories/TrapSpellFactory/TrapSpellFactory.hpp"
#include "../../../Factories/SpellFactories/AllySpellFactory/AllySpellFactory.hpp"
#include "../../../Factories/SpellFactories/BuffSpellFactory/BuffSpellFactory.hpp"
#include "../../../Services/CastService/CastResultProcessor/DamageCastResultProcessor/DamageCastResultProcessor.hpp"
#include "../../../Services/CastService/CastResultProcessor/SpawnCastResultProcessor/SpawnCastResultProcessor.hpp"
#include "../../../ChangeableFields/MoveChangeableFields/MoveChangeableFields.hpp"
#include "../../../ChangeableFields/SwitchChangeableFields/SwitchChangeableFields.hpp"
#include "../../../ChangeableFields/BuySpellFields/BuySpellFields.hpp"

class PlayerTurnProcessor : public TurnProcessor {
    private:
    PlayerMovementService movementService;
    CombatService combatService;
    CastService castService;
    BuySpellService buySpellService;
    std::string action;
    ChangeableFields* changeableFields;

    public:
    PlayerTurnProcessor (size_t id, EntityManager& manager);

    void setAction (std::string action);
    void setChangeableFields (ChangeableFields* changeableFields);
    TurnResult run () override;

    private:
    void processPlayerTurn (TurnResult& turnRes);
    bool processMove ();
    bool processFight (Cell* currentCell, Cell* targetCell);
    bool processCast ();
    bool processSwitch ();
    bool processBuy ();
    void initializeCastService ();
};

#endif