#include <Menu.hpp>
#include <Utils.hpp>
#include <Constants.hpp>
#include <SiegeArrow.hpp>
#include <RainOfArrows.hpp>
#include <PlaceTrapSpell.hpp>
#include <SummonDecoySpell.hpp>
#include <EmpowerSpell.hpp>
#include <fstream>
#include <stdexcept>
#include <filesystem>
#include <iostream>
#include <ArcherTower.hpp>
#include <DecoyAlly.hpp>
#include <PlayerTrap.hpp>
#include <Enemy.hpp>
#include <Player.hpp>
#include <EnemyBase.hpp>
#include <string>
#include <cctype>

static const std::vector<SpellType> PLAYER_LEARNABLE_SPELLS = {
    SpellType::SiegeArrow,
    SpellType::RainOfArrows,
    SpellType::PlaceTrap,
    SpellType::SummonDecoy,
    SpellType::Empower
};

Menu::Menu(int yMax, int xMax) : yMax(yMax), xMax(xMax), savesList(Utils::createSaves()) {

    std::string img = Utils::readFileToString(START_BUTTON);
    options.push_back(img);

    img = Utils::readFileToString(LOAD_BUTTON);
    options.push_back(img);

    img = Utils::readFileToString(EXIT_BUTTON);
    options.push_back(img);

    img = Utils::readFileToString(CONTINUE_BUTTON);
    optionsPauseMenu.push_back(img);

    img = Utils::readFileToString(SAVE_BUTTON);
    optionsPauseMenu.push_back(img);

    img = Utils::readFileToString(LOAD_BUTTON);
    optionsPauseMenu.push_back(img);

    img = Utils::readFileToString(EXIT_BUTTON);
    optionsPauseMenu.push_back(img);

    img = Utils::readFileToString(LEFT_UP_CORNER);
    elements.push_back(img);

    img = Utils::readFileToString(RIGHT_UP_CORNER);
    elements.push_back(img);

    img = Utils::readFileToString(LEFT_DOWN_CORNER);
    elements.push_back(img);

    img = Utils::readFileToString(RIGHT_DOWN_CORNER);
    elements.push_back(img);
}

void Menu::drawStartMenu() {
    clear();

    Utils::printMultiline(0, 0, elements[0]);
    Utils::printMultiline(0, xMax - 40, elements[1]);
    Utils::printMultiline(yMax - 18, xMax - 40, elements[2]);
    Utils::printMultiline(yMax - 18, 0, elements[3]);

    Utils::printMultiline(yMax/2 - 12, xMax/6, Utils::readFileToString(KNIGHT_ICON));
    int step = 8;

    for (size_t i = 0; i < options.size(); ++i) {
        if (static_cast<int>(i) == selected) {
            Utils::printMultiline(yMax/4 + step * i, xMax/2 - 12, options[i]);
            Utils::printMultiline(yMax/4 + step * i + 2, xMax/2 - 22, Utils::readFileToString(ARROW));
        } else {
            Utils::printMultiline(yMax/4 + step * i, xMax/2 - 24, options[i]);
        }
    }
    refresh();
}

void Menu::drawPauseMenu() {
    clear();
    Utils::printMultiline(0, 0, Utils::readFileToString(PAUSE_MENU_ICON));

    int step = 8;

    for (size_t i = 0; i < optionsPauseMenu.size(); ++i) {
        if (static_cast<int>(i) == selected) {
            Utils::printMultiline(5+ step * i, xMax/2 - 12, optionsPauseMenu[i]);
            Utils::printMultiline(5 + step * i + 2, xMax/2 - 22, Utils::readFileToString(ARROW));
        } else {
            Utils::printMultiline(5 + step * i, xMax/2 - 24, optionsPauseMenu[i]);
        }
    }
    refresh();
}

