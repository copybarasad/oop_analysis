#include "GameOverState.h"
#include "GameContext.h"
#include "MainMenuState.h"
#include "PlayingState.h"
#include <iostream>

GameOverState::GameOverState(bool won) 
    : playerWon(won) {
    menuOptions = {
        "Начать заново",
        "Главное меню", 
        "Выйти из игры"
    };
}

void GameOverState::handleInput(GameContext& context) {
    auto& inpHandler = context.getInputHandler();
    char input = inpHandler.getCommand();
    
    switch (input) {
    case '1':
        restartGame(context);
        break;
    case '2':
        context.transitionTo("MainMenu");
        break;
    case '3':
        context.requestQuit();
        break;
    default:
        std::cout << "Неизвестная команда!" << std::endl;
        context.getInputHandler().waitForEnter("Нажмите Enter для продолжения...");
        break;
    }
}

void GameOverState::render(GameContext& context) {
    system("clear");
    
    if (playerWon) {
        std::cout << "=== ПОБЕДА! ===" << std::endl;
        std::cout << "Вы успешно прошли все уровни!" << std::endl;
    } else {
        std::cout << "=== ПОРАЖЕНИЕ ===" << std::endl;
        std::cout << "Ваше приключение завершилось..." << std::endl;
    }
    
    std::cout << std::endl;

    auto& player = context.getController().getWorld().getPlayer();
    std::cout << "=== ФИНАЛЬНАЯ СТАТИСТИКА ===" << std::endl;
    std::cout << "Уровень: " << context.getLevelManager().getCurrentLevelNumber() << std::endl;
    std::cout << "Здоровье: " << player.getHealth() << " HP" << std::endl;
    std::cout << "Очки: " << player.getScore() << std::endl;
    std::cout << "Монеты: " << player.getCoins() << std::endl;
    
    std::cout << std::endl;

    std::cout << "=== ДОСТИЖЕНИЯ ===" << std::endl;
    std::cout << "- Пройдено уровней: " << (context.getLevelManager().getCurrentLevelNumber() - 1) << std::endl;
    std::cout << "- Набрано очков: " << player.getScore() << std::endl;
    std::cout << "- Собрано монет: " << player.getCoins() << std::endl;

    if (playerWon) {
        std::cout << "-Игра пройдена!" << std::endl;
    }
    std::cout << std::endl;
    std::cout << "Выберите действие:" << std::endl;
    
    for (int i = 0; i < menuOptions.size(); ++i) {
        std::cout << i + 1 << ". " << menuOptions[i] << std::endl;
    }
    
    std::cout << std::endl;
}

void GameOverState::restartGame(GameContext& context) {
    context.getLevelManager() = LevelManager();
    context.initializeNewLevel();
    context.transitionTo("Playing");
}