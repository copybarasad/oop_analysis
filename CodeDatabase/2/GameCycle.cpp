#include "GameCycle.hpp"
#include "SpellDirectDamage.hpp"
#include "SpellSplash.hpp"
#include "SpellTrap.hpp"
#include "SpellSummon.hpp"
#include "SpellEnhancement.hpp"

#include <iostream>
#include <stdexcept>
#include <random>
#include <vector>
#include <memory>
#include <string>
#include <ctime>
#include <fstream>
GameCycle::GameCycle()
    : currentLevel(1),
      playerTurn(true),
      gameOver(false),
      victory(false),
      castingSpell(false),
      selectedSpellIndex(-1),
      saveLoadMenu(false),
      hp_text(font),
      mana_text(font),
      mode_text(font),
      turn_text(font),
      spell_text(font),
      game_over_text(font),
      victory_text(font),
      casting_text(font)
      {

}

void GameCycle::startNewGame(int length, int width) {
    try {
        std::cout << "Creating board " << length << "x" << width << std::endl;
        board = std::make_unique<Board>(length, width);

        if (!player) {
            player = std::make_unique<Player>();
            std::cout << "New player created with HP: " << player->get_max_hp() << std::endl;
        } else {
            std::cout << " Using existing player with improved HP: " << player->get_max_hp() << std::endl;
        }

        if (!playerHand) {
            playerHand = std::make_unique<PlayersHand>();
        }

        std::cout << "DEBUG: Board created successfully" << std::endl;

        // СОЗДАЕМ БАШНЮ-СПАВНЕР
        spawnTower = std::make_unique<EnemyTower>(length - 1, width - 1, currentLevel);
        std::cout << "DEBUG: Spawn tower created at " << length - 1 << "," << width - 1 << std::endl;
        // СОЗДАЕМ АТАКУЮЩУЮ БАШНЮ
        int a_towerX, a_towerY;
        std::srand(std::time(nullptr));
        int attempts = 0;
        bool position_found = false;

        while (attempts < length * width && !position_found) {
            a_towerX = std::rand() % length;
            a_towerY = std::rand() % width;

            if ((a_towerX != 0 || a_towerY != 0) &&
                (a_towerX != length - 1 || a_towerY != width - 1) &&
                board->can_move_to(a_towerX, a_towerY) &&
                !board->get_cell(a_towerX, a_towerY).is_obstacle_here() &&
                !board->get_cell(a_towerX, a_towerY).is_player_here() &&
                !board->get_cell(a_towerX, a_towerY).is_enemy_here()) {
                position_found = true;
            }
            attempts++;
        }

        if (!position_found) {
            a_towerX = length / 2;
            a_towerY = width / 2;
        }

        attackTower = std::make_unique<EnemyAttackTower>(a_towerX, a_towerY, currentLevel);
        std::cout << " Attack tower created at " << a_towerX << "," << a_towerY << std::endl;

        initializeLevel();

        std::cout << "New game started! Level " << currentLevel << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Exception in startNewGame: " << e.what() << std::endl;
        throw std::runtime_error("Failed to start new game: " + std::string(e.what()));
    }
}

void GameCycle::showSaveLoadMenu(bool isSaveMenu) {
    if (gameOver) return;

    // Временно приостанавливаем основное окно
    sf::RenderWindow menuWindow(sf::VideoMode({800, 600}),
                               isSaveMenu ? "Save Game" : "Load Game");

    saveLoadMenu.setMode(isSaveMenu);
    if (saveLoadMenu.initialize()) {
        int slot = saveLoadMenu.show(menuWindow);
        if (slot > 0) {
            if (isSaveMenu) {
                if (saveManager.saveGame(slot, *this)) {
                    std::cout << "Game saved to slot " << slot << std::endl;
                } else {
                    std::cout << "Failed to save to slot " << slot << std::endl;
                }
            } else {
                if (saveManager.loadGame(slot, *this)) {
                    std::cout << "Game loaded from slot " << slot << std::endl;
                } else {
                    std::cout << "Failed to load from slot " << slot << std::endl;
                }
            }
        }
    }
    menuWindow.close();
}

