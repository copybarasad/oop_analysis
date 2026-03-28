#pragma once
#include "command.h"
#include "input_reader.h"
#include "game.h"
#include "help_funcs.h"
#include <iostream>
#include <sstream> 
using namespace std;
class InputReader;

class MoveUpCommand : public Command {
public:
    bool execute(Game& game, string& resultMessage) override {
        Player* player = game.getPlayer();
        Field* field = game.getField();
        
        if (!player) return false;
        
        int oldX = player->getX();
        int oldY = player->getY();
        int newX = oldX;
        int newY = oldY - 1;

        if (field->validpos(oldX, oldY)) {
            field->getNode(oldX, oldY).removeEntity();
        }
        
        bool willTrap = field->validpos(newX, newY) && field->getNode(newX, newY).hasTrap();
        
        string message;
        bool success = player->moveUp(message);
        
        if (success) {
            if (field->validpos(newX, newY)) {
                field->getNode(newX, newY).setPlayer();
            }
            resultMessage = "Игрок переместился на (" + to_string(newX) + "," + to_string(newY) + ")";
            if (willTrap) {
                resultMessage += "\nИгрок наступил на ловушку! Урон: 4";
            }
        } else {
            if (field->validpos(oldX, oldY)) {
                field->getNode(oldX, oldY).setPlayer();
            }
            resultMessage = message;
        }
        return success;
    }

    string getType() const override {
        return "MOVE_UP";
    }
};

class MoveDownCommand : public Command {
public:
    bool execute(Game& game, string& resultMessage) override {
        Player* player = game.getPlayer();
        Field* field = game.getField();
        
        if (!player) return false;
        
        int oldX = player->getX();
        int oldY = player->getY();
        int newX = oldX;
        int newY = oldY + 1;

        if (field->validpos(oldX, oldY)) {
            field->getNode(oldX, oldY).removeEntity();
        }

        bool willTrap = field->validpos(newX, newY) && field->getNode(newX, newY).hasTrap();
        
        string message;
        bool success = player->moveDown(message);
        
        if (success) {
            if (field->validpos(newX, newY)) {
                field->getNode(newX, newY).setPlayer();
            }
            resultMessage = "Игрок переместился на (" + to_string(newX) + "," + to_string(newY) + ")";
            if (willTrap) {
                resultMessage += "\nИгрок наступил на ловушку! Урон: 4";
            }
        } else {
            if (field->validpos(oldX, oldY)) {
                field->getNode(oldX, oldY).setPlayer();
            }
            resultMessage = message;
        }
        return success;
    }
    string getType() const override {
        return "MOVE_DOWN";
    }
};

class MoveLeftCommand : public Command {
public:
    bool execute(Game& game, string& resultMessage) override {
        Player* player = game.getPlayer();
        Field* field = game.getField();
        
        if (!player) return false;
        
        int oldX = player->getX();
        int oldY = player->getY();
        int newX = oldX - 1;
        int newY = oldY;

        if (field->validpos(oldX, oldY)) {
            field->getNode(oldX, oldY).removeEntity();
        }

        bool willTrap = field->validpos(newX, newY) && field->getNode(newX, newY).hasTrap();
        
        string message;
        bool success = player->moveLeft(message);

        if (success) {
            if (field->validpos(newX, newY)) {
                field->getNode(newX, newY).setPlayer();
            }
            resultMessage = "Игрок переместился на (" + to_string(newX) + "," + to_string(newY) + ")";
            if (willTrap) {
                resultMessage += "\nИгрок наступил на ловушку! Урон: 4";
            }
        } else {
            if (field->validpos(oldX, oldY)) {
                field->getNode(oldX, oldY).setPlayer();
            }
            resultMessage = message;
        }
        return success;
    }

    string getType() const override {
        return "MOVE_LEFT";
    }
};

