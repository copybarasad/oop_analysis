#ifndef SETTINGS_H
#define SETTINGS_H

#include <string>

class Settings {
public:
    char moveUp;
    char moveDown;
    char moveLeft;
    char moveRight;
    char switchCombatMode;
    char attack;
    char castSpell;
    char buySpell;
    char saveGame;
    char loadGame;
    char help;
    char exitGame;
    char startNewGame;

    Settings();

    static Settings createDefault();
    bool isValid() const;

};

#endif

