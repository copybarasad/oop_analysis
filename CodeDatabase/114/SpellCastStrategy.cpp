#include "SpellCastStrategy.h"
#include "../PlayerController.h"
#include "../HandController.h"
#include "InputController.h"
#include "../../models/spells/ISpell.h"
#include "../../models/events/EventNotifier.h"
#include "../../models/events/PrintMessageEvent.h"
#include <iostream>
#include <limits>
#include <sstream>

SpellCastStrategy::SpellCastStrategy(PlayerController* playerController, HandController* handController, InputController* inputController)
    : playerController(playerController), handController(handController), inputController(inputController) {}

bool SpellCastStrategy::handle() {
    if (!inputController) {
        return true;
    }
    
    auto& player = playerController->getPlayer();
    const auto& spells = player.getHand().getSpells();
    if (spells.empty()) {
        PrintMessageEvent emptyEvent("В руке нет заклинаний.");
        EventNotifier::getInstance().notify(emptyEvent);
        return true;
    }

    PrintMessageEvent headerEvent("Заклинания в руке:");
    EventNotifier::getInstance().notify(headerEvent);
    
    for (std::size_t i = 0; i < spells.size(); ++i) {
        std::ostringstream spellMsg;
        spellMsg << i << ": ";
        bool requiresTarget = false;
        switch (spells[i]) {
            case SpellType::DIRECT_DAMAGE: spellMsg << "Прямой урон"; requiresTarget = true; break;
            case SpellType::AREA_DAMAGE: spellMsg << "Урон по площади"; requiresTarget = true; break;
            case SpellType::TRAP: spellMsg << "Ловушка"; requiresTarget = false; break;
            case SpellType::IMPROVE: spellMsg << "Улучшение"; requiresTarget = false; break;
            case SpellType::SUMMON: spellMsg << "Призыв союзника"; requiresTarget = false; break;
            default: spellMsg << "Неизвестно"; break;
        }
        spellMsg << (requiresTarget ? " (требует цель)" : "");
        PrintMessageEvent spellEvent(spellMsg.str());
        EventNotifier::getInstance().notify(spellEvent);
    }

    int idx = inputController->getIntInput("Выберите индекс заклинания: ");
    if (idx < 0 || static_cast<std::size_t>(idx) >= spells.size()) {
        PrintMessageEvent invalidEvent("Неверный индекс.");
        EventNotifier::getInstance().notify(invalidEvent);
        return true;
    }

    bool requiresTarget = (spells[idx] == SpellType::DIRECT_DAMAGE || spells[idx] == SpellType::AREA_DAMAGE);
    bool success = false;
    if (requiresTarget) {
        int tx, ty;
        inputController->getIntPairInput("Введите координаты цели (x y): ", tx, ty);
        success = handController->castSpellFromHandIndex(static_cast<std::size_t>(idx), tx, ty);
    } else {
        success = handController->castSpellFromHandIndex(static_cast<std::size_t>(idx));
    }

    if (success) {
        PrintMessageEvent successEvent("Заклинание применено.");
        EventNotifier::getInstance().notify(successEvent);
    }
    return true;
}

