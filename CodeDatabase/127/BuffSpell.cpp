#include "BuffSpell.h"
#include "Game.h"
#include "Player.h"
#include <iostream>
#include <string>

BuffSpell::BuffSpell(const SpellEnhancement& enh, int price)
    : enhancement_(enh), price_(price) {
}

int BuffSpell::cost() const {
    return price_;
}

bool BuffSpell::apply(Game&, Player& player, int, int, const SpellEnhancement&, std::string* errorMessage) {
    player.addPendingEnhancement(enhancement_);
    // Сообщение о применении баффа выводится через рендерер или логирование
    // Здесь просто применяем бафф без вывода
    return true;
}

std::string BuffSpell::getDescription() const {
    std::string s;
    if (enhancement_.extraDamage > 0) s += "+Урон:" + std::to_string(enhancement_.extraDamage) + " ";
    if (enhancement_.extraRange > 0) s += "+Радиус:" + std::to_string(enhancement_.extraRange) + " ";
    if (enhancement_.extraArea > 0) s += "+Область:" + std::to_string(enhancement_.extraArea) + " ";
    if (enhancement_.extraSummons > 0) s += "+Призыв:" + std::to_string(enhancement_.extraSummons) + " ";
    if (!s.empty() && s.back() == ' ') s.pop_back();
    if (s.empty()) s = "<пустой бафф>";
    return std::string("Накладывает усиление: ") + s;
}