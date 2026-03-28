#ifndef CONCRETE_COMMANDS_H
#define CONCRETE_COMMANDS_H

#include "IGameCommand.h"
#include "player.h"
#include "gamefield.h"
#include "enemy.h"
#include "EnemyManager.h"
#include "LevelManager.h"
#include "GameSaver.h"
#include "GameLoader.h"
#include "spellcard.h"
#include <iostream>
#include <memory>
 
class MoveCommand : public IGameCommand {
protected:
    int dx, dy;
    std::string direction;
    std::string englishName;
    
public:
    MoveCommand(int deltaX, int deltaY, const std::string& dir, const std::string& engName) 
        : dx(deltaX), dy(deltaY), direction(dir), englishName(engName) {}
    
    bool execute(Player& player, GameField& field, Enemy& enemy, 
                EnemyManager& enemyManager, LevelManager& levelManager) override {
        return player.move(field, dx, dy);
    }
    
    std::string getDescription() const override {
        return "Движение " + direction;
    }
    
    bool consumesTurn() const override { return true; }
    
    std::string getName() const override { 
        return "move_" + englishName;
    }
     
    virtual std::unique_ptr<IGameCommand> clone() const override = 0;
};

class MoveUpCommand : public MoveCommand {
public:
    MoveUpCommand() : MoveCommand(0, -1, "вверх", "up") {}
    
    std::unique_ptr<IGameCommand> clone() const override {
        return std::make_unique<MoveUpCommand>(*this);
    }
};

class MoveDownCommand : public MoveCommand {
public:
    MoveDownCommand() : MoveCommand(0, 1, "вниз", "down") {}
    
    std::unique_ptr<IGameCommand> clone() const override {
        return std::make_unique<MoveDownCommand>(*this);
    }
};

class MoveLeftCommand : public MoveCommand {
public:
    MoveLeftCommand() : MoveCommand(-1, 0, "влево", "left") {}
    
    std::unique_ptr<IGameCommand> clone() const override {
        return std::make_unique<MoveLeftCommand>(*this);
    }
};

class MoveRightCommand : public MoveCommand {
public:
    MoveRightCommand() : MoveCommand(1, 0, "вправо", "right") {}
    
    std::unique_ptr<IGameCommand> clone() const override {
        return std::make_unique<MoveRightCommand>(*this);
    }
};
 
class AttackCommand : public IGameCommand {
public:
    bool execute(Player& player, GameField& field, Enemy& enemy, 
                EnemyManager& enemyManager, LevelManager& levelManager) override {
        return player.getCombatSystem().attackEnemy(field, enemy);
    }
    
    std::string getDescription() const override {
        return "Атака врага";
    }
    
    bool consumesTurn() const override { return true; }
    
    std::string getName() const override { return "attack"; }
    
    std::unique_ptr<IGameCommand> clone() const override {
        return std::make_unique<AttackCommand>(*this);
    }
};
 
class SwitchAttackCommand : public IGameCommand {
public:
    bool execute(Player& player, GameField& field, Enemy& enemy, 
                EnemyManager& enemyManager, LevelManager& levelManager) override {
        player.getCombatSystem().switchAttackType();
        std::cout << "Тип атаки: " << player.getCombatSystem().getAttackTypeString() << std::endl;
        return true;
    }
    
    std::string getDescription() const override {
        return "Смена типа атаки";
    }
    
    bool consumesTurn() const override { return false; }
    
    std::string getName() const override { return "switch_attack"; }
    
    std::unique_ptr<IGameCommand> clone() const override {
        return std::make_unique<SwitchAttackCommand>(*this);
    }
};
 
class CastSpellCommand : public IGameCommand {
public:
    bool execute(Player& player, GameField& field, Enemy& enemy, 
                EnemyManager& enemyManager, LevelManager& levelManager) override {
        if (player.getSpellHand().getSize() == 0) {
            std::cout << "У вас нет заклинаний!\n";
            return false;
        }
        int targetX, targetY;
        std::cout << "Введите координаты цели (x y): ";
        std::cin >> targetX >> targetY;
        while (std::cin.get() != '\n') continue;
        
        return player.castSpell(field, targetX, targetY, enemy);
    }
    
    std::string getDescription() const override {
        return "Применить заклинание";
    }
    
    bool consumesTurn() const override { return true; }
    
    std::string getName() const override { return "cast_spell"; }
    
    std::unique_ptr<IGameCommand> clone() const override {
        return std::make_unique<CastSpellCommand>(*this);
    }
};

class PrevSpellCommand : public IGameCommand {
public:
    bool execute(Player& player, GameField& field, Enemy& enemy, 
                EnemyManager& enemyManager, LevelManager& levelManager) override {
        player.getSpellHand().selectPrevious();
        auto spell = player.getSpellHand().getSelectedSpell();
        if (spell) {
            std::cout << "Выбрано: " << spell->getName() << std::endl;
        }
        return true;
    }
    
