#include "Game.h"

Game::Game() : player(true, true, ATKtype::melee, "ꙮ ", 9, 3, 1, 1, 1), 
             master(player),
             inputController(keyBinder) 
{
    srand(time(0));
    
    if (!keyBinder.loadFromFile("keys.cfg")) 
    {
        std::cout << "Using default key configuration.\n";
    }
}

void Game::showKeyConfigMenu() 
{
    bool inMenu = true;
    while (inMenu) 
    {
        std::cout << "\033c";
        keyBinder.displayCurrentBindings();
        
        std::cout << "\n=== Key Configuration Menu ===\n";
        std::cout << "1. Change individual keys\n";
        std::cout << "2. Load preset\n";
        std::cout << "3. Save current configuration\n";
        std::cout << "4. Reset to default\n";
        std::cout << "5. Back to game\n";
        std::cout << "================================\n";
        std::cout << "Choice: ";
        
        int choice;
        std::cin >> choice;
        
        switch(choice) 
        {
            case 1: 
            {
                const char* actionNames[] = 
                {
                    "Move Up", "Move Down", "Move Left", "Move Right",
                    "Attack", "Switch Attack", "Open Shop", "Upgrade Menu",
                    "Save Menu", "Load Menu", "Toggle Animations", "Key Config",
                    "Back to Menu"
                };
                
                std::cout << "\nSelect action to change (1-13): ";
                int action;
                std::cin >> action;
                
                if (action >= 1 && action <= KeyBinder::ACTION_COUNT) 
                {
                    std::cout << "Current key for '" << actionNames[action-1] << "': '"
                              << keyBinder.getKey(action-1) << "'\n";
                    std::cout << "Enter new key: ";
                    char newKey;
                    std::cin >> newKey;
                    keyBinder.setKey(action-1, newKey);
                    std::cout << "Key updated!\n";
                } 
                else 
                {
                    std::cout << "Invalid action!\n";
                }
                break;
            }
            case 2: 
            {
                std::cout << "\nAvailable presets:\n";
                std::cout << "1. Default (WASD)\n";
                std::cout << "2. Arrow Keys (ASCII)\n";
                std::cout << "3. Numpad\n";
                std::cout << "Choice: ";
                
                int presetChoice;
                std::cin >> presetChoice;
                
                switch(presetChoice) 
                {
                    case 1: keyBinder.loadPreset("default"); break;
                    case 2: keyBinder.loadPreset("arrows"); break;
                    case 3: keyBinder.loadPreset("numpad"); break;
                    default: std::cout << "Invalid choice!\n";
                }
                break;
            }
            case 3: 
            {
                std::cout << "Enter filename to save (e.g., 'mykeys.cfg'): ";
                std::string filename;
                std::cin >> filename;
                if (keyBinder.saveToFile(filename)) 
                {
                    std::cout << "Configuration saved!\n";
                } 
                else 
                {
                    std::cout << "Error saving configuration!\n";
                }
                break;
            }
            case 4:
                keyBinder.resetToDefault();
                std::cout << "Reset to default configuration!\n";
                break;
            case 5:
                inMenu = false;
                break;
            default:
                std::cout << "Invalid choice!\n";
        }
        
        if (inMenu) 
        {
            std::cout << "\nPress any key to continue...";
            std::cin.ignore();
            std::cin.get();
        }
    }
}

void Game::run() 
{
    GameLogger::initialize();
    GameLogger::logEvent("Game started");
    animPlayer.play("StartGame", 1);
    std::cout << "Welcome to DungeonsAndDeadlines!\n";
    int randomSpell = rand() % 4;
    player.addSpell(randomSpell, 1);
    mainMenu();
}

