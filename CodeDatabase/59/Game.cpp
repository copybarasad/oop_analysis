#include "Game.h"
#include "Being.h"
#include <cmath>
#include <cstdlib>
#include <format>
#include <iostream> 

#include "EnemyTower.h"
#include "TrapSpell.h"
#include "Constants.h"

Game::Game(GameData& data, Loader& loader)
        : player(data.player), enemies(data.enemies),
          field(data.field), state(GameState::Going),
          spellShop(data.spellShop), playerHand(data.playerHand),
          towers(data.towers), active_traps(data.active_traps),
          loader(loader), game_data(data) {}

void Game::log(const std::string& msg) {
    message_log.push_back(msg);
}

const std::vector<std::string>& Game::getMessageLog() const {
    return message_log;
}

std::string Game::getFieldString() const {
    return field.to_string();
}

std::string Game::getPlayerHud() const {
    return player.getHud();
}

bool Game::isRunning() const {
    return state == GameState::Going || state == GameState::Level_up;
}

void Game::setGameState(GameState newState) {
    state = newState;
}

template<typename T>
int Game::find_by_position(const std::vector<T>& container, const Coordinates& coordinates) {
    for (int i = 0; i < static_cast<int>(container.size()); i++) {
        if (container[i].getPosition() == coordinates) return i;
    }
    return -1;
}

char Game::get_player_input(std::string info, std::string allowed_commands) {
    std::string input;
    char action = 0;
    bool valid_input = false;
    while (!valid_input) {
        std::cout << info;
        std::getline(std::cin, input);
        if (input.length() != 1) {
            std::cout << "Некорректный ввод.\n";
            continue;
        }
        action = input[0];
        if (allowed_commands.find(action) != std::string::npos) {
            valid_input = true;
        } else {
            std::cout << "Некорректный символ.\n";
        }
    }
    return action;
}

Coordinates Game::get_coordinates_from_player(std::string info) {
    std::string input;
    Coordinates coords{0, 0};
    bool valid_input = false;
    while (!valid_input) {
        std::cout << info;
        std::getline(std::cin, input);
        size_t space_pos = input.find(' ');
        if (space_pos == std::string::npos) {
            std::cout << "Введите две координаты через пробел\n";
            continue;
        }
        try {
            coords.x = std::stoi(input.substr(0, space_pos));
            coords.y = std::stoi(input.substr(space_pos + 1));
            if (field.inBounds(coords)) valid_input = true;
            else std::cout << "Координаты за пределами поля.\n";
        }
        catch (...) {
            std::cout << "Неверный формат.\n";
        }
    }
    return coords;
}

void Game::processMovement(int dx, int dy) {
    message_log.clear();
    Coordinates offset{dx, dy};
    bool wasBow = player.isBowEquipped();
    Cell::Type result = field.moveBeing(player, offset);

    switch (result) {
        case Cell::Type::Wall:
            log("Вы не умеете ходить сквозь стены.");
            break;
        case Cell::Type::Enemy:
        case Cell::Type::Tower:
            if (!wasBow) {
                attack(player.attack(), player.getPosition() + offset);
            }
            break;
        case Cell::Type::Trap: {
            log("Вы наступили на свою же ловушку.");
            int trap_id = find_by_position(active_traps, player.getPosition());
            if(trap_id != -1) {
                attack(active_traps[trap_id].getDamage(), active_traps[trap_id].getPosition());
                active_traps.erase(active_traps.begin() + trap_id);
            }
            break;
        }
        default: break;
    }

    if (wasBow && (offset.x != 0 || offset.y != 0)) {
        shoot_arrow_from(player.getPosition(), offset);
    }
}

void Game::togglePlayerWeapon() {
    message_log.clear();
    player.toggleBow();
    log("Оружие переключено: " + std::string(player.isBowEquipped() ? "Лук" : "Меч"));
}

void Game::processShop() {
    message_log.clear();
    std::cout << spellShop.toString() << std::endl;
    int spell_num;
    bool valid_input = false;
    while (!valid_input) {
        char selected_spell = get_player_input("Выберите заклинание для покупки (0-отмена):", "0123456789");
        spell_num = selected_spell - '0';
        if (spell_num > spellShop.getCount()) continue;
        if (spell_num == 0) return;
        if (spellShop.getSpellPrice(spell_num - 1) > player.getPoints()) {
            std::cout << "Недостаточно очков.\n";
            continue;
        }
        valid_input = true;
    }
    player.changePoints(-spellShop.getSpellPrice(spell_num - 1));
    playerHand.addSpell(spellShop.buySpell(spell_num - 1));
    log("Заклинание куплено.");
}

void Game::processSpellCast() {
    message_log.clear();
    bool res = cast_spell_internal();
    if (res) log("Заклинание использовано.");
    else log("Использование заклинания отменено.");
}

