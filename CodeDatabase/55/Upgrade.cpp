#include "Upgrade.h"


Upgrade::Upgrade(int i) : id(i) {
    if(id == 0){
        name = "УТЯЖЕЛИТЕЛИ НА РУКИ";
        cost = 30;
        amount = 15;
        description = "увеличивает урон ближнего боя на " + std::to_string(amount) + " ед., стоимость - " + std::to_string(cost) + " очков опыта";
    }else if(id == 1){
        name = "БЛАГОСЛОВЕНИЕ ДЕЛЬФИНОВ";
        cost = 30;
        amount = 10;
        description = "восстанавливает здоровье на " + std::to_string(amount) + " ед., стоимость - " + std::to_string(cost) + " очков опыта";
    }else if(id == 2){
        name = "БРЮХО ПЕЛИКАНА";
        cost = 50;
        amount = 1;
        description = "увеличивает размер сумки с заклинаниями на " + std::to_string(amount) + " ед., стоимость - " + std::to_string(cost) + " очков опыта";
    }
}

int Upgrade::getId(){
    return id;
}

int Upgrade::getCost(){
    return cost;
}

int Upgrade::getAmount(){
    return amount;
}

bool Upgrade::check(int score){
    return (score - cost) < 0;
}

const string Upgrade::getName(){
    return name;
}

const string Upgrade::getDescription(){
    return description;
}