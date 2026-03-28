#include "ConsoleRenderer.h"
#include <iostream>
#include <cstdlib>
#include <map>

#ifdef _WIN32
    #define CLEAR_COMMAND "cls"
#else
    #define CLEAR_COMMAND "clear"
#endif

void ConsoleRenderer::clearScreen() {
    system(CLEAR_COMMAND);
}

void ConsoleRenderer::renderField(GameState& gameState){
    Field field = gameState.getField();

    int height = field.getHeight();
    int width = field.getWidth();
    char** outputField = new char*[height];
    for(int i = 0; i < height; ++i){
        outputField[i] = new char[width];
    }

    for(int i = 0; i < height; ++i){
        for(int j = 0; j < width; ++j){
            Cell& cell = field.getCell(j + 1, i + 1);
            if (cell.getCellType() == CellType::Wall){
                outputField[height - i - 1][j] = '#';
            }
            else if (cell.getCellType() == CellType::Slowing){
                outputField[height - i - 1][j] = '~';
            }
            else{
                outputField[height - i - 1][j] = '.';
            }
        }
    }
    
    std::pair<int, int> playerPos = gameState.getPlayer().getPos();
    if(field.isValidPos(playerPos)){
        outputField[height - playerPos.second][playerPos.first - 1] = 'P';
    }
    
    std::vector<Enemy> enemies = gameState.getEnemies();
    for(int i = 0; i < enemies.size(); ++i){
        std::pair<int, int> enemyPos = enemies[i].getPos();
        if(field.isValidPos(enemyPos)){
            outputField[height - enemyPos.second][enemyPos.first - 1] = 'E';
        }
    }

    std::vector<Ally> allies = gameState.getAllies();
    for(int i = 0; i < allies.size(); ++i){
        std::pair<int, int> allyPos = allies[i].getPos();
        if(field.isValidPos(allyPos)){
            outputField[height - allyPos.second][allyPos.first - 1] = 'A';
        }
    }

    std::vector<EnemyTower> towers = gameState.getTowers();
    for(int i = 0; i < towers.size(); ++i){
        std::pair<int, int> towerPos = towers[i].getPos();
        if(field.isValidPos(towerPos)){
            outputField[height - towerPos.second][towerPos.first - 1] = 'T';
        }
    }

    std::vector<EnemyBuilding> builds = gameState.getBuilds();
    for(int i = 0; i < builds.size(); ++i){
        std::pair<int, int> buildPos = builds[i].getPos();
        if(field.isValidPos(buildPos)){
            outputField[height - buildPos.second][buildPos.first - 1] = 'B';
        }
    }

    std::vector<Trap> traps = gameState.getTraps();
    for(int i = 0; i < traps.size(); ++i){
        std::pair<int, int> trapPos = traps[i].getPos();
        if(field.isValidPos(trapPos)){
            outputField[height - trapPos.second][trapPos.first - 1] = 'D';
        }
    }

    for(int i = 0; i < height; ++i){
        for(int j = 0; j < width; ++j){
            std::cout << (char)outputField[i][j];
            if (j == width - 1){
                std::cout << '\n';
            }
            else{
                std::cout << ' ';
            }
        }
    }

    std::cout << std::endl;

    for(int i = 0; i < width; ++i){
        delete[] outputField[i];
    }
    delete[] outputField;
}

void ConsoleRenderer::renderShop(Player& player){
    int spellList[5] = {
        DIR_SPELL_COST, 
        AREA_SPELL_COST, 
        SUMMON_SPELL_COST, 
        UPGRADE_SPELL_COST, 
        TRAP_SPELL_COST
    };
    
    std::map<SpellType, std::string> spellTypeToString = {
        {SpellType::DirDMGSpell, "Direct damage spell"},
        {SpellType::AreaDMGSpell, "Area damage spell"},
        {SpellType::SummonSpell, "Summon spell"},
        {SpellType::TrapSpell, "Trap spell"},
        {SpellType::UpgradeSpell, "Upgrade spell"},
    };

    std::cout << "\n===== Spell Shop =====\n";
    std::cout << "Your experience points: " << player.getEXP() << "\n";
    std::cout << "Choose which spell to buy from 1 to 5, or 0 to cancel\n";
    
    for (int i = 0; i < 5; ++i) {
        std::cout << i + 1 << ". " << spellTypeToString[(SpellType)i] << ": " << spellList[i] << "\n";
    }
}

