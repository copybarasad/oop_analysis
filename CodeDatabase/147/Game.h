#pragma once
#include <memory>
#include <vector>
#include "GameController.h"
#include "player.h"
#include "hand.h"
#include "enemy.h"
#include "tower.h"
#include "SaveManager.h"
#include "Logger.h"
#include "ConfigLoader.h"

class ConsoleCommandReader;
template <typename InputReader> class GameManager;
class GameRenderer;
template <typename Renderer> class GameVisualizer;

class Game {
public:
    Game();
    ~Game();

    void start(); // основной игровой цикл
    void saveToFile(const std::string& filename) const;
    void loadFromFile(const std::string& filename);   // НОВОЕ
    void restartLevelAfterLoad();      // пересоздать контроллер после загрузки
    void setEventBus(EventBus* bus);
    void setBindings(const std::unordered_map<char, std::string>& bindings);

    // Геттеры для доступа к состоянию игры
    Player& getPlayer() { return _player; }
    const Player& getPlayer() const { return _player; }
    Hand& getHand() { return _hand; }
    const Hand& getHand() const { return _hand; }
    std::vector<std::unique_ptr<Enemy>>& getEnemies() { return _enemies; }
    const std::vector<std::unique_ptr<Enemy>>& getEnemies() const { return _enemies; }
    std::vector<std::unique_ptr<SpawnerTower>>& getTowers() { return _towers; }
    const std::vector<std::unique_ptr<SpawnerTower>>& getTowers() const { return _towers; }
    int getCurrentLevel() const { return _currentLevel; }
    int getRequiredPoints() const { return _requiredPoints; }

    GameController* getController() const { return _controller.get(); }

private:
    void playLevel();                  // логика одного уровня
    void playerTurn();                 // обработка хода игрока
    //void levelUpMenu();                // меню улучшений после уровня
    bool checkEndConditions() const;   // проверка: жив ли игрок, набрал ли очки  

    void resetLevelState();            // очистка врагов и башен перед новым уровнем
    
private:
    // Базовые параметры игрока
    int base_health = 100;
    int base_damage = 10;

    // Параметры для будущих настроек
    int base_health_e = 100;
    int enemy_count = 5;
    int tower_count = 1;

    unsigned int _seed = 0;            // seed уровня (пригодится для сохранений)

    Player _player;
    Hand _hand;

    // Контейнеры врагов и башен теперь принадлежат Game
    std::vector<std::unique_ptr<Enemy>> _enemies;    
    std::vector<std::unique_ptr<SpawnerTower>> _towers; 

    // Контроллер работает по ссылке на существующие объекты
    std::unique_ptr<GameController> _controller;

    int _requiredPoints;   // сколько очков нужно для победы на уровне
    int _currentLevel;
    bool _gameOver;

    std::unique_ptr<GameManager<ConsoleCommandReader>> _manager;
    std::unique_ptr<GameVisualizer<GameRenderer>> _visualizer;
    EventBus* _bus = nullptr; // не владеет
    std::unordered_map<char, std::string> _bindings;
};