void GameCycle::quickSave(int slot) {
    if (gameOver) return;

    try {
        if (saveManager.saveGame(slot, *this)) {
            std::cout << "Quick save to slot " << slot << std::endl;
        } else {
            std::cerr << "Quick save failed" << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "Quick save failed: " << e.what() << std::endl;
    }
}

void GameCycle::quickLoad(int slot) {
    try {
        if (saveManager.loadGame(slot, *this)) {
            std::cout << "Quick load from slot " << slot << std::endl;
        } else {
            std::cerr << "Quick load failed: save file not found" << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "Quick load failed: " << e.what() << std::endl;
    }
}

void GameCycle::initializeLevel() {
    std::cout << "DEBUG: initializeLevel called" << std::endl;

    enemies.clear();
    allies.clear();

    if (!board || !player) {
        std::cerr << "Error: Board or Player not initialized!" << std::endl;
        return;
    }

    std::cout << "DEBUG: Clearing board entities..." << std::endl;

    for (int y = 0; y < board->get_width(); y++) {
        for (int x = 0; x < board->get_length(); x++) {
            Cell& cell = board->get_cell(x, y);
            cell.set_player(false);
            cell.set_enemy(false);
            cell.set_ally(false);
            cell.set_tower(false);
            cell.set_attack_tower(false);
        }
    }

    // 1. Устанавливаем игрока
    player->set_position(0, 0);
    board->get_cell(0, 0).set_player(true);
    std::cout << "DEBUG: Player set at 0,0" << std::endl;

    // 2. Устанавливаем башню-спавнер
    int spawnX, spawnY;
    spawnTower->get_coords(spawnX, spawnY);
    board->get_cell(spawnX, spawnY).set_tower(true);
    std::cout << "DEBUG: Spawn tower set at " << spawnX << "," << spawnY << std::endl;

    // 3. Устанавливаем атакующую башню
    int attackX, attackY;
    attackTower->get_coords(attackX, attackY);
    board->get_cell(attackX, attackY).set_attack_tower(true);
    std::cout << "DEBUG: Attack tower set at " << attackX << "," << attackY << std::endl;

    // 4. Создаем начальных врагов
    std::vector<std::pair<int, int>> spawnPositions = {
        {board->get_length() - 2, board->get_width() - 1},
        {board->get_length() - 1, board->get_width() - 2},
        {board->get_length() - 2, board->get_width() - 2}
    };

    int enemiesCreated = 0;
    int maxInitialEnemies = std::min(1 + (currentLevel - 1) / 2, 3);

    for (const auto& pos : spawnPositions) {
        if (enemiesCreated >= maxInitialEnemies) break;

        if (board->can_move_to(pos.first, pos.second) &&
            !board->get_cell(pos.first, pos.second).is_obstacle_here() &&
            !board->get_cell(pos.first, pos.second).is_tower_here() &&
            !board->get_cell(pos.first, pos.second).is_attack_tower_here() &&
            !board->get_cell(pos.first, pos.second).is_player_here()) {

            enemies.emplace_back(pos.first, pos.second, currentLevel);
            board->get_cell(pos.first, pos.second).set_enemy(true);
            enemiesCreated++;
            std::cout << "DEBUG: Enemy created at " << pos.first << "," << pos.second << std::endl;
        }
    }
    // 5. Добавляем заклинание
    playerHand->AddRandomSpell();

    // 6. Проверяем состояние доски
    int playerCount = 0, enemyCount = 0, towerCount = 0, attackTowerCount = 0;
    for (int y = 0; y < board->get_width(); y++) {
        for (int x = 0; x < board->get_length(); x++) {
            Cell& cell = board->get_cell(x, y);
            if (cell.is_player_here()) playerCount++;
            if (cell.is_enemy_here()) enemyCount++;
            if (cell.is_tower_here()) towerCount++;
            if (cell.is_attack_tower_here()) attackTowerCount++;
        }
    }

    std::cout << "DEBUG: After initialization - Players: " << playerCount
              << ", Enemies: " << enemyCount
              << ", Towers: " << towerCount
              << ", Attack Towers: " << attackTowerCount << std::endl;

    std::cout << "Level " << currentLevel << " initialized: "
              << enemiesCreated << " initial enemies, "
              << playerHand->getSpellCount() << " spells in hand" << std::endl;
}

void GameCycle::updateBoardPositions() {
    if (!board) return;

    for (int y = 0; y < board->get_width(); y++) {
        for (int x = 0; x < board->get_length(); x++) {
            Cell& cell = board->get_cell(x, y);
            cell.set_player(false);
            cell.set_enemy(false);
            cell.set_ally(false);
            // НЕ очищаем tower и attack_tower - они статичны
        }
    }

    if (player) {
        int x, y;
        player->get_coords(x, y);
        if (x >= 0 && x < board->get_length() && y >= 0 && y < board->get_width()) {
            board->get_cell(x, y).set_player(true);
        }
    }

    // Добавляем врагов
    for (auto& enemy : enemies) {
        if (!enemy.is_dead()) {
            int x, y;
            enemy.get_coords(x, y);
            if (x >= 0 && x < board->get_length() && y >= 0 && y < board->get_width()) {
                board->get_cell(x, y).set_enemy(true);
            }
        }
    }

    // ДОБАВЛЯЕМ СОЮЗНИКОВ
    for (auto& ally : allies) {
        if (!ally.is_dead()) {
            int x, y;
            ally.get_coords(x, y);
            if (x >= 0 && x < board->get_length() && y >= 0 && y < board->get_width()) {
                board->get_cell(x, y).set_ally(true);
                std::cout << "DEBUG: Ally placed on board at " << x << "," << y << std::endl;
            }
        }
    }


    int allyCount = 0;
    for (int y = 0; y < board->get_width(); y++) {
        for (int x = 0; x < board->get_length(); x++) {
            if (board->get_cell(x, y).is_ally_here()) allyCount++;
        }
    }

    if (spawnTower && !spawnTower->is_death()) {
        int x, y;
        spawnTower->get_coords(x, y);
        if (x >= 0 && x < board->get_length() && y >= 0 && y < board->get_width()) {
            board->get_cell(x, y).set_tower(true);
            std::cout << "DEBUG: Spawn tower placed at " << x << "," << y << std::endl;
        }
    }

    if (attackTower && !attackTower->is_death()) {
        int x, y;
        attackTower->get_coords(x, y);
        if (x >= 0 && x < board->get_length() && y >= 0 && y < board->get_width()) {
            board->get_cell(x, y).set_attack_tower(true);
            std::cout << "DEBUG: Attack tower placed at " << x << "," << y << std::endl;
        }
    }
}

void GameCycle::run() {
    if (!board) {
        throw std::runtime_error("Game not initialized. Call startNewGame first.");
    }

    sf::RenderWindow window(sf::VideoMode({1400, 1000}), "OOP Game - Level " + std::to_string(currentLevel), sf::Style::Default);

    loadTextures();
    loadFonts();
    setupUI();

    std::cout << "STARTING GAME CYCLE - Level: " << currentLevel
              << ", Board: " << board->get_length() << "x" << board->get_width()
              << ", Player HP: " << player->get_hp() << "/" << player->get_max_hp() << std::endl;

    std::cout << "Game started! Use WASD to move, Q to change attack mode, SPACE to skip your step" << std::endl;

    bool levelCompleted = false;

    while (window.isOpen() && !gameOver) {
        processInput(window);

        if (!playerTurn && !gameOver) {
            processEntityTurn();
        }

        checkWinCondition();
        checkLossCondition();

        if (victory && !levelCompleted) {
            levelCompleted = true;
            std::cout << "Level " << currentLevel << " completed! Setting up next level..." << std::endl;

            window.close();
            setupNextLevel();

            run();
            return;
        }

        render(window);
    }

    if (gameOver && !victory) {
        showGameOverScreen(window);
    }
}

void GameCycle::loadTextures() {
    bool texturesLoaded = true;

    if (!texture_player.loadFromFile("assets/player.png")) {
        std::cout << "DEBUG: Failed to load player.png, using colored shapes" << std::endl;
        texturesLoaded = false;
    }
    if (!texture_enemy.loadFromFile("assets/enemy.png")) {
        std::cout << "DEBUG: Failed to load enemy.png, using colored shapes" << std::endl;
        texturesLoaded = false;
    }
    if (!texture_ally.loadFromFile("assets/Ally.png")) {
        std::cout << "DEBUG: Failed to load Ally.png, using colored shapes" << std::endl;
        texturesLoaded = false;
    }
    if (!texture_empty_cell.loadFromFile("assets/empty_cell.png")) {
        std::cout << "DEBUG: Failed to load empty_cell.png, using colored shapes" << std::endl;
        texturesLoaded = false;
    }
    if (!texture_obstacle_cell.loadFromFile("assets/obstacle_cell.png")) {
        std::cout << "DEBUG: Failed to load obstacle_cell.png, using colored shapes" << std::endl;
        texturesLoaded = false;
    }
    if (!texture_slow_cell.loadFromFile("assets/slow_cell.png")) {
        std::cout << "DEBUG: Failed to load slow_cell.png, using colored shapes" << std::endl;
        texturesLoaded = false;
    }
    if (!texture_trap_cell.loadFromFile("assets/trap_cell.png")) {
        std::cout << "DEBUG: Failed to load trap_cell.png, using colored shapes" << std::endl;
        texturesLoaded = false;
    }
    if (!texture_tower.loadFromFile("assets/tower.png")) {
        std::cout << "DEBUG: Failed to load tower.png, using colored shapes" << std::endl;
        texturesLoaded = false;
    }
    if (!texture_attack_tower.loadFromFile("assets/attack_tower.png")) {
        std::cout << "DEBUG: Failed to load attack_tower.png, using colored shapes" << std::endl;
        texturesLoaded = false;
    }

    if (texturesLoaded) {
        sprite_player = std::make_unique<sf::Sprite>(texture_player);
        sprite_enemy = std::make_unique<sf::Sprite>(texture_enemy);
        sprite_ally = std::make_unique<sf::Sprite>(texture_ally);
        sprite_empty_cell = std::make_unique<sf::Sprite>(texture_empty_cell);
        sprite_obstacle_cell = std::make_unique<sf::Sprite>(texture_obstacle_cell);
        sprite_slow_cell = std::make_unique<sf::Sprite>(texture_slow_cell);
        sprite_trap_cell = std::make_unique<sf::Sprite>(texture_trap_cell);
        sprite_tower = std::make_unique<sf::Sprite>(texture_tower);
        sprite_attack_tower = std::make_unique<sf::Sprite>(texture_attack_tower);

        std::cout << "DEBUG: All textures loaded and sprites created successfully!" << std::endl;
    } else {
        std::cout << "DEBUG: Some textures failed to load, using colored shapes" << std::endl;
    }
}

void GameCycle::loadFonts() {
    if (!font.openFromFile("assets/ArialRegular.ttf")) {
        throw std::runtime_error("Failed to load font ArialRegular.ttf");
    }
    std::cout << "Font loaded successfully!" << std::endl;
}

void GameCycle::setupUI() {
    hp_text.setFont(font);
    hp_text.setCharacterSize(20);
    hp_text.setFillColor(sf::Color::White);
    hp_text.setPosition({50.0f, 20.0f});

    mana_text.setFont(font);
    mana_text.setCharacterSize(20);
    mana_text.setFillColor(sf::Color::Blue);
    mana_text.setPosition({50.0f, 45.0f});

    mode_text.setFont(font);
    mode_text.setCharacterSize(20);
    mode_text.setFillColor(sf::Color::White);
    mode_text.setPosition({50.0f, 70.0f});

    turn_text.setFont(font);
    turn_text.setCharacterSize(20);
    turn_text.setFillColor(sf::Color::Green);
    turn_text.setPosition({50.0f, 95.0f});

    spell_text.setFont(font);
    spell_text.setCharacterSize(16);
    spell_text.setFillColor(sf::Color::White);
    spell_text.setPosition({300.0f, 20.0f});

    game_over_text.setFont(font);
    game_over_text.setString("GAME OVER");
    game_over_text.setCharacterSize(50);
    game_over_text.setFillColor(sf::Color::Red);
    game_over_text.setPosition({400.0f, 350.0f});

    victory_text.setFont(font);
    victory_text.setString("VICTORY!");
    victory_text.setCharacterSize(50);
    victory_text.setFillColor(sf::Color::Green);
    victory_text.setPosition({400.0f, 350.0f});

    casting_text.setFont(font);
    casting_text.setString("Select target for spell");
    casting_text.setCharacterSize(20);
    casting_text.setFillColor(sf::Color::Yellow);
    casting_text.setPosition({400.0f, 20.0f});

}

void GameCycle::processInput(sf::RenderWindow& window) {
    while (const std::optional event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            autoSave();
            window.close();
        }
        else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
            if (keyPressed->scancode == sf::Keyboard::Scancode::Escape) {
                autoSave();
                window.close();
            } else if (keyPressed->scancode == sf::Keyboard::Scancode::F5) {
                quickSave(1); // Быстрое сохранение в слот 1
            }
            else if (keyPressed->scancode == sf::Keyboard::Scancode::F6) {
                showSaveLoadMenu(true); // Меню сохранения
            }
            else if (keyPressed->scancode == sf::Keyboard::Scancode::F9) {
                quickLoad(1); // Быстрая загрузка из слота 1
            }
            else if (keyPressed->scancode == sf::Keyboard::Scancode::F10) {
                showSaveLoadMenu(false); // Меню загрузки
            }
            else if (playerTurn && !gameOver) {

                switch (keyPressed->scancode) {
                    case sf::Keyboard::Scancode::W:
                        std::cout << "DEBUG: W pressed" << std::endl;
                        processPlayerInput('W');
                        break;
                    case sf::Keyboard::Scancode::A:
                        processPlayerInput('A');
                        break;
                    case sf::Keyboard::Scancode::S:
                        processPlayerInput('S');
                        break;
                    case sf::Keyboard::Scancode::D:
                        processPlayerInput('D');
                        break;
                    case sf::Keyboard::Scancode::Q:
                        processPlayerInput('Q');
                        break;
                    case sf::Keyboard::Scancode::Space:
                        processPlayerInput(' ');
                        break;
                    case sf::Keyboard::Scancode::Num1:
                        std::cout << "DEBUG: Num1 pressed" << std::endl;
                        processPlayerInput('1');
                        break;
                    case sf::Keyboard::Scancode::Num2:
                        processPlayerInput('2');
                        break;
                    case sf::Keyboard::Scancode::Num3:
                        processPlayerInput('3');
                        break;
                    case sf::Keyboard::Scancode::Num4:
                        processPlayerInput('4');
                        break;
                    case sf::Keyboard::Scancode::Num5:
                        processPlayerInput('5');
                        break;
                    default:
                        std::cout << "DEBUG: Unknown scancode: " << static_cast<int>(keyPressed->scancode) << std::endl;
                        break;
                }
            }
        }
        else if (const auto* mousePressed = event->getIf<sf::Event::MouseButtonPressed>()) {
            if (mousePressed->button == sf::Mouse::Button::Left && castingSpell) {
                std::cout << "DEBUG: Mouse click during casting spell" << std::endl;
                handleSpellTargetSelection(window);
            }
        }
    }
}

