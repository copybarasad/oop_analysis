#include "AttackStrategy.h"
#include "controllers/PlayerController.h"
#include "InputController.h"
#include "models/events/EventNotifier.h"
#include "models/events/PrintMessageEvent.h"
#include <iostream>
#include <limits>

AttackStrategy::AttackStrategy(PlayerController* playerController, InputController* inputController)
    : playerController(playerController), inputController(inputController) {}

bool AttackStrategy::handle() {
    if (!inputController) {
        return true;
    }
    
    int tx, ty;
    inputController->getIntPairInput("Введите координаты цели (x y): ", tx, ty);
    if (!playerController->playerAttack(tx, ty)) {
        PrintMessageEvent failEvent("Атака не удалась: проверьте дистанцию/цель.");
        EventNotifier::getInstance().notify(failEvent);
    }
    return true;
}

