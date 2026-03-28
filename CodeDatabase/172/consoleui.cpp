#include "consoleui.h"
#include "field.h"
#include "player.h"
#include "fight.h"
#include "ispell.h"
#include "gameconfig.h"

#include <iostream>

namespace game {
    void ConsoleUI::clearScreen() const 
    {
        #ifdef _WIN32
        system("cls");
        #else
        std::cout << "\033[2J\033[H";
        #endif
    }

    void ConsoleUI::drawField(const Field& f) const
    {   
        // First border line (walls)
        std::cout << Color::Border;

        for (int x = 0; x < f.getW() + 2; ++x) {
            std::cout << Icons::Wall << ' ';
        }
        std::cout << Color::Default << '\n';

        // Main field
        for (int y = 0; y < f.getH(); ++y) {
            // Left wall - border
            std::cout << Color::Border
                      << Icons::Wall
                      << Color::Default << ' ';

            // Current line
            for (int x = 0; x < f.getW(); ++x) {
                char currentIcon = f.iconAt(Pos(x, y));

                switch (currentIcon) {
                    case Icons::Enemy:      std::cout << Color::Enemy;  break;
                    case Icons::Player:     std::cout << Color::Player; break;
                    case Icons::Corpse:     std::cout << Color::Corpse; break;

                    case Icons::Wall:       std::cout << Color::Wall;   break;
                    case Icons::Slime:      std::cout << Color::Slime;  break;

                    case Icons::EBuilding:  std::cout << Color::Accent; break;
                    case Icons::ETower:     std::cout << Color::Enemy;  break;

                    default:                std::cout << Color::Text;   break;
                }

                if (f.getCell(x, y).hasTrap()) {
                    if (currentIcon == Icons::Floor) {
                        std::cout << Color::Accent
                                  << Icons::Trap << ' '
                                  << Color::Default;
                        continue;
                    }
                }
                std::cout << currentIcon << ' ' << Color::Default;
            }

            // Right wall - border
            std::cout << Color::Border
                      << Icons::Wall
                      << Color::Default << '\n';
        }

        // Last border line (walls)
        std::cout << Color::Border;

        for (int x = 0; x < f.getW() + 2; ++x) {
            std::cout << Icons::Wall << ' ';
        }
        std::cout << Color::Default << '\n';
    }

    void ConsoleUI::drawHUD(const Field& f) const 
    {
        const Player& p = f.getPlayer();

        const char* hpColor = Color::Default;
        if (p.getHP() < 20) { hpColor = Color::Enemy; }

        // Main info
        std::cout << Color::Title << "\nTurn:  "
                  << Color::Text << f.getTurn()

                  << Color::Title << "\nHP:    " 
                  << Color::Text << hpColor << p.getHP()

                  << Color::Title << "\nPower: " 
                  << Color::Text << p.getDamage()

                  << Color::Title << "\nMode:  " 
                  << Color::Text << fModeToString(p.getFightMode())

                  << Color::Title << "\n\nYour spells: " 
                  << Color::Text << getSpells(p)

                  << Color::Title << "\n\nAlive enemies: " 
                  << Color::Text << f.enemiesAlive()

                  << Color::Title << "\n\nSpawn new enemy in: " 
                  << Color::Text << f.getNextSpawnTurn();

        // List of enemies
        std::cout << Color::Accent << "\n\n[ Enemies ]"
                  << Color::Default << '\n';

        int cnt = 0;
        for (const Enemy& e: f.getEnemies()) {
            std::cout << Color::Enemy << "\n(Enemy " << cnt+1 << ") "
                      << Color::Title << "\tHP: " << Color::Text
                      << e.getHP()
                      << Color::Title << " \tPower: "   << Color::Text
                      << e.getDamage();
            cnt++;
        }
    }

    void ConsoleUI::printFieldSizeHeader() const
    {   
        clearScreen();

        std::cout << Color::Slime << "[FIELD SIZE]\n\n"
                  << Color::Default
                  << "\tAvailable from "
                  << Color::Slime   << "10x10"
                  << Color::Default << " to "
                  << Color::Slime   << "25x25"
                  << Color::Default << "\n\n";
    }

