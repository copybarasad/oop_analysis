#include "EmpowerSpell.h"
#include "Player.h"

EmpowerSpell::EmpowerSpell(int rDelta, int areaDelta, int dmgDelta, int summonDelta){
    gain_.radiusDelta = rDelta;
    gain_.aoeSizeDelta = areaDelta;
    gain_.damageDelta = dmgDelta;
    gain_.summonCountDelta = summonDelta;
}

const char* EmpowerSpell::GetName() const{
    return "Empower";
}

bool EmpowerSpell::NeedsTarget() const{
    return true;
}

int EmpowerSpell::GetRange() const{
    return 0;
}

bool EmpowerSpell::CanCast(SpellContext& context){
    return context.player != 0;
}

bool EmpowerSpell::Cast(SpellContext& context){
    if(context.player == 0){
        return false;
    }
    context.player->AddPendingModifier(gain_);
    return true;
}

void EmpowerSpell::ApplyModifier(const SpellModifier& mod){
    gain_.radiusDelta += mod.radiusDelta;
    gain_.aoeSizeDelta += mod.aoeSizeDelta;
    gain_.damageDelta += mod.damageDelta;
    gain_.summonCountDelta += mod.summonCountDelta;
}

bool EmpowerSpell::ConsumesModifier() const{
    return false;
}

bool EmpowerSpell::IsPersistent() const{
    return false;
}

ISpell* EmpowerSpell::CloneNew() const{
    return new EmpowerSpell(gain_.radiusDelta, gain_.aoeSizeDelta, gain_.damageDelta, gain_.summonCountDelta);
}