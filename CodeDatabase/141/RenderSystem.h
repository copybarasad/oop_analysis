#ifndef RENDER_SYSTEM_H
#define RENDER_SYSTEM_H

#include "Field.h"
#include "Player.h"
#include "Enemy.h"
#include "EnemyBuilding.h"
#include "EnemyTower.h"
#include "Trap.h"
#include "Ally.h"
#include "Logger.h"
#include "ControlMapping.h"
#include <vector>
#include <deque>
#include <string>
#include <iostream>

class ILogSink;

class RenderSystem {
private:
    static const int MAX_MESSAGES = 10;
    mutable std::deque<std::string> messageLog;
    mutable std::vector<std::string> currentTurnMessages;
    ILogSink *sink = nullptr;
    bool consoleLogEnabled = true;
    const ControlMapping *controls = nullptr;

public:
    RenderSystem();

    void render(const Field &field,
                const Player &player,
                const std::vector<Enemy> &enemies,
                const std::vector<EnemyBuilding> &buildings,
                const std::vector<EnemyTower> &towers,
                const std::vector<Trap> &traps,
                const std::vector<Ally> &allies,
                int enemiesKilled,
                int scoreTarget,
                int currentLevel) const;

    void addMessage(const std::string &message) const;

    void clearCurrentTurnMessages();

    // Мгновенный вывод подсказок для ввода (без буфера логов).
    void promptInput(const std::string &text) const;

    void displaySpellSelection(const Hand &hand) const;

    void setLogSink(ILogSink *sinkPtr);

    void setConsoleLogEnabled(bool enabled);

    void setControls(const ControlMapping *mapping);
};

#endif