void GameCycle::render(sf::RenderWindow& window) {
    window.clear(sf::Color(50, 50, 50));

    if (!board) return;

    sf::Vector2u windowSize = window.getSize();
    float windowWidth = static_cast<float>(windowSize.x);
    float windowHeight = static_cast<float>(windowSize.y);

    float max_cell_size_x = (windowWidth - 300.0f) / board->get_length();
    float max_cell_size_y = (windowHeight - 200.0f) / board->get_width();

    float cell_size = std::min(max_cell_size_x, max_cell_size_y);
    cell_size = std::max(20.0f, std::min(cell_size, 60.0f));

    float board_width = board->get_length() * cell_size;
    float board_height = board->get_width() * cell_size;

    float offset_x = (windowWidth - board_width - 250.0f) / 2;
    float offset_y = 120.0f;

    if (board_width > windowWidth - 300.0f || board_height > windowHeight - 200.0f) {
        static bool warning_shown = false;
        if (!warning_shown) {
            std::cout << "WARNING: Board too large! Cell size reduced to: " << cell_size << "px" << std::endl;
            warning_shown = true;
        }
    }

    static float last_cell_size = 0;
    if (std::abs(last_cell_size - cell_size) > 1.0f) {
        std::cout << "=== RENDER DEBUG ===" << std::endl;
        std::cout << "Window: " << windowWidth << "x" << windowHeight << std::endl;
        std::cout << "Board: " << board->get_length() << "x" << board->get_width() << std::endl;
        std::cout << "Cell size: " << cell_size << "px" << std::endl;
        std::cout << "Board area: " << board_width << "x" << board_height << std::endl;
        std::cout << "Offset: " << offset_x << ", " << offset_y << std::endl;
        std::cout << "=== END RENDER DEBUG ===" << std::endl;
        last_cell_size = cell_size;
    }

    // ПЕРВЫЙ ПРОХОД: отрисовка ФОНА клеток
    for (int y = 0; y < board->get_width(); y++) {
        for (int x = 0; x < board->get_length(); x++) {
            Cell& cell = board->get_cell(x, y);
            float pos_x = offset_x + x * cell_size;
            float pos_y = offset_y + y * cell_size;

            // Отрисовка только ФОНА клетки (без ловушек)
            if (cell.is_obstacle_here() && sprite_obstacle_cell) {
                sprite_obstacle_cell->setPosition({pos_x, pos_y});
                float scale_x = cell_size / texture_obstacle_cell.getSize().x;
                float scale_y = cell_size / texture_obstacle_cell.getSize().y;
                sprite_obstacle_cell->setScale({scale_x, scale_y});
                window.draw(*sprite_obstacle_cell);
            }
            else if (cell.is_slow_here() && sprite_slow_cell) {
                sprite_slow_cell->setPosition({pos_x, pos_y});
                float scale_x = cell_size / texture_slow_cell.getSize().x;
                float scale_y = cell_size / texture_slow_cell.getSize().y;
                sprite_slow_cell->setScale({scale_x, scale_y});
                window.draw(*sprite_slow_cell);
            }
            else if (sprite_empty_cell) {
                // Пустая клетка - базовый фон для ВСЕХ клеток
                sprite_empty_cell->setPosition({pos_x, pos_y});
                float scale_x = cell_size / texture_empty_cell.getSize().x;
                float scale_y = cell_size / texture_empty_cell.getSize().y;
                sprite_empty_cell->setScale({scale_x, scale_y});
                window.draw(*sprite_empty_cell);
            }
        }
    }

    // отрисовка ЛОВУШЕК поверх фона
    for (int y = 0; y < board->get_width(); y++) {
        for (int x = 0; x < board->get_length(); x++) {
            Cell& cell = board->get_cell(x, y);
            float pos_x = offset_x + x * cell_size;
            float pos_y = offset_y + y * cell_size;

            if (cell.is_trap_here() && sprite_trap_cell) {
                sprite_trap_cell->setPosition({pos_x, pos_y});
                float scale_x = cell_size / texture_trap_cell.getSize().x;
                float scale_y = cell_size / texture_trap_cell.getSize().y;
                sprite_trap_cell->setScale({scale_x, scale_y});
                window.draw(*sprite_trap_cell);
            }
        }
    }

    // отрисовка СУЩНОСТЕЙ поверх всего
    for (int y = 0; y < board->get_width(); y++) {
        for (int x = 0; x < board->get_length(); x++) {
            Cell& cell = board->get_cell(x, y);
            float pos_x = offset_x + x * cell_size;
            float pos_y = offset_y + y * cell_size;

            // Отрисовка игрока
            if (cell.is_player_here() && sprite_player) {
                sprite_player->setPosition({pos_x, pos_y});
                float scale_x = cell_size / texture_player.getSize().x;
                float scale_y = cell_size / texture_player.getSize().y;
                sprite_player->setScale({scale_x, scale_y});
                window.draw(*sprite_player);
            }

            // Отрисовка врагов
            if (cell.is_enemy_here() && sprite_enemy) {
                sprite_enemy->setPosition({pos_x, pos_y});
                float scale_x = cell_size / texture_enemy.getSize().x;
                float scale_y = cell_size / texture_enemy.getSize().y;
                sprite_enemy->setScale({scale_x, scale_y});
                window.draw(*sprite_enemy);
            }

            // Отрисовка союзников
            if (cell.is_ally_here() && sprite_ally) {
                sprite_ally->setPosition({pos_x, pos_y});
                float scale_x = cell_size / texture_ally.getSize().x;
                float scale_y = cell_size / texture_ally.getSize().y;
                sprite_ally->setScale({scale_x, scale_y});
                window.draw(*sprite_ally);
            }

            // Отрисовка башни-спавнера
            if (cell.is_tower_here() && spawnTower && !spawnTower->is_death() && sprite_tower) {
                sprite_tower->setPosition({pos_x, pos_y});
                float scale_x = cell_size / texture_tower.getSize().x;
                float scale_y = cell_size / texture_tower.getSize().y;
                sprite_tower->setScale({scale_x, scale_y});
                window.draw(*sprite_tower);
            }

            // Отрисовка атакующей башни
            if (cell.is_attack_tower_here() && attackTower && !attackTower->is_death() && sprite_attack_tower) {
                sprite_attack_tower->setPosition({pos_x, pos_y});
                float scale_x = cell_size / texture_attack_tower.getSize().x;
                float scale_y = cell_size / texture_attack_tower.getSize().y;
                sprite_attack_tower->setScale({scale_x, scale_y});
                window.draw(*sprite_attack_tower);
            }

            if (castingSpell) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                int debug_x = static_cast<int>((mousePos.x - offset_x) / cell_size);
                int debug_y = static_cast<int>((mousePos.y - offset_y) / cell_size);

                debug_x = std::max(0, std::min(debug_x, board->get_length() - 1));
                debug_y = std::max(0, std::min(debug_y, board->get_width() - 1));

                sf::RectangleShape debug_rect(sf::Vector2f(cell_size - 4, cell_size - 4));
                debug_rect.setPosition({offset_x + debug_x * cell_size + 2, offset_y + debug_y * cell_size + 2});
                debug_rect.setFillColor(sf::Color::Transparent);
                debug_rect.setOutlineColor(sf::Color::Red);
                debug_rect.setOutlineThickness(2);
                window.draw(debug_rect);

                // Текст с координатами
                sf::Text coord_text(font);
                coord_text.setString(std::to_string(debug_x) + "," + std::to_string(debug_y));
                coord_text.setCharacterSize(12);
                coord_text.setFillColor(sf::Color::Red);
                coord_text.setPosition({offset_x + debug_x * cell_size, offset_y + debug_y * cell_size});
                window.draw(coord_text);
            }
        }
    }

    updateUI();
    drawUI(window);

    window.display();
}

