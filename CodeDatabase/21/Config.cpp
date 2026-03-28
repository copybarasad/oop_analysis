#include "Config.hpp"


int GlobalGameConfig::fieldWidth = 25;
int GlobalGameConfig::fieldHeight = 25;
int GlobalGameConfig::gameLevel = 1;
const int GlobalGameConfig::enemyAttack = 10;
const int GlobalGameConfig::enemyHealth = 25;
const int GlobalGameConfig::barracksHealth = 100;
int GlobalGameConfig::difficulty = GlobalGameConfig::EASY;

int GlobalMenuSelector::mainMenuSelected = 0;
const int GlobalMenuSelector::mainMenuMod = 4;
int GlobalMenuSelector::levelUpSelected = 0;
const int GlobalMenuSelector::levelUpMenuMod = 3;
int GlobalMenuSelector::pauseMenuSelected = 0;
const int GlobalMenuSelector::pauseMenuMod = 4;
int GlobalMenuSelector::gameOverSelected = 0;
const int GlobalMenuSelector::gameOverMod = 3;
int GlobalMenuSelector::loadMenuSelected = 0;
int GlobalMenuSelector::startListLoadMenu = 0;
int GlobalMenuSelector::endListLoadMenu = 10;
bool GlobalMenuSelector::firstCallLoadMenu = false;

std::string GlobalMenuSelector::errMessage = "";