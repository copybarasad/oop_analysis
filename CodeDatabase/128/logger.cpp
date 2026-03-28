#include "logger.hpp"
#include "windows.h"
#include <iostream>

Logger::Logger(LogOutput output, const std::string& filename)
    : output_(output)
{
    switch (output_) {
        case LogOutput::None:
            std::cout << "Log mode: NONE" << std::endl;
            break;

        case LogOutput::Console:
            std::cout << "Log mode: CONSOLE" << std::endl;
            break;

        case LogOutput::File:
            std::cout << "Log mode: FILE" << std::endl;
            break;

        case LogOutput::Both:
            std::cout << "Log mode: BOTH (console + file)" << std::endl;
            break;
    }

    if (output_ == LogOutput::File || output_ == LogOutput::Both) {
        std::cout << "Trying to open file: " << filename << std::endl;
        file_.open(filename);

        if (file_.is_open()) {
            std::cout << "✔ Log file opened OK" << std::endl;
        } else {
            std::cout << "❌ FAILED to open log file" << std::endl;
        }
    }

    if (output_ != LogOutput::None) {
        Subscribe();
    }
}


Logger::~Logger() {
    if (file_.is_open()) {
        file_.close();
    }
}

void Logger::Write(const std::string& text) {
    if (output_ == LogOutput::None)
        return;

    if (output_ == LogOutput::Console || output_ == LogOutput::Both) {
        std::cout << "[LOG] " << text << "\n";
    }

    if ((output_ == LogOutput::File || output_ == LogOutput::Both) && file_.is_open()) {
        file_ << text << "\n";
    }
}


void Logger::Subscribe() {
    auto& disp = EventDispatcher::instance();

    disp.subscribe<DamageEvent>([this](const DamageEvent& ev) { OnDamage(ev); });
    disp.subscribe<MoveEvent>([this](const MoveEvent& ev) { OnMove(ev); });
    disp.subscribe<DeadEvent>([this](const DeadEvent& ev) { OnDead(ev); });
    disp.subscribe<SpellUsedEvent>([this](const SpellUsedEvent& ev) { OnSpellUsed(ev); });
    disp.subscribe<UiMessageEvent>([this](const UiMessageEvent& ev) { OnUiMessage(ev); });
}

void Logger::OnDamage(const DamageEvent& ev) {
    Write(
        "Damage: attacker=" + ev.attacker.toString() +
        " target=" + ev.target.toString() +
        " dmg=" + std::to_string(ev.damage) +
        " HP: " + std::to_string(ev.oldHp) + "→" + std::to_string(ev.newHp)
    );
}

void Logger::OnDead(const DeadEvent& ev) {
    Write("Death: " + ev.entity.toString());
}

void Logger::OnMove(const MoveEvent& ev) {
    Write(
        "Move: " + ev.entity.toString() +
        " (" + std::to_string(ev.from.first) + "," + std::to_string(ev.from.second) +  
        ") → (" +
        std::to_string(ev.to.first) + "," + std::to_string(ev.to.second) + ")" 
    );
}

void Logger::OnSpellUsed(const SpellUsedEvent& ev) {
    std::string spellTypeStr;
    switch (ev.type) {
        case SpellType::kDirectDamage: spellTypeStr = "DirectDamage"; break;
        case SpellType::kAreaDamage:   spellTypeStr = "AreaDamage"; break;
        case SpellType::kTrapped:      spellTypeStr = "Trapped"; break;
        case SpellType::kSummon:       spellTypeStr = "Summon"; break;
        case SpellType::kEnhancement:  spellTypeStr = "Enhancement"; break;
        default:                       spellTypeStr = "Unknown"; break;
    }
    Write(
        "SpellUsed: caster=" + ev.caster.toString() +
        " type=" + spellTypeStr +
        " at (" + std::to_string(ev.x) + "," + std::to_string(ev.y) + ")" 
    );
}

void Logger::OnUiMessage(const UiMessageEvent& ev) {
    Write("UI: " + ev.text);
}