void GameCycle::updateUI() {
    if (player) {
        hp_text.setString("HP: " + std::to_string(player->get_hp()) + "/" + std::to_string(player->get_max_hp()));
        mana_text.setString("Mana: " + std::to_string(player->get_mana()) + "/" + std::to_string(player->get_max_mana()));
        mode_text.setString("Attack: " + std::string(player->get_mode() == fight_mode::MELEE ? "MELEE" : "RANGE"));

        // Цвет маны
        if (player->get_mana() < 5) {
            mana_text.setFillColor(sf::Color::Red);
        } else if (player->get_mana() < 10) {
            mana_text.setFillColor(sf::Color::Yellow);
        } else {
            mana_text.setFillColor(sf::Color::Cyan);
        }
    }

    if (playerTurn) {
        turn_text.setString("Turn: Player");
        turn_text.setFillColor(sf::Color::Green);
    } else {
        turn_text.setString("Turn: Enemy");
        turn_text.setFillColor(sf::Color::Red);
    }

    // ОТОБРАЖЕНИЕ ЗАКЛИНАНИЙ
    std::string spells_info = "Spells:\n";
    for (int i = 0; i < playerHand->getSpellCount(); ++i) {
        Spell* spell = playerHand->getSpell(i);
        if (spell) {
            std::string cost_str = std::to_string(spell->get_mana_cost());
            bool canCast = (player->get_mana() >= spell->get_mana_cost());

            if (canCast) {
                cost_str = "[" + cost_str + "]"; // Можно кастовать
            } else {
                cost_str = "(" + cost_str + ")"; // Не хватает маны
            }

            // Сокращаем длинные названия для компактности
            std::string spell_name = spell->get_name();
            if (spell_name.length() > 15) {
                spell_name = spell_name.substr(0, 10) + "..";
            }

            spells_info += std::to_string(i + 1) + ". " + spell_name + " " + cost_str + "\n";
        }
    }
    spell_text.setString(spells_info);
}

void GameCycle::drawUI(sf::RenderWindow& window) {
    sf::Vector2u windowSize = window.getSize();
    float windowWidth = static_cast<float>(windowSize.x);

    // Левая панель - статистика игрока
    hp_text.setPosition({20.0f, 20.0f});
    mana_text.setPosition({20.0f, 45.0f});
    mode_text.setPosition({20.0f, 70.0f});
    turn_text.setPosition({20.0f, 95.0f});

    // Правая панель - заклинания (фиксированная позиция справа)
    float spellsX = windowWidth - 250.0f; // Фиксированный отступ от правого края
    spell_text.setPosition({spellsX, 20.0f});

    window.draw(hp_text);
    window.draw(mana_text);
    window.draw(mode_text);
    window.draw(turn_text);
    window.draw(spell_text);

    // Если выбираем цель для заклинания
    if (castingSpell) {
        casting_text.setString("Select target for spell (Click on cell)");
        casting_text.setPosition({windowWidth / 2 - 150.0f, 20.0f}); // Центрируем
        window.draw(casting_text);
    }
}

void GameCycle::showGameOverScreen(sf::RenderWindow& window) {
    std::cout << "Game Over screen displayed" << std::endl;

    sf::Text restart_text(font);
    restart_text.setString("Press R to restart or ESC to exit");
    restart_text.setCharacterSize(24);
    restart_text.setFillColor(sf::Color::White);
    restart_text.setPosition({250.0f, 450.0f});

    bool waitingForInput = true;
    while (waitingForInput && window.isOpen()) {

        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
                waitingForInput = false;
            }
            else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                if (keyPressed->scancode == sf::Keyboard::Scancode::Escape) {
                    window.close();
                    waitingForInput = false;
                }
                else if (keyPressed->scancode == sf::Keyboard::Scancode::R) {

                    window.close();
                    waitingForInput = false;


                    GameCycle newGame;
                    newGame.startNewGame(15, 15);
                    newGame.run();
                    return;
                }
            }
        }

        //  ОТРИСОВКА ЭКРАНА GAME OVER
        window.clear(sf::Color(30, 30, 30));

        if (victory) {
            victory_text.setPosition({300.0f, 350.0f});
            window.draw(victory_text);
        } else {
            game_over_text.setPosition({300.0f, 350.0f});
            window.draw(game_over_text);
        }
        window.draw(restart_text);
        window.display();
    }
}

void GameCycle::handleSpellTargetSelection(const sf::RenderWindow& window) {
    if (!board || !castingSpell) return;

    sf::Vector2u windowSize = window.getSize();
    float windowWidth = static_cast<float>(windowSize.x);
    float windowHeight = static_cast<float>(windowSize.y);

    // ТОТ ЖЕ РАСЧЕТ ЧТО И В RENDER()
    float max_cell_size_x = (windowWidth - 300.0f) / board->get_length();
    float max_cell_size_y = (windowHeight - 200.0f) / board->get_width();
    float cell_size = std::min(max_cell_size_x, max_cell_size_y);
    cell_size = std::max(20.0f, std::min(cell_size, 60.0f));

    float board_width = board->get_length() * cell_size;
    float board_height = board->get_width() * cell_size;

    float offset_x = (windowWidth - board_width - 250.0f) / 2;
    float offset_y = 120.0f;

    sf::Vector2i mousePos = sf::Mouse::getPosition(window);

    // ПРОВЕРЯЕМ ГРАНИЦЫ
    if (mousePos.x < offset_x || mousePos.y < offset_y ||
        mousePos.x >= offset_x + board_width ||
        mousePos.y >= offset_y + board_height) {
        std::cout << "Click outside board!" << std::endl;
        return;
        }

    // РАСЧЕТ КООРДИНАТ
    int target_x = static_cast<int>((mousePos.x - offset_x) / cell_size);
    int target_y = static_cast<int>((mousePos.y - offset_y) / cell_size);

    // ОГРАНИЧИВАЕМ КООРДИНАТЫ
    target_x = std::max(0, std::min(target_x, board->get_length() - 1));
    target_y = std::max(0, std::min(target_y, board->get_width() - 1));

    std::cout << "Spell target: " << target_x << ", " << target_y << std::endl;

    processSpellCast(selectedSpellIndex, target_x, target_y);
    setCastingSpell(false);
}

void GameCycle::processSpellCast(int spellIndex, int targetX, int targetY) {
    if (!playerTurn || gameOver || !player || !playerHand) return;

    if (spellIndex < 0 || spellIndex >= playerHand->getSpellCount()) {
        std::cout << "Invalid spell index!" << std::endl;
        return;
    }

    Spell* spell = playerHand->getSpell(spellIndex);
    if (!spell) return;

    std::cout << "=== SPELL CAST DEBUG ===" << std::endl;
    std::cout << "Spell: " << spell->get_name() << ", Cost: " << spell->get_mana_cost() << std::endl;
    std::cout << "Player mana before: " << player->get_mana() << std::endl;
    std::cout << "Enhancement points available: " << SpellEnhancement::get_enhancement_points() << std::endl;

    bool spellCast = false;

    if (spell->requires_target() && targetX != -1 && targetY != -1) {
        // Для заклинаний с целью проверяем координаты
        if (targetX < 0 || targetX >= board->get_length() ||
            targetY < 0 || targetY >= board->get_width()) {
            std::cout << "Invalid target coordinates!" << std::endl;
            return;
        }
        spellCast = spell->cast(*player, *board, enemies, allies, *attackTower, *spawnTower, targetX, targetY);
    } else if (!spell->requires_target()) {
        // Для мгновенных заклинаний координаты не важны
        spellCast = spell->cast(*player, *board, enemies, allies, *attackTower, *spawnTower, 0, 0);
    }

    if (spellCast) {
        std::cout << "Spell cast successfully!" << std::endl;

        // РАТИМ МАНУ ТОЛЬКО ПРИ УСПЕХЕ
        player->set_mana(player->get_mana() - spell->get_mana_cost());
        std::cout << "Mana spent: " << spell->get_mana_cost() << ", Mana after: " << player->get_mana() << std::endl;

        playerHand->removeSpell(spellIndex);

        playerTurn = false;
        castingSpell = false;
        selectedSpellIndex = -1;

        updateBoardPositions();
    } else {
        std::cout << "Spell cast failed! Mana unchanged: " << player->get_mana() << std::endl;
    }

    std::cout << "=== SPELL CAST END ===" << std::endl;

    checkWinCondition();
    checkLossCondition();
}

void GameCycle::processPlayerInput(char input) {
    if (!playerTurn || gameOver || !player || !board) return;

    int old_x, old_y;
    player->get_coords(old_x, old_y);

    switch (input) {
        case 'W':
        case 'A':
        case 'S':
        case 'D':
            player->move(input, *board, enemies, *spawnTower, *attackTower);
            break;
        case 'Q':
            player->change_attack_mode();
            std::cout << "Attack mode changed to: "
                      << (player->get_mode() == fight_mode::MELEE ? "MELEE" : "RANGE")
                      << std::endl;
            break;
        case ' ': // Пропуск хода
            playerTurn = false;
            std::cout << "Turn skipped" << std::endl;
            break;
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        {
            int spell_index = input - '1';
            std::cout << "DEBUG: Spell key pressed - index: " << spell_index << std::endl;

            if (spell_index < playerHand->getSpellCount()) {
                Spell* spell = playerHand->getSpell(spell_index);
                if (spell) {
                    std::cout << "DEBUG: Spell found: " << spell->get_name()
                              << ", Cost: " << spell->get_mana_cost()
                              << ", Player mana: " << player->get_mana() << std::endl;

                    if (player->get_mana() >= spell->get_mana_cost()) {
                        if (spell->requires_target()) {
                            castingSpell = true;
                            selectedSpellIndex = spell_index;
                            std::cout << "Select target for " << spell->get_name()
                                      << " (Cost: " << spell->get_mana_cost() << " mana)" << std::endl;
                        } else {
                            std::cout << "DEBUG: Casting instant spell: " << spell->get_name() << std::endl;
                            processSpellCast(spell_index);
                        }
                    } else {
                        std::cout << "Not enough mana! Need: " << spell->get_mana_cost()
                                  << ", Have: " << player->get_mana() << std::endl;
                    }
                }
            }
            break;
        }
        default:
            std::cout << "DEBUG: Unknown input: " << input << std::endl;
            return;
    }

    // Обновление позиции на доске
    int new_x, new_y;
    player->get_coords(new_x, new_y);
    if (old_x != new_x || old_y != new_y) {
        board->get_cell(old_x, old_y).set_player(false);
        board->get_cell(new_x, new_y).set_player(true);
        std::cout << "Player moved to: " << new_x << ", " << new_y << std::endl;
    }

    if (!player->get_can_act()) {
        playerTurn = false;
        std::cout << "Player turn ended" << std::endl;
    }

    checkWinCondition();
    checkLossCondition();
}

