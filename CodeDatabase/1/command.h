#pragma once

#include <string>

class Game;
class EventSystem;
struct Position;

struct CommandResult {
    bool success;
    bool turnConsumed;
    std::string message;
    
    CommandResult(bool s = false, bool t = false, const std::string& msg = "");
};

class Command {
public:
    virtual ~Command() = default;
    
    virtual CommandResult execute(Game& game, EventSystem* event_system = nullptr) = 0;
    virtual bool consumesTurn() const = 0;
    virtual std::string getName() const = 0;
    virtual std::string getDescription() const = 0;
};

// Базовые классы-категории
class TurnConsumingCommand : public Command {
public:
    bool consumesTurn() const override { return true; }
};

class InstantCommand : public Command {
public:
    bool consumesTurn() const override { return false; }
};

// Команды движения
class MoveUpCommand : public TurnConsumingCommand {
public:
    CommandResult execute(Game& game, EventSystem* event_system = nullptr) override;
    std::string getName() const override { return "MOVE_UP"; }
    std::string getDescription() const override { return "Движение вверх"; }
};

class MoveDownCommand : public TurnConsumingCommand {
public:
    CommandResult execute(Game& game, EventSystem* event_system = nullptr) override;
    std::string getName() const override { return "MOVE_DOWN"; }
    std::string getDescription() const override { return "Движение вниз"; }
};

class MoveLeftCommand : public TurnConsumingCommand {
public:
    CommandResult execute(Game& game, EventSystem* event_system = nullptr) override;
    std::string getName() const override { return "MOVE_LEFT"; }
    std::string getDescription() const override { return "Движение влево"; }
};

class MoveRightCommand : public TurnConsumingCommand {
public:
    CommandResult execute(Game& game, EventSystem* event_system = nullptr) override;
    std::string getName() const override { return "MOVE_RIGHT"; }
    std::string getDescription() const override { return "Движение вправо"; }
};

// Боевые команды
class AttackCommand : public TurnConsumingCommand {
public:
    CommandResult execute(Game& game, EventSystem* event_system = nullptr) override;
    std::string getName() const override { return "ATTACK"; }
    std::string getDescription() const override { return "Атака Дракулы"; }
};

class SwitchCombatCommand : public TurnConsumingCommand {
public:
    CommandResult execute(Game& game, EventSystem* event_system = nullptr) override;
    std::string getName() const override { return "SWITCH_COMBAT"; }
    std::string getDescription() const override { return "Смена режима боя"; }
};

class CastSpellCommand : public TurnConsumingCommand {
public:
    CommandResult execute(Game& game, EventSystem* event_system = nullptr) override;
    std::string getName() const override { return "CAST_SPELL"; }
    std::string getDescription() const override { return "Применение заклинания"; }
};

// UI команды
class ShowStatusCommand : public InstantCommand {
public:
    CommandResult execute(Game& game, EventSystem* event_system = nullptr) override;
    std::string getName() const override { return "SHOW_STATUS"; }
    std::string getDescription() const override { return "Показать статус"; }
};

class LookAroundCommand : public InstantCommand {
public:
    CommandResult execute(Game& game, EventSystem* event_system = nullptr) override;
    std::string getName() const override { return "LOOK_AROUND"; }
    std::string getDescription() const override { return "Показать карту"; }
};

class ShowSpellsCommand : public InstantCommand {
public:
    CommandResult execute(Game& game, EventSystem* event_system = nullptr) override;
    std::string getName() const override { return "SHOW_SPELLS"; }
    std::string getDescription() const override { return "Показать заклинания"; }
};

class HelpCommand : public InstantCommand {
public:
    CommandResult execute(Game& game, EventSystem* event_system = nullptr) override;
    std::string getName() const override { return "HELP"; }
    std::string getDescription() const override { return "Показать помощь"; }
};

// Системные команды
class SaveGameCommand : public InstantCommand {
public:
    CommandResult execute(Game& game, EventSystem* event_system = nullptr) override;
    std::string getName() const override { return "SAVE_GAME"; }
    std::string getDescription() const override { return "Сохранить игру"; }
};

class LoadGameCommand : public InstantCommand {
public:
    CommandResult execute(Game& game, EventSystem* event_system = nullptr) override;
    std::string getName() const override { return "LOAD_GAME"; }
    std::string getDescription() const override { return "Загрузить игру"; }
};

class QuitCommand : public InstantCommand {
public:
    CommandResult execute(Game& game, EventSystem* event_system = nullptr) override;
    std::string getName() const override { return "QUIT"; }
    std::string getDescription() const override { return "Выйти из игры"; }
};

class LibraryInteractCommand : public InstantCommand {
public:
    CommandResult execute(Game& game, EventSystem* event_system = nullptr) override;
    std::string getName() const override { return "LIBRARY_INTERACT"; }
    std::string getDescription() const override { return "Взаимодействие с библиотекой"; }
};