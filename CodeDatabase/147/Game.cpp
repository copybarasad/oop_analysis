#include "Game.h"
#include "GameManager.h"
#include "GameVisualizer.h"
#include "GameRenderer.h"
#include "ConsoleCommandReader.h"
#include <iostream>
#include <stdexcept>
#include <fstream>
#include <sstream>

Game::Game()
    : _player(base_health, base_damage),
      _hand(5),
      _enemies(),          // изначально пустые
      _towers(),           // изначально пустые
      _controller(nullptr),
      _requiredPoints(500),
      _currentLevel(1),
      _gameOver(false),
      _manager(std::make_unique<GameManager<ConsoleCommandReader>>()),
      _visualizer(std::make_unique<GameVisualizer<GameRenderer>>()),
      _bindings(ConsoleCommandReader::defaultBindings())
{
}

void Game::restartLevelAfterLoad() {
    _controller = std::make_unique<GameController>(
        _player,
        _hand,
        _enemies,
        _towers,
        _seed,
        _currentLevel
    );
    _controller->startLevel();
    if (_visualizer) {
        _visualizer->render(*this, *_controller);
    }
}

void Game::setEventBus(EventBus* bus) {
    _bus = bus;
    if (_manager) {
        _manager->setBus(bus);
    }
}

void Game::setBindings(const std::unordered_map<char, std::string>& bindings) {
    _bindings = bindings.empty() ? ConsoleCommandReader::defaultBindings() : bindings;
    if (_manager) {
        _manager->setBindings(_bindings);
    }
}

Game::~Game() = default;

void Game::resetLevelState() {
    // Полная очистка врагов и башен перед новым уровнем,
    // пока не используем систему сохранений.
    _enemies.clear();
    _towers.clear();
}

void Game::start() {

    while (!_gameOver) {
        // 1) очищаем состояние уровня (враги/башни)
        if (_seed == 0) resetLevelState();

        _controller = std::make_unique<GameController>(
            _player,
            _hand,
            _enemies,
            _towers,
            _seed,
            _currentLevel,
            _bus
        );

        // 3) запускаем уровень
        _controller->startLevel();
        _visualizer->render(*this, *_controller);
        playLevel();

        if (_player.get_health() <= 0) {
            _gameOver = true;
        }
        if (_gameOver) break;

        // если игрок прошёл уровень
        _seed = 0;
        _currentLevel++;
        _player.set_score(0);
        _requiredPoints += 200; // усложняем
    }
}

void Game::playLevel() {
    std::cout << "=== Уровень " << _currentLevel << " ===\n";
    while (checkEndConditions()) {
        try {
            playerTurn();                // ход игрока
            _controller->cleanupDeadUnits(); // чистим поле
            _controller->turnEnemies();  // враги
            _controller->turnTowers();   // башни
        }
        catch (const std::exception& e) {
            std::cerr << "Ошибка: " << e.what() << "\n";
        }
    }
}

void Game::playerTurn() {
    if (!_manager || !_visualizer || !_controller) return;
    bool keepPlaying = _manager->processTurn(*this, *_controller, *_visualizer);
    if (!keepPlaying) {
        _gameOver = true;
    }
}

bool Game::checkEndConditions() const {
    if (_player.get_health() <= 0)
        return false;

    if (_player.get_score() >= _requiredPoints)
        return false;

    return true;
}

// void Game::levelUpMenu() {
//     std::cout << "\n=== Вы прошли уровень! ===\n";
//     std::cout << "Вы можете улучшить характеристики:\n";
//     std::cout << "1) Увеличить силу атаки\n";
//     std::cout << "2) Увеличить здоровье\n";
//     std::cout << "Выбор: ";

//     int choice;
//     std::cin >> choice;

//     if (choice == 1) {
//         base_damage = _controller->getLevelIndex() * 10 + 10;
//         std::cout << "Сила атаки увеличена на " << base_damage << "!\n";
//     } else if (choice == 2) {
//         base_health = _controller->getLevelIndex() * 20 + 100;
//         std::cout << "Здоровье увеличено на " << base_health << "!\n";
//     }
//     _player.change_damage(base_damage);
//     _player.change_health(base_health);
//     std::cout << "Здоровье: " << _player.get_health() << "\n";
//     std::cout << "Сила атаки: " << _player.get_damage() << "\n";
//     std::cout << "Переход на следующий уровень...\n";
// }



void Game::saveToFile(const std::string& filename) const {
    std::ofstream out(filename);
    if (!out.is_open()) {
        throw SaveWriteException("cannot open file '" + filename + "' for writing");
    }

    // сигнатура и версия
    out << "SAVE_V1\n";

    // --- Примитивные поля игры ---
    SaveManager::writePair(out, "LEVEL", std::to_string(_currentLevel));
    SaveManager::writePair(out, "SEED",  std::to_string(_controller->getSeed()));
    SaveManager::writePair(out, "REQ_POINTS", std::to_string(_requiredPoints));

    // --- Игрок ---
    SaveManager::writePair(out, "PLAYER_HEALTH", std::to_string(_player.get_health()));
    SaveManager::writePair(out, "PLAYER_DAMAGE", std::to_string(_player.get_damage()));
    SaveManager::writePair(out, "PLAYER_SCORE",  std::to_string(_player.get_score()));

    {
        Position ppos = _player.getPosition();
        std::ostringstream oss;
        oss << ppos.getRow() << " " << ppos.getColumn();
        SaveManager::writePair(out, "PLAYER_POS", oss.str());
    }

    // --- Рука (только названия заклинаний) ---
    SaveManager::writePair(out, "HAND_SIZE", std::to_string(_hand.size()));
    for (size_t i = 0; i < _hand.size(); ++i) {
        ISpell* s = _hand.getSpell(i);
        // сохраняем только имя, остальное зададим по умолчанию при загрузке
        SaveManager::writePair(out, "SPELL", s->name());
    }

    // --- Враги ---
    SaveManager::writePair(out, "ENEMY_COUNT", std::to_string(_enemies.size()));
    for (const auto& ePtr : _enemies) {
        const Enemy* e = ePtr.get();
        Position ep = e->getPosition();
        std::ostringstream oss;
        oss << e->get_health() << " "
            << e->get_damage() << " "
            << ep.getRow() << " "
            << ep.getColumn();
        SaveManager::writePair(out, "ENEMY", oss.str());
    }

    // --- Башни ---
    SaveManager::writePair(out, "TOWER_COUNT", std::to_string(_towers.size()));
    for (const auto& tPtr : _towers) {
        const SpawnerTower* t = tPtr.get();
        Position tp = t->getPosition();

        
        std::ostringstream oss;
        oss << t->getSpawnCooldown() << " "
            << tp.getRow() << " "
            << tp.getColumn();
        SaveManager::writePair(out, "TOWER", oss.str());
    }

    // маркер конца файла
    out << "END\n" << "1\n";

    if (!out.good()) {
        throw SaveWriteException("finalize error for '" + filename + "'");
    }
}