int Menu::init(Field& field) { 
    int ch;
    selected = 0;

    while (true) {
        drawStartMenu();
        ch = getch();

        switch (ch) {
            case KEY_UP:
                if (selected > 0) selected--;
                break;
            case KEY_DOWN:

                if (selected < static_cast<int>(options.size()) - 1) selected++;
                break;
            case '\n': 
                if (selected == 0) { 
                    clear();
                    refresh();
                    return 1; 
                } else if (selected == 1) { 
                    if (openSaves(2, field)) { 
                        return 2; 
                    }
                } else if (selected == 2) { 
                    return 0; 
                }
                break;
            case 'q':
            case 27: 
                return 0; 
        }
    }
}

bool Menu::openPauseMenu(Field& field) {
    int ch;
    bool running = true;
    int choice;

    selected = 0;

    while (true) {
        drawPauseMenu();
        ch = getch();

        switch (ch) {
            case KEY_UP:
                if (selected > 0) selected--;
                break;
            case KEY_DOWN:
                if (selected < static_cast<int>(optionsPauseMenu.size()) - 1) selected++;
                break;
            case '\n': 
                if (selected == 0) {
                    return 1;
                } else if (selected == 1) {
                    choice = openSaves(selected, field);
                    if (choice) {return 1;}
                    else {break;}
                } else if (selected == 2) {
                    choice = openSaves(selected, field);
                    if (choice) {return 1;}
                    else {break;}
                } else if (selected == 3) {
                    clear();
                    refresh();
                    return 0;
                }
                break;
            case 'q':
            case 27: 
                running = false;
                break;
        }
    }
}

int Menu::openSaves(int option, Field& field) {
    clear();
    Utils::printMultiline(1, 0, Utils::readFileToString(SAVE_MENU_ICON));

    int menuHeight = 4 * yMax/5;
    int menuWidth = xMax/2;
    int saveWinHeight = yMax/5;
    int startY = yMax/10;
    int startX = xMax/4;
    std::string newSaveName;

    int n = 4;
    int selected = 0;
    int ch = 0;

    WINDOW* savesContainer = newwin(menuHeight, menuWidth, startY, startX);
    box(savesContainer, 0, 0);
    keypad(savesContainer, TRUE);

    std::vector<WINDOW*> saveWins;
    for (int i = 0; i < n; i++) {
        saveWins.push_back(derwin(savesContainer, saveWinHeight, menuWidth - 2, 1 + (saveWinHeight * i), 1));
    }

    while (true) {
        box(savesContainer, 0, 0);
        for (int i = 0; i < n; i++) {
            if (selected == i) {
                wattron(saveWins[i], COLOR_PAIR(2));
                box(saveWins[i], 0, 0);
                wattroff(saveWins[i], COLOR_PAIR(2));
            } else {
                box(saveWins[i], 0, 0);
            }

            if (i < savesList.size()) {
                wattron(saveWins[i], A_BOLD);
                mvwprintw(saveWins[i], 1, 1, "%s", savesList[i].displayName.c_str());
                wattroff(saveWins[i], A_BOLD);
                wattron(saveWins[i], A_UNDERLINE);
                mvwprintw(saveWins[i], saveWinHeight - 2, menuWidth - 25, "   %s   ",savesList[i].data.c_str());
                wattroff(saveWins[i], A_UNDERLINE);
                mvwprintw(saveWins[i], 3, 1, "Knight lvl: %d", savesList[i].playerLvl);
                mvwprintw(saveWins[i], 4, 1, "Stage: %d", savesList[i].stage);
            }
            
            refresh();
            wrefresh(saveWins[i]);
        }
        wrefresh(savesContainer);
        
        ch = wgetch(savesContainer);
        
        switch (ch) {
            case KEY_UP:
                if (selected > 0) selected--;
                break;
            case KEY_DOWN:
                if (selected < static_cast<int>(saveWins.size()) - 1) selected++;
                break;
            case '\n':
                if (option == 1) {
                    newSaveName = inputSaveName(selected);
                    if (newSaveName != "") {
                        if (selected < static_cast<int>(savesList.size())) {
                            Utils::deleteSaveFile(savesList[selected].fullname);
                            savesList.erase(savesList.begin() + selected);
                        }
                        saveGame(field, newSaveName);
                    }
                    wclear(saveWins[selected]);
                    continue;
                }
                if (option == 2) {
                    if (selected < static_cast<int>(savesList.size())) {
                        return loadGame(field, savesList[selected].fullname);
                    }
                }
                continue;
            case ESC: 
                return 0;
        }
    }

    for (int i = 0; i < n; i++) {
        delwin(saveWins[i]);
    }
    delwin(savesContainer);

    touchwin(stdscr);
    refresh();
    flushinp();
    return 1;
}