class MoveRightCommand : public Command {
public:
    bool execute(Game& game, string& resultMessage) override {
        Player* player = game.getPlayer();
        Field* field = game.getField();
        
        if (!player) return false;
        
        int oldX = player->getX();
        int oldY = player->getY();
        int newX = oldX + 1;
        int newY = oldY;

        if (field->validpos(oldX, oldY)) {
            field->getNode(oldX, oldY).removeEntity();
        }

        bool willTrap = field->validpos(newX, newY) && field->getNode(newX, newY).hasTrap();
        
        string message;
        bool success = player->moveRight(message);
        
        if (success) {
            if (field->validpos(newX, newY)) {
                field->getNode(newX, newY).setPlayer();
            }
            resultMessage = "Игрок переместился на (" + to_string(newX) + "," + to_string(newY) + ")";
            if (willTrap) {
                resultMessage += "\nИгрок наступил на ловушку! Урон: 4";
            }
        } else {
            if (field->validpos(oldX, oldY)) {
                field->getNode(oldX, oldY).setPlayer();
            }
            resultMessage = message;
        }
        return success;
    }

    string getType() const override {
        return "MOVE_RIGHT";
    }
};

class AttackCommand : public Command {
public:
    bool execute(Game& game, string& resultMessage) override {
        Player* player = game.getPlayer();
        Enemy* enemy = game.getEnemy();
        int oldEnemyLives = enemy->getLives();
        string message;
        bool success = player->attack_force(*enemy, message);
        
        if (success) {
            resultMessage = message + "\nОчки: " + to_string(player->getPoints());
        } else {
            resultMessage = message + "\nОчки: " + to_string(player->getPoints());
        }
        return success;
    }

    string getType() const override {
        return "ATTACK";
    }
};

class UseSpellCommand : public Command {
public:
    bool execute(Game& game, string& resultMessage) override {
        resultMessage = "UseSpellCommand: используйте executeWithVisualization";
        return false;
    }
    
    template<typename RendererType>
    bool executeWithVisualization(Game& game, GameVisualization<RendererType>& visualization, string& resultMessage) {
        Player* player = game.getPlayer();
        if (!player) {
            resultMessage = "Ошибка: игрок не найден!";
            return false;
        }
        
        string handOutput;
        player->getHand().showHand(handOutput);
        resultMessage = handOutput;
        
        visualization.showPrompt(resultMessage + "Введите индекс и координаты (index X Y): ");
        
        InputReader reader;
        vector<int> numbers = reader.readNumbers(3);
        
        if (numbers.size() == 3) {
            int index = numbers[0];
            int x = numbers[1];
            int y = numbers[2];
            
            Enemy* enemy = game.getEnemy();
            Field* field = game.getField();
            
            string spellResult;
            bool success = player->useSpell(index, x, y, *field, *enemy, spellResult);
            resultMessage = spellResult;
            
            return success;
        } else {
            resultMessage = "Ошибка: нужно ввести 3 числа!";
            return false;
        }
    }
    
    string getType() const override { return "USE_SPELL"; }
};
class ShowSpellsCommand : public Command {
public:
    bool execute(Game& game, string& resultMessage) override {
        Player* player = game.getPlayer();
        if (!player) {
            resultMessage = "Ошибка: игрок не найден!";
            return false;
        }
        
        string handOutput;
        player->getHand().showHand(handOutput);
        
        resultMessage = handOutput;
        return true;
    }
    
    string getType() const override {
        return "SHOW_SPELLS";
    }
};

class BuySpellCommand : public Command {
public:
    bool execute(Game& game, string& resultMessage) override {
        return false;
    }

    template<typename RendererType>
    bool executeWithVisualization(Game& game, GameVisualization<RendererType>& visualization, string& resultMessage) {
        Player* player = game.getPlayer();
        if (!player) {
            resultMessage = "Ошибка: игрок не найден!";
            return false;
        }
        
        renderSpellShop(player->getPoints());
        
        visualization.showPrompt("Введите индекс заклинания для покупки: ");
        
        InputReader reader;
        int choice = reader.readInteger();

        int& cost = player->getPointsRef();
        string buyResult;
        bool success = player->getHand().buySpell(choice, cost, buyResult);
        
        resultMessage += "\n" + buyResult;
        return success;
    }
    
