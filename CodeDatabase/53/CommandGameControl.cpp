#include "CommandGameControl.h"
#include "Game.h"
#include "GameController.h"
#include <iostream>

void PlayeMoVeCommand::execute() {
    if (player && field) {
        player->move(direction, *field);
    }
}

void PlayeMoVeCommand::parse(const std::string& input) {
    name = "PlayeMoVeCommand";
}

void PlayerAttackCommand::execute() {
    if (player && field) {
        player->attack(attackDirection, *field);
    }
}

void PlayerAttackCommand::parse(const std::string& input) {
    name = "PlayerAttackCommand";
}


void SwitchAttackTypeCommand::execute() {
    if (player) {
        player->switchAttackType();
    }
}

void SwitchAttackTypeCommand::parse(const std::string& input) {
    name = "SwitchAttackTypeCommand";
}

void UseSpellCommand::execute() {
    if (!player || !field) return;
    
    if (player->getHandSpell()) {
        
        bool success = player->getHandSpell()->useSpell(spellChoice, x, y, *field, *player);
        
        if (!success) {
            std::cout << "Не удалось использовать заклинание!" << std::endl;
        }
    }
}

void UseSpellCommand::parse(const std::string& input) {
    name = "UseSpellCommand";
}

void BuySpellCommand::execute() {
    if (!player) return;
    
    int cost = 0;
    switch(spellID) {
        case BULLETARAIN: cost = COST_BULLETRAIN * count; break;
        case MEGASHOTGUN: cost = COST_MEGASHOTGUN * count; break;
        case CHEAPWHISKEY: cost = COST_CHEAPWHISKEY * count; break;
        case GHOSTOFJHONNY: cost = COST_JOHNYSGHOST * count; break;
        case UPDATESPELL: cost = COST_UPDATE_SPELL * count; break;
        default: 
            std::cout << "Неверный тип заклинания!" << std::endl;
            return;
    }
    
    if(player->addSpell(spellID, count)) {
        player->setCoins(player->getCoins() - cost);
        std::cout << "Вы успешно купили " << count << " заклинаний!" << std::endl;
    } else {
        std::cout << "Не удалось купить заклинание!" << std::endl;
    }
}

void BuySpellCommand::parse(const std::string& input) {
    name = "BuySpellCommand";
}

void SaveGameCommand::execute() {
    try {
        game.getSaveManagerRef().saveGame(saveName, game.getPlayerRef(), 
                                        game.getFieldRef(), game.getTurnRef(), 
                                        game.getCurrentLevelRef());
        std::cout << "Игра успешно сохранена!" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Ошибка сохранения: " << e.what() << std::endl;
        std::cerr << "Попробуйте использовать другое имя сохранения." << std::endl;
    }
}

void SaveGameCommand::parse(const std::string& input) {
    name = "SaveGameCommand";
}

void LoadGameCommand::execute() {
    game.getGameControl().showLoadMenu();
}

void LoadGameCommand::parse(const std::string& input) {
    name = "LoadGameCommand";
}

void QuitCommand::execute() {
    game.getGameControl().showMainMenu();
}

void QuitCommand::parse(const std::string& input) {
    name = "QuitCommand";
}