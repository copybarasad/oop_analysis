#ifndef GAME_CONTROLLER_H
#define GAME_CONTROLLER_H

#include "input_reader.h"
#include "game.h"

/**
 * Шаблонный класс управления игрой
 * @tparam InputReaderType класс, отвечающий за считывание ввода
 */
template<typename InputReaderType>
class GameController {
private:
    Game& game;                    // ссылка на игру (не создает!)
    InputReaderType inputReader;   // создает объект параметра шаблона
    
public:
    // Конструктор: создает InputReaderType, принимает Game по ссылке
    explicit GameController(Game& gameInstance) 
        : game(gameInstance), inputReader() {}
    
    // Обработать один ввод
    bool processInput() {
        Command cmd = inputReader.readCommand();
        
        if (!cmd.isValid()) {
            game.displayMessage("Invalid command");
            return true;
        }
        
        // Вызываем соответствующий метод Game
        switch (cmd.getType()) {
            case Command::Type::MOVE_UP:
                game.movePlayer(0, -1);
                break;
            case Command::Type::MOVE_DOWN:
                game.movePlayer(0, 1);
                break;
            case Command::Type::MOVE_LEFT:
                game.movePlayer(-1, 0);
                break;
            case Command::Type::MOVE_RIGHT:
                game.movePlayer(1, 0);
                break;
            case Command::Type::SWITCH_MODE:
                game.switchCombatMode();
                break;
            case Command::Type::RANGED_ATTACK:
                game.rangedAttack();
                break;
            case Command::Type::CAST_SPELL_1:
                game.castSpell(0);
                break;
            case Command::Type::CAST_SPELL_2:
                game.castSpell(1);
                break;
            case Command::Type::CAST_SPELL_3:
                game.castSpell(2);
                break;
            case Command::Type::CAST_SPELL_4:
                game.castSpell(3);
                break;
            case Command::Type::CAST_SPELL_5:
                game.castSpell(4);
                break;
            case Command::Type::SAVE_GAME:
                game.save();
                break;
            case Command::Type::LOAD_GAME:
                game.load();
                break;
            case Command::Type::RESTART_GAME:
                game.restart();
                break;
            // В методе handleCommand в game_controller.h:
            case Command::Type::QUIT_GAME:
                game.quitToMenu();  // вместо game.quit()
                return false;
            case Command::Type::INVALID:
                break;
        }
        
        return true;
    }
    
    // Загрузить настройки управления
    void loadKeyBindings(const std::string& filename) {
        inputReader.loadKeyBindings(filename);
    }
};

#endif // GAME_CONTROLLER_H