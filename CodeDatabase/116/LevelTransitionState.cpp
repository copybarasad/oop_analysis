#include "LevelTransitionState.h"
#include "GameContext.h"
#include "PlayingState.h"
#include "../world/GameWorld.h"
#include "../spells/SpellManager.h"
#include <iostream>
#include <random> 

LevelTransitionState::LevelTransitionState(){
    upgradeOptions = {
        "Увеличить максимальное здоровье (+20 HP)",
        "Увеличить урон (+5 урона)",
        "Усилить все заклинания",
        "Получить случайное заклинание"
    };
}

void LevelTransitionState::handleInput(GameContext& context) {
    char input = context.getInputHandler().getCommand("Выберите номер улучшения: ");
    if(input >= '1' && input <= '4'){
        applyUpgradeAndContinue(context, input - '0');
    }
    else{
        std::cout << "Неизвестная команда!" << std::endl;
    }
}

void LevelTransitionState::render(GameContext& context) {
    system("clear");
    std::cout << "=== УРОВЕНЬ " << context.getLevelManager().getCurrentLevelNumber() << " ПРОЙДЕН! ===" << std::endl;
    std::cout << "Переход на уровень " << context.getLevelManager().getCurrentLevelNumber() << std::endl;
    std::cout << std::endl;
    std::cout << "Выберите улучшение:" << std::endl;
    std::cout << std::endl;
    
    for (int i = 0; i < upgradeOptions.size(); ++i) {
        std::cout << i+1 << ". " << upgradeOptions[i] << std::endl;
    }
    std::cout << std::endl;
}

void LevelTransitionState::applyUpgradeAndContinue(GameContext& context, int selectedUpgrade) {
    auto& player = context.getController().getWorld().getPlayer();
    SpellManager spellManager;
    removeHalfSpells(context);
    switch (selectedUpgrade) {
    case 1: {
        int newMaxHealth = player.getMaxHealth() + 20;
        player.setMaxHealth(newMaxHealth);
        std::cout << "Максимальное здоровье увеличено до " << newMaxHealth << " HP" << std::endl;
        break;
    }
    case 2: {
        auto damages = player.getPlayerDamages();
        player.setMeleeDamage(damages.first + 5);
        player.setRangedDamage(damages.second + 5);
        std::cout << "Урон увеличен: ближний " << (damages.first + 5) 
                  << ", дальний " << (damages.second + 5) << std::endl;
        break;
    }  
    case 3: {
        spellManager.upgradeAllSpells(player);
        break;
    }
    case 4: {
        spellManager.addRandomSpell(player);
        break;
    }
    default:
        std::cout << "Неизвестное улучшение!" << std::endl;
        break;
    }
    
    player.setHealth(player.getMaxHealth());
    player.setCoins(player.getCoins()/4);
    context.getInputHandler().waitForEnter("Нажмите Enter для продолжения...");
    
    context.initializeNewLevel();
    context.transitionTo("Playing");
}

void LevelTransitionState::removeHalfSpells(GameContext& context) {
    auto& world = context.getController().getWorld();
    auto& player = world.getPlayer();
    auto& hand = player.getPlayerHand();
    int currentSize = hand.getCurrentSize();
    
    if (currentSize > 1) {
        int spellsToRemove = currentSize / 2;
        std::random_device rd;
        std::mt19937 gen(rd());
        
        std::vector<int> nonEmptySlots;
        for (int i = 0; i < 5; ++i) {
            if (hand.getSpellNum(i) > 0) {
                nonEmptySlots.push_back(i);
            }
        }
        
        for (int i = 0; i < spellsToRemove && !nonEmptySlots.empty(); ++i) {
            std::uniform_int_distribution<> dis(0, nonEmptySlots.size() - 1);
            int slotIndex = dis(gen);
            int slotToRemove = nonEmptySlots[slotIndex];

            hand.removeSpell(slotToRemove);

            if (hand.getSpellNum(slotToRemove) == 0) {
                nonEmptySlots.erase(nonEmptySlots.begin() + slotIndex);
            }
        }
        std::cout << "Удалено " << spellsToRemove << " заклинаний." << std::endl;
    }
}