void GameCycle::processEntityTurn() {
    if (playerTurn || gameOver || !board || !player) return;

    std::cout << "=== ENEMY TURN START ===" << std::endl;
    std::cout << "Enemies: " << enemies.size() << ", Allies: " << allies.size() << std::endl;

    // Атака башни
    if (!attackTower->is_death()) {
        std::cout << "Attack tower attacking..." << std::endl;
        attackTower->attack_in_range(*player, allies);
    } else {
        std::cout << "Attack tower is dead" << std::endl;
    }

    // Спавн врагов
    if (!spawnTower->is_death()) {
        int spawnX, spawnY;
        if (spawnTower->spawn_enemy(spawnX, spawnY, *board)) {
            enemies.emplace_back(spawnX, spawnY, currentLevel);
            board->get_cell(spawnX, spawnY).set_enemy(true);
            std::cout << "NEW ENEMY spawned at (" << spawnX << ", " << spawnY << ")" << std::endl;
        }
    }

    // Движение врагов
    for (auto& enemy : enemies) {
        if (!enemy.is_dead()) {
            int oldX, oldY;
            enemy.get_coords(oldX, oldY);
            enemy.move(*player, *board, allies);
            int newX, newY;
            enemy.get_coords(newX, newY);
            if (oldX != newX || oldY != newY) {
                std::cout << "Enemy moved from (" << oldX << "," << oldY << ") to (" << newX << "," << newY << ")" << std::endl;
            }
        }
    }

    // Движение союзников
    for (auto& ally : allies) {
        if (!ally.is_dead()) {
            int oldX, oldY;
            ally.get_coords(oldX, oldY);
            ally.move(*board, enemies, *spawnTower, *attackTower);
            int newX, newY;
            ally.get_coords(newX, newY);
            if (oldX != newX || oldY != newY) {
                std::cout << "Ally moved from (" << oldX << "," << oldY << ") to (" << newX << "," << newY << ")" << std::endl;
            }
        }
    }

    updateBoardPositions();
    cleanupDeadEntities();

    // Добавление заклинания
    int oldSpellCount = playerHand->getSpellCount();
    playerHand->AddRandomSpell();
    std::cout << "Spell added. Hand size: " << oldSpellCount << " -> " << playerHand->getSpellCount() << std::endl;

    resetTurnStates();

    // ВОССТАНАВЛИВАЕМ МАНУ ПЕРЕД началом хода игрока
    int oldMana = player->get_mana();
    player->set_mana(std::min(player->get_mana() + 2, player->get_max_mana()));
    std::cout << "Mana restored for player turn: " << oldMana << " -> " << player->get_mana() << std::endl;

    playerTurn = true;
    std::cout << "=== ENEMY TURN END ===" << std::endl;
}

void GameCycle::cleanupDeadEntities() {
    if (!board) return;

    // Удаляем мертвых врагов
    auto enemyIt = enemies.begin();
    while (enemyIt != enemies.end()) {
        if (enemyIt->is_dead()) {
            int x, y;
            enemyIt->get_coords(x, y);
            board->get_cell(x, y).set_enemy(false);
            enemyIt = enemies.erase(enemyIt);
            std::cout << "DEBUG: Dead enemy removed" << std::endl;
        } else {
            ++enemyIt;
        }
    }

    // Удаляем мертвых союзников
    auto allyIt = allies.begin();
    while (allyIt != allies.end()) {
        if (allyIt->is_dead()) {
            int x, y;
            allyIt->get_coords(x, y);
            board->get_cell(x, y).set_ally(false);
            allyIt = allies.erase(allyIt);
            std::cout << "DEBUG: Dead ally removed" << std::endl;
        } else {
            ++allyIt;
        }
    }

    // Убираем разрушенные башни с доски
    if (spawnTower && spawnTower->is_death()) {
        int x, y;
        spawnTower->get_coords(x, y);
        board->get_cell(x, y).set_tower(false);
        std::cout << "DEBUG: Dead spawn tower removed from board" << std::endl;
    }

    if (attackTower && attackTower->is_death()) {
        int x, y;
        attackTower->get_coords(x, y);
        board->get_cell(x, y).set_attack_tower(false);
        std::cout << "DEBUG: Dead attack tower removed from board" << std::endl;
    }
}

void GameCycle::checkWinCondition() {
    if (!spawnTower || !attackTower || !player) {
        return;
    }

    if (spawnTower->is_death() && attackTower->is_death()) {
        victory = true;
        std::cout << "VICTORY! All enemy towers destroyed! Preparing next level..." << std::endl;
        victory_text.setString("VICTORY! Level " + std::to_string(currentLevel) + " Completed!");

        logVictoryStats();
    }
}

void GameCycle::logVictoryStats() {
    std::cout << "=== LEVEL " << currentLevel << " COMPLETED ===" << std::endl;
    std::cout << "Player final HP: " << player->get_hp() << "/20" << std::endl;
    std::cout << "Player mana: " << player->get_mana() << "/" << player->get_max_mana() << std::endl;
    std::cout << "Spells in hand: " << playerHand->getSpellCount() << std::endl;
    std::cout << "Enemies defeated: " << enemies.size() << std::endl;
    std::cout << "Allies remaining: " << allies.size() << std::endl;

    // Подсчет живых союзников
    int aliveAllies = 0;
    for (const auto& ally : allies) {
        if (!ally.is_dead()) {
            aliveAllies++;
        }
    }
    std::cout << "Allies alive: " << aliveAllies << std::endl;
}

void GameCycle::checkLossCondition() {
    if (player->is_dead()) {
        gameOver = true;
        victory = false;
        std::cout << "GAME OVER! Player died." << std::endl;
    }
}

