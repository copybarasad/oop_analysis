#ifndef MANAGERS_H
#define MANAGERS_H

#include <iostream>
#include <utility>
#include <algorithm>
#include "Game.h"
#include "command.h"

template <typename InputProvider>
class GameController {
private:
    InputProvider input_provider;

    bool handleAttack(Game& game, const Command& cmd) {
        Player& player = game.getPlayer();
        Field& field = game.getField();
        
        Cell* target_cell = field.get_cell(cmd.x, cmd.y);

        if (!target_cell || target_cell->getObject() == static_cast<char>(ObjectType::Empty)) {
            std::cout << "Нельзя атаковать пустую ячейку.\n";
            return false;
        }

        if((std::abs(player.get_current_cell()->getX() - target_cell->getX()) + std::abs(player.get_current_cell()->getY() - target_cell->getY())) > player.getRange()){
            std::cout << "Выбрана точка за пределами дистанции\n";
            return false;
        }

        Creature* target = target_cell->getCreature();
        
        std::cout << "Игрок атакует цель в " << cmd.x << "." << cmd.y << "\n";

        target->deal_damage(player.getCurrentDamage());
        
        int& enemies_count = game.getCounterEnemies();
        int& towers_count = game.getCounterTowers();
        
        if (target->getIs_dead()) {
            field.remove_object_field(cmd.x, cmd.y);
            std::cout << "Цель уничтожена!\n";

            Enemy* enemy_ptr = dynamic_cast<Enemy*>(target);
            if (enemy_ptr) { 
                enemies_count--; 
                player.kill_up();
            } else {
                Tower* tower_ptr = dynamic_cast<Tower*>(target);
                if (tower_ptr) {
                    towers_count--;
                }
            }
        }
        return true; 
    }

public:
    explicit GameController(InputProvider ip = InputProvider()) 
        : input_provider(std::move(ip)) {}

    bool performPlayerAction(Game& game, const Command& cmd) {
        Player& player = game.getPlayer();
        Field& field = game.getField();
        
        if (player.getIs_dead()) return false;

        switch (cmd.type) {
            case CommandType::MoveUp:
            case CommandType::MoveDown:
            case CommandType::MoveLeft:
            case CommandType::MoveRight: {
                int dx = (cmd.type == CommandType::MoveRight) - (cmd.type == CommandType::MoveLeft);
                int dy = (cmd.type == CommandType::MoveDown) - (cmd.type == CommandType::MoveUp);
                
                if (player.move(dx, dy, &field)) { 
                    std::cout << "Игрок переместился.\n";
                    return true;
                }
                
                std::cout << "Перемещение невозможно (вне поля или ячейка занята/непроходима).\n";
                return false; 
            }

            case CommandType::AttackCell:
                return handleAttack(game, cmd);

            case CommandType::CastSpell: {
                Player& player = game.getPlayer();
                Field& field = game.getField();

                std::string coords_str = std::to_string(cmd.x) + "." + std::to_string(cmd.y);
                
                int result = game.getHand().cast_from_hand(
                    cmd.spell_index, 
                    coords_str, 
                    &field, 
                    &game.getCounterEnemies(), 
                    &game.getCounterTowers(), 
                    &player
                );
                
                if (result == 1 || result == 2) {
                    std::cout << "Заклинание " << cmd.spell_index << " применено.\n";
                    return true;
                } else {
                    std::cout << "Ошибка применения заклинания (Неверный индекс или цель).\n";
                    return false;
                }
            }

            case CommandType::ChangeStyle:
                std::cout << "Смена боевого стиля.\n";
                game.getPlayer().change_fight_style();
                return true; 

            case CommandType::Skip:
                std::cout << "Ход пропущен.\n";
                return true;

            case CommandType::Save: {
                std::cout << "Введите название файла для сохранения: ";
                std::string save_filename;
                std::cin >> save_filename; 
                
                if(game.save_game(save_filename)) {
                    std::cout << "Игра сохранена.\n";
                }
                return false;
            }

            case CommandType::Quit:
                std::cout << "Выход из игры.\n";
                return true; 

            case CommandType::Invalid:
            default:
                std::cout << "Неизвестная команда.\n";
                return false;
        }
    }

    bool run_turn(Game& game) {
        
        Command cmd;
        bool turn_successful = false;

        while (!turn_successful) {
            cmd = input_provider.getCommand();
            
            if (cmd.type == CommandType::Quit) return false;
            
            turn_successful = performPlayerAction(game, cmd);
        }
        
        if (game.getPlayer().getIs_dead()) {
            std::cout << "Игрок мертв. Игра окончена.\n";
            return false;
        }

        return true; 
    }
};

template <typename RendererT>
class GameView {
private:
    RendererT renderer;

public:
    explicit GameView(RendererT r = RendererT()) : renderer(std::move(r)) {}

    void update(Game& game) { 
        renderer.render(game);
    }
};

#endif