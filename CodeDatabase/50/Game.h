#pragma once
#include <iostream>
#include <limits>
#include <cstdlib>
#include <algorithm>    
#include <random> 
#include <fstream>

#include "GameField.h"
#include "Player.h"
#include "BattleSystem.h"
#include "PlayerHand.h"
#include "SaveManager.h"
#include "EntityManager.h"
#include "GameExceptions.h"
#include "Command.h"


#include "LevelController.h"


class Game{
private:

    int turns;
    int maxEnemCnt;
    int maxTowerCnt;
    int currentLevel;


    Field field;
    Player player; 
    PlayerHand hand; 
    BattleSystem battle;
    EntityManager entManager;


public:
    Game();


     // Геттеры
    int getCurrentLevel() const { return currentLevel; }
    int getTurns() const { return turns; }

    Player& getPlayer() { return player; }
    const Player& getPlayer() const { return player; }

    Field& getField() { return field; }
    const Field& getField() const { return field; }

    EntityManager& getEntityManager() { return entManager; }
    const EntityManager& getEntityManager() const { return entManager; }

    PlayerHand& getHand() { return hand; }
    const PlayerHand& getHand() const { return hand; }

    // Сеттеры
    void setCurrentLevel(int lvl) { currentLevel = lvl; }
    void setTurns(int t) { turns = t; }


    //---

    template<typename InputHandler, typename Renderer>
    bool runMenuCycle(InputHandler& input, Renderer& renderer);

    template<typename InputHandler, typename Renderer>
    void runLevelCycle(InputHandler& input, Renderer& renderer);


    void resetState();
        
};

template<typename InputHandler, typename Renderer>
void Game::runLevelCycle(InputHandler& input,Renderer& renderer)
{
    bool exitToMenu = false;

    while (!exitToMenu) {
        bool levelCompleted = false;

        while (player.isAlive() && !levelCompleted) {
            

            renderer.renderGame(*this);
            std::cout << "\n----- ВЫБОР ОПЦИИ -----\n";
            input.printGameBindings();
            std::cout << "Ваш выбор: ";

            Command cmd = input.readCommand();
            int dx = 0, dy = 0;

            switch (cmd.type) {

                case CommandType::MoveUp:    dy = -1; break;
                case CommandType::MoveDown:  dy =  1; break;
                case CommandType::MoveLeft:  dx = -1; break;
                case CommandType::MoveRight: dx =  1; break;

                case CommandType::Attack:
                    battle.playerAttack(field, player, hand);
                    break;

                case CommandType::CastSpell:
                    battle.castSpell(field, player, hand);
                    break;

                case CommandType::SwitchAttackType:
                    player.switchAttackType();
                    break;

                case CommandType::SaveGame:
                    try {
                        SaveManager::saveGame(*this, "savegame.txt");
                        std::cout << "Игра сохранена.\n";
                    } catch (const GameException& e) {
                        std::cout << "Ошибка сохранения: " << e.what() << "\n";
                    }
                    continue;
                    break;

                case CommandType::LoadGame:
                    try {
                        SaveManager::loadGame(*this, "savegame.txt");
                        std::cout << "Игра загружена.\n";
                    } catch (const GameException& e) {
                        std::cout << "Ошибка загрузки: " << e.what() << "\n";
                    }
                    continue;
                    break;

                case CommandType::ExitToMenu:
                    exitToMenu = true;
                    return;

                default:
                    continue;
                    break;
            }

            // перемещение игрока
            int oldX = player.getX();
            int oldY = player.getY();
            int newX = oldX + dx;
            int newY = oldY + dy;

            if (field.canMoveTo(newX, newY)) {
                field.setCellEntity(&player, newX, newY);
                field.clearCellEnt(oldX, oldY);
                player.setPosition(newX, newY);
            }

            // движение врагов
            if (entManager.moveEnemies(field, player)) {
                std::cout << "Игрок умер!\n";
                return;
            }

            // атака башен
            entManager.updateTowers(field, player);
            if (!player.isAlive()) {
                std::cout << "Игрок погиб от башни!\n";
                return;
            }

            // ловушки
            battle.updateTraps(field, hand);

            // победа
            if (entManager.countAlive() == 0) {
                std::cout << "Уровень пройден!\n";
                levelCompleted = true;
            }

            turns++;
        }

        if (levelCompleted) {
            currentLevel++;
            LevelController::levelUp(*this);
            LevelController::initLevel(*this);
        }
    }
}


template<typename InputHandler, typename Renderer>
bool Game::runMenuCycle(InputHandler& input, Renderer& renderer)
{
    int menu_flag=1;
    renderer.renderMenu();
    while (menu_flag) {
        
        input.printMenuBindings();
        std::cout << "Ваш выбор: ";

        Command cmd = input.readCommand();

        switch (cmd.type) {

            case CommandType::NewGame:
                resetState();
                LevelController::initLevel(*this);
                return true;

            case CommandType::LoadGame:
                try {
                    LevelController::initLevel(*this);
                    SaveManager::loadGame(*this, "savegame.txt");
                    std::cout << "Игра загружена.\n";
                    return true;
                } catch (const GameException& ex) {
                    std::cout << "Ошибка загрузки: " << ex.what() << "\n";
                }
                break;

            case CommandType::QuitGame:
                menu_flag = 0;
                return false;

            default:
                std::cout << "Некорректная команда в меню!\n";
                break;
        }
    }
    return false;
}

