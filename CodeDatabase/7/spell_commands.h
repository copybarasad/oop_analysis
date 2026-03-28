#pragma once
#include "command.h"
#include "Game.h"
#include "Player.h"
#include "Hand.h"
#include <string>
#include <iostream>
#include <limits>

class CastSpellCommand : public Command {
public:
    bool execute(Game& game, std::string& resultMessage) override {
        // Сначала покажем руку, чтобы игрок знал, что выбирать
        // Здесь приходится лезть в игрока, но только для чтения (const), это разрешено
        game.getPlayer()->getHand().printHand();
        
        std::cout << "Введите номер заклинания: ";
        int spellIdx;
        std::cin >> spellIdx;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        
        std::cout << "Введите координаты цели (X Y): ";
        int x, y;
        std::cin >> x >> y;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        
        // Само действие выполняет Game
        resultMessage = game.useSpell(spellIdx, x, y);
        return true;
    }
    
    std::string getType() const override { return "CAST_SPELL"; }
};

class BuySpellCommand : public Command {
public:
    bool execute(Game& game, std::string& resultMessage) override {
        resultMessage = game.buySpell();
        return true;
    }
    
    std::string getType() const override { return "BUY_SPELL"; }
};