int Menu::openSpellMenu(std::vector<std::shared_ptr<Spell>> spells) {
    if (spells.empty()) {
        return -1;
    }

    int maxNameLength = 0;
    for (const auto& spell : spells) {
        if (spell->getName().length() > maxNameLength) {
            maxNameLength = spell->getName().length();
        }
    }

    int menuHeight = spells.size() + 2; 
    int menuWidth = maxNameLength + 4; 

    int yMax, xMax;
    getmaxyx(stdscr, yMax, xMax);
    int startY = (yMax - menuHeight) - 8 / yMax;
    int startX = (xMax - menuWidth) - 8 / xMax;

    WINDOW* skillMenu = newwin(menuHeight, menuWidth, startY, startX);
    keypad(skillMenu, TRUE);
    box(skillMenu, 0, 0);

    int highlight = 0;
    int choice = -1;

    while (true) {

        for (int i = 0; i < spells.size(); ++i) {

            if (i == highlight) {
                wattron(skillMenu, A_REVERSE);
            }
            mvwprintw(skillMenu, i + 1, 2, "%s", spells[i]->getName().c_str());
            if (i == highlight) {
                wattroff(skillMenu, A_REVERSE);
            }
        }
        wrefresh(skillMenu);

        int ch = wgetch(skillMenu);

        switch (ch) {
            case KEY_UP:
                highlight--;
                if (highlight < 0) { 
                    highlight = spells.size() - 1;
                }
                break;
            case KEY_DOWN:
                highlight++;
                if (highlight >= spells.size()) { 
                    highlight = 0;
                }
                break;
            case ENTER: 
                choice = highlight;
                break;
            case ESC: 
                choice = -5;
                break;
            default:
                break;
        }

        if (choice != -1 && ch != KEY_UP && ch != KEY_DOWN) {
            break;
        }
    }

    delwin(skillMenu);

    touchwin(stdscr);
    refresh();
    flushinp();
    return choice;
}