    std::string getDescription() const override {
        return "Предыдущее заклинание";
    }
    
    bool consumesTurn() const override { return false; }
    
    std::string getName() const override { return "prev_spell"; }
    
    std::unique_ptr<IGameCommand> clone() const override {
        return std::make_unique<PrevSpellCommand>(*this);
    }
};

class NextSpellCommand : public IGameCommand {
public:
    bool execute(Player& player, GameField& field, Enemy& enemy, 
                EnemyManager& enemyManager, LevelManager& levelManager) override {
        player.getSpellHand().selectNext();
        auto spell = player.getSpellHand().getSelectedSpell();
        if (spell) {
            std::cout << "Выбрано: " << spell->getName() << std::endl;
        }
        return true;
    }
    
    std::string getDescription() const override {
        return "Следующее заклинание";
    }
    
    bool consumesTurn() const override { return false; }
    
    std::string getName() const override { return "next_spell"; }
    
    std::unique_ptr<IGameCommand> clone() const override {
        return std::make_unique<NextSpellCommand>(*this);
    }
};
 
class SaveGameCommand : public IGameCommand {
public:
    bool execute(Player& player, GameField& field, Enemy& enemy, 
                EnemyManager& enemyManager, LevelManager& levelManager) override {
        std::cout << "Введите слот для сохранения (1-5): ";
        int slot;
        std::cin >> slot;
        while (std::cin.get() != '\n') continue;
        
        if (slot >= 1 && slot <= 5) {
            try {
                GameSaver::saveGame(slot, field, player, enemy, enemyManager, levelManager);
                std::cout << "Игра сохранена в слот " << slot << "!\n";
            } catch (const std::exception& ex) {
                std::cout << "Ошибка сохранения: " << ex.what() << "\n";
            }
        } else {
            std::cout << "Неверный номер слота!\n";
        }
        return false;
    }
    
    std::string getDescription() const override {
        return "Сохранить игру";
    }
    
    bool consumesTurn() const override { return false; }
    
    std::string getName() const override { return "save_game"; }
    
    std::unique_ptr<IGameCommand> clone() const override {
        return std::make_unique<SaveGameCommand>(*this);
    }
};

class LoadGameCommand : public IGameCommand {
public:
    bool execute(Player& player, GameField& field, Enemy& enemy, 
                EnemyManager& enemyManager, LevelManager& levelManager) override {
        std::cout << "Введите слот для загрузки (1-5): ";
        int slot;
        std::cin >> slot;
        while (std::cin.get() != '\n') continue;
        
        if (slot >= 1 && slot <= 5) {
            try {
                if (GameLoader::loadGame(slot, field, player, enemy, enemyManager, levelManager)) {
                    std::cout << "Игра загружена из слота " << slot << "!\n";
                }
            } catch (const std::exception& ex) {
                std::cout << "Ошибка загрузки: " << ex.what() << "\n";
            }
        } else {
            std::cout << "Неверный номер слота!\n";
        }
        return false;
    }
    
    std::string getDescription() const override {
        return "Загрузить игру";
    }
    
    bool consumesTurn() const override { return false; }
    
    std::string getName() const override { return "load_game"; }
    
    std::unique_ptr<IGameCommand> clone() const override {
        return std::make_unique<LoadGameCommand>(*this);
    }
};
 
class BuySpellCommand : public IGameCommand {
public:
    bool execute(Player& player, GameField& field, Enemy& enemy, 
                EnemyManager& enemyManager, LevelManager& levelManager) override {
        return player.buySpell(100);
    }
    
    std::string getDescription() const override {
        return "Купить случайное заклинание";
    }
    
    bool consumesTurn() const override { return true; }
    
    std::string getName() const override { return "buy_spell"; }
    
    std::unique_ptr<IGameCommand> clone() const override {
        return std::make_unique<BuySpellCommand>(*this);
    }
};

class OpenShopCommand : public IGameCommand {
public:
    bool execute(Player& player, GameField& field, Enemy& enemy, 
                EnemyManager& enemyManager, LevelManager& levelManager) override {
        player.openSpellShop();
        std::cout << "Введите номер заклинания для покупки (0 для выхода): ";
        int choice;
        std::cin >> choice;
        while (std::cin.get() != '\n') continue;
        
        if (choice == 0) return true;
        if (choice >= 1 && choice <= player.getSpellShop().getSpellCount()) {
            return player.buySpellFromShop(choice - 1);
        }
        std::cout << "Неверный номер заклинания!\n";
        return false;
    }
    
    std::string getDescription() const override {
        return "Открыть магазин заклинаний";
    }
    
    bool consumesTurn() const override { return false; }
    
    std::string getName() const override { return "open_shop"; }
    
    std::unique_ptr<IGameCommand> clone() const override {
        return std::make_unique<OpenShopCommand>(*this);
    }
};

#endif 