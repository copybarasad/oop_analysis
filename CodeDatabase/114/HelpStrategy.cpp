#include "HelpStrategy.h"
#include "../../models/events/EventNotifier.h"
#include "../../models/events/PrintMessageEvent.h"
#include <iostream>

bool HelpStrategy::handle() {
    PrintMessageEvent msg1("w/a/s/d - движение (вверх/влево/вниз/вправо)");
    EventNotifier::getInstance().notify(msg1);
    PrintMessageEvent msg2("x - атаковать цель (введите координаты x y)");
    EventNotifier::getInstance().notify(msg2);
    PrintMessageEvent msg3("c - сменить режим боя (ближний/дальний)");
    EventNotifier::getInstance().notify(msg3);
    PrintMessageEvent msg4("b - купить заклинание (если хватает денег)");
    EventNotifier::getInstance().notify(msg4);
    PrintMessageEvent msg5("v - применить заклинание из руки");
    EventNotifier::getInstance().notify(msg5);
    PrintMessageEvent msg6("z - сохранить игру");
    EventNotifier::getInstance().notify(msg6);
    PrintMessageEvent msg7("l - загрузить игру");
    EventNotifier::getInstance().notify(msg7);
    PrintMessageEvent msg8("h - показать эту справку");
    EventNotifier::getInstance().notify(msg8);
    PrintMessageEvent msg9("q - выйти из игры");
    EventNotifier::getInstance().notify(msg9);
    return true;
}

