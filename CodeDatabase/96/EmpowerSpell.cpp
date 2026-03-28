#include "EmpowerSpell.h"



bool EmpowerSpell::Cast(SpellContext& ctx) {
    if (ctx.spell == nullptr){
        return 0;
    }
    for (int i = 0; i<bonus;i++) {
        ctx.spell->enhance();
    }
    return 1;
}

void EmpowerSpell::enhance() {
    bonus += 1;
}

std::shared_ptr<dataSpell> EmpowerSpell::Save(){
    auto data = std::make_shared<empowerSpell>();
    data->type = SpellType::empower;
    data->bonus = bonus;
    return data;
}