bool Game::cast_spell_internal() {
    std::cout << playerHand.toString() << std::endl;
    if (playerHand.getSpellCount() == 0) return false;
    
    int spell_num;
    bool valid_input = false;
    while (!valid_input) {
        char selected_spell = get_player_input("Выберите заклинание (0-отмена):", "0123456789");
        spell_num = selected_spell - '0';
        if (spell_num > playerHand.getSpellCount()) continue;
        if (spell_num == 0) return false;
        valid_input = true;
    }
    spell_num--;

    Coordinates target;
    BasicSpell::Type spell_type = playerHand.getSpellType(spell_num);
    std::cout << "Выбрано: " << playerHand.getSpellName(spell_num) << std::endl;

    bool valid_input_2 = false;
    while (!valid_input_2) {
        std::string info = (spell_type == BasicSpell::Type::Trap) ? 
            "Координаты ловушки: " : "Координаты цели: ";
        target = get_coordinates_from_player(info);

        if (!playerHand.doesSpellReach(spell_num, player.getPosition(), target)) {
            std::cout << "Слишком далеко.\n";
            continue;
        }
        if (spell_type == BasicSpell::Type::Trap && field.getCellType(target) != Cell::Type::Empty) {
            std::cout << "Место занято.\n";
            continue;
        }
        valid_input_2 = true;
    }

    BasicSpell* basic_spell = playerHand.getSpell(spell_num);
    DamagingSpell* damaging_spell = nullptr;
    TrapSpell* trap_spell = nullptr;

    switch (spell_type) {
        case BasicSpell::Type::Direct:
        case BasicSpell::Type::AoE:
            damaging_spell = dynamic_cast<DamagingSpell*>(basic_spell);
            if (damaging_spell) {
                std::vector<Coordinates> target_cells = damaging_spell->getAttackedCells(target);
                int spell_damage = damaging_spell->getDamage();
                for (auto cell : target_cells) {
                    if (field.getCellType(cell) == Cell::Type::Enemy || field.getCellType(cell) == Cell::Type::Tower) {
                        attack(spell_damage, cell);
                    }
                }
                playerHand.removeSpell(spell_num);
            }
            break;
        case BasicSpell::Type::Trap:
            trap_spell = dynamic_cast<TrapSpell*>(basic_spell);
            if (trap_spell) {
                TrapSpell trap_copy = *trap_spell;
                trap_copy.setTrap(target);
                playerHand.removeSpell(spell_num);
                field.setCellType(target, Cell::Type::Trap);
                active_traps.push_back(std::move(trap_copy));
            }
            break;
        default: return false;
    }
    return true;
}

void Game::processSave() {
    message_log.clear();
    std::cout << "Слот (1-5): " << std::endl;
    char slot_char = get_player_input("", "12345");
    int slot = slot_char - '0';
    if (loader.saveGame(slot, game_data)) {
        log("Игра сохранена в слот " + std::to_string(slot));
    } else {
        log("Ошибка сохранения.");
    }
}

void Game::attack(float damage, const Coordinates& target_pos) {
    switch (field.getCellType(target_pos)) {
        case Cell::Type::Player:
            player.takeDamage(damage);
            if (player.getHealth() <= 0.0f) state = GameState::Player_lose;
            else log(std::format("Вам нанесено урона: {}", (int)damage));
            break;
        case Cell::Type::Enemy: {
            int idx = find_by_position(enemies, target_pos);
            if (idx == -1) return;
            enemies[idx].takeDamage(damage);
            if (enemies[idx].getHealth() <= 0.0f) {
                field.setCellType(target_pos, Cell::Type::Empty);
                enemies.erase(enemies.begin() + idx);
                log("Враг побеждён!");
                player.changePoints(5);
                if (enemies.empty() && towers.empty()) state = GameState::Level_up;
            } else {
                log(std::format("Врагу нанесено урона: {}", (int)damage));
            }
            break;
        }
        case Cell::Type::Tower: {
            int idx = find_by_position(towers, target_pos);
            if (idx == -1) return;
            towers[idx].takeDamage(damage);
            if (towers[idx].getHealth() <= 0.0f) {
                field.setCellType(target_pos, Cell::Type::Empty);
                towers.erase(towers.begin() + idx);
                log("Башня разрушена!");
                player.changePoints(POINTS_FOR_KILL);
                if (towers.empty() && enemies.empty()) state = GameState::Level_up;
            } else {
                 log(std::format("Башне нанесено урона: {}", (int)damage));
            }
            break;
        }
        default: break;
    }
}

void Game::shoot_arrow_from(Coordinates start, Coordinates dir) {
    if (dir.x == 0 && dir.y == 0) return;
    Coordinates pos = start + dir;
    while (field.inBounds(pos)) {
        Cell::Type ct = field.getCellType(pos);
        if (ct == Cell::Type::Wall) {
            log("Стрела разбилась о стену.");
            return;
        }
        if (ct == Cell::Type::Enemy || ct == Cell::Type::Tower) {
            attack(player.attack(), pos);
            return;
        }
        pos = pos + dir;
    }
    log("Стрела улетела в пустоту.");
}

void Game::updateWorld() {
    if (state == GameState::Level_up) {
        game_data.goToNextLevel();
        player = game_data.player;
        enemies = game_data.enemies;
        field = game_data.field;
        state = GameState::Going;
        log("Переход на следующий уровень!");
        return;
    }
    enemies_move();
    towers_move();
}

void Game::enemies_move() {
    for (Enemy& enemy : enemies) {
        int dx = player.getPosition().x - enemy.getPosition().x;
        int dy = player.getPosition().y - enemy.getPosition().y;
        Coordinates offset{0,0};
        
        if (std::abs(dx) > std::abs(dy)) offset.x = (dx > 0) ? 1 : -1;
        else offset.y = (dy > 0) ? 1 : -1;

        Coordinates target = enemy.getPosition() + offset;
        if (target == player.getPosition()) {
            attack(enemy.attack(), player.getPosition());
            continue;
        }
        if (field.isCellFree(target)) {
            Cell::Type res = field.moveBeing(enemy, offset);
            if (res == Cell::Type::Trap) {
                log("Враг попал в ловушку!");
                int trap_id = find_by_position(active_traps, enemy.getPosition());
                if (trap_id != -1) {
                    attack(active_traps[trap_id].getDamage(), active_traps[trap_id].getPosition());
                    active_traps.erase(active_traps.begin() + trap_id);
                }
            }
        }
    }
}

void Game::towers_move() {
    for (auto& tower : towers) {
        tower.tick_delay();
        if (tower.can_attack(player.getPosition())) {
            log("Башня атакует!");
            attack(tower.attack(), player.getPosition());
        }
    }
}