void GameCycle::showLevelUpScreen(sf::RenderWindow& window) {
    if (levelUpSystem.getAvailablePoints() == 0) {
        std::cout << "No level up points available" << std::endl;
        return;
    }

    std::cout << "=== LEVEL UP SCREEN ===" << std::endl;
    std::cout << "Available points: " << levelUpSystem.getAvailablePoints() << std::endl;

    // Сохраняем текущие характеристики до улучшений для отладки
    int oldHP = player->get_max_hp();
    int oldMana = player->get_max_mana();
    int oldDamage = player->get_damage();

    // Создаем UI элементы
    sf::Text titleText(font);
    titleText.setString("LEVEL UP! Available Points: " + std::to_string(levelUpSystem.getAvailablePoints()));
    titleText.setCharacterSize(30);
    titleText.setFillColor(sf::Color::Yellow);
    titleText.setPosition({200.0f, 100.0f});

    sf::Text option1Text(font);
    option1Text.setString("1 - Improve Health (+5 Max HP)");
    option1Text.setCharacterSize(24);
    option1Text.setFillColor(sf::Color::Green);
    option1Text.setPosition({250.0f, 200.0f});

    sf::Text option2Text(font);
    option2Text.setString("2 - Improve Mana (+5 Max Mana)");
    option2Text.setCharacterSize(24);
    option2Text.setFillColor(sf::Color::Cyan);
    option2Text.setPosition({250.0f, 250.0f});

    sf::Text option3Text(font);
    option3Text.setString("3 - Improve Damage (+1 Damage)");
    option3Text.setCharacterSize(24);
    option3Text.setFillColor(sf::Color::Red);
    option3Text.setPosition({250.0f, 300.0f});

    // ОБНОВЛЯЕМ текст текущих характеристик ПЕРЕД циклом
    sf::Text currentStatsText(font);
    currentStatsText.setString("Current Stats - HP: " + std::to_string(player->get_max_hp()) +
                             ", Mana: " + std::to_string(player->get_max_mana()) +
                             ", Damage: " + std::to_string(player->get_damage()));
    currentStatsText.setCharacterSize(20);
    currentStatsText.setFillColor(sf::Color::White);
    currentStatsText.setPosition({200.0f, 350.0f});

    sf::Text continueText(font);
    continueText.setString("SPACE - Continue to Level " + std::to_string(currentLevel));
    continueText.setCharacterSize(24);
    continueText.setFillColor(sf::Color::White);
    continueText.setPosition({250.0f, 400.0f});

    bool choosing = true;
    while (choosing && window.isOpen() && levelUpSystem.getAvailablePoints() > 0) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
                choosing = false;
            }
            else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                if (keyPressed->scancode == sf::Keyboard::Scancode::Num1) {
                    if (levelUpSystem.improvePlayerHealth(*player)) {
                        std::cout << "DEBUG: HP improved from " << oldHP << " to " << player->get_max_hp() << std::endl;
                        // СРАЗУ ОБНОВЛЯЕМ текст после улучшения
                        currentStatsText.setString("Current Stats - HP: " + std::to_string(player->get_max_hp()) +
                                                 ", Mana: " + std::to_string(player->get_max_mana()) +
                                                 ", Damage: " + std::to_string(player->get_damage()));
                        titleText.setString("LEVEL UP! Available Points: " + std::to_string(levelUpSystem.getAvailablePoints()));
                    }
                }
                else if (keyPressed->scancode == sf::Keyboard::Scancode::Num2) {
                    if (levelUpSystem.improvePlayerMana(*player)) {
                        std::cout << "DEBUG: Mana improved from " << oldMana << " to " << player->get_max_mana() << std::endl;
                        currentStatsText.setString("Current Stats - HP: " + std::to_string(player->get_max_hp()) +
                                                 ", Mana: " + std::to_string(player->get_max_mana()) +
                                                 ", Damage: " + std::to_string(player->get_damage()));
                        titleText.setString("LEVEL UP! Available Points: " + std::to_string(levelUpSystem.getAvailablePoints()));
                    }
                }
                else if (keyPressed->scancode == sf::Keyboard::Scancode::Num3) {
                    if (levelUpSystem.improvePlayerDamage(*player)) {
                        std::cout << "DEBUG: Damage improved from " << oldDamage << " to " << player->get_damage() << std::endl;
                        currentStatsText.setString("Current Stats - HP: " + std::to_string(player->get_max_hp()) +
                                                 ", Mana: " + std::to_string(player->get_max_mana()) +
                                                 ", Damage: " + std::to_string(player->get_damage()));
                        titleText.setString("LEVEL UP! Available Points: " + std::to_string(levelUpSystem.getAvailablePoints()));
                    }
                }
                else if (keyPressed->scancode == sf::Keyboard::Scancode::Space) {
                    choosing = false;
                    std::cout << "Level up choices completed. Remaining points: " << levelUpSystem.getAvailablePoints()
                              << ", Final HP: " << player->get_max_hp()
                              << ", Final Mana: " << player->get_max_mana()
                              << ", Final Damage: " << player->get_damage() << std::endl;
                }
            }
        }

        window.clear(sf::Color(30, 30, 60));

        window.draw(titleText);
        window.draw(option1Text);
        window.draw(option2Text);
        window.draw(option3Text);
        window.draw(currentStatsText);

        if (levelUpSystem.getAvailablePoints() > 0) {
            sf::Text pointsRemainingText(font);
            pointsRemainingText.setString("You have " + std::to_string(levelUpSystem.getAvailablePoints()) + " points remaining");
            pointsRemainingText.setCharacterSize(20);
            pointsRemainingText.setFillColor(sf::Color::Yellow);
            pointsRemainingText.setPosition({250.0f, 450.0f});
            window.draw(pointsRemainingText);
        } else {
            window.draw(continueText);
        }

        window.display();
    }

    std::cout << "Level up completed. Final stats - Max HP: " << player->get_max_hp()
              << ", Current HP: " << player->get_hp()
              << ", Max Mana: " << player->get_max_mana()
              << ", Current Mana: " << player->get_mana()
              << ", Damage: " << player->get_damage() << std::endl;
}

void GameCycle::setupNextLevel() {
    std::cout << "=== SETUP NEXT LEVEL ===" << std::endl;
    std::cout << "Current level: " << currentLevel << " -> " << (currentLevel + 1) << std::endl;

    currentLevel++;

    levelUpSystem.addLevelUpPoints(2);
    std::cout << "Level up points available: " << levelUpSystem.getAvailablePoints() << std::endl;

    if (levelUpSystem.getAvailablePoints() > 0) {
        sf::RenderWindow levelUpWindow(sf::VideoMode({800, 600}), "Level Up! - Choose Improvements");
        showLevelUpScreen(levelUpWindow);
    }

    std::cout << "Before restoration - Player HP: " << player->get_hp() << "/" << player->get_max_hp() << std::endl;

    if (player && !player->is_dead()) {
        int oldHP = player->get_hp();
        player->set_hp(player->get_max_hp());
        std::cout << "HP restored: " << oldHP << " -> " << player->get_hp()
                  << " (Max HP: " << player->get_max_hp() << ")" << std::endl;
    }

    if (player) {
        player->set_mana(player->get_max_mana());
        std::cout << "Mana restored to maximum: " << player->get_mana()
                  << " (Max Mana: " << player->get_max_mana() << ")" << std::endl;
    }

    // Удаление половины заклинаний
    int oldSpellCount = playerHand->getSpellCount();
    int spellsToRemove = oldSpellCount / 2;
    std::cout << "Removing " << spellsToRemove << " of " << oldSpellCount << " spells" << std::endl;

    std::random_device rd;
    std::mt19937 gen(rd());
    for (int i = 0; i < spellsToRemove && playerHand->getSpellCount() > 0; ++i) {
        std::uniform_int_distribution<> dist(0, playerHand->getSpellCount() - 1);
        int randomIndex = dist(gen);
        playerHand->removeSpell(randomIndex);
    }

    int newLength = 10 + currentLevel;
    int newWidth = 10 + currentLevel;

    if (currentLevel <= 10) {
        newLength = std::min(newLength, 20);
        newWidth = std::min(newWidth, 20);
    } else {
        newLength = std::min(newLength, 25);
        newWidth = std::min(newWidth, 25);
    }

    std::cout << "DEBUG: New board size: " << newLength << "x" << newWidth << std::endl;

    restartGameForNextLevel(newLength, newWidth);

    std::cout << "=== LEVEL " << currentLevel << " READY ===" << std::endl;
}

void GameCycle::restartGameForNextLevel(int length, int width) {
    board = std::make_unique<Board>(length, width);

    spawnTower = std::make_unique<EnemyTower>(length - 1, width - 1, currentLevel);

    int a_towerX, a_towerY;
    std::srand(std::time(nullptr));
    int attempts = 0;
    bool position_found = false;

    while (attempts < length * width && !position_found) {
        a_towerX = std::rand() % length;
        a_towerY = std::rand() % width;

        if ((a_towerX != 0 || a_towerY != 0) &&
            (a_towerX != length - 1 || a_towerY != width - 1) &&
            board->can_move_to(a_towerX, a_towerY) &&
            !board->get_cell(a_towerX, a_towerY).is_obstacle_here() &&
            !board->get_cell(a_towerX, a_towerY).is_player_here() &&
            !board->get_cell(a_towerX, a_towerY).is_enemy_here()) {
            position_found = true;
            }
        attempts++;
    }

    if (!position_found) {
        a_towerX = length / 2;
        a_towerY = width / 2;
    }

    attackTower = std::make_unique<EnemyAttackTower>(a_towerX, a_towerY, currentLevel);

    enemies.clear();
    allies.clear();

    initializeLevel();

    // Сбрасываем флаги
    playerTurn = true;
    castingSpell = false;
    selectedSpellIndex = -1;
    gameOver = false;
    victory = false;

    std::cout << "DEBUG: Game restarted for level " << currentLevel
              << ", Player HP: " << player->get_hp() << "/" << player->get_max_hp() << std::endl;
}

void GameCycle::resetTurnStates() {
    if (player) {
        player->reset_step();
    }
    for (auto& enemy : enemies) {
        enemy.reset_turn();
    }
    for (auto& ally : allies) {
        ally.reset_turn();
    }
}

