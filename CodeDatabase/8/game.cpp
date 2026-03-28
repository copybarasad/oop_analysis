#include "game.h"
#include "input_reader.h"
#include "console_renderer.h" 
#include "game_controller.h"
#include "game_vizualizator.h"
#include "fileinput.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <algorithm>
using namespace std;

Game::Game(const std::string& configPath) 
    : field(nullptr), 
      player(nullptr), 
      enemy(nullptr), 
      tower(nullptr),
      currentLevel(1), 
      gameRunning(true),
      configFile(configPath) {
    srand(time(0));
}

Game::~Game() {
    cleanup();
}
void Game::cleanup() {
    delete field;
    delete player;
    delete enemy;
    delete tower;
    
    field = nullptr;
    player = nullptr;
    enemy = nullptr;
    tower = nullptr;
}
template<typename RendererType>
void Game::saveGame(GameVisualization<RendererType>& visualization) {
    try {
        saveSystem.saveGame(*this);
    } catch (const exception& e) {
        visualization.renderMessage("Ошибка сохранения: " + string(e.what()));
    }
}

template<typename RendererType>
void Game::initializeField(GameVisualization<RendererType>& visualization) {
    visualization.renderMessage("=== СОЗДАДИМ ПОЛЕ ===");
    visualization.showPrompt("Введите ширину и высоту поля: ");

    InputReader reader;
    int w = reader.readInteger();
    int h = reader.readInteger();
    
    field = new Field(w, h);
    
    visualization.showPrompt("Хотите заполнить поле препятствиями? YES/NO: ");
    string yesno = reader.readInputLine();
    
    if (yesno == "YES") {
        visualization.showPrompt("Хотите заполнить поле препятствиями? YES/NO: ");
        string truefalse = reader.readInputLine();
        
        if (truefalse == "TRUE") {
            delete field;
            field = new Field(w, h, true);
        } else {
            visualization.showPrompt("Введите процент заполненности препятствиями (без знака %): ");
            int perc = reader.readInteger();
            delete field;
            field = new Field(w, h, false, perc);
        }
    }
}

template<typename RendererType>
void Game::initializePlayer(GameVisualization<RendererType>& visualization) {
    visualization.showPrompt("Введите имя вашего игрока: ");
    InputReader reader;
    string name = reader.readInputLine();
    name.erase(remove(name.begin(), name.end(), ' '), name.end());
    
    if (name.empty()) name = "Player";
    player = new Player(name, *field, 0, 0);
    field->placePlayer(0, 0);
}

void Game::initializeEnemy() {
    if (enemy != nullptr) {
        delete enemy;
    }
    /*int randX, randY;
    int w = field->getwidth();
    int h = field->getlength();
    
    do { 
        randX = 5 + rand() % (w - 5);
        randY = 5 + rand() % (h - 5);
    } while (!field->getNode(randX, randY).isPassable());*/
    int randX = 3, randY = 3;
    enemy = new Enemy("Goblin", *field, randX, randY);

    if (currentLevel > 1) {
        int bonusHP = (currentLevel - 1) * 4;
        int bonusDamage = currentLevel - 1;
        
        enemy->addLives(bonusHP);
        enemy->setDamage(1 + bonusDamage);
        
    }
    field->placeEnemy(randX, randY);
}

void Game::initializeTower() {
    int randX, randY;
    int w = field->getwidth();
    int h = field->getlength();
    
    do { 
        randX = 7 + rand() % (w - 7);
        randY = 7 + rand() % (h - 7);
    } while (!field->getNode(randX, randY).isPassable());
    
    tower = new EnemyTower("Сторожевая башня", randX, randY);
    
    field->getNode(randX, randY).setTower();
}

