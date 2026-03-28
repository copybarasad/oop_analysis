#ifndef GAME_H
#define GAME_H

#include "GameSession.h"
#include "GameManager.h"
#include "GameVisualizer.h"
#include "CommandInputHandler.h"
#include "GameRenderer.h"
#include <memory>
#include <string>
#include <iostream>
#include <limits>
#include <cstdlib>

// Шаблонный класс Game
template<typename InputHandlerType = CommandInputHandler,
    typename RendererType = GameRenderer>
class Game {
private:
    std::unique_ptr<GameSession> m_session;
    GameManager<InputHandlerType> m_gameManager;
    GameVisualizer<RendererType> m_gameVisualizer;
    bool m_isRunning;

public:
    // Конструктор
    Game()
        : m_session(std::make_unique<GameSession>())
        , m_gameManager(*m_session)
        , m_gameVisualizer(*m_session)
        , m_isRunning(false) {
    }

    void run() {
        std::cout << "=== GAME LAUNCH ===" << std::endl;
        m_isRunning = true;

        showMainMenu();

        if (m_session->isRunning()) {
            gameLoop();
        }

        stop();
    }

    void stop() {
        m_isRunning = false;
    }

private:
    void showMainMenu() {
        int choice;

        do {
            std::cout << "\n=== MAIN MENU ===" << std::endl;
            std::cout << "1. New Game" << std::endl;
            std::cout << "2. Load Game" << std::endl;
            std::cout << "3. Exit" << std::endl;
            std::cout << "Choice: ";

            std::cin >> choice;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            switch (choice) {
            case 1:
                if (m_session->initializeNewGame()) {
                    m_gameVisualizer.renderGameStart();
                    return;
                }
                break;
            case 2:
                if (offerLoadGame()) {
                    return;
                }
                break;
            case 3:
                stop();
                return;
            default:
                std::cout << "Invalid choice." << std::endl;
                break;
            }
        } while (choice != 3);
    }

    void gameLoop() {
        while (m_isRunning && m_session->isRunning()) {
            // Очистка экрана
#ifdef _WIN32
            system("cls");
#else
            system("clear");
#endif

            // Отрисовка через GameVisualizer
            m_gameVisualizer.renderGameState();

            // Проверка состояния игры
            if (m_session->isGameOver()) {
                handleGameOver();
                break;
            }

            if (m_session->isVictory()) {
                handleVictory();
                if (m_session->isRunning()) {
                    continue;
                }
                else {
                    break;
                }
            }

            // Обработка ввода через GameManager
            m_gameManager.processNextCommand();

            // Обработка хода врагов
            m_session->processEnemyTurns();

            // Завершение хода
            m_session->processTurn();
        }
    }

    void handleGameOver() {
        m_gameVisualizer.renderGameOver();
        std::cout << "\n*** GAME OVER ***" << std::endl;
        std::cout << "Press Enter to continue..." << std::endl;
        std::cin.get();
    }

    void handleVictory() {
        m_gameVisualizer.renderVictory();
        std::cout << "\n*** LEVEL " << m_session->getGameState().level << " COMPLETED! ***" << std::endl;
        std::cout << "Do you want to proceed to next level? (y/n): ";

        char choice;
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (choice == 'y' || choice == 'Y') {
            m_session->advanceToNextLevel();
            std::cout << "Press Enter to continue..." << std::endl;
            std::cin.get();
        }
        else {
            m_session->stop();
        }
    }

    bool offerLoadGame() const {
        if (m_session->loadGame()) {
            std::cout << "Game loaded successfully!" << std::endl;
            return true;
        }
        else {
            std::cout << "Failed to load game. Starting new game instead." << std::endl;
            return false;
        }
    }
};

#endif // GAME_H