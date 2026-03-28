#pragma once
#include <ncurses.h>
#include <vector>
#include <string>
#include <Spell.hpp>
#include <Gauntlet.hpp>
#include <Field.hpp>
#include <Save.hpp>

class Menu {
public:
    Menu(int yMax, int xMax);

    int init(Field& field);
    bool openPauseMenu(Field& field);
    static int openSpellMenu(std::vector<std::shared_ptr<Spell>> spells);
    static bool openLearnMenu(Gauntlet& gauntlet);
    void gameOver();
    void refreshSaves();
    const std::vector<Save>& getSavesList() const;
    int openSaves(int option, Field& field);

private:
    void drawStartMenu();
    void drawPauseMenu();
    std::string inputSaveName(int step);
    bool saveGame(Field& field, std::string filename);
    bool loadGame(Field& field, std::string filename);

    int yMax;
    int xMax;
    std::vector<Save> savesList;
    
    std::vector<std::string> options;
    std::vector<std::string> optionsPauseMenu;
    std::vector<std::string> elements;
    int selected = 0;
};