template<typename InputProvider, typename RendererType>
void Game::RestartOrExit(GameVisualization<RendererType>& visualization) {
    visualization.showPrompt("Хотите начать заново? (YES/NO): ");
    InputReader reader;
    string choice = reader.readInputLine();
    if (choice == "YES") {
        restartGame<InputProvider, RendererType>(visualization);
    } else {
        gameRunning = false;
        visualization.renderMessage("Спасибо за игру! До свидания!");
    }
}

template<typename InputProvider, typename RendererType>
void Game::restartGame(GameVisualization<RendererType>& visualization) {
    visualization.renderMessage("=== ПЕРЕЗАПУСК ИГРЫ ===");
    cleanup();
    currentLevel = 1;
    gameRunning = true;
    startGame<InputProvider, RendererType>(visualization);
}
bool Game::loadGame() {
        return saveSystem.loadGame(*this);
    }

void Game::prepareNextLevel() {
    currentLevel++;
    int currentHP = player->getLives();
    if (currentHP < 10) {
        player->addLives(10 - currentHP);
    }
    
    int spellCount = player->getHand().getSpellCount();
    if (spellCount > 0) {
        player->getHand().removeHalfSpells();
    }
    
    int oldWidth = field->getwidth();
    int oldHeight = field->getlength();
    int newWidth = min(25, oldWidth + 2);
    int newHeight = min(25, oldHeight + 2);
    int obstaclePercent = 10 + (currentLevel - 1) * 5;
    
    field->resizeWithObstacles(newWidth, newHeight, obstaclePercent);
    
    player->setPosition(0, 0);
    field->getNode(0, 0).setPlayer();

    delete enemy;
    enemy = nullptr;
    initializeEnemy();
   
    delete tower;
    tower = nullptr;
    initializeTower();
}

template<typename InputProvider, typename RendererType>
void Game::runGameLoop(GameVisualization<RendererType>& visualization) {
    while (currentLevel <= MAX_LEVELS && gameRunning) {
        bool levelPassed = playLevel<InputProvider, RendererType>(visualization);
        
        if (!levelPassed) {
            visualization.renderMessage("Игра окончена на уровне " + to_string(currentLevel));
            RestartOrExit<InputProvider, RendererType>(visualization);
            return;
        }
        
        if (currentLevel < MAX_LEVELS) {
            prepareNextLevel();
            visualization.renderMessage("\n=== УРОВЕНЬ " + to_string(currentLevel) + " ===");
            visualization.renderMessage("Создано поле: " + to_string(field->getwidth()) + " * " + to_string(field->getlength()));
            visualization.renderMessage("Создан враг: " + enemy->getname() + " (жизни: " + to_string(enemy->getLives()) + ", урон: " + to_string(enemy->getDamage()) + ")");
            visualization.renderMessage("Башня установлена в (" + to_string(tower->getX()) + ", " + to_string(tower->getY()) + ")");
            visualization.renderMessage("Игрок в: " + to_string(player->getX()) + "," + to_string(player->getY()));
            visualization.renderMessage("Враг в: " + to_string(enemy->getX()) + "," + to_string(enemy->getY()));
            visualization.renderMessage("Жизни игрока: " + to_string(player->getLives()));
            string handOutput;
            player->getHand().showHand(handOutput);
        } else {
            break;
        }
    }
    
    if (gameRunning) {
        visualization.renderMessage("=== ВЫ ВЫИГРАЛИ ИГРУ! ===");
        visualization.renderMessage("Поздравляем! Вы прошли все " + to_string(MAX_LEVELS) + " уровня!");
        RestartOrExit<InputProvider, RendererType>(visualization);
    }
}