void ConsoleRenderer::renderPlayerInfo(Player player) {
    std::cout << "\n======== Player Info ========\n";
    std::cout << "Health: " << player.getHP() << "/" << player.getMaxHP() << "\n";
    std::cout << "EXP: " << player.getEXP() << "\n";
    if (player.getDMGMode() == DamageMode::Melee){
        std::cout << "Attack type: melee\n";
        std::cout << "Damage (Melee/Range): " << player.getDMG();
        player.switchDMGMode();
        std::cout << "/" << player.getDMG() << "\n";
        player.switchDMGMode();
    }
    else{
        std::cout << "Attack type: ranged\n";
        player.switchDMGMode();
        std::cout << "Damage (Melee/Range): " << player.getDMG();
        player.switchDMGMode();
        std::cout << "/" << player.getDMG() << "\n";
    }
    std::pair<int, int> playerPos = player.getPos();
    if (playerPos.first == 0 && playerPos.second == 0){
        std::cout << "Position is not initialized\n";
    }
    else{
        std::cout << "Position: (" << playerPos.first << ' ' << playerPos.second << ")\n";
    }
    if (player.getCondition() == CharacterCondition::Normal){
        std::cout << "Character condition: normal\n";
    }
    else{
        std::cout << "Character condition: slowed\n";
    }
}

void ConsoleRenderer::renderHand(Hand& hand) {
    std::map<SpellType, std::string> spellTypeToString = {
        {SpellType::DirDMGSpell, "Direct damage spell"},
        {SpellType::AreaDMGSpell, "Area damage spell"},
        {SpellType::SummonSpell, "Summon spell"},
        {SpellType::TrapSpell, "Trap spell"},
        {SpellType::UpgradeSpell, "Upgrade spell"},
    };

    std::cout << "\n=== Spells in Hand ===\n";
    if (hand.getCount() == 0){
        std::cout << "No spells available\n";
    }
    else{
        for (size_t i = 0; i < hand.getCount(); ++i) {
            std::cout << i + 1 << ". " << spellTypeToString[hand.getSpell(i)] << "\n";
        }
    }
    std::cout << "Upgrade Power = " << hand.getUpgradePower() << std::endl;
}

void ConsoleRenderer::renderMessage(const std::string& message) {
    std::cout << "### " << message << " ###" << std::endl;
}

void ConsoleRenderer::renderErrorMessage(const std::string& message) {
    std::cout << ">>> " << message << " <<<" << std::endl;
}

void ConsoleRenderer::renderGameState(GameState& gameState) {
    clearScreen();
    renderField(gameState);
}

void ConsoleRenderer::renderMainMenu() {
    clearScreen();
    std::cout << "=================================\n";
    std::cout << "  OOP GAME by Smirnov A.D. 4382\n";
    std::cout << "=================================\n";
    std::cout << "      1. Start New Game\n";
    std::cout << "         2. Load Save\n";
    std::cout << "           3. Exit\n";
    std::cout << "=================================" << std::endl;
}

void ConsoleRenderer::renderLevelComplete() {
    clearScreen();
    std::cout << "=========================\n";
    std::cout << "     Level complete!\n";
    std::cout << "=========================\n";
    std::cout << "Want to go to next level?\n";
    std::cout << "         1. Yes\n";
    std::cout << "         0. No\n";
}

void ConsoleRenderer::renderUpgrade(){
    std::cout << "\nChoose upgrade:\n";
    std::cout << "1. Health\n";
    std::cout << "2. Melee Damage\n";
    std::cout << "3. Range Damage\n";
    std::cout << "4. Spell" << std::endl;
}

void ConsoleRenderer::renderGameOver() {
    clearScreen();
    std::cout << "=========================\n";
    std::cout << "        GAME OVER\n";
    std::cout << "=========================\n";
    std::cout << "    1. Restart Level\n";
    std::cout << "        2. Exit\n";
    std::cout << "=========================" << std::endl;
}

void ConsoleRenderer::renderKeyboardSettings(std::map<char, ActionType> keyboardSettings) {
    std::map<ActionType, std::string> actionToString = {
        {ActionType::MoveUp, "Move Up"},
        {ActionType::MoveDown, "Move Down"},
        {ActionType::MoveLeft, "Move Left"},
        {ActionType::MoveRight, "Move Right"},
        {ActionType::Attack, "Attack"},
        {ActionType::Load, "Load"},
        {ActionType::Save, "Save"},
        {ActionType::Info, "Info"},
        {ActionType::Surrender, "Surrender"},
        {ActionType::Switch, "Switch"},
        {ActionType::Pass, "Pass"},
        {ActionType::UseSpell, "Use Spell"},
        {ActionType::BuySpell, "Buy Spell"}
    };
    std::cout << "\n================Keyboard Settings ==============\n";
    int count = 0;
    for (auto [key, action] : keyboardSettings){
        count += 1;
        std::cout << key << " - " << actionToString[action];
        if (count == keyboardSettings.size()){
            std::cout << '.' << std::endl;
        }
        else if (count % 3 == 0){
            std::cout << '\n';
        }
        else{
            std::cout << ", ";
        }
        
    }
}