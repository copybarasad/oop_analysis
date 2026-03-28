#include "game.h"
#include "command.h"
#include "exceptions.h"
#include "game_saver.h"
#include "spell_factory.h"
#include <limits>
#include <random>
#include <string>

Game::Game() : is_running_(false), current_level_(1){
}

void Game::resetGame() {
    enemies_.clear();
    player_.reset(nullptr);
    field_.reset(nullptr);
    is_running_ = false;
}

void Game::loadExistingGame(const std::string& filename) {
    try {
        GameSaver::loadGame(*this, filename);
        is_running_ = true;
        std::cout << "\n--- Игра успешно загружена! ---" << std::endl;
    }
    catch (const GameException& e) {
        std::cerr << "Ошибка загрузки игры: " << e.what() << std::endl;
        std::cerr << "Запуск новой игры..." << std::endl;
        setupNewGame();
    }
}

void Game::saveCurrentGame(const std::string& filename) {
    try {
        GameSaver::saveGame(*this, filename);
        std::cout << "Игра успешно сохранена в файл: " << filename << std::endl;
    }
    catch (const GameException& e) {
        std::cerr << "Ошибка сохранения игры: " << e.what() << std::endl;
    }
}

void Game::createAndPlaceEnemy(int health, int damage, const std::string& name,
    int x, int y) {
    auto enemy = std::make_unique<Enemy>(health, damage, name);
    field_->placeBattler(x, y, enemy.get());
    enemies_.push_back(std::move(enemy));
}

void Game::setupNewGame() {
    resetGame();
    current_level_ = 1;

    const int field_width = 10;
    const int field_height = 10;
    field_ = std::make_unique<PlayingField>(field_height, field_width);
    std::cout<<"\nИгровое поле создано. Размер: " +
        std::to_string(field_->getWidth()) + "x" +
        std::to_string(field_->getHeight()) + " клеток." << std::endl;

    const size_t player_hand_size = 2;
    player_ = std::make_unique<Player>(50, 15, "Герой", 0, 5, player_hand_size);

    const int start_x = 1;
    const int start_y = 1;
    field_->placeBattler(start_x, start_y, player_.get());
    std::cout << "Игрок " + player_->getName() +
        " начинает игру на позиции (" +
        std::to_string(start_x) + ", " +
        std::to_string(start_y) + ")." << std::endl;

    player_->getHand().addSpell(SpellFactory::createRandomSpell());

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> width_dist(0, field_width - 1);
    std::uniform_int_distribution<> height_dist(0, field_height - 1);

    int enemies_to_create = 2;
    std::cout << "Создание врагов..." << std::endl;
    for (int i = 0; i < enemies_to_create; ++i) {
        int x, y;
        do {
            x = width_dist(gen);
            y = height_dist(gen);
        } while (field_->getBattlerAt(x, y) != nullptr);

        std::string name = "Враг #" + std::to_string(i + 1);
        int health = 10 + i * 5;
        int damage = 5 + i;

        createAndPlaceEnemy(health, damage, name, x, y);
        std::cout << "  " + name + " размещен на клетке (" +
            std::to_string(x) + ", " + std::to_string(y) +
            ")" + " [Здоровье: " + std::to_string(health) +
            ", Урон: " + std::to_string(damage) + "]" << std::endl;
    }

    is_running_ = true;
    std::cout << "\n--- Игра началась! ---" << std::endl;
}