template<typename InputProvider, typename RendererType>
void Game::startGame(GameVisualization<RendererType>& visualization) {
    visualization.renderMessage("=== ДОБРО ПОЖАЛОВАТЬ В ИГРУ! ===");

    if (saveSystem.saveExists()) {
        visualization.showPrompt("Обнаружено сохранение. Хотите загрузить? (YES/NO): ");
        InputReader reader;
        string choice = reader.readInputLine();
        
        if (choice == "YES") {
            field = new Field(10, 10);
            player = new Player("Temp", *field, 0, 0);
            enemy = new Enemy("TempEnemy", *field, 1, 1);
            tower = new EnemyTower("TempTower", 5, 5);
            if (saveSystem.loadGame(*this)) {
                visualization.renderMessage("Загрузка завершена! Начинаем игру...");
                runGameLoop<InputProvider, RendererType>(visualization);
                return;
            } else {
                visualization.renderMessage("Не удалось загрузить игру. Начинаем новую...");
            }
        }
    }
    
    try {
        initializeField(visualization);
        initializePlayer(visualization);
        initializeEnemy();
        initializeTower();
        visualization.renderMessage("\n=== УРОВЕНЬ " + to_string(currentLevel) + " ===");
        visualization.renderMessage("Создано поле: " + to_string(field->getwidth()) + " * " + to_string(field->getlength()));
        visualization.renderMessage("Создан враг: " + enemy->getname() + " (жизни: " + to_string(enemy->getLives()) + ", урон: " + to_string(enemy->getDamage()) + ")");
        visualization.renderMessage("Башня установлена в (" + to_string(tower->getX()) + ", " + to_string(tower->getY()) + ")");
        visualization.renderMessage("Игрок в: " + to_string(player->getX()) + "," + to_string(player->getY()));
        visualization.renderMessage("Враг в: " + to_string(enemy->getX()) + "," + to_string(enemy->getY()));
        visualization.renderMessage("Жизни игрока: " + to_string(player->getLives()));
        string handOutput;
        player->getHand().showHand(handOutput);
        currentLevel = 1;
        runGameLoop<InputProvider, RendererType>(visualization);
        
    } catch (const exception& e) {
        cout << "Ошибка: " << e.what() << endl;
        gameRunning = false;
    }
}

template<typename InputProvider, typename RendererType>
bool Game::playLevel(GameVisualization<RendererType>& visualization) {

    CommandMapper commandMapper;
    commandMapper.initialize(configFile);
    setCommandMapper(&commandMapper);
    displayMenu(commandMapper);

    InputProvider inputProvider(cin, commandMapper);
    GameController<InputProvider, RendererType> gameController(inputProvider);
    
    string lastAction = "";
    while (player->isAlive() && enemy->isAlive() && gameRunning) {

        visualization.renderGame();

        bool success = false;
        string towerMessage;
        string enemyMessage;
        bool canPlayerMove = false;
        vector<pair<int, int>> directions = {{0,-1}, {0,1}, {-1,0}, {1,0}};
        for (pair<int, int> dir : directions) {
            int testX = player->getX() + dir.first;
            int testY = player->getY() + dir.second;
            if (field->validpos(testX, testY) && 
                field->getNode(testX, testY).isPassable()) {
                canPlayerMove = true;
                break;
            }
        }        
        if (!canPlayerMove) {
            visualization.renderMessage("Игрок заблокирован со всех сторон! Игра окончена.");
            gameRunning = false;
            break;
        }

        success = gameController.processInputUntilSuccess(*this, visualization); // ввод команд через шаблонный класс
        lastAction = gameController.getLast();

        if (enemy->isAlive() && gameRunning) {
            if (!lastAction.empty() && lastAction != "SAVE" && lastAction != "SHOW_STATUS" && lastAction != "SHOW_MENU" && lastAction != "BUY_SPELL" && lastAction != "LOAD" && lastAction != "SHOW_SPELLS") {
                enemy->moveTo(0, 0, *player, enemyMessage);
            }
        }
        if (!enemyMessage.empty()) {
            visualization.renderMessage(enemyMessage);
        }

        if (gameRunning) {
            if (!lastAction.empty() && lastAction != "SAVE" && lastAction != "SHOW_STATUS" && lastAction != "SHOW_MENU" && lastAction != "BUY_SPELL" && lastAction != "LOAD" && lastAction != "SHOW_SPELLS") {
                tower->update(*player, towerMessage);
            }
        }
        if (!towerMessage.empty()) {
            visualization.renderMessage(towerMessage);
        }
    }
    
    if (!player->isAlive()) {
        visualization.renderMessage("=== ПРОИГРЫШ НА УРОВНЕ " + to_string(currentLevel) + " ===");
        visualization.renderMessage("Ваш герой погиб.");
        return false;
    }
    if (!enemy->isAlive()) {
        visualization.renderMessage("=== ПОБЕДА НА УРОВНЕ " + to_string(currentLevel) + " ===");
        visualization.renderMessage("Вы победили врага!");
        player->addPoints(10);
        visualization.renderMessage("Очки: " + to_string(player->getPoints()));
        return true;
    }
    return false;
}

















