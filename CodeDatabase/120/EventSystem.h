#pragma once
#include <fstream>
#include <optional>
#include <vector>
#include <filesystem>
#include <iostream>
enum class LogTarget {
    Screen,
    File
};

struct DamageEvent {
    std::string target;
    int damage;
    int x, y;
};

struct SpellEvent {
    std::string spellName;
    std::optional<int> x, y;
};

struct MoveEvent {
    std::string entityName;
    int x, y;
};

struct GameMessageEvent {
    std::string message;
};

class EventSystem {
    LogTarget target = LogTarget::Screen;
    std::ofstream file;
    std::vector<std::string> recent;
    size_t maxRecent = 15;
    void Push(const std::string& text);
public:
    EventSystem() = default;
    ~EventSystem() { if(file.is_open()) file.close(); }
    std::vector<std::string>& GetRecentEvents() { return recent; }
    const std::vector<std::string>& GetRecentEvents() const { return recent; }
    void SetLogTarget(LogTarget t, const std::string& filename = "");
    LogTarget GetTarget() const { return target; }
    void Notify(const DamageEvent& e);
    void Notify(const SpellEvent& e);
    void Notify(const MoveEvent& e);
    void Notify(const GameMessageEvent& e);
};