void GameCycle::saveGame(const std::string& filename) const {
    try {

        std::ofstream file(filename, std::ios::binary);
        if (!file.is_open()) {
            throw std::runtime_error("Cannot open file for writing: " + filename);
        }
        if (!player || !board || !spawnTower || !attackTower || !playerHand) {
            throw std::runtime_error("Game state is inconsistent - missing critical objects");
        }
        std::cout << "Saving game to: " << filename << std::endl;
        const char* signature = "OOPG";
        file.write(signature, 4);
        int file_version = 1;
        file.write(reinterpret_cast<const char*>(&file_version), sizeof(file_version));
        file.write(reinterpret_cast<const char*>(&currentLevel), sizeof(currentLevel));
        file.write(reinterpret_cast<const char*>(&playerTurn), sizeof(playerTurn));
        file.write(reinterpret_cast<const char*>(&gameOver), sizeof(gameOver));
        file.write(reinterpret_cast<const char*>(&victory), sizeof(victory));
        file.write(reinterpret_cast<const char*>(&castingSpell), sizeof(castingSpell));
        file.write(reinterpret_cast<const char*>(&selectedSpellIndex), sizeof(selectedSpellIndex));
        if (player) {
            int player_x, player_y;
            player->get_coords(player_x, player_y);
            file.write(reinterpret_cast<const char*>(&player_x), sizeof(player_x));
            file.write(reinterpret_cast<const char*>(&player_y), sizeof(player_y));

            int player_hp = player->get_hp();
            int player_max_hp = player->get_max_hp();
            int player_mana = player->get_mana();
            int player_max_mana = player->get_max_mana();
            fight_mode player_mode = player->get_mode();

            file.write(reinterpret_cast<const char*>(&player_hp), sizeof(player_hp));
            file.write(reinterpret_cast<const char*>(&player_max_hp), sizeof(player_max_hp));
            file.write(reinterpret_cast<const char*>(&player_mana), sizeof(player_mana));
            file.write(reinterpret_cast<const char*>(&player_max_mana), sizeof(player_max_mana));
            file.write(reinterpret_cast<const char*>(&player_mode), sizeof(player_mode));
        } else {
            // Заполнитель если игрока нет
            int empty_data = -1;
            file.write(reinterpret_cast<const char*>(&empty_data), sizeof(empty_data));
            file.write(reinterpret_cast<const char*>(&empty_data), sizeof(empty_data));
        }

        // 3. Сохраняем информацию о башнях
        if (spawnTower) {
            int spawn_x, spawn_y;
            spawnTower->get_coords(spawn_x, spawn_y);
            int spawn_hp = spawnTower->get_hp();
            bool spawn_dead = spawnTower->is_death();

            file.write(reinterpret_cast<const char*>(&spawn_x), sizeof(spawn_x));
            file.write(reinterpret_cast<const char*>(&spawn_y), sizeof(spawn_y));
            file.write(reinterpret_cast<const char*>(&spawn_hp), sizeof(spawn_hp));
            file.write(reinterpret_cast<const char*>(&spawn_dead), sizeof(spawn_dead));
        }

        if (attackTower) {
            int attack_x, attack_y;
            attackTower->get_coords(attack_x, attack_y);
            int attack_hp = attackTower->get_hp();
            bool attack_dead = attackTower->is_death();

            file.write(reinterpret_cast<const char*>(&attack_x), sizeof(attack_x));
            file.write(reinterpret_cast<const char*>(&attack_y), sizeof(attack_y));
            file.write(reinterpret_cast<const char*>(&attack_hp), sizeof(attack_hp));
            file.write(reinterpret_cast<const char*>(&attack_dead), sizeof(attack_dead));
        }

        // 4. Сохраняем врагов
        size_t enemy_count = enemies.size();
        file.write(reinterpret_cast<const char*>(&enemy_count), sizeof(enemy_count));

        for (const auto& enemy : enemies) {
            if (!enemy.is_dead()) {
                int enemy_x, enemy_y;
                enemy.get_coords(enemy_x, enemy_y);
                int enemy_hp = enemy.get_hp();

                file.write(reinterpret_cast<const char*>(&enemy_x), sizeof(enemy_x));
                file.write(reinterpret_cast<const char*>(&enemy_y), sizeof(enemy_y));
                file.write(reinterpret_cast<const char*>(&enemy_hp), sizeof(enemy_hp));
            }
        }

        // 5. Сохраняем союзников
        size_t ally_count = allies.size();
        file.write(reinterpret_cast<const char*>(&ally_count), sizeof(ally_count));

        for (const auto& ally : allies) {
            if (!ally.is_dead()) {
                int ally_x, ally_y;
                ally.get_coords(ally_x, ally_y);
                int ally_hp = ally.get_hp();

                file.write(reinterpret_cast<const char*>(&ally_x), sizeof(ally_x));
                file.write(reinterpret_cast<const char*>(&ally_y), sizeof(ally_y));
                file.write(reinterpret_cast<const char*>(&ally_hp), sizeof(ally_hp));
            }
        }

        // 6. Сохраняем заклинания в руке
        if (playerHand) {
            int spell_count = playerHand->getSpellCount();
            file.write(reinterpret_cast<const char*>(&spell_count), sizeof(spell_count));

            std::cout << "Saving " << spell_count << " spells..." << std::endl;

            for (int i = 0; i < spell_count; ++i) {
                Spell* spell = playerHand->getSpell(i);
                if (spell) {
                    // Сохраняем тип заклинания и стоимость маны
                    SpellType spell_type = UNKNOWN_SPELL;
                    std::string spell_name = spell->get_name();

                    // Определяем тип
                    if (spell_name.find("Direct") != std::string::npos) spell_type = DIRECT_DAMAGE;
                    else if (spell_name.find("Splash") != std::string::npos) spell_type = SPLASH_DAMAGE;
                    else if (spell_name.find("Trap") != std::string::npos) spell_type = TRAP;
                    else if (spell_name.find("Summon") != std::string::npos) spell_type = SUMMON_ALLY;
                    else if (spell_name.find("Enhancement") != std::string::npos) spell_type = ENHANCEMENT;

                    int mana_cost = spell->get_mana_cost();

                    file.write(reinterpret_cast<const char*>(&spell_type), sizeof(spell_type));
                    file.write(reinterpret_cast<const char*>(&mana_cost), sizeof(mana_cost));

                    std::cout << "  - " << spell_name << " (Type: " << spell_type
                              << ", Cost: " << mana_cost << ")" << std::endl;
                }
            }
        } else {
            int spell_count = 0;
            file.write(reinterpret_cast<const char*>(&spell_count), sizeof(spell_count));
        }

        if (!file) {
            throw std::runtime_error("Failed to write data to file: " + filename);
        }

        std::cout << "Game saved successfully! Level: " << currentLevel
                  << ", Player HP: " << (player ? player->get_hp() : 0)
                  << ", Enemies: " << enemies.size() << std::endl;

    } catch (const std::exception& e) {
        throw std::runtime_error("Save game error: " + std::string(e.what()));
    }
}

