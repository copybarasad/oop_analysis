#ifndef MENUSYSTEM_H
#define MENUSYSTEM_H

#include <string>
#include <vector>
#include <iostream>

class Player;
class PlayerUpgradeSystem;
struct Upgrade;

/**
 * @file MenuSystem.h
 * @brief Система меню игры (ОБНОВЛЕНО)
 */

class MenuSystem {
public:
    enum class MainMenuChoice {
        NEW_GAME,
        LOAD_GAME,
        EXIT
    };
    
    enum class DefeatMenuChoice {
        NEW_GAME,
        EXIT
    };
    
    static MainMenuChoice showMainMenu();
    static std::string showLoadMenu();
    static const Upgrade* showUpgradeMenu(const PlayerUpgradeSystem* upgradeSystem);
    static DefeatMenuChoice showDefeatMenu();
    static bool askNextLevel();
    
    
    static bool askSaveBeforeExit();
    static std::string askSaveName();
    
private:
    static int getChoice(int min, int max);
    static std::string getString();
};

#endif // MENUSYSTEM_H