void Game::mainMenu() 
{
    while (true) 
    {
        std::cout << "\nMain Menu:\n";
        std::cout << "1. New Game\n";
        std::cout << "2. Load Game\n";
        std::cout << "3. Exit\n";
        
        int choice;
        std::cin >> choice;
        
        switch(choice) 
        {
            case 1: 
            {
                animPlayer.play("Load", 1);
                
                player.reset();
                
                int randomSpell = rand() % 4;
                player.addSpell(randomSpell, 1);
                
                master.initializeLevel(1);
                
                gameLoop();
                break;
            }                    
            case 2: 
            {
                std::cout << "\nAvailable save files:\n";
                std::vector<std::string> saveFiles;
                
                std::vector<std::string> possibleFiles = 
                {
                    "autosave.txt", "save1.txt", "save2.txt", 
                    "save3.txt", "save4.txt", "save5.txt"
                };
                
                int saveNumber = 1;
                for (const auto& filename : possibleFiles) 
                {
                    std::ifstream testFile(filename);
                    if (testFile.good()) 
                    {
                        std::cout << saveNumber++ << ". " << filename << "\n";
                        saveFiles.push_back(filename);
                        testFile.close();
                    }
                }
                
                if (saveFiles.empty()) 
                {
                    std::cout << "No save files found!\n";
                    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                    break;
                }
                
                std::cout << saveNumber << ". Back to Main Menu\n";
                
                int loadChoice;
                std::cin >> loadChoice;
                
                if (loadChoice == saveNumber) 
                {
                    break;
                }
                
                if (loadChoice >= 1 && loadChoice <= saveFiles.size()) 
                {
                    std::string filename = saveFiles[loadChoice - 1];
                    if (master.loadGame(filename)) 
                    {
                        gameLoop();
                    }
                } 
                else 
                {
                    std::cout << "Invalid choice!\n";
                }
                break;
            }
                    
            case 3: 
                return;
                    
            default: 
                std::cout << "Invalid choice!\n";
        }
    }
}