void GameCycle::loadGame(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file for reading: " + filename);
    }

    enemies.clear();
    allies.clear();
    if (playerHand) {
        playerHand->clearSpells();
    }

    int oldLevel = currentLevel;
    bool oldPlayerTurn = playerTurn;
    try {
        std::cout << "Loading game from: " << filename << std::endl;
        char signature[5] = {0};
        file.read(signature, 4);
        if (std::string(signature) != "OOPG") {
            throw std::runtime_error("Invalid save file format - not an OOP Game save file");
        }

        int file_version;
        file.read(reinterpret_cast<char*>(&file_version), sizeof(file_version));
        if (file_version != 1) {
            throw std::runtime_error("Incompatible save file version: " + std::to_string(file_version));
        }
        // 1. Загружаем базовую информацию о состоянии игры
        file.read(reinterpret_cast<char*>(&currentLevel), sizeof(currentLevel));
        file.read(reinterpret_cast<char*>(&playerTurn), sizeof(playerTurn));
        file.read(reinterpret_cast<char*>(&gameOver), sizeof(gameOver));
        file.read(reinterpret_cast<char*>(&victory), sizeof(victory));
        file.read(reinterpret_cast<char*>(&castingSpell), sizeof(castingSpell));
        file.read(reinterpret_cast<char*>(&selectedSpellIndex), sizeof(selectedSpellIndex));

        // 2. Загружаем информацию об игроке
        if (player) {
            int player_x, player_y;
            file.read(reinterpret_cast<char*>(&player_x), sizeof(player_x));
            file.read(reinterpret_cast<char*>(&player_y), sizeof(player_y));
            player->set_position(player_x, player_y);

            if (player_x < 0 || player_y < 0) {
                std::cerr << "WARNING: Invalid player coordinates, resetting to (0,0)" << std::endl;
                player_x = 0;
                player_y = 0;
                player->set_position(0, 0);
            }

            int player_hp, player_max_hp, player_mana, player_max_mana;
            fight_mode player_mode;

            file.read(reinterpret_cast<char*>(&player_hp), sizeof(player_hp));
            file.read(reinterpret_cast<char*>(&player_max_hp), sizeof(player_max_hp));
            file.read(reinterpret_cast<char*>(&player_mana), sizeof(player_mana));
            file.read(reinterpret_cast<char*>(&player_max_mana), sizeof(player_max_mana));
            file.read(reinterpret_cast<char*>(&player_mode), sizeof(player_mode));

            // Валидация загруженных данных
            if (player_hp < 1) player_hp = 1;
            if (player_max_hp < 10) player_max_hp = 10; // минимальное значение
            if (player_mana < 0) player_mana = 0;
            if (player_max_mana < 10) player_max_mana = 10;

            // Восстанавливаем состояние игрока
            player->set_max_hp(player_max_hp);
            player->set_hp(player_hp);
            player->set_max_mana(player_max_mana);
            player->set_mana(player_mana);
            if (player->get_mode() != player_mode) {
                player->change_attack_mode();
            }
        } else {
            // Пропускаем данные если игрока нет
            int empty_data;
            file.read(reinterpret_cast<char*>(&empty_data), sizeof(empty_data));
            file.read(reinterpret_cast<char*>(&empty_data), sizeof(empty_data));
            file.read(reinterpret_cast<char*>(&empty_data), sizeof(empty_data));
            file.read(reinterpret_cast<char*>(&empty_data), sizeof(empty_data));
            file.read(reinterpret_cast<char*>(&empty_data), sizeof(empty_data));
            file.read(reinterpret_cast<char*>(&empty_data), sizeof(empty_data));
        }

        // 3. Загружаем информацию о башнях
        if (spawnTower) {
            int spawn_x, spawn_y, spawn_hp;
            bool spawn_dead;

            file.read(reinterpret_cast<char*>(&spawn_x), sizeof(spawn_x));
            file.read(reinterpret_cast<char*>(&spawn_y), sizeof(spawn_y));
            file.read(reinterpret_cast<char*>(&spawn_hp), sizeof(spawn_hp));
            file.read(reinterpret_cast<char*>(&spawn_dead), sizeof(spawn_dead));

            // Восстанавливаем HP башни-спавнера restored
            spawnTower->set_hp(spawn_hp);
            spawnTower->set_position(spawn_x, spawn_y);
            std::cout << "Spawn Tower: HP = " << spawn_hp << ", Position = (" << spawn_x << ", " << spawn_y << ")" << std::endl;
        }

        if (attackTower) {
            int attack_x, attack_y, attack_hp;
            bool attack_dead;

            file.read(reinterpret_cast<char*>(&attack_x), sizeof(attack_x));
            file.read(reinterpret_cast<char*>(&attack_y), sizeof(attack_y));
            file.read(reinterpret_cast<char*>(&attack_hp), sizeof(attack_hp));
            file.read(reinterpret_cast<char*>(&attack_dead), sizeof(attack_dead));

            // Восстанавливаем HP атакующей башни
            attackTower->set_hp(attack_hp);
            attackTower->set_position(attack_x, attack_y);
            std::cout << "Attack Tower: HP = " << attack_hp << ", Position = (" << attack_x << ", " << attack_y << ")" << std::endl;
        }

        // 4. Загружаем врагов
        size_t enemy_count;
        if (!file.read(reinterpret_cast<char*>(&enemy_count), sizeof(enemy_count))) {
            throw std::runtime_error("Failed to read enemy count");
        }
        if (enemy_count > 100) {
            std::cerr << "WARNING: Corrupted enemy count (" << enemy_count << "), limiting to 10" << std::endl;
            enemy_count = 10;
        }
        enemies.clear(); // Очищаем текущих врагов

        std::cout << "Loading " << enemy_count << " enemies..." << std::endl;

        for (size_t i = 0; i < enemy_count; ++i) {
            int enemy_x, enemy_y, enemy_hp;
            if (!file.read(reinterpret_cast<char*>(&enemy_x), sizeof(enemy_x)) ||
                !file.read(reinterpret_cast<char*>(&enemy_y), sizeof(enemy_y)) ||
                !file.read(reinterpret_cast<char*>(&enemy_hp), sizeof(enemy_hp))) {
                std::cerr << "WARNING: Failed to read enemy data, stopping enemy loading" << std::endl;
                break;
            }
            if (enemy_x < 0 || enemy_y < 0) {
                std::cerr << "WARNING: Invalid enemy coordinates, skipping enemy" << std::endl;
                continue;
            }

            if (enemy_hp <= 0) {
                enemy_hp = 5 + currentLevel; // Разумное значение по умолчанию
            }
            // Создаем врага на нужной позиции с нужным HP
            enemies.emplace_back(enemy_x, enemy_y, currentLevel);

            // Устанавливаем HP врага
            Enemy& enemy = enemies.back();
            enemy.set_hp(enemy_hp);

            std::cout << "  - Enemy at (" << enemy_x << ", " << enemy_y << ") HP = " << enemy_hp << std::endl;
        }

        // 5. Загружаем союзников
        size_t ally_count;
        file.read(reinterpret_cast<char*>(&ally_count), sizeof(ally_count));

        // ДОБАВЬ ВАЛИДАЦИЮ КОЛИЧЕСТВА СОЮЗНИКОВ
        if (ally_count > 20) {
            std::cerr << "WARNING: Corrupted ally count (" << ally_count << "), limiting to 5" << std::endl;
            ally_count = 5;
        }

        allies.clear();

        std::cout << "Loading " << ally_count << " allies..." << std::endl;

        for (size_t i = 0; i < ally_count; ++i) {
            int ally_x, ally_y, ally_hp;

            // ДОБАВЬ ПРОВЕРКУ ЧТЕНИЯ ДАННЫХ
            if (!file.read(reinterpret_cast<char*>(&ally_x), sizeof(ally_x)) ||
                !file.read(reinterpret_cast<char*>(&ally_y), sizeof(ally_y)) ||
                !file.read(reinterpret_cast<char*>(&ally_hp), sizeof(ally_hp))) {
                std::cerr << "WARNING: Failed to read ally data, stopping ally loading" << std::endl;
                break;
                }

            // ДОБАВЬ ВАЛИДАЦИЮ ДАННЫХ
            if (ally_x < 0 || ally_y < 0) {
                std::cerr << "WARNING: Invalid ally coordinates, skipping ally" << std::endl;
                continue;
            }

            if (ally_hp <= 0 || ally_hp > 50) {
                std::cerr << "WARNING: Invalid ally HP, setting to default" << std::endl;
                ally_hp = 10;
            }

            // Создаем союзника на нужной позиции с нужным HP
            allies.emplace_back(ally_x, ally_y);

            // Устанавливаем HP союзника
            Ally& ally = allies.back();
            ally.set_hp(ally_hp);

            std::cout << "  - Ally at (" << ally_x << ", " << ally_y << ") HP = " << ally_hp << std::endl;
        }

        // 6. Загружаем заклинания в руке
        if (playerHand) {
            int spell_count;
            file.read(reinterpret_cast<char*>(&spell_count), sizeof(spell_count));

            playerHand->clearSpells();

            std::cout << "Loading " << spell_count << " spells..." << std::endl;

            for (int i = 0; i < spell_count; ++i) {
                SpellType spell_type;
                int mana_cost;

                file.read(reinterpret_cast<char*>(&spell_type), sizeof(spell_type));
                file.read(reinterpret_cast<char*>(&mana_cost), sizeof(mana_cost));

                // Создаем заклинание по типу
                std::unique_ptr<Spell> new_spell;
                std::string spell_name;

                switch (spell_type) {
                    case DIRECT_DAMAGE:
                        new_spell = std::make_unique<SpellDirectDamage>();
                        spell_name = "Direct Damage";
                        break;
                    case SPLASH_DAMAGE:
                        new_spell = std::make_unique<SpellSplash>();
                        spell_name = "Splash Damage";
                        break;
                    case TRAP:
                        new_spell = std::make_unique<SpellTrap>();
                        spell_name = "Trap";
                        break;
                    case SUMMON_ALLY:
                        new_spell = std::make_unique<SpellSummon>();
                        spell_name = "Summon Ally";
                        break;
                    case ENHANCEMENT:
                        new_spell = std::make_unique<SpellEnhancement>();
                        spell_name = "Enhancement";
                        break;
                    default:
                        std::cerr << "Unknown spell type: " << spell_type << std::endl;
                        continue;
                }

                if (playerHand->addSpell(std::move(new_spell))) {
                    std::cout << "  - " << spell_name << " (Cost: " << mana_cost << " mana)" << std::endl;
                } else {
                    std::cerr << "  - Failed to add " << spell_name << " (hand full)" << std::endl;
                }
            }
        } else {
            int spell_count;
            file.read(reinterpret_cast<char*>(&spell_count), sizeof(spell_count));
            std::cout << "Skipping " << spell_count << " spells (no playerHand)" << std::endl;
        }

        // Проверяем успешность чтения
        if (!file) {
            throw std::runtime_error("Failed to read data from file: " + filename);
        }

        // Проверяем достигли ли конца файла
        if (file.peek() != EOF) {
            std::cout << "Warning: Extra data in save file" << std::endl;
        }

        // Обновляем игровое поле
        updateBoardPositions();
        int player_x, player_y;
        player->get_coords(player_x,player_y);
        // Обновляем UI текст победы
        victory_text.setString("VICTORY! Level " + std::to_string(currentLevel) + " Completed!");

        std::cout << "=== GAME LOADED SUCCESSFULLY ===" << std::endl;
        std::cout << "Level: " << currentLevel << std::endl;
        std::cout << "Player: HP = " << (player ? player->get_hp() : 0)
                  << ", Mana = " << (player ? player->get_mana() : 0)
                  << ", Position = (" << (player ? player_x : 0) << ", " << (player ? player_y : 0) << ")" << std::endl;
        std::cout << "Enemies: " << enemies.size() << std::endl;
        std::cout << "Allies: " << allies.size() << std::endl;
        std::cout << "Spells: " << (playerHand ? playerHand->getSpellCount() : 0) << std::endl;
        std::cout << "Player Turn: " << (playerTurn ? "Yes" : "No") << std::endl;
        std::cout << "Game Over: " << (gameOver ? "Yes" : "No") << std::endl;
        std::cout << "Victory: " << (victory ? "Yes" : "No") << std::endl;

    } catch (const std::exception& e) {
        currentLevel = oldLevel;
        playerTurn = oldPlayerTurn;
        gameOver = false;
        victory = false;

        throw std::runtime_error("Load game error: " + std::string(e.what()));
    }
}

void GameCycle::autoSave() {
    try {
        if (gameOver) return;

        std::cout << "Performing autosave..." << std::endl;
        saveGame("autosave.dat");
        std::cout << "Autosave completed successfully!" << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Autosave failed: " << e.what() << std::endl;
    }
}