    string getType() const override {
        return "BUY_SPELL";
    }
};


class ChangeCombatTypeCommand : public Command {
public:
    bool execute(Game& game, string& resultMessage) override {
        return false;
    }

    template<typename RendererType>
    bool executeWithVisualization(Game& game, GameVisualization<RendererType>& visualization, string& resultMessage) {
        Player* player = game.getPlayer();
        if (!player) {
            resultMessage = "Ошибка: игрок не найден!";
            visualization.renderMessage(resultMessage);
            return false;
        }
        
        visualization.showPrompt("Введите тип боя (NEAR/FAR): ");
        
        InputReader reader;
        string combatType = reader.readInputLine();
        
        if (combatType == "NEAR") {
            player->turnCombat(Player::CombatType::NEAR);
            resultMessage = "Вы переключились на тип боя: NEAR";
            return true;
        } else if (combatType == "FAR") {
            player->turnCombat(Player::CombatType::FAR);
            resultMessage = "Вы переключились на тип боя: FAR";
            return true;
        } else {
            resultMessage = "Недопустимый тип боя!";
            return false;
        }
    }
    
    string getType() const override { return "CHANGE_TYPE"; }
};


class ShowStatusCommand : public Command{
public:
    bool execute(Game& game, string& resultMessage) override {
        return false;
    }

    template<typename RendererType>
    bool executeWithVisualization(Game& game, GameVisualization<RendererType>& visualization, string& resultMessage) {
        visualization.showPrompt("Введите персонажа (PLAYER/ENEMY): ");
        
        InputReader reader;
        string entity = reader.readInputLine();
        
        if (entity == "PLAYER") {
            Player* player = game.getPlayer();
            if (player) {
                stringstream stats;
                stats << "\n=== СТАТИСТИКА ИГРОКА ===" << endl;
                stats << "Имя: " << player->getname() << endl;
                stats << "Жизни: " << player->getLives() << endl;
                stats << "Урон: " << player->getDamage() << endl;
                stats << "Очки: " << player->getPoints() << endl;
                stats << "Тип боя: " << (player->getCombatType() == Player::CombatType::NEAR ? "Ближний" : "Дальний") << endl;
                stats << "Сила атаки: " << player->getPower() << endl;
                stats << "Радиус атаки: " << player->getRadius() << endl;
                stats << "Позиция: (" << player->getX() << "," << player->getY() << ")" << endl;
                stats << "Заклинаний в руке: " << player->getHand().getSpellCount() << "/" << player->getHand().getMaxSize() << endl;
                stats << "========================" << endl;
                resultMessage = stats.str();
            } else {
                resultMessage = "Игрок не найден!";
            }
            return true;
        } else if (entity == "ENEMY") {
            Enemy* enemy = game.getEnemy();
            if (enemy) {
                stringstream stats;
                stats << "\n=== СТАТИСТИКА ВРАГА ===" << endl;
                stats << "Имя: " << enemy->getname() << endl;
                stats << "Жизни: " << enemy->getLives() << endl;
                stats << "Урон: " << enemy->getDamage() << endl;
                stats << "Позиция: (" << enemy->getX() << "," << enemy->getY() << ")" << endl;
                stats << "========================" << endl;
                resultMessage = stats.str();
            } else {
                resultMessage = "Враг не найден!";
            }
            return true;
        } else {
            resultMessage = "Неизвестный персонаж!";
            return false;
        }
    }
    
    string getType() const override { return "SHOW_STATUS"; }
};

class SaveCommand : public Command {
public:
    bool execute(Game& game, string& resultMessage) override {
        return false;
    }

