#include "CombatModeStrategy.h"
#include "../PlayerController.h"
#include "../../models/events/EventNotifier.h"
#include "../../models/events/CombatModeSwitchEvent.h"
#include "../../models/events/PrintMessageEvent.h"
#include <iostream>

CombatModeStrategy::CombatModeStrategy(PlayerController* playerController)
    : playerController(playerController) {}

bool CombatModeStrategy::handle() {
    auto& player = playerController->getPlayer();
    player.switchCombatMode();
    player.setCanMove(false);
    
    // Уведомляем о смене режима боя
    CombatModeSwitchEvent switchEvent(player.getCombatMode());
    EventNotifier::getInstance().notify(switchEvent);
    
    PrintMessageEvent modeEvent("Режим боя изменен!");
    EventNotifier::getInstance().notify(modeEvent);
    return true;
}

