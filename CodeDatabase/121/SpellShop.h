#ifndef SPELL_SHOP_H
#define SPELL_SHOP_H

#include "Player.h"
#include "TextSystem.h"
#include "SpellFactory.h"
#include "GameConfig.h"
#include <memory>
#include <vector>
#include <utility>


// Магазин заклинаний
// Позволяет игроку покупать заклинания за очки.
// Использует конфигурацию для определения цен заклинаний.


class SpellShop {
private:
    std::shared_ptr<TextSystem> textSystem;
    const GameConfig& config;  
    
    // Вектор пар: тип заклинания и его стоимость
    std::vector<std::pair<SpellType, int>> availableSpells;

public:
    SpellShop(std::shared_ptr<TextSystem> textSystem, const GameConfig& config);
    
    // Показывает интерфейс магазина игроку
    void showShop(const Player& player);
    
    // Покупает выбранное заклинание для игрока
    bool purchaseSpell(Player& player, int spellIndex);

private:
    // Инициализирует список доступных заклинаний и их цены
    void initializeSpells();
};

#endif