void Game::gameLoop() 
{
    master.updateMap();
    
    while (master.isGameRunning() && player.checkPulse()) 
    {
        std::cout << "\033c";
        
        visualizer.drawMap(master.game_map);
        visualizer.drawUI(player, master.getCurrentLevel(), player.listSpells());
        visualizer.drawControls(keyBinder);
        
        char input = inputController.getInput();
        
        Direction dir = Direction::KNOWHERE;
        bool attack = false, switchAttack = false, openShop = false;
        bool openUpgrade = false, openSave = false, openLoad = false;
        bool toggleAnim = false, keyConfig = false, backToMenu = false;
        
        bool processed = inputController.processInput(
            input, dir, attack, switchAttack, openShop, openUpgrade,
            openSave, openLoad, toggleAnim, keyConfig, backToMenu
        );
        
        if (!processed && isdigit(input)) 
        {
            int spellNum = input - '0';
            auto spells = player.listSpells();
            if(spellNum > 0 && spellNum <= static_cast<int>(spells.size())) 
            {
                SpellType selectedSpell = static_cast<SpellType>(spells[spellNum-1].first);
                SpellHand spell(selectedSpell);

                int effectiveRange = spell.getRange();
                if(selectedSpell == SpellType::DEATH_NOTE)
                    effectiveRange = (player.getAttackType() == ATKtype::melee) ? 1 : 3;

                AimingSystem aim;
                switch(selectedSpell) 
                {
                    case SpellType::DEATH_NOTE: aim.setDeathNoteAim(); break;
                    case SpellType::PSI_STORM: aim.setPsiStormAim(spell.getAoE()); break;
                    case SpellType::TRAP: aim.setTrapAim(); break;
                    case SpellType::DOPPELGANGER: aim.setDoppelgangerAim(); break;
                    case SpellType::LEVEL_UP: 
                    {
                        SpellLvlUP lvlShop(player);
                        lvlShop.displayUpgrades();
                        int upgradeChoice;
                        std::cin >> upgradeChoice;
                        if(lvlShop.upgradeSpell(upgradeChoice)) 
                        {
                            std::cout << "Spell upgraded!\n";
                        } 
                        else std::cout << "Upgrade failed!\n";
                        master.updateGameWorld();
                        continue;
                    }
                }
                
                Coordinates aimPos(player.getX_coord(), player.getY_coord());
                bool aiming = true;
                while(aiming) 
                {
                    Map tempMap = master.game_map;
                    aim.drawAim(tempMap, aimPos);
                    std::cout << "\033c";
                    tempMap.draw();
                    std::cout << "Aiming: Use " << keyBinder.getKey(KeyBinder::MOVE_UP) 
                              << "-Up, " << keyBinder.getKey(KeyBinder::MOVE_DOWN) 
                              << "-Down, " << keyBinder.getKey(KeyBinder::MOVE_LEFT)
                              << "-Left, " << keyBinder.getKey(KeyBinder::MOVE_RIGHT)
                              << "-Right, 'x' to cast, 'c' to cancel\n";
                    char aimInput;
                    std::cin >> aimInput;
                    Coordinates newAimPos = aimPos;
                    aimInput = toupper(aimInput);
                    
                    if (aimInput == keyBinder.getKey(KeyBinder::MOVE_UP)) 
                    {
                        newAimPos.setXY_coord(newAimPos.getX_coord(), newAimPos.getY_coord()-1);
                    } 
                    else if (aimInput == keyBinder.getKey(KeyBinder::MOVE_DOWN)) 
                    {
                        newAimPos.setXY_coord(newAimPos.getX_coord(), newAimPos.getY_coord()+1);
                    } 
                    else if (aimInput == keyBinder.getKey(KeyBinder::MOVE_LEFT)) 
                    {
                        newAimPos.setXY_coord(newAimPos.getX_coord()-1, newAimPos.getY_coord());
                    } 
                    else if (aimInput == keyBinder.getKey(KeyBinder::MOVE_RIGHT)) 
                    {
                        newAimPos.setXY_coord(newAimPos.getX_coord()+1, newAimPos.getY_coord());
                    } 
                    else if (aimInput == 'X') 
                    {
                        if(master.processSpellCast(selectedSpell, aimPos)) 
                        {
                            if(selectedSpell != SpellType::DEATH_NOTE && selectedSpell != SpellType::LEVEL_UP)
                                player.spendSpell(static_cast<int>(selectedSpell));
                            std::cout << "Spell cast successfully!\n";
                            master.updateGameWorld();
                        } 
                        else std::cout << "Spell failed! No valid target.\n";
                        aiming = false;
                    } 
                    else if (aimInput == 'C') 
                    {
                        aiming = false;
                    } 
                    else 
                    {
                        continue;
                    }
                    
                    if(aiming && master.game_map.iswithinlimits(newAimPos.getX_coord(), newAimPos.getY_coord())) 
                    {
                        int distX = abs(newAimPos.getX_coord() - player.getX_coord());
                        int distY = abs(newAimPos.getY_coord() - player.getY_coord());
                        if(distX <= effectiveRange && distY <= effectiveRange) aimPos = newAimPos;
                    }
                }
                continue;
            }
        }
        
        if (toggleAnim) 
        {
            enable_animations = !enable_animations;
            std::cout << "Animations " << (enable_animations ? "ENABLED" : "DISABLED") << "\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            continue;
        }
        
        if (keyConfig) 
        {
            showKeyConfigMenu();
            continue;
        }
        
        if (openShop) 
        {
            SpellShop shop(player);
            bool inShop = true;
            while(inShop) 
            {
                shop.displayShop();
                std::cout << "6. Exit shop\n";
                std::cout << "Choose spell (1-5) or 6 to exit: ";                     
                std::string shopInput;
                std::cin >> shopInput;
                if(shopInput == "6") inShop = false;
                else if(shopInput.length() == 1 && shopInput[0] >= '1' && shopInput[0] <= '5') 
                {
                    int choice = shopInput[0] - '0';
                    if(shop.buySpell(choice)) 
                    {
                        std::cout << "Spell purchased!\n";
                        std::cout << "\033c";
                    } 
                    else std::cout << "Purchase failed!\n";
                } 
                else std::cout << "Invalid choice!\n";
            }
            continue;
        }
        
        if (openUpgrade) 
        {
            master.showUpgradeMenu();
            continue;
        }
        
        if (backToMenu) 
        {
            return;
        }
        
        if (openSave) 
        {
            std::cout << "Save Game:\n";
            
            std::cout << "1. Quick Save (autosave.txt)\n";
            std::cout << "2. Save Slot 1 (save1.txt)\n";
            std::cout << "3. Save Slot 2 (save2.txt)\n";
            std::cout << "4. Save Slot 3 (save3.txt)\n";
            std::cout << "5. Save Slot 4 (save4.txt)\n";
            std::cout << "6. Save Slot 5 (save5.txt)\n";
            std::cout << "7. Cancel\n";
            
            int saveChoice;
            std::cin >> saveChoice;
            
            switch(saveChoice) 
            {
                case 1: master.saveGame("autosave.txt"); break;
                case 2: master.saveGame("save1.txt"); break;
                case 3: master.saveGame("save2.txt"); break;
                case 4: master.saveGame("save3.txt"); break;
                case 5: master.saveGame("save4.txt"); break;
                case 6: master.saveGame("save5.txt"); break;
                case 7: break;
                default: std::cout << "Invalid choice!\n";
            }
            continue;
        }
        
        if (openLoad) 
        {
            std::cout << "Load Game:\n";
            
            std::vector<std::pair<std::string, std::string>> availableSaves;
            
            std::vector<std::pair<std::string, std::string>> allSaves = 
            {
                {"autosave.txt", "Quick Save"},
                {"save1.txt", "Slot 1"},
                {"save2.txt", "Slot 2"},
                {"save3.txt", "Slot 3"},
                {"save4.txt", "Slot 4"},
                {"save5.txt", "Slot 5"}
            };
            
            int optionNumber = 1;
            for (const auto& [filename, description] : allSaves) 
            {
                std::ifstream file(filename);
                if (file.good()) 
                {
                    std::cout << optionNumber++ << ". " << description << " (" << filename << ")\n";
                    availableSaves.push_back({filename, description});
                    file.close();
                }
            }
            
            if (availableSaves.empty()) 
            {
                std::cout << "No save files found!\n";
                std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                continue;
            }
            
            std::cout << optionNumber << ". Cancel\n";
            
            int loadChoice;
            std::cin >> loadChoice;
            
            if (loadChoice == optionNumber) 
            {
                continue; // Cancel
            }
            
            if (loadChoice >= 1 && loadChoice <= availableSaves.size()) 
            {
                std::string filename = availableSaves[loadChoice - 1].first;
                if (master.loadGame(filename)) 
                {
                    std::cout << "Game loaded successfully!\n";
                    std::this_thread::sleep_for(std::chrono::milliseconds(500));
                }
            } 
            else 
            {
                std::cout << "Invalid choice!\n";
            }
            continue;
        }
        
        if (attack) 
        {
            master.processPlayerAttack();
            master.updateGameWorld();
            continue;
        }
        
        if (switchAttack) 
        {
            player.switchAttackType();
            if (player.getAttackType() == ATKtype::melee) 
            {
                animPlayer.play("MeleeStance", 1);
            } 
            else 
            {
                animPlayer.play("RangedStance", 1);
            }
            continue;
        }

        Coordinates target = player.moveto(dir);
        MasterCall result = master.processMoveRequest(
            Coordinates(player.getX_coord(), player.getY_coord()), 
            target
        );
        
        bool playerMoved = player.move(result.getDestination(), result.getDamage(), 
                player.getSpeed() - result.getSlowdown(), result.getPoints());

        if (!playerMoved || !player.checkPulse()) 
        {
            animPlayer.play("Dead", 1);
            GameLogger::logEvent("Player died at level " + std::to_string(master.getCurrentLevel()));
            std::cout << "Game Over! Press any key to continue...\n";
            std::cin.ignore();
            std::cin.get();
            break;
        }
        
        if (master.checkLevelCompletion()) continue;
        
        master.updateGameWorld();
    }
    
    if (!player.checkPulse()) 
    {
        std::cout << "You died! Better luck next time!\n";
    } 
    else if (!master.isGameRunning()) 
    {
        std::cout << "Game completed! Press any key to return to menu...\n";
        std::cin.ignore();
        std::cin.get();
    }
    
    keyBinder.saveToFile("keys.cfg");
}