bool Menu::openLearnMenu(Gauntlet& gauntlet) {

    const std::vector<SpellType>& allLearnableSpells = PLAYER_LEARNABLE_SPELLS;

    const auto& knownSpells = gauntlet.getKnownSpells();

    if (knownSpells.size() >= allLearnableSpells.size()) {

        return false; 
    }

    std::vector<SpellType> spellsToLearn;
    for (const auto& potentialSpell : allLearnableSpells) {
        bool alreadyLearned = false;

        for (const auto& ownedSpell : knownSpells) {
            if (ownedSpell->getType() == potentialSpell) {
                alreadyLearned = true;
                break; 
            }
        }

        if (!alreadyLearned) {
            spellsToLearn.push_back(potentialSpell);
        }
    }

    if (spellsToLearn.empty()) {
        return false;
    }

    std::vector<std::string> spellNames;
    for (const auto& type : spellsToLearn) { 
        switch (type) {
            case SpellType::SiegeArrow: spellNames.push_back("Siege Arrow"); break;
            case SpellType::RainOfArrows: spellNames.push_back("Rain of Arrows"); break;
            case SpellType::PlaceTrap: spellNames.push_back("Place Trap"); break;
            case SpellType::SummonDecoy: spellNames.push_back("Summon Shadow"); break;
            case SpellType::Empower: spellNames.push_back("Empower"); break;
        }
    }

    int maxNameLength = 0;
    for (const auto& name : spellNames) {
        if (name.length() > maxNameLength) maxNameLength = name.length();
    }
    int menuHeight = spellNames.size() + 2; 
    int menuWidth = maxNameLength + 4; 
    int yMax, xMax;
    getmaxyx(stdscr, yMax, xMax);
    int startY = (yMax - menuHeight) - (yMax - menuHeight) / 8;
    int startX = (xMax - menuWidth) - (xMax - menuWidth) / 8;

    WINDOW* learnMenu = newwin(menuHeight, menuWidth, startY, startX);
    keypad(learnMenu, TRUE);
    box(learnMenu, 0, 0);

    int highlight = 0;
    int choice = -1; 
    bool learnedSuccessfully = false; 

    while (true) {
        for (int i = 0; i < spellNames.size(); ++i) {
            if (i == highlight) wattron(learnMenu, A_REVERSE);
            mvwprintw(learnMenu, i + 1, 2, "%s", spellNames[i].c_str());
            if (i == highlight) wattroff(learnMenu, A_REVERSE);
        }
        wrefresh(learnMenu);

        int ch = wgetch(learnMenu);
        if (ch == ENTER) {
            choice = highlight;
            break;
        }
        if (ch == ESC) {
            choice = -1;
            break;
        }

        switch (ch) {
            case KEY_UP:
                highlight = (highlight == 0) ? spellNames.size() - 1 : highlight - 1;
                break;
            case KEY_DOWN:
                highlight = (highlight == spellNames.size() - 1) ? 0 : highlight + 1;
                break;
        }
    }

    delwin(learnMenu);
    touchwin(stdscr);
    refresh();

    if (choice != -1) {

        SpellType chosenType = spellsToLearn[choice];
        std::shared_ptr<Spell> newSpell = nullptr;

        switch (chosenType) {
            case SpellType::SiegeArrow:
                newSpell = std::make_shared<SiegeArrow>();
                break;
            case SpellType::RainOfArrows:
                newSpell = std::make_shared<RainOfArrows>();
                break;
            case SpellType::PlaceTrap:
                newSpell = std::make_shared<PlaceTrapSpell>();
                break;
            case SpellType::SummonDecoy:
                newSpell = std::make_shared<SummonDecoySpell>();
                break;
            case SpellType::Empower:
                newSpell = std::make_shared<EmpowerSpell>();
                break;
        }

        if (newSpell) {
            learnedSuccessfully = gauntlet.learnSpell(newSpell);
        }
    }
    flushinp();
    return learnedSuccessfully; 
}

void Menu::gameOver() {
    flushinp();
    Utils::printMultiline(yMax/2 - 3, xMax/2 - 35, Utils::readFileToString(GAMEOVER));
    Utils::printMultiline(yMax/2 - 12, 0, Utils::readFileToString(DEFEATED_KNIGHT));
    mvprintw(yMax/2 + 4, xMax/2 - 6, "press any key");
    getch();
}

const std::vector<Save>& Menu::getSavesList() const {
    return savesList;
}

void Menu::refreshSaves() {
    savesList = Utils::createSaves();
}