    void ConsoleUI::printMainMenu() const {
        // Header
        std::cout << '\n'
                  << Color::Title
                  << "\t\t\t\t\tMAIN MENU\n\n"
                  << Color::Default;
        
        // Top-line
        std::cout << Color::Border
                  << "   / - - - - - - - - - - - \\"
                  << "      / - - - - - - - - - - - \\"
                  << "      / - - - - - - - - - - - \\" << '\n';

        // Line with buttons
        std::cout << Color::Border << "   | "
                  << Color::Text   << "[N] New Game          "
                  << Color::Border << "|"
                  << "      | "
                  << Color::Text   << "[L] Load Game         "
                  << Color::Border << "|"
                  << "      | "
                  << Color::Text   << "[Q] Quit              "
                  << Color::Border << "|" << '\n';
        
        // Bottom-line
        std::cout << Color::Border
                  << "   \\ - - - - - - - - - - - /"
                  << "      \\ - - - - - - - - - - - /"
                  << "      \\ - - - - - - - - - - - /" << '\n'
                  << Color::Default << "\n\n\n";
        
        // Footer
        std::cout << Color::Text << "Your choice: " << Color::Default;
    }

    void ConsoleUI::printHelpMessage() const 
    {
        clearScreen();
        std::cout << Color::Title << "[ENEMIES vs. YOU]" 
                  << Color::Accent << " by lilprovod"
                  << Color::Default << "\n\n";

        std::cout << Color::Accent << "[!] Control:" << Color::Default << "\n\n";

        std::cout << Color::Title  << "\tWASD" << Color::Default << " -- move\n";
        std::cout << Color::Title  << "\tF"    << Color::Default << " -- attack\n";
        std::cout << Color::Title  << "\tM"    << Color::Default << " -- change fight mode\n";
        std::cout << Color::Title  << "\t1-9"  << Color::Default << " -- use spells\n";
        std::cout << Color::Enemy  << "\tQ"    << Color::Default << " -- quit the game\n\n";

        std::cout << Color::Title  << "\tK"    << Color::Default << " -- save game\n";
        std::cout << Color::Title  << "\tL"    << Color::Default << " -- load game\n";

        std::cout << Color::Accent << "[$] Icons:" << Color::Default << "\n\n";

        std::cout << "\t[ " << Color::Player << Icons::Player << Color::Default << " ] -- YOU!\n";
        std::cout << "\t[ " << Color::Enemy  << Icons::Enemy  << Color::Default << " ] -- enemy\n";
    std::cout << "\t[ " << Color::Accent  << Icons::EBuilding << Color::Default << " ] -- enemy building\n";
        std::cout << "\t[ " << Color::Wall   << Icons::Wall   << Color::Default << " ] -- wall\n";
        std::cout << "\t[ " << Color::Slime  << Icons::Slime  << Color::Default << " ] -- slime\n";
        std::cout << "\t[ " << Color::Corpse << Icons::Corpse << Color::Default << " ] -- corpse\n\n";
        std::cout << "\t[ " << Color::Accent << Icons::Trap   << Color::Default << " ] -- placed trap\n";
        std::cout << "\t[ " << Color::Enemy  << Icons::ETower << Color::Default << " ] -- enemy tower\n";

        std::cout << Color::Accent << "[=] Menu:" << Color::Default << "\n\n";

        std::cout << "(1)\t" << "Press " << Color::Title << "any key"   << Color::Default
                                << " and " << Color::Title << "<ENTER>" << Color::Default << " to start game...\n";
        std::cout << "(2)\t" << "Press " << Color::Title <<   "<Q>"   << Color::Default
                                << " and " << Color::Title << "<ENTER>" << Color::Default << " to quit!\n";

        std::cout << "(3)\t" << "Press " << Color::Title <<   "<M>"   << Color::Default
                                << " and " << Color::Title << "<ENTER>" << Color::Default
                                << " to set field size (default 10x10).\n";

        std::cout << "\tYour choice: ";
    }

    void ConsoleUI::printWinMessage(int level) const 
    {
        clearScreen();

        std::cout << Color::Slime;

        std::cout << "=======================================\n";
        std::cout << "=                                     =\n";
        std::cout << "=   "
                  << "HANDSOME! YOU'VE PASSED LEVEL " + std::to_string(level)
                  << "   =\n";
        std::cout << "=                                     =\n";
        std::cout << "=======================================\n\n\n";

        std::cout << Color::Default;
    }

