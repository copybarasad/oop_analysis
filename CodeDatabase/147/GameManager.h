#pragma once

#include <iostream>
#include <memory>
#include "Command.h"
#include "ConsoleCommandReader.h"
#include "GameController.h"
#include "hand.h"
#include "player.h"
#include "position.h"
#include "DirectDamageSpell.h"
#include "AreaDamageSpell.h"
#include "Logger.h"

class Game; // forward declaration
template <typename Renderer> class GameVisualizer;

template <typename InputReader>
class GameManager {
public:
    explicit GameManager(EventBus* bus = nullptr) : _bus(bus) {}

    template <typename Visualizer>
    bool processTurn(Game& game, GameController& controller, Visualizer& visualizer);

    void setBindings(const std::unordered_map<char, std::string>& bindings) {
        reader.setBindings(bindings);
    }

    void setBus(EventBus* bus) { _bus = bus; }

private:
    InputReader reader;
    EventBus* _bus = nullptr; // не владеет
};

// ---- Реализация шаблонных методов ----

template <typename InputReader>
template <typename Visualizer>
bool GameManager<InputReader>::processTurn(Game& game, GameController& controller, Visualizer& visualizer) {
    Command cmd = reader.readCommand(game.getPlayer(), game.getHand(), game.getCurrentLevel(), game.getRequiredPoints());

    switch (cmd.type) {
    case CommandType::Move: {
        controller.turnPlayer(cmd.direction);
        if (_bus) {
            Position p = game.getPlayer().getPosition();
            _bus->publish("Игрок переместился в (" + std::to_string(p.getRow()) + ", " + std::to_string(p.getColumn()) + ")");
        }
        visualizer.render(game, controller);
        break;
    }
    case CommandType::ToggleAttackMode: {
        game.getPlayer().change_attack_version();
        std::cout << "Режим атаки изменён. Текущий: "
                  << (game.getPlayer().get_attack_version() == 'F' ? "Дальний" : "Заклинания") << "\n";
        if (_bus) {
            _bus->publish("Игрок сменил режим атаки");
        }
        break;
    }
    case CommandType::CastSpell: {
        Hand& hand = game.getHand();
        if (hand.size() == 0) {
            std::cout << "У вас нет заклинаний!\n";
            return true;
        }
        if (cmd.spellIndex < 0 || cmd.spellIndex >= static_cast<int>(hand.size())) {
            std::cout << "Нет такого заклинания!\n";
            return true;
        }
        ISpell* spell = hand.getSpell(static_cast<size_t>(cmd.spellIndex));
        std::string spellType = spell->name();

        if (spellType == "Direct Damage") {
            auto enemiesInRange =
                controller.getEnemiesInRadius(game.getPlayer().getPosition(),
                                              spell->radius());
            if (enemiesInRange.empty()) {
                std::cout << "Рядом нет врагов для заклинания прямого урона.\n";
                return true;
            }

            std::cout << "Враги в радиусе:\n";
            for (size_t i = 0; i < enemiesInRange.size(); ++i) {
                Position pos = enemiesInRange[i]->getPosition();
                std::cout << i << ") (" << pos.getRow() << ", " << pos.getColumn() << ")\n";
            }
            int idx = reader.askEnemyIndex(enemiesInRange.size());
            if (idx < 0 || static_cast<size_t>(idx) >= enemiesInRange.size()) {
                std::cout << "Нет такого врага!\n";
                return true;
            }
            Position targetPos = enemiesInRange[static_cast<size_t>(idx)]->getPosition();
            controller.turnPlayer(targetPos, "DS", cmd.spellIndex);
            if (_bus) {
                _bus->publish("Игрок применил прямое заклинание по врагу в (" +
                    std::to_string(targetPos.getRow()) + ", " + std::to_string(targetPos.getColumn()) + ")");
            }
        } else if (spellType == "Area Damage") {
            std::cout << "Вы применили заклинание урона по площади!\n";
            controller.turnPlayer(game.getPlayer().getPosition(), "AS", cmd.spellIndex);
            if (_bus) {
                _bus->publish("Игрок применил заклинание урона по площади");
            }
        } else {
            std::cout << "Неизвестный тип заклинания!\n";
        }
        visualizer.render(game, controller);
        break;
    }
    case CommandType::RangedAttack: {
        auto enemiesInRange = controller.getEnemiesInRadius(game.getPlayer().getPosition(), 5);
        if (enemiesInRange.empty()) {
            std::cout << "Рядом нет врагов для атаки.\n";
            return true;
        }
        std::cout << "Враги в радиусе:\n";
        for (size_t i = 0; i < enemiesInRange.size(); ++i) {
            Position pos = enemiesInRange[i]->getPosition();
            std::cout << i << ") (" << pos.getRow() << ", " << pos.getColumn() << ")\n";
        }
        int idx = reader.askEnemyIndex(enemiesInRange.size());
        if (idx < 0 || static_cast<size_t>(idx) >= enemiesInRange.size()) {
            std::cout << "Нет такого врага!\n";
            return true;
        }
        Position targetPos = enemiesInRange[static_cast<size_t>(idx)]->getPosition();
        controller.turnPlayer(targetPos, "Far", 0);
        if (_bus) {
            _bus->publish("Игрок атаковал дальним боем врага в (" +
                std::to_string(targetPos.getRow()) + ", " + std::to_string(targetPos.getColumn()) + ")");
        }
        visualizer.render(game, controller);
        break;
    }
    case CommandType::BuySpell: {
        Player& player = game.getPlayer();
        Hand& hand = game.getHand();
        const int costDirect = 100;
        const int costArea = 200;
        int score = player.get_score();

        if (cmd.spellName == "Direct Damage" && score >= costDirect && hand.size() < 5) {
            player.set_score(score - costDirect);
            hand.addSpell(std::make_unique<DirectDamageSpell>(10, 3));
            std::cout << "Вы купили заклинание 'Direct Damage'.\n";
            if (_bus) _bus->publish("Игрок купил заклинание Direct Damage");
        } else if (cmd.spellName == "Area Damage" && score >= costArea && hand.size() < 5) {
            player.set_score(score - costArea);
            hand.addSpell(std::make_unique<AreaDamageSpell>(5, 2));
            std::cout << "Вы купили заклинание 'Area Damage'.\n";
            if (_bus) _bus->publish("Игрок купил заклинание Area Damage");
        } else {
            std::cout << "Покупка невозможна.\n";
        }
        break;
    }
    case CommandType::SaveGame: {
        game.saveToFile(cmd.filename);
        std::cout << "Игра сохранена в '" << cmd.filename << "'.\n";
        if (_bus) _bus->publish("Игра сохранена в " + cmd.filename);
        break;
    }
    case CommandType::LoadGame: {
        try {
            game.loadFromFile(cmd.filename);
            game.restartLevelAfterLoad();
            std::cout << "Игра загружена из '" << cmd.filename << "'.\n";
            if (_bus) _bus->publish("Игра загружена из " + cmd.filename);
            visualizer.render(game, controller);
        } catch (const std::exception& e) {
            std::cout << "Не удалось загрузить: " << e.what() << "\n";
        }
        break;
    }
    case CommandType::ExitGame: {
        game.getPlayer().change_health(0);
        if (_bus) _bus->publish("Игрок завершил игру");
        return false;
    }
    case CommandType::None:
    default:
        std::cout << "Неизвестная команда. Ход пропущен.\n";
        break;
    }

    return true;
}