// Явное инстанцирование для ConsoleRenderer
template void Game::startGame<InputReader, ConsoleRenderer>(GameVisualization<ConsoleRenderer>&);
template void Game::startGame<DemoInput, ConsoleRenderer>(GameVisualization<ConsoleRenderer>&);
template bool Game::playLevel<InputReader, ConsoleRenderer>(GameVisualization<ConsoleRenderer>&);
template bool Game::playLevel<DemoInput, ConsoleRenderer>(GameVisualization<ConsoleRenderer>&);
template void Game::runGameLoop<InputReader, ConsoleRenderer>(GameVisualization<ConsoleRenderer>&);
template void Game::runGameLoop<DemoInput, ConsoleRenderer>(GameVisualization<ConsoleRenderer>&);
template void Game::initializeField<ConsoleRenderer>(GameVisualization<ConsoleRenderer>&);
template void Game::initializePlayer<ConsoleRenderer>(GameVisualization<ConsoleRenderer>&);
template void Game::RestartOrExit<InputReader, ConsoleRenderer>(GameVisualization<ConsoleRenderer>&);
template void Game::RestartOrExit<DemoInput, ConsoleRenderer>(GameVisualization<ConsoleRenderer>&);
template void Game::saveGame<ConsoleRenderer>(GameVisualization<ConsoleRenderer>&);
template void Game::restartGame<InputReader, ConsoleRenderer>(GameVisualization<ConsoleRenderer>&);
template void Game::restartGame<DemoInput, ConsoleRenderer>(GameVisualization<ConsoleRenderer>&);

// Явное инстанцирование для ColorfulRenderer  
template void Game::startGame<InputReader, ColorfulRenderer>(GameVisualization<ColorfulRenderer>&);
template void Game::startGame<DemoInput, ColorfulRenderer>(GameVisualization<ColorfulRenderer>&);
template bool Game::playLevel<InputReader, ColorfulRenderer>(GameVisualization<ColorfulRenderer>&);
template bool Game::playLevel<DemoInput, ColorfulRenderer>(GameVisualization<ColorfulRenderer>&);
template void Game::runGameLoop<InputReader, ColorfulRenderer>(GameVisualization<ColorfulRenderer>&);
template void Game::runGameLoop<DemoInput, ColorfulRenderer>(GameVisualization<ColorfulRenderer>&);
template void Game::initializeField<ColorfulRenderer>(GameVisualization<ColorfulRenderer>&);
template void Game::initializePlayer<ColorfulRenderer>(GameVisualization<ColorfulRenderer>&);
template void Game::RestartOrExit<InputReader, ColorfulRenderer>(GameVisualization<ColorfulRenderer>&);
template void Game::RestartOrExit<DemoInput, ColorfulRenderer>(GameVisualization<ColorfulRenderer>&);
template void Game::saveGame<ColorfulRenderer>(GameVisualization<ColorfulRenderer>&);
template void Game::restartGame<InputReader, ColorfulRenderer>(GameVisualization<ColorfulRenderer>&);
template void Game::restartGame<DemoInput, ColorfulRenderer>(GameVisualization<ColorfulRenderer>&);
