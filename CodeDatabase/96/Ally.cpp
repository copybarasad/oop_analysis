#include "Ally.h"


Ally::Ally(dataAlly data):Entity(data.health, data.damage) {
    SetStunDuration(data.stunDuration);
}

dataAlly Ally::Save(){
    dataAlly data;
    data.damage=damage;
    data.health=health;
    data.stunDuration=stunDuration;
    return data;
}