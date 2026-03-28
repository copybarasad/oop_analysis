#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include "enums.hpp"
#include "event_dispatcher.hpp"
#include "events.hpp"

class Logger {
public:
    Logger(LogOutput output, const std::string& filename = "game_log.txt");
    ~Logger();

private:
    LogOutput output_;
    std::ofstream file_;

    void Write(const std::string& text);
    void Subscribe();

    void OnDamage(const DamageEvent& ev);
    void OnDead(const DeadEvent& ev);
    void OnMove(const MoveEvent& ev);
    void OnSpellUsed(const SpellUsedEvent& ev);
    void OnUiMessage(const UiMessageEvent& ev);
};