bool Menu::saveGame(Field& field, std::string filename) {

    try {
        if (!std::filesystem::exists("saves")) {
            std::filesystem::create_directory("saves");
        }

        std::ofstream saveFile("saves/" + filename);
        if (!saveFile.is_open()) {
            throw std::runtime_error("Cannot open file for saving: " + filename);
        }

        saveFile << "[Field]\n";
        saveFile << "turnCounter=" << field.getTurnCounter() << "\n";
        saveFile << "currentLevel=" << field.getCurrentLevel() << "\n"; 

        auto player = field.getPlayer();
        if (player) {
            saveFile << "[Player]\n";
            saveFile << "x=" << player->getX() << "\n";
            saveFile << "y=" << player->getY() << "\n";
            saveFile << "health=" << player->getHealth() << "\n";
            saveFile << "mp=" << player->getMp() << "\n";
            saveFile << "mpMax=" << player->getMpMax() << "\n";
            saveFile << "level=" << player->getLvl() << "\n";
            saveFile << "exp=" << player->getExp() << "\n";
            saveFile << "expToLvlUp=" << player->getExpToLvlUp() << "\n";
            saveFile << "damage=" << player->getDamage() << "\n";
            saveFile << "weaponMode=" << player->getMode() << "\n";
            saveFile << "isBeingTargeted=" << player->isTargeted() << "\n";
            saveFile << "empowermentCharges=" << player->getEmpowermentCharges() << "\n";
            saveFile << "wasAttacked=" << player->getWasAttacked() << "\n";
            saveFile << "inTrap=" << player->getInTrap() << "\n";
            saveFile << "scale=" << player->getScale() << "\n";

            const auto& spells = player->getSpells();
            saveFile << "spells_count=" << spells.size() << "\n";
            for (const auto& spell : spells) {
                saveFile << "spell=" << Utils::spellTypeToString(spell->getType()) << "\n";
            }
        }

        auto livingEntities = field.getEntities();
        saveFile << "[LivingEntities]\n";
        saveFile << "count=" << (livingEntities.empty() ? 0 : livingEntities.size() - (player ? 1 : 0)) << "\n";

        int entity_index = 0;
        for (const auto& entity : livingEntities) {
            if (entity->getType() == Entity::Type::PLAYER) continue;

            saveFile << "[LivingEntity_" << entity_index << "]\n";
            saveFile << "type=" << Utils::entityTypeToString(entity->getType()) << "\n";
            saveFile << "x=" << entity->getX() << "\n";
            saveFile << "y=" << entity->getY() << "\n";
            saveFile << "health=" << entity->getHealth() << "\n";
            saveFile << "expValue=" << entity->getExp() << "\n";

            if (entity->getType() == Entity::Type::ENEMY) {
                auto enemy = std::dynamic_pointer_cast<Enemy>(entity);
                if (enemy) {
                    saveFile << "level=" << enemy->getLvl() << "\n";
                }
            } else if (entity->getType() == Entity::Type::ENEMY_BASE) {
                auto base = std::dynamic_pointer_cast<EnemyBase>(entity);
                if (base) {
                    saveFile << "level=" << base->getLvl() << "\n";
                }
            }

            else if (entity->getType() == Entity::Type::ARCHER_TOWER) { 
                auto tower = std::dynamic_pointer_cast<ArcherTower>(entity);
                if (tower) {
                    saveFile << "state=" << tower->getStateAsString() << "\n";
                    saveFile << "cooldown=" << tower->getCurrentCooldown() << "\n";
                }
            } else if (entity->getType() == Entity::Type::DECOY_ALLY) {
                auto decoyAlly = std::dynamic_pointer_cast<DecoyAlly>(entity);
                if (decoyAlly) {
                    saveFile << "targetX=" << decoyAlly->getTargetX() << "\n";
                    saveFile << "targetY=" << decoyAlly->getTargetY() << "\n";
                    saveFile << "lifetimeCounter=" << decoyAlly->getLifeTimeCounter() << "\n";
                    saveFile << "reachedTarget=" << decoyAlly->getReachedTarget() << "\n";
                }
            }
            entity_index++;
        }

        saveFile << "[StaticEntities]\n";
        std::vector<std::shared_ptr<Entity>> staticList;
        for (int y = 0; y < field.getH(); ++y) {
            for (int x = 0; x < field.getW(); ++x) {
                auto entity = field.getEntityAt(y, x);
                if (entity && (entity->getType() == Entity::Type::PLAYER_TRAP || entity->getType() == Entity::Type::SPIKED_TRAP)) {
                    staticList.push_back(entity);
                }
            }
        }

        saveFile << "count=" << staticList.size() << "\n";
        int static_index = 0;
        for (const auto& entity : staticList) {
            saveFile << "[StaticEntity_" << static_index << "]\n";
            saveFile << "type=" << Utils::entityTypeToString(entity->getType()) << "\n";
            saveFile << "x=" << entity->getX() << "\n";
            saveFile << "y=" << entity->getY() << "\n";
            if (entity->getType() == Entity::Type::PLAYER_TRAP) {
                auto trap = std::dynamic_pointer_cast<PlayerTrap>(entity);
                if (trap) saveFile << "damage=" << trap->getDamage() << "\n";
            }
            static_index++;
        }

    } catch (const std::exception& e) {
        std::string errorMessage = "Save Error: " + std::string(e.what()) + ". Press any key.";
        mvprintw(yMax, xMax - errorMessage.size()/2, "Save Error: %s. Press any key.", e.what());
        refresh();
        getch();
        return false;
    }

    refreshSaves();
    return true;
}

