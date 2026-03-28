#ifndef COMMAND_H
#define COMMAND_H

#include "ICommand.h"
#include <string>
#include <memory>
#include <vector>

// Базовый класс для всех команд
class BaseCommand : public ICommand {
protected:
    std::string m_description;

public:
    std::string getDescription() const override { return m_description; }
};

// Команда перемещения
class MoveCommand : public BaseCommand {
private:
    int m_deltaX;
    int m_deltaY;

public:
    MoveCommand(int deltaX, int deltaY) : m_deltaX(deltaX), m_deltaY(deltaY) {
        m_description = "Move " + std::string(deltaX < 0 ? "left" : deltaX > 0 ? "right" : "") +
            (deltaY != 0 ? std::string(deltaY < 0 ? "up" : "down") : "");
    }

    void execute(GameSession& session) override;
};

// Команда атаки
class AttackCommand : public BaseCommand {
public:
    AttackCommand() { m_description = "Attack"; }
    void execute(GameSession& session) override;
};

// Команда смены типа атаки
class SwitchAttackCommand : public BaseCommand {
public:
    SwitchAttackCommand() { m_description = "Switch attack type"; }
    void execute(GameSession& session) override;
};

// Команда каста заклинания
class CastSpellCommand : public BaseCommand {
private:
    int m_spellIndex;

public:
    CastSpellCommand(int spellIndex) : m_spellIndex(spellIndex) {
        m_description = "Cast spell at slot " + std::to_string(spellIndex);
    }

    void execute(GameSession& session) override;
};

// Команда сохранения
class SaveCommand : public BaseCommand {
public:
    SaveCommand() { m_description = "Save game"; }
    void execute(GameSession& session) override;
};

// Команда загрузки
class LoadCommand : public BaseCommand {
public:
    LoadCommand() { m_description = "Load game"; }
    void execute(GameSession& session) override;
};

// Команда выхода
class QuitCommand : public BaseCommand {
public:
    QuitCommand() { m_description = "Quit game"; }
    void execute(GameSession& session) override;
};

// Команда информации
class InfoCommand : public BaseCommand {
public:
    InfoCommand() { m_description = "Show game info"; }
    void execute(GameSession& session) override;
};

// Фабрика команд
class CommandFactory {
public:
    static std::unique_ptr<ICommand> createCommand(char inputChar);
};

#endif // COMMAND_H