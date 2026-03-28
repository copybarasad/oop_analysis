#include "SpellCasting.h"
#include "Player.h"
#include "Field.h"
#include "Spell.h"
#include "GameVisualize.h"
#include "DirectDamageSpell.h"
#include "AreaDamageSpell.h"
#include <random>
#include <chrono>

// Метод для применения заклинания
bool SpellCasting::castSpell(Player& player, int spellIndex, GameField& field, int targetX, int targetY) {
    static ConsoleGameVisualizer visualizer;  // Статический визуализатор
    
    if (!player.isAlive()) {
        return false;
    }
    
    Spell* spell = player.getSpellHand().getSpell(spellIndex);
    if (spell == nullptr) {
        return false;
    }
    
    // ПЕРВОЕ: проверяем может ли игрок кастовать это заклинание
    if (!player.canCastSpell(*spell)) {
        visualizer.visualizeOperationResult("spell", "cannot_cast", "not enough mana or other restriction");
        return false;
    }
    
    // ВТОРОЕ: применяем заклинание (оно само проверит ману внутри своего cast метода)
    bool success = spell->cast(field, player, targetX, targetY);
    
    // ТРЕТЬЕ: если заклинание успешно, тратим ману и движение
    if (success) {
        // Мана уже должна быть потрачена внутри spell->cast()
        player.setMovement(false);
        visualizer.visualizeSpellCastSuccess(player.getMana()); 
    } else {
        visualizer.visualizeOperationResult("spell", "failed", "");
    }
    
    return success;
}

// Остальные методы без изменений
std::unique_ptr<Spell> SpellCasting::getRandomStarterSpell() {
    // Создаем генератор случайных чисел
    auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    std::mt19937 gen(seed);
    std::uniform_int_distribution<> dis(0, 1);
    
    int spellType = dis(gen);
    
    switch (spellType) {
        case 0:
            return std::make_unique<DirectDamageSpell>();
        case 1:
            return std::make_unique<AreaDamageSpell>();
        default:
            return std::make_unique<DirectDamageSpell>(); 
    }
}

// Метод для проверки, есть ли у игрока заклинания
bool SpellCasting::hasSpells(const Player& player) {
    return player.getSpellHand().getSpellCount() > 0;
}

// Метод для отображения информации о мане и заклинаниях
void SpellCasting::displaySpellInfo(const Player& player) {
    static ConsoleGameVisualizer visualizer;  // Статический визуализатор
    visualizer.visualizeSpellInfo(player);
}

// Метод для покупки заклинания
bool SpellCasting::buySpell(Player& player, std::unique_ptr<Spell> spell, int cost) {
    static ConsoleGameVisualizer visualizer;  // Статический визуализатор

    if (spell == nullptr) {
        return false;
    }
    
    // Проверяем, хватает ли очков
    if (player.getScore() < cost) {
        visualizer.visualizeInsufficientResource("score", cost, player.getScore());
        return false;
    }
    
    // Пытаемся добавить заклинание в руку
    if (player.getSpellHand().addSpell(std::move(spell))) {
        // Вычитаем стоимость из очков
        player.addScore(-cost);
        visualizer.visualizeOperationResult("purchase", "success", "");
        return true;
    } else {
        visualizer.visualizeOperationResult("purchase", "hand_full", "");
        return false;
    }
}