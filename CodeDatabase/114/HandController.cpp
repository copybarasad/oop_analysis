#include "HandController.h"
#include "../models/events/EventNotifier.h"
#include "../models/events/SpellCastEvent.h"
#include "../models/events/SpellObtainedEvent.h"
#include "ISpell.h"
#include "Player.h"
#include "Hand.h"
#include <memory>
#include <optional>
#include <functional>

bool HandController::castSpellFromHandIndex(std::size_t index, std::optional<int> targetX, std::optional<int> targetY) {
    Player& player = field->getPlayer();
    Hand& hand = player.getHand();
    const auto& spells = hand.getSpells();
    if (index >= spells.size()) return false;

    if (hand.getSpellCastDelayActive()) {
        hand.setSpellCastDelayActive(false);
        return false;
    }

    SpellType spellType = spells[index];
    
    // Используем фабрику для создания заклинания
    std::unique_ptr<ISpell> spell = spellFactory->createSpell(spellType);
    if (!spell) return false;

    Entity* target = nullptr;
    if (spell->getHasTarget()) {
        if (!targetX.has_value() || !targetY.has_value()) return false;
        auto entityOpt = field->findEntityAt(*targetX, *targetY);
        if (!entityOpt.has_value()) return false;
        target = &entityOpt.value().get();
    }

    int boostToUse = hand.getPendingBoostMultiplier();
    bool applied = spell->cast(boostToUse, target, field);
    
    // Уведомляем о применении заклинания
    std::optional<int> targetXOpt = targetX;
    std::optional<int> targetYOpt = targetY;
    SpellCastEvent castEvent(spellType, applied, targetXOpt, targetYOpt);
    EventNotifier::getInstance().notify(castEvent);
    
    if (!applied) return false;

    // Мертвые враги будут удалены перед ходом врага в GameLoopController
    // Это позволяет видеть результат заклинания до хода врага

    player.getHand().removeSpell(index);

    if (spellType != SpellType::IMPROVE) {
        hand.setPendingBoostMultiplier(1);
    }

    const int RADIUS = 3;
    bool nearTower = false;
    for (const auto& building : field->getBuilding()) {
        int dx = std::abs(player.getX() - building.getX());
        int dy = std::abs(player.getY() - building.getY());
        int dist = dx + dy;
        if (dist <= RADIUS) { nearTower = true; break; }
    }
    if (nearTower) {
        hand.setSpellCastDelayActive(true);
    }
    return true;
}