    template<typename RendererType>
    bool executeWithVisualization(Game& game, GameVisualization<RendererType>& visualization, string& resultMessage) {
        try {
            game.saveGame(visualization);
            return true;
        } catch (const exception& e) {
            resultMessage = "Ошибка сохранения: " + string(e.what());
            return false;
        }
    }
    
    string getType() const override { return "SAVE"; }
};

class LoadCommand : public Command {
public:
    bool execute(Game& game, string& resultMessage) override {
        if (game.loadGame()) {
            resultMessage = "=== ИГРА УСПЕШНО ЗАГРУЖЕНА ===";
            return true;
        } else {
            resultMessage = "Не удалось загрузить игру!";
            return false;
        }
    }
    
    string getType() const override {
        return "LOAD";
    }
};

class QuitCommand : public Command {
public:
    bool execute(Game& game, string& resultMessage) override {
        game.setGameRunning(false);
        resultMessage = "Выход из игры...";
        return true;
    }
    
    string getType() const override {
        return "QUIT";
    }
};

class ShowMenuCommand : public Command {
public:
    bool execute(Game& game, string& resultMessage) override {
        CommandMapper* mapper = game.getCommandMapper();
        
        if (mapper) {
            stringstream menu;
            menu << "\n=== МЕНЮ ДЕЙСТВИЙ ===" << endl;
            menu << mapper->getFormattedCommand("UP") << " - Движение вверх" << endl;
            menu << mapper->getFormattedCommand("DOWN") << " - Движение вниз" << endl;
            menu << mapper->getFormattedCommand("LEFT") << " - Движение влево" << endl;
            menu << mapper->getFormattedCommand("RIGHT") << " - Движение вправо" << endl;
            menu << mapper->getFormattedCommand("ATTACK") << " - Атаковать" << endl;
            menu << mapper->getFormattedCommand("CHANGE_TYPE") << " - Сменить тип боя" << endl;
            menu << mapper->getFormattedCommand("SHOW_STATUS") << " - Показать статистику" << endl;
            menu << mapper->getFormattedCommand("SHOW_MENU") << " - Показать меню" << endl;
            menu << mapper->getFormattedCommand("USE_SPELL") << " - Использовать заклинание" << endl;
            menu << mapper->getFormattedCommand("BUY_SPELL") << " - Купить заклинание" << endl;
            menu << mapper->getFormattedCommand("SHOW_SPELLS") << " - Показать заклинания в руке" << endl; 
            menu << mapper->getFormattedCommand("SAVE") << " - Сохранить состояние" << endl; 
            menu << mapper->getFormattedCommand("LOAD") << " - Загрузить из файла" << endl; 
            menu << mapper->getFormattedCommand("QUIT") << " - Выйти из игры" << endl;
            resultMessage = menu.str();
        } else {
            resultMessage = getDefaultMenu();
        }
        return true;
    }

private:
    string getDefaultMenu() const {
        stringstream menu;
        menu << "\n=== МЕНЮ ДЕЙСТВИЙ ===" << endl;
        menu << "UP - Движение вверх" << endl;
        menu << "DOWN - Движение вниз" << endl;
        menu << "LEFT - Движение влево" << endl;
        menu << "RIGHT - Движение вправо" << endl;
        menu << "ATTACK - Атаковать" << endl;
        menu << "CHANGE_TYPE - Сменить тип боя" << endl;
        menu << "SHOW_STATUS - Показать статистику" << endl;
        menu << "SHOW_MENU - Показать меню" << endl;
        menu << "USE_SPELL - Использовать заклинание" << endl;
        menu << "BUY_SPELL - Купить заклинание" << endl;
        menu << "SHOW_SPELLS - Показать заклинания в руке" << endl; 
        menu << "SAVE - Сохранить состояние" << endl; 
        menu << "LOAD - Загрузить из файла" << endl; 
        menu << "QUIT - Выйти из игры" << endl;
        return menu.str();
    }
    
    string getType() const override {
        return "SHOW_MENU";
    }
};