#include "PauseState.h"
#include "GameContext.h"
#include "PlayingState.h"
#include "MainMenuState.h"
#include "SaveLoadManager.h"
#include <iostream>

PauseState::PauseState() 
    : selectedOption(0) {
    menuOptions = {
        "Продолжить игру",
        "Сохранить игру",
        "Загрузить игру", 
        "Выйти в главное меню",
        "Выйти из игры"
    };
}

void PauseState::handleInput(GameContext& context){
    auto& inpHandler = context.getInputHandler();
    char comand = inpHandler.getCommand();
    switch (comand)
    {
    case '1':
        context.transitionTo("Playing");
        break;
    case '2':
        saveGame(context);
        break;  
    case '3':
        loadGame(context);
        break;
    case '4':
        context.transitionTo("MainMenu");
        break;
    case '5':
        context.requestQuit();
        break;
    default:
        std::cout << "Неизвестная команда!" << std::endl;
    }
}

void PauseState::render(GameContext& context){
    system("clear");
    auto& player = context.getController().getWorld().getPlayer();
    std::cout << "===Пауза==" << std::endl;
    std::cout << "\n=== СТАТИСТИКА ИГРОКА ===" << std::endl;
    std::cout << "Здоровье: " << player.getHealth() << std::endl;
    std::cout << "Очки: " << player.getScore() << std::endl;
    std::cout << "Монеты: " << player.getCoins() << std::endl;
    std::cout << "Текущий уровень: " << context.getLevelManager().getCurrentLevelNumber() 
              << " из " << context.getLevelManager().getTotalLevels() << std::endl;
    std::cout << std::endl;
    for(int i = 0; i < menuOptions.size(); i++){
        std::cout << i+1 << ". " << menuOptions[i] << std::endl;
    }
}

void PauseState::saveGame(GameContext& context) {
    SaveLoadManager saveManager;
    
    if (saveManager.saveGame(context)) {
        std::cout << "Игра успешно сохранена!" << std::endl;
    } else {
        std::cout << "Не удалось сохранить игру." << std::endl;
    }
    
    context.getInputHandler().waitForEnter("Нажмите Enter для продолжения...");
}

void PauseState::loadGame(GameContext& context) {
    auto& saveManager = context.getSaveLoadManager();
    
    if (saveManager.loadGame(context)) {
        context.transitionTo("Playing");
        return;
    } else {
        std::cout << "Не удалось загрузить игру." << std::endl;
    }
    
    context.getInputHandler().waitForEnter("Нажмите Enter для продолжения...");
}