    void ConsoleUI::printLoseMessage(int level) const 
    {
        clearScreen();

        std::cout << Color::Enemy;

        std::cout << "=======================================\n";
        std::cout << "=                                     =\n";
        std::cout << "=  "
                  << "SAD! YOU'VE DIDN'T PASSED LEVEL " + std::to_string(level)
                  << "  =\n";
        std::cout << "=                                     =\n";
        std::cout << "=======================================\n\n\n";

        std::cout << Color::Default;
    }

    void ConsoleUI::printRestartMessage() const 
    {
        std::cout << Color::Border << "Press <R> to restart or ANY OTHER KEY to quit: ";
    }

    void ConsoleUI::printSkipMessage() const 
    {   
        std::cout << Color::Text   << "\nPress "
                  << Color::Player << "any key"
                  << Color::Text   << " and "
                  << Color::Player << "<ENTER>"
                  << Color::Text   << " to continue...\n";
    }

    void ConsoleUI::printLoadError(const std::string context) const
    {
        clearScreen();
        std::cout << Color::Enemy << "LoadError: " << context << "\n";
    }

    void ConsoleUI::printSaveError(const std::string context) const
    {
        clearScreen();
        std::cout << Color::Enemy << "SaveError: " << context << "\n";
    }

    void ConsoleUI::printLoadSuccess() const
    {
        clearScreen();
        std::cout << Color::Slime << "Game sucessfully loaded!\n";
    }

    void ConsoleUI::printSaveSuccess() const
    {
        clearScreen();
        std::cout << Color::Slime << "Game successfully saved!\n";
    }

    void ConsoleUI::printLevelUpMenu(const GameConfig& cfg) const
    {
        int hp     = cfg.hpUpgradeValue;
        int damage = cfg.damageUpgradeValue;

        std::cout << Color::Title << "[ LEVEL UP ]\n\n" << Color::Default;

        std::cout << Color::Text << "You've passed level! Choose upgrade:\n\n";

        std::cout << Color::Title << "  [1] "
                  << Color::Text  << "+" << hp     << " HP\n";
        
        std::cout << Color::Title << "  [2] "
                  << Color::Text  << "+" << damage << " damage\n";
        
        
        std::cout << Color::Title  << "  [3] "
                  << Color::Text   << "Upgrade random spell...\n\n";
        

        std::cout << Color::Accent << "Your choice: " << Color::Default;
    }

    void ConsoleUI::printAboutSpellUpgrade(const ISpell& spell) const
    {
        std::cout << '\n'
                  << Color::Accent << "Spell upgraded:\n\n"
                  << Color::Border << "\t"    << spell.name()
                  << Color::Text   << " (D: " << spell.getDamage()
                                   << ", R: " << spell.getRadius()
                                   << ")\n";
    }

    void ConsoleUI::printNoSpellsToUpgrade() const
    {
        std::cout << '\n'
                  << Color::Title << "No spells to upgrade at your hand. Maybe spells are disabled?\n";
    }

    void ConsoleUI::printLevelBanner(int level, int fieldWidth) const 
    {   
        std::string banner = " Level " + std::to_string(level) + " ";

        int bannerWidth = fieldWidth * 2 + 4;

        if ((int)(banner.size()) >= bannerWidth) {
            std::cout << banner << "\n\n";
            return;
        }

        int totalPadding = bannerWidth - (int)(banner.size());
        int left  = totalPadding / 2;
        int right = totalPadding / 2;

        std::cout << Color::Border << std::string(left,  '=')
                  << Color::Title  << banner
                  << Color::Border << std::string(right, '=')
                  << "\n\n";
    }

    const std::string ConsoleUI::getSpells(const Player& p)
    {
        std::string spellString;

        const PlayerHand& hand = p.getHand();
        if (hand.empty()) return " (no spells)";
        
        const size_t size      = hand.size();
        const size_t perLine   = 3;

        for (size_t i = 0; i < size; i++) {
            const ISpell* spell = hand.at(i);
            if (!spell) continue;

            if (!spellString.empty() && ((i + 1) % 4 != 0)) spellString += " | "; // divider

            spellString += "[" + std::to_string(i + 1) + "] "
                        +  Color::Border + spell->name() + Color::Default
                        + " (D: "  + std::to_string(spell->getDamage())
                        + ", R: "  + std::to_string(spell->getRadius())
                        + ")";

            if ( (i + 1) % perLine == 0 && i + 1 < size ) {
                spellString += "\n             ";
            }
        }

        return spellString;
    }
}