#include "Game.h"
#include "GameController.h"
#include "GameRenderer.h"    
#include "OutputTerminal.h"



Game::Game() 
    : field(DEFAULT_FIELD_WIDTH, DEFAULT_FIELD_HEIGHT), 
      player("Jhonny", DEFAULT_PLAYER_X, DEFAULT_PLAYER_Y), 
      saveManager("saves"),
      turn(DEFAULT_START_TURN), 
      currentLevel(1),
      gameRunning(true),
      inputHandler()
     {
    gameRendererPtr = std::make_unique<GameRenderer<OutputTerminal>>(this);
    gameControlPtr = std::make_unique<GameController<Input>>(this);
    field.setCellType(DEFAULT_PLAYER_X, DEFAULT_PLAYER_Y, PLAYER);
}

Game::~Game() {
    gameControlPtr.reset(); 
    gameRendererPtr.reset();
}

void Game::initializeLevel(int level) { 
    switch (level) {
        case 1:
            firstLevel();
            break;
        case 2:
            secondLevel();
            break;
        case 3:
            thirdLevel();
            break;
        default:
            firstLevel();
            break;
    }
}

void Game::firstLevel() {
    field = GameField(12, 12);
    field.spawn(20, PIT);           
    field.spawn(1, ENEMY);         
    field.spawn(2, COWBOYTOWER);   
    field.spawn(1, BARRACK);
    field.spawn(3, SALOON);
    cout << "\n=== Уровень 1: Ранчо 'Заброшенный Каньон' ===" << endl;
    cout << "Вы прибыли на заброшенное ранчо, где бандиты устроили свою базу. Аккуратенее вокруг много ям)" << endl;
    cout << "Очистите территорию от нежелательных гостей!" << endl;
}

void Game::secondLevel() {
    field = GameField(15, 15);
    field.spawn(30, PIT);           
    field.spawn(1, ENEMY);        
    field.spawn(3, COWBOYTOWER);   
    field.spawn(2, BARRACK);  
    field.spawn(2, SALOON);      
    cout << "\n=== Уровень 2: Город-призрак 'Стимпунк-Сити' ===" << endl;
    cout << "Город-призрак кишит бандитами. Будьте осторожны в узких переулках!" << endl;
    cout << "Используйте укрытия и тактику для победы." << endl;
}

void Game::thirdLevel() {
    field = GameField(18, 18);
    field.spawn(40, PIT);           
    field.spawn(1, ENEMY);         
    field.spawn(4, COWBOYTOWER);    
    field.spawn(3, BARRACK);  
    field.spawn(1, SALOON);     
    cout << "\n=== Уровень 3: Шахта 'Проклятый Рубин' ===" << endl;
    cout << "Глубоко в шахтах вас ждут самые опасные преступники Дикого Запада." << endl;
    cout << "Света мало, пространства меньше... Удачи, ковбой!" << endl;
}


int Game::getCurrentLevel() const { 
    return currentLevel; 
}

int Game::getCurrentTurn() const {
    return turn;
}

const Player& Game::getPlayer() const { 
    return player; 
}

const GameField& Game::getField() const { 
    return field; 
}

int Game::getTurn() const { 
    return turn; 
}

bool Game::isRunning() const { 
    return gameRunning; 
}

void Game::setCurrentTurn(int newTurn) { 
    turn = newTurn; 
}


Player& Game::getPlayerRef() { return player; }
GameField& Game::getFieldRef() { return field; }
GameSaveManager& Game::getSaveManagerRef() { return saveManager; }
Input& Game::getInputHandlerRef() { return inputHandler; }
int& Game::getTurnRef() { return turn; }
int& Game::getCurrentLevelRef() { return currentLevel; }
bool& Game::getGameRunningRef() { return gameRunning; }
GameController<Input>& Game::getGameControl() { return *gameControlPtr; }
GameRenderer<OutputTerminal>& Game::getGameRender() { return *gameRendererPtr; }