void Game::loadFromFile(const std::string& filename) {
    std::ifstream in(filename);
    if (!in.is_open()) {
        throw SaveReadException("cannot open file '" + filename + "' for reading");
    }

    // Проверяем сигнатуру
    std::string header;
    if (!std::getline(in, header)) {
        throw SaveFormatException("empty save file '" + filename + "'");
    }
    if (header != "SAVE_V1") {
        throw SaveFormatException("unsupported save version in '" + filename + "': '" + header + "'");
    }

    // Перед загрузкой очищаем текущее состояние уровня
    _enemies.clear();
    _towers.clear();
    _hand.clear();     
    _currentLevel = 0;
    _requiredPoints = 500; 

    std::string key;
    std::string value;

    while (SaveManager::readPair(in, key, value)) {
        if (key == "END") {
            break;
        }
        else if (key == "LEVEL") {
            std::istringstream iss(value);
            if (!(iss >> _currentLevel)) {
                throw SaveFormatException("bad LEVEL value: '" + value + "'");
            }
        }
        else if (key == "SEED") {
            std::istringstream iss(value);
            if (!(iss >> _seed)) {
                throw SaveFormatException("bad SEED value: '" + value + "'");
            }
        }
        else if (key == "REQ_POINTS") {
            std::istringstream iss(value);
            if (!(iss >> _requiredPoints)) {
                throw SaveFormatException("bad REQ_POINTS value: '" + value + "'");
            }
        }
        else if (key == "PLAYER_HEALTH") {
            int hp;
            std::istringstream iss(value);
            if (!(iss >> hp)) {
                throw SaveFormatException("bad PLAYER_HEALTH value: '" + value + "'");
            }
            _player.change_health(hp);
        }
        else if (key == "PLAYER_DAMAGE") {
            int dmg;
            std::istringstream iss(value);
            if (!(iss >> dmg)) {
                throw SaveFormatException("bad PLAYER_DAMAGE value: '" + value + "'");
            }
            _player.change_damage(dmg);
        }
        else if (key == "PLAYER_SCORE") {
            int score;
            std::istringstream iss(value);
            if (!(iss >> score)) {
                throw SaveFormatException("bad PLAYER_SCORE value: '" + value + "'");
            }
            _player.set_score(score); // нужен сеттер счёта; если нет — добавь
        }
        else if (key == "PLAYER_POS") {
            int row, col;
            std::istringstream iss(value);
            if (!(iss >> row >> col)) {
                throw SaveFormatException("bad PLAYER_POS value: '" + value + "'");
            }
            _player.setPosition(Position(row, col));
        }
        else if (key == "HAND_SIZE") {
            // ничего не делаем напрямую, просто знаем, сколько SPELL дальше;
            // можно проверить консистентность при желании
            continue;
        }
        else if (key == "SPELL") {
            // value содержит только имя заклинания
            std::string spellName = value;

            if (spellName == "Direct Damage") {
                _hand.addSpell(std::make_unique<DirectDamageSpell>(10, 3));
            } else if (spellName == "Area Damage") {
                _hand.addSpell(std::make_unique<AreaDamageSpell>(5, 2));
            } else {
                throw SaveFormatException("unknown spell name: '" + spellName + "'");
            }
        }
        else if (key == "ENEMY_COUNT") {
            // можно просто проигнорировать, реальные ENEMY читаем ниже
            continue;
        }
        else if (key == "ENEMY") {
            int hp, dmg, row, col;
            std::istringstream iss(value);
            if (!(iss >> hp >> dmg >> row >> col)) {
                throw SaveFormatException("bad ENEMY value: '" + value + "'");
            }
            auto enemy = std::make_unique<Enemy>(hp, dmg);
            enemy->setPosition(Position(row, col));
            _enemies.push_back(std::move(enemy));
        }
        else if (key == "TOWER_COUNT") {
            // аналогично, просто пропускаем, реальные TOWER позже
            continue;
        }
        else if (key == "TOWER") {
            int cooldown, row, col;
            std::istringstream iss(value);
            if (!(iss >> cooldown >> row >> col)) {
                throw SaveFormatException("bad TOWER value: '" + value + "'");
            }
            auto tower = std::make_unique<SpawnerTower>(cooldown);
            tower->setPosition(Position(row, col));
            _towers.push_back(std::move(tower));
        }
        else {
            throw SaveFormatException("unknown key in save file: '" + key + "'");
        }
    }
}