bool Menu::loadGame(Field& field, std::string filename) {
    if (filename.empty()) {
        return false;
    }

    Field tempField;
    tempField.setScreen(field.getScreen()); 

    try {
        std::ifstream loadFile("saves/" + filename);
        if (!loadFile.is_open()) {
            throw std::runtime_error("Save file not found or cannot be opened.");
        }

        int width, height;
        int fieldTurnCounter = 0;
        int fieldCurrentLevel = 1;
        std::shared_ptr<Player> loadedPlayer = nullptr;
        std::vector<std::shared_ptr<LivingEntity>> loadedEntities;
        std::vector<std::tuple<std::string, int, int, int>> staticEntitiesData;

        std::string line;
        while (std::getline(loadFile, line)) {
            if (line.empty() || line[0] == '#') continue;

            if (line == "[Field]") {
                std::getline(loadFile, line); fieldTurnCounter = std::stoi(Utils::getValueFromLine(line));
                if (std::getline(loadFile, line) && line.rfind("currentLevel=", 0) == 0) {
                    fieldCurrentLevel = std::stoi(Utils::getValueFromLine(line));
                }
                width = FIELD_WIDTH + (fieldCurrentLevel - 1) * 4;  
                height = FIELD_HEIGHT + (fieldCurrentLevel - 1) * 2; 
            } 
            else if (line == "[Player]") {
                int x, y, health, mp, mpMax, level, exp, expToLvlUp, damage, empowermentCharges;
                bool weaponMode, isTargeted, wasAttacked, inTrap;
                float scale = 1.0f;

                std::getline(loadFile, line); x = std::stoi(Utils::getValueFromLine(line));
                std::getline(loadFile, line); y = std::stoi(Utils::getValueFromLine(line));
                int worldY = y - tempField.getScreen()->yMax / 2 + height / 2;
                int worldX = x - tempField.getScreen()->xMax / 2 + width / 2;
                if (worldX >= width - 1 || worldX <= 0 || worldY <= 0 || worldY >= height - 1) {
                    std::string error = "Save file not found or cannot be opened. Valid coordinates";
                    throw std::runtime_error(error);
                }
                std::getline(loadFile, line); health = std::stoi(Utils::getValueFromLine(line));
                std::getline(loadFile, line); mp = std::stoi(Utils::getValueFromLine(line));
                std::getline(loadFile, line); mpMax = std::stoi(Utils::getValueFromLine(line));
                std::getline(loadFile, line); level = std::stoi(Utils::getValueFromLine(line));
                std::getline(loadFile, line); exp = std::stoi(Utils::getValueFromLine(line));
                std::getline(loadFile, line); expToLvlUp = std::stoi(Utils::getValueFromLine(line));
                std::getline(loadFile, line); damage = std::stoi(Utils::getValueFromLine(line));
                std::getline(loadFile, line); weaponMode = std::stoi(Utils::getValueFromLine(line));
                std::getline(loadFile, line); isTargeted = std::stoi(Utils::getValueFromLine(line));
                std::getline(loadFile, line); empowermentCharges = std::stoi(Utils::getValueFromLine(line));
                std::getline(loadFile, line); wasAttacked = std::stoi(Utils::getValueFromLine(line));
                std::getline(loadFile, line); inTrap = std::stoi(Utils::getValueFromLine(line));
                if (std::getline(loadFile, line) && line.rfind("scale=", 0) == 0) {
                     scale = std::stof(Utils::getValueFromLine(line));
                }

                loadedPlayer = std::make_shared<Player>(x, y);
                loadedPlayer->setHealth(health);
                loadedPlayer->setMp(mp);
                loadedPlayer->setMpMax(mpMax);
                loadedPlayer->setLvl(level);
                loadedPlayer->setExp(exp);
                loadedPlayer->setExpToLvlUp(expToLvlUp);
                loadedPlayer->setDamage(damage);
                loadedPlayer->setMode(weaponMode);
                loadedPlayer->setTargeted(isTargeted);
                loadedPlayer->setEmpowermentCharges(empowermentCharges);
                loadedPlayer->setWasAttacked(wasAttacked);
                if (inTrap) loadedPlayer->setInTrap(); else loadedPlayer->unSetInTrap();
                loadedPlayer->setScale(scale);

                loadedPlayer->clearSpells();
                std::getline(loadFile, line);
                int spells_count = std::stoi(Utils::getValueFromLine(line));
                for (int i = 0; i < spells_count; ++i) {
                    std::getline(loadFile, line);
                    auto newSpell = Utils::spellFromString(Utils::getValueFromLine(line));
                    if (newSpell) loadedPlayer->learnSpell(newSpell);
                }
            }
            else if (line == "[LivingEntities]") {
                std::getline(loadFile, line);
                int count = std::stoi(Utils::getValueFromLine(line));
                for (int i = 0; i < count; ++i) {
                    std::getline(loadFile, line); 
                    std::string type_str;
                    int x, y, health, expValue;
                    std::getline(loadFile, line); type_str = Utils::getValueFromLine(line);
                    std::getline(loadFile, line); x = std::stoi(Utils::getValueFromLine(line));
                    std::getline(loadFile, line); y = std::stoi(Utils::getValueFromLine(line));

                    int worldY = y - tempField.getScreen()->yMax / 2 + height / 2;
                    int worldX = x - tempField.getScreen()->xMax / 2 + width / 2;
                    if (worldX >= width - 1 || worldX <= 0 || worldY <= 0 || worldY >= height - 1) {
                        std::string error = "Save file not found or cannot be opened. Valid coordinates";
                        throw std::runtime_error(error);
                    }
                    
                    std::getline(loadFile, line); health = std::stoi(Utils::getValueFromLine(line));
                    std::getline(loadFile, line); expValue = std::stoi(Utils::getValueFromLine(line));
                    Entity::Type type = Utils::entityTypeFromString(type_str);
                    std::shared_ptr<LivingEntity> newEntity = nullptr;
                    int entityLevel = 1; 

                    if (type == Entity::Type::ENEMY) {
                        std::getline(loadFile, line); entityLevel = std::stoi(Utils::getValueFromLine(line)); 
                        newEntity = std::make_shared<Enemy>(x, y, entityLevel); 
                    } else if (type == Entity::Type::ENEMY_BASE) {
                        std::getline(loadFile, line); entityLevel = std::stoi(Utils::getValueFromLine(line)); 
                        newEntity = std::make_shared<EnemyBase>(x, y, entityLevel); 
                    } else if (type == Entity::Type::ARCHER_TOWER) {
                        newEntity = std::make_shared<ArcherTower>(x, y); 
                        std::getline(loadFile, line); std::string state = Utils::getValueFromLine(line);
                        std::getline(loadFile, line); int cooldown = std::stoi(Utils::getValueFromLine(line));
                        auto tower = std::dynamic_pointer_cast<ArcherTower>(newEntity);
                        if(tower) tower->setState(state, cooldown);
                    } else if (type == Entity::Type::DECOY_ALLY) {
                        int targetX, targetY, lifetime; bool reached;
                        std::getline(loadFile, line); targetX = std::stoi(Utils::getValueFromLine(line));
                        std::getline(loadFile, line); targetY = std::stoi(Utils::getValueFromLine(line));
                        std::getline(loadFile, line); lifetime = std::stoi(Utils::getValueFromLine(line));
                        std::getline(loadFile, line); reached = std::stoi(Utils::getValueFromLine(line));
                        newEntity = std::make_shared<DecoyAlly>(x, y, targetX, targetY);
                        auto decoy = std::dynamic_pointer_cast<DecoyAlly>(newEntity);
                        if(decoy) decoy->setState(lifetime, reached);
                    }

                    if (newEntity) {
                        newEntity->setHealth(health);
                        newEntity->setExp(expValue);
                        loadedEntities.push_back(newEntity);
                    }
                }
            }
            else if (line == "[StaticEntities]") {
                std::getline(loadFile, line);
                int count = std::stoi(Utils::getValueFromLine(line));
                for (int i = 0; i < count; ++i) {
                    std::getline(loadFile, line); 
                    std::string type_str; int x, y, damage = 0;
                    std::getline(loadFile, line); type_str = Utils::getValueFromLine(line);
                    std::getline(loadFile, line); x = std::stoi(Utils::getValueFromLine(line));
                    std::getline(loadFile, line); y = std::stoi(Utils::getValueFromLine(line));
                    if (type_str == "PLAYER_TRAP") {
                        std::getline(loadFile, line); damage = std::stoi(Utils::getValueFromLine(line));
                    }
                    staticEntitiesData.emplace_back(type_str, x, y, damage);
                }
            }
        }
        loadFile.close();

        tempField.setCurrentLevel(fieldCurrentLevel);
        tempField.setTurnCounter(fieldTurnCounter);

        tempField.initLevel(false); 

        if (loadedPlayer) {
            tempField.addPlayer(loadedPlayer);
        }
        for (const auto& entity : loadedEntities) {
            tempField.addEntity(entity);
        }

        for (const auto& data : staticEntitiesData) {
            std::string type_str = std::get<0>(data);
            int x = std::get<1>(data);
            int y = std::get<2>(data);
            int damage = std::get<3>(data);

            if (type_str == "PLAYER_TRAP") {
                tempField.createPlayerTrapAt(x, y, damage);
            } else if (type_str == "SPIKED_TRAP") {
                tempField.createSpikedTrapAt(x, y);
            }
        }

    } catch (const std::exception& e) {
        clear();
        std::string errorMessage = "Load Error: " + std::string(e.what()) + ". Press any key.";
        mvprintw(yMax/2, xMax/2 - errorMessage.size()/2, "Load Error: %s. Press any key.", e.what());
        refresh();
        getch();
        return false; 
    }

    field = std::move(tempField);
    return true; 
}