bool Game::handleShop() {
    std::cout << "\n--- Магазин заклинаний ---" << std::endl;
    std::cout << "Ваш баланс: " + std::to_string(player_->getScore()) + " очков." << std::endl;

    if (player_->getHand().isFull()) {
        std::cout << "Ваша рука заполнена! Вы не можете купить новое заклинание." << std::endl;
        return false;
    }

    std::cout << "Доступные заклинания:" << std::endl;
    for (int i = 0; i < static_cast<int>(SpellType::COUNT); ++i) {
        SpellType current_type = static_cast<SpellType>(i);
        std::cout << "  " + std::to_string(i + 1) + ". " +
            SpellFactory::getSpellName(current_type) +
            " - " + std::to_string(SpellFactory::getSpellCost(current_type)) + " очков." << std::endl;
    }

    std::cout << "Введите номер заклинания для покупки (или 0 для выхода): " << std::endl;

    int choice;
    std::cin >> choice;

    if (std::cin.fail() || choice < 0 || choice > static_cast<int>(SpellType::COUNT)) {
        std::cout << "Некорректный ввод." << std::endl;
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return false;
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');


    if (choice == 0) {
        std::cout << "Вы покинули магазин." << std::endl;
        return false;
    }

    SpellType chosen_type = static_cast<SpellType>(choice - 1);
    int spell_cost = SpellFactory::getSpellCost(chosen_type);

    if (player_->getScore() >= spell_cost) {
        player_->addScore(-spell_cost);
        player_->getHand().addSpell(SpellFactory::createSpell(chosen_type));
        std::cout << "Покупка совершена! Ваш баланс: " + std::to_string(player_->getScore()) + " очков." << std::endl;
    }
    else {
        std::cout << "Недостаточно очков для покупки!" << std::endl;
    }
    return true;
}

bool Game::areAllEnemiesDefeated() const {
    for (const auto& enemy : enemies_) {
        if (enemy->isAlive()) {
            return false;
        }
    }
    return true;
}

bool Game::processPlayerCommand(const Command& command) {
    bool turn_is_made = true;
    MoveResult move_result = MoveResult::INVALID_BATTLER;

    switch (command.getType()) {
    case CommandType::MOVE_UP:
        move_result = field_->moveUp(player_.get());
        break;
    case CommandType::MOVE_DOWN:
        move_result = field_->moveDown(player_.get());
        break;
    case CommandType::MOVE_LEFT:
        move_result = field_->moveLeft(player_.get());
        break;
    case CommandType::MOVE_RIGHT:
        move_result = field_->moveRight(player_.get());
        break;
    case CommandType::USE_SPELL:
        if (player_->getHand().getSpellsCount() > 0) {
            player_->showSpells();
            std::cout << "Выберите номер заклинания (или 0 для отмены): ";
            int spell_choice;
            std::cin >> spell_choice; 

            if (std::cin.fail() || spell_choice < 0 || spell_choice > player_->getHand().getSpellsCount()) {
                std::cout << "Неверный ввод." << std::endl;
                turn_is_made = false;
            }
            else if (spell_choice > 0) {
                SpellCastResult result = player_->useSpell(spell_choice - 1, *field_);
                if (result == SpellCastResult::INVALID_TARGET) turn_is_made = false;
            }
            else {
                turn_is_made = false;
            }
        }
        else {
            std::cout << "У вас нет заклинаний!" << std::endl;
            turn_is_made = false;
        }
        break;
    case CommandType::OPEN_SHOP:
        turn_is_made = handleShop();
        break;
    case CommandType::SAVE_GAME:
    {
        std::cout << "Введите имя файла для сохранения: ";
        std::string filename;
        std::getline(std::cin, filename);
        saveCurrentGame(filename);
        turn_is_made = false; 
    }
    break;
    case CommandType::QUIT:
        is_running_ = false;
        break;
    case CommandType::UNKNOWN:
    default:
        std::cout << "Неверный ввод! Попробуйте снова." << std::endl;
        turn_is_made = false;
        break;
    }

    if (move_result == MoveResult::TARGET_KILLED) {
        player_->processKillReward();
    }

    return turn_is_made;
}

bool Game::updateGameState() {
    if (!player_->isAlive()) {
        is_running_ = false;
        return false;
    }

    if (areAllEnemiesDefeated()) {
        if (is_running_) {
            startNextLevel();
            return true;
        }
    }
    return false;
}

const PlayingField* Game::getField() const {
    return field_.get();
}

Player* Game::getPlayer() const {
    return player_.get();
}

bool Game::isRunning() const {
    return is_running_;
}

void Game::handleEnemyTurn() {
    std::cout << "\n---------------------------------" << std::endl;
    std::cout << "---          ХОД Врагов       ---" << std::endl;
    std::cout << "---------------------------------" << std::endl;

    auto player_coords = field_->getBattlerCoords(player_.get());
    if (player_coords.first == -1)
        return;

    for (const auto& enemy : enemies_) {
        if (enemy->isAlive()) {
            auto enemy_coords = field_->getBattlerCoords(enemy.get());
            if (enemy_coords.first == -1)
                continue;

            int dx = player_coords.first - enemy_coords.first;
            int dy = player_coords.second - enemy_coords.second;

            Direction chosen_direction;
            if (std::abs(dx) > std::abs(dy)) {
                chosen_direction = (dx > 0) ? Direction::RIGHT : Direction::LEFT;
            }
            else {
                chosen_direction = (dy > 0) ? Direction::UP : Direction::DOWN;
            }

            MoveResult move_result = field_->move(enemy.get(), chosen_direction);
            if (move_result == MoveResult::PLAYER_DIED) {
                is_running_ = false;
                break;
            }
        }
    }
}

void Game::startNextLevel() {
    current_level_++;

    std::cout << "\n*********************************" << std::endl;
    std::cout << "***    УРОВЕНЬ " << current_level_ - 1 << " ПРОЙДЕН!     ***" << std::endl;
    std::cout << "*********************************" << std::endl;

    field_->clearAllEnemies();
    enemies_.clear();

    int health_restored = 25;
    player_->heal(health_restored);
    std::cout << "\nВы восстановили " + std::to_string(health_restored) +
        " здоровья! Текущее здоровье: " + std::to_string(player_->getHealth()) << std::endl;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> width_dist(0, field_->getWidth() - 1);
    std::uniform_int_distribution<> height_dist(0, field_->getHeight() - 1);

    const int base_enemy_count = 2;
    int enemies_to_create = base_enemy_count + (current_level_ - 1);
    std::cout << "Создание врагов для уровня " + std::to_string(current_level_) + "..." << std::endl;

    for (int i = 0; i < enemies_to_create; ++i) {
        int x, y;
        do {
            x = width_dist(gen);
            y = height_dist(gen);
        } while (field_->getBattlerAt(x, y) != nullptr);

        std::string name = "Враг#" + std::to_string(i + 1);
        int base_health = 30 + (current_level_ - 1) * 10;
        int base_damage = 5 + (current_level_ - 1) * 2;
        int health = base_health + i * 5;
        int damage = base_damage + i;

        createAndPlaceEnemy(health, damage, name, x, y);
        std::cout << "  " + name + " размещен на (" + std::to_string(x) + ", " + std::to_string(y) + ")" +
            " [Здоровье: " + std::to_string(health) + ", Урон: " + std::to_string(damage) + "]" << std::endl;
    }

    std::cout << "\n--- Игровое поле - Уровень " + std::to_string(current_level_) + " ---" << std::endl;
}