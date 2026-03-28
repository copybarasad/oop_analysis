#pragma once

#include "fight.h"
#include <string>

namespace game {
    class GameConfig;
    class Field;
    class Player;
    class ISpell;
    // === ANSI colors ===
    namespace Color {
        inline constexpr const char* Default    = "\033[0m";

        inline constexpr const char* Player     = "\033[38;2;180;140;255m";
        inline constexpr const char* Enemy      = "\033[38;2;255;120;140m";
        inline constexpr const char* Corpse     = "\033[38;2;190;190;205m";
        inline constexpr const char* Wall       = "\033[38;2;160;160;220m";
        inline constexpr const char* Slime      = "\033[38;2;150;230;170m";

        inline constexpr const char* Text       = "\033[38;2;242;242;252m";
        inline constexpr const char* Title      = "\033[38;2;200;130;235m";
        inline constexpr const char* Accent     = "\033[38;2;235;150;235m";

        inline constexpr const char* Border     = "\033[38;2;185;175;225m";
    }

    /// @brief Manipulate with output in console
    class ConsoleUI {
        public:
            void clearScreen() const;

            void drawField(const Field& f) const;
            void drawHUD(const Field& f) const;

            void printFieldSizeHeader() const;

            void printMainMenu() const;

            void printHelpMessage() const;

            void printWinMessage(int level)  const;
            void printLoseMessage(int level) const;

            void printRestartMessage() const;
            void printSkipMessage() const;

            void printLoadError(const std::string context) const;
            void printSaveError(const std::string context) const;

            void printLoadSuccess() const;
            void printSaveSuccess() const;
            
            void printLevelUpMenu(const GameConfig& cfg) const;
            void printAboutSpellUpgrade(const ISpell& spell) const;
            void printNoSpellsToUpgrade() const;

            void printLevelBanner(int level, int fieldWidth) const;
        private:
            static const std::string getSpells(const Player& p);

            static const std::string fModeToString(FightMode fmode) {
                switch(fmode) { case FightMode::Melee:  return "Melee";
                                case FightMode::Ranged: return "Ranged";
                                default:                return "Unknown";
                }
            }
    };
}