#include "SpellPurchaseStrategy.h"
#include "../PlayerController.h"
#include "../HandController.h"
#include "InputController.h"
#include "../../models/spells/ISpell.h"
#include "../../models/events/EventNotifier.h"
#include "../../models/events/SpellObtainedEvent.h"
#include "../../models/events/PrintMessageEvent.h"
#include <iostream>
#include <limits>
#include <sstream>

SpellPurchaseStrategy::SpellPurchaseStrategy(PlayerController* playerController, HandController* handController, InputController* inputController)
    : playerController(playerController), handController(handController), inputController(inputController) {}

bool SpellPurchaseStrategy::handle() {
    if (!inputController) {
        return true;
    }
    
    auto& player = playerController->getPlayer();
    int money = player.getMoney();
    const int costDirect = 50;
    const int costArea = 70;
    const int costTrap = 40;
    const int costImprove = 30;
    const int costSummon = 80;

    PrintMessageEvent headerEvent("Выберите заклинание для покупки:");
    EventNotifier::getInstance().notify(headerEvent);
    
    std::ostringstream option1, option2, option3, option4, option5;
    option1 << "1 - Прямой урон (" << costDirect << ")";
    option2 << "2 - Урон по площади (" << costArea << ")";
    option3 << "3 - Ловушка (" << costTrap << ")";
    option4 << "4 - Улучшение (" << costImprove << ")";
    option5 << "5 - Призыв союзника (" << costSummon << ")";
    
    PrintMessageEvent opt1Event(option1.str());
    EventNotifier::getInstance().notify(opt1Event);
    PrintMessageEvent opt2Event(option2.str());
    EventNotifier::getInstance().notify(opt2Event);
    PrintMessageEvent opt3Event(option3.str());
    EventNotifier::getInstance().notify(opt3Event);
    PrintMessageEvent opt4Event(option4.str());
    EventNotifier::getInstance().notify(opt4Event);
    PrintMessageEvent opt5Event(option5.str());
    EventNotifier::getInstance().notify(opt5Event);
    
    int choice = inputController->getIntInput("Введите номер: ");

    SpellType spellType;
    int cost = 0;
    switch (choice) {
        case 1: spellType = SpellType::DIRECT_DAMAGE; cost = costDirect; break;
        case 2: spellType = SpellType::AREA_DAMAGE; cost = costArea; break;
        case 3: spellType = SpellType::TRAP; cost = costTrap; break;
        case 4: spellType = SpellType::IMPROVE; cost = costImprove; break;
        case 5: spellType = SpellType::SUMMON; cost = costSummon; break;
        default:
            PrintMessageEvent unknownEvent("Неизвестный выбор.");
            EventNotifier::getInstance().notify(unknownEvent);
            return true;
    }

    if (money < cost) {
        std::ostringstream errorMsg;
        errorMsg << "Недостаточно денег. Нужно: " << cost << ", у вас: " << money;
        PrintMessageEvent errorEvent(errorMsg.str());
        EventNotifier::getInstance().notify(errorEvent);
        return true;
    }

    bool added = player.getHand().addSpell(spellType);
    if (!added) {
        PrintMessageEvent fullHandEvent("Заклинание не было добавлено (возможно, рука заполнена).");
        EventNotifier::getInstance().notify(fullHandEvent);
        return true;
    }

    player.setMoney(money - cost);
    
    // Уведомляем о получении заклинания
    SpellObtainedEvent obtainedEvent(spellType, "покупка");
    EventNotifier::getInstance().notify(obtainedEvent);
    
    std::ostringstream successMsg;
    successMsg << "Заклинание успешно куплено! Остаток: " << player.getMoney();
    PrintMessageEvent successEvent(successMsg.str());
    EventNotifier::getInstance().notify(successEvent);
    return true;
}

