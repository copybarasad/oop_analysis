#ifndef OOP_LOGGER_H_
#define OOP_LOGGER_H_

#include "game_event.h"
#include <fstream>
#include <iostream>
#include <string>
#include <chrono>
#include <iomanip>
#include <sstream>

namespace rpg {
    class Logger : public IEventListener {
    public:
        ~Logger() override = default;

        void OnEvent(const GameEvent& event) override {
            std::string log_message = FormatEvent(event);
            WriteLog(log_message);
        }

    protected:
        [[nodiscard]] virtual std::string FormatEvent(const GameEvent& event) const {
            std::ostringstream oss;
            
            oss << "[" << GetTimestamp() << "] ";
            oss << "[" << EventTypeToString(event.type) << "] ";
            oss << event.description;

            if (!event.data.empty()) {
                oss << " {";
                bool first = true;
                for (const auto& [key, value] : event.data) {
                    if (!first) oss << ", ";
                    oss << key << "=" << value;
                    first = false;
                }
                oss << "}";
            }
            
            return oss.str();
        }

        virtual void WriteLog(const std::string& message) = 0;

    private:
        [[nodiscard]] std::string GetTimestamp() const {
            auto now = std::chrono::system_clock::now();
            auto time = std::chrono::system_clock::to_time_t(now);
            std::ostringstream oss;
#ifdef _WIN32
            struct tm timeinfo;
            localtime_s(&timeinfo, &time);
            oss << std::put_time(&timeinfo, "%Y-%m-%d %H:%M:%S");
#else
            oss << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S");
#endif
            return oss.str();
        }

        [[nodiscard]] std::string EventTypeToString(GameEventType type) const {
            switch (type) {
                case GameEventType::kPlayerMoved: return "PLAYER_MOVED";
                case GameEventType::kPlayerDamaged: return "PLAYER_DAMAGED";
                case GameEventType::kEnemyDamaged: return "ENEMY_DAMAGED";
                case GameEventType::kEnemyKilled: return "ENEMY_KILLED";
                case GameEventType::kAllyDamaged: return "ALLY_DAMAGED";
                case GameEventType::kAllyKilled: return "ALLY_KILLED";
                case GameEventType::kSpellCast: return "SPELL_CAST";
                case GameEventType::kSpellBought: return "SPELL_BOUGHT";
                case GameEventType::kTrapActivated: return "TRAP_ACTIVATED";
                case GameEventType::kBuildingSpawned: return "BUILDING_SPAWNED";
                case GameEventType::kTowerAttacked: return "TOWER_ATTACKED";
                case GameEventType::kCombatModeChanged: return "COMBAT_MODE_CHANGED";
                case GameEventType::kLevelCompleted: return "LEVEL_COMPLETED";
                case GameEventType::kGameOver: return "GAME_OVER";
                case GameEventType::kGameSaved: return "GAME_SAVED";
                case GameEventType::kGameLoaded: return "GAME_LOADED";
                default: return "UNKNOWN";
            }
        }
    };

    class FileLogger final : public Logger {
    public:
        explicit FileLogger(const std::string& filename) 
            : filename_(filename) {
            file_.open(filename_, std::ios::app);
            if (!file_.is_open()) {
                std::cerr << "Ошибка: не удалось открыть файл лога: " << filename_ << "\n";
            } else {
                file_ << "\n=== Новая сессия игры ===\n";
                file_.flush();
            }
        }

        ~FileLogger() override {
            if (file_.is_open()) {
                file_ << "=== Конец сессии ===\n\n";
                file_.close();
            }
        }

        FileLogger(const FileLogger&) = delete;
        FileLogger& operator=(const FileLogger&) = delete;

    protected:
        void WriteLog(const std::string& message) override {
            if (file_.is_open()) {
                file_ << message << "\n";
                file_.flush();
            }
        }

    private:
        std::string filename_;
        std::ofstream file_;
    };

    class ConsoleLogger final : public Logger {
    public:
        ConsoleLogger() = default;
        ~ConsoleLogger() override = default;

    protected:
        void WriteLog(const std::string& message) override {
            std::cout << "[LOG] " << message << "\n";
        }
    };

    class MultiLogger final : public IEventListener {
    public:
        MultiLogger() = default;
        ~MultiLogger() override = default;

        void AddLogger(IEventListener* logger) {
            if (logger) {
                loggers_.push_back(logger);
            }
        }

        void OnEvent(const GameEvent& event) override {
            for (auto* logger : loggers_) {
                logger->OnEvent(event);
            }
        }

    private:
        std::vector<IEventListener*> loggers_;
    };
}

#endif

