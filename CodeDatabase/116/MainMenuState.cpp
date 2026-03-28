#include "MainMenuState.h"
#include "GameContext.h"
#include "PlayingState.h"
#include "SaveLoadManager.h"
#include <iostream>

MainMenuState::MainMenuState(){
    options = {
        "Начать новую игру",
        "Загрузить игру", 
        "Выход"
    };
}

void MainMenuState::handleInput(GameContext& context) {
    auto& inpHandler = context.getInputHandler();
    std::cout << "Введите номер команды для продолжения: ";
    char input = inpHandler.getCommand("");
    
    switch (input) {
    case '1': {
        context.initializeNewLevel(true);
        context.transitionTo("Playing");
        break;
    }
    case '2': {
        auto& saveManager = context.getSaveLoadManager();
        
        if (!saveManager.saveExists()) {
            std::cout << "Сохранение не найдено!" << std::endl;
            context.getInputHandler().waitForEnter("Нажмите Enter для возврата в меню...");
            return;
        }
        
        if (saveManager.loadGame(context)) {
            std::cout << "Игра успешно загружена!" << std::endl;
            context.transitionTo("Playing");
        } else {
            std::cout << "Ошибка загрузки сохранения!" << std::endl;
            context.getInputHandler().waitForEnter("Нажмите Enter для возврата в меню...");
        }
        break;
    }
    case '3': {
        context.requestQuit();
        break;
    }
    default: {
        std::cout << "Неизвестная команда!" << std::endl;
        context.getInputHandler().waitForEnter("Нажмите Enter для продолжения...");
        break;
    }
    }
}

void MainMenuState::render(GameContext& context) {
    system("clear");
    std::cout << "=== ГЛАВНОЕ МЕНЮ ===" << std::endl;
    std::cout << std::endl;
    
    for(int i = 0; i < options.size(); i++) {
        std::cout << i + 1 << ". " << options[i] << std::endl;
    }
    
    std::cout << std::endl;

    SaveLoadManager saveManager;
    if (saveManager.saveExists()) {
        std::cout << "Доступно сохранение" << std::endl;
    } else {
        std::cout << "Сохранение не найдено" << std::endl;
    }
    
    std::cout << std::endl;
}