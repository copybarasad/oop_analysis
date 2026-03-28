#include <EmpowerSpell.hpp>
#include <Constants.hpp>

EmpowerSpell::EmpowerSpell() : name("Empower"), cost(EMPOWER_COST), type(SpellType::Empower) {}

std::string EmpowerSpell::getName() const { return name; }
int EmpowerSpell::getCost() const { return cost; }
SpellType EmpowerSpell::getType() const { return type; }

bool EmpowerSpell::apply(std::shared_ptr<Player> player, std::vector<std::vector<Ceil>>&, std::vector<std::shared_ptr<LivingEntity>>&, const ScreenSize*) {
    if (player->getMp() >= cost) {
        player->useSkill(cost);
        player->addEmpowermentCharge();
        return true;
    }
    return false;
}