std::string Menu::inputSaveName(int step) {
    std::string saveName = ""; 

    WINDOW* inputWin = newwin(yMax / 5, xMax / 5, yMax / 10 + (yMax / 5 * step) + 1, xMax / 4 + xMax / 2);
    keypad(inputWin, TRUE); 
    curs_set(1); 
    noecho(); 

    int ch;
    while (true) {

        werase(inputWin); 
        box(inputWin, 0, 0); 
        mvwprintw(inputWin, 1, 10, "Enter save name:");
        mvwprintw(inputWin, 6, 5, "12 chars max, alphanumeric");
        mvwprintw(inputWin, 3, 2, saveName.c_str()); 
        wmove(inputWin, 3, 2 + saveName.length()); 
        wrefresh(inputWin); 

        ch = wgetch(inputWin); 

        if (ch == '\n' || ch == KEY_ENTER) {
            break; 
        }
        if (ch == 27) {
            saveName = "";
            break;
        }
        else if (ch == KEY_BACKSPACE || ch == 127 || ch == 8) { 
            if (!saveName.empty()) {
                saveName.pop_back(); 
            }
        }
        else if (std::isalnum(ch) && saveName.length() < 12) { 
            saveName += ch; 
        }

    }

    curs_set(0);
    delwin(inputWin);
    touchwin(stdscr);
    refresh();
    flushinp(); 

    if (!saveName.empty()) {
        char digit_char = step + '0'; 
        saveName += "_";
        saveName += digit_char;
        saveName += ".sav";
    }

    return saveName;
}