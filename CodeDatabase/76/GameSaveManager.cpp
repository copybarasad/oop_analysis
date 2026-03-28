#include "GameSaveManager.h"
#include "Game.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <memory>
#include <filesystem>

using namespace std;

GameSaveManager::GameSaveManager(Game* gameInstance) : game(gameInstance) {}

bool GameSaveManager::SaveGame(const string& filename) 
{
    try 
    {
        ofstream file(filename);
        if (!file.is_open()) 
        {
            throw runtime_error("Cannot open file: " + filename);
        }
        
        string gameData = SerializeGameState();
        file << gameData;
        file.close();
        
        cout << "Game successfully saved to: " << filename << endl;
        return true;
        
    } 
    catch (const exception& e) 
    {
        cout << "Error saving game: " << e.what() << endl;
        return false;
    }
}

bool GameSaveManager::LoadGame(const string& filename) 
{
    try 
    {
        ifstream file(filename);
        if (!file.is_open()) 
        {
            throw runtime_error("Cannot open file: " + filename);
        }
        
        stringstream buffer;
        buffer << file.rdbuf();
        string gameData = buffer.str();
        file.close();
        
        ClearGameState();
        bool success = DeserializeGameState(gameData);
        
        if (success) 
        {
            cout << "Game successfully loaded from: " << filename << endl;
            cout << "Game loaded successfully from: " << filename << endl;
            return true;
        } 
        else 
        {
            throw runtime_error("Failed to parse save data");
        }
        
        return success;
        
    } 
    catch (const exception& e) 
    {
        cout << "Error loading game: " << e.what() << endl;
        return false;
    }
}

string GameSaveManager::SerializeGameState() 
{
    stringstream data;
    
    data << "GAME_SAVE_V3\n";
    data << game->currentLevel << "\n";
    data << game->isRunning << "\n";
    
    data << game->player.GetNickname_player() << "\n";
    data << game->player.GetX_player() << "\n";
    data << game->player.GetY_player() << "\n";
    data << game->player.GetHp_player() << "\n";
    data << game->player.GetMana() << "\n";
    data << game->player.GetBattle() << "\n";
    data << game->player.GetDamage_player() << "\n";
    
    data << game->enemy.GetNickname_enemy() << "\n";
    data << game->enemy.GetX_enemy() << "\n";
    data << game->enemy.GetY_enemy() << "\n";
    data << game->enemy.GetHp_enemy() << "\n";
    data << game->enemy.GetDamage_enemy() << "\n";
    
    if (game->field) 
    {
        data << game->field->GetWidth() << "\n";
        data << game->field->GetHeight() << "\n";
        
        vector<pair<int, int>> barrierPositions = game->field->GetBarrierCoordinates();
        
        cout << "Found " << barrierPositions.size() << " barriers" << endl;
        
        data << barrierPositions.size() << "\n";
        
        for (const auto& barrierPos : barrierPositions) {
            data << barrierPos.first << "\n";
            data << barrierPos.second << "\n";
        }
        
        const vector<EnemyTower>& towers = game->field->GetTowers();
        data << towers.size() << "\n";
        for (const auto& tower : towers) 
        {
            data << tower.GetX() << "\n";
            data << tower.GetY() << "\n";
            data << tower.GetHp() << "\n";
            data << tower.GetMaxHp() << "\n";
            data << tower.GetDamage() << "\n";
        }
    } 
    else 
    {
        data << "0\n0\n0\n0\n"; 
    }
    
    data << game->spellHand.GetSpellCount() << "\n";
    
    for (int i = 0; i < game->spellHand.GetSpellCount(); ++i) 
    {
        auto spell = game->spellHand.GetSpell(i);
        if (spell) {
            data << spell->GetName() << "\n";
            data << spell->GetManaCost() << "\n";
        }
    }
    
    return data.str();
}

bool GameSaveManager::DeserializeGameState(const string& data) 
{
    try 
    {
        stringstream ss(data);
        string version;
        getline(ss, version);
        
        if (version != "GAME_SAVE_V1" && version != "GAME_SAVE_V2" && version != "GAME_SAVE_V3") 
        {
            throw runtime_error("Unsupported save file version: " + version);
        }
        
        ss >> game->currentLevel;
        ss >> game->isRunning;
        ss.ignore();
        
        string playerName;
        getline(ss, playerName);
        game->player.SetNickname_player(playerName);
        
        int x, y, hp, mana, battle, damage;
        ss >> x >> y >> hp >> mana >> battle >> damage;
        game->player.SetX_player(x);
        game->player.SetY_player(y);
        game->player.SetHp_player(hp);
        game->player.SetMana(mana);
        game->player.SetBattle(battle);
        game->player.SetDamage_player(damage);
        ss.ignore();
        
        string enemyName;
        getline(ss, enemyName);
        
        int ex, ey, ehp, edamage;
        ss >> ex >> ey >> ehp >> edamage;
        ss.ignore();
        
        game->enemy.SetNickname_enemy(enemyName);
        game->enemy.SetX_enemy(ex);
        game->enemy.SetY_enemy(ey);
        game->enemy.SetHp_enemy(ehp);
        game->enemy.SetDamage_enemy(edamage);
        
        int width, height;
        ss >> width >> height;
        
        if (width > 0 && height > 0) 
        {
            if (game->field) delete game->field;
            
            game->field = new PlayingField(width, height, 0);
            
            game->player.SetField(game->field);
            game->enemy.SetField(game->field);
            
            int barriersCount;
            ss >> barriersCount;
            ss.ignore();

            vector<pair<int, int>> barrierPositions;
            for (int i = 0; i < barriersCount; ++i) {
                int bx, by;
                ss >> bx >> by;
                ss.ignore();
                
                if (bx >= 0 && bx < width && by >= 0 && by < height) {
                    barrierPositions.push_back({bx, by});
                }
            }

            if (!barrierPositions.empty()) {
                game->field->RestoreBarriers(barrierPositions);
            }
            
            int towersCount;
            ss >> towersCount;
            ss.ignore();
            
            for (int i = 0; i < towersCount; ++i) 
            {
                int tx, ty, thp, tmaxHp, tdamage;
                ss >> tx >> ty >> thp >> tmaxHp >> tdamage;
                ss.ignore();
                               
                if (tx >= 0 && tx < width && ty >= 0 && ty < height) {
                    try {
                        EnemyTower* tower = game->field->AddTower(tx, ty);
                        if (tower) 
                        {
                            tower->SetHp(thp);
                            tower->SetMaxHp(tmaxHp);
                            tower->SetDamage(tdamage);
                        }
                    } catch (const exception& e) {
                        cout << "Failed to add tower: " << e.what() << endl;
                    }
                }
            }
        }
        
        int spellCount;
        ss >> spellCount;
        ss.ignore();
        
        game->spellHand.Clear();
        
        for (int i = 0; i < spellCount; ++i) 
        {
            string spellName;
            int spellMana;
            
            getline(ss, spellName);
            ss >> spellMana;
            ss.ignore();
            
            shared_ptr<SpellCard> spell;
            if (spellName == "Fireball") 
            {
                spell = make_shared<DirectDamageSpell>("Fireball", 15, spellMana, 3);
            } 
            else if (spellName == "Bomb") 
            {
                spell = make_shared<AreaDamageSpell>("Bomb", 12, spellMana, 2);
            } 
            else if (spellName == "Bear Trap") 
            {
                spell = make_shared<TrapSpell>("Bear Trap", 10, spellMana);
            }
            
            if (spell) 
            {
                game->spellHand.AddSpell(spell);
            }
        }
        
        cout << "Game loaded successfully" << endl;
        cout << "Player: " << game->player.GetNickname_player() << " at (" << game->player.GetX_player() << ", " << game->player.GetY_player() << ")" << endl;
        cout << "Enemy: " << game->enemy.GetNickname_enemy() << " at (" << game->enemy.GetX_enemy() << ", " << game->enemy.GetY_enemy() << ")" << endl;
        cout << "Level: " << game->currentLevel << endl;
        cout << "Spells: " << game->spellHand.GetSpellCount() << endl;
        
        return true;
        
    } 
    catch (const exception& e) 
    {
        cout << "Error loading game: " << e.what() << endl;
        return false;
    }
}

void GameSaveManager::ClearGameState() 
{
    if (game->field) 
    {
        delete game->field;
        game->field = nullptr;
    }
    game->spellHand.Clear();
}

void GameSaveManager::ShowSaveMenu() 
{
    cout << "=== Save Game ===" << endl;
    cout << "Enter save file name (or 'cancel' to return): ";
    
    string filename;
    cin >> filename;
    
    if (filename == "cancel") 
    {
        cout << "Save cancelled." << endl;
        return;
    }
    
    if (filename.find('.') == string::npos) 
    {
        filename += ".save";
    }
    
    if (SaveGame(filename)) 
    {
        cout << "\nGame saved successfully!" << endl;
    } 
    else 
    {
        cout << "Failed to save game. Please try again." << endl;
    }
}

void GameSaveManager::ShowLoadMenu() 
{
    cout << "=== Load Game ===" << endl;
    
    vector<string> saveFiles = GetSaveFiles();
    
    if (saveFiles.empty()) 
    {
        cout << "No save files found." << endl;
        return;
    }
    
    cout << "Available saves:" << endl;
    for (size_t i = 0; i < saveFiles.size(); ++i) 
    {
        cout << i + 1 << ". " << saveFiles[i] << endl;
    }
    cout << "0. Cancel" << endl;
    cout << "Select save file: ";
    
    int choice;
    cin >> choice;
    
    if (choice == 0) 
    {
        cout << "Load cancelled." << endl;
        return;
    }
    
    if (choice > 0 && choice <= static_cast<int>(saveFiles.size())) 
    {
        if (LoadGame(saveFiles[choice - 1])) 
        {
            game->isRunning = true;
        } 
        else 
        {
            cout << "Failed to load game. Please try again." << endl;
        }
    } 
    else 
    {
        cout << "Invalid selection." << endl;
    }
}

bool GameSaveManager::ShowLoadMenuAtStart() 
{
    cout << "=== Load Game ===" << endl;
    
    vector<string> saveFiles = GetSaveFiles();
    
    if (saveFiles.empty()) 
    {
        cout << "No save files found." << endl;
        return false;
    }
    
    cout << "Available saves:" << endl;
    for (size_t i = 0; i < saveFiles.size(); ++i) 
    {
        cout << i + 1 << ". " << saveFiles[i] << endl;
    }
    cout << "0. Start new game" << endl;
    cout << "Select save file: ";
    
    int choice;
    cin >> choice;
    
    if (choice == 0) 
    {
        cout << "Starting new game..." << endl;
        return false;
    }
    
    if (LoadGame(saveFiles[choice - 1])) 
    {
        game->isRunning = true;
        return true;
    }
    return false;
}

vector<string> GameSaveManager::GetSaveFiles() 
{
    vector<string> saveFiles;
    try 
    {
        for (const auto& entry : filesystem::directory_iterator(".")) 
        {
            if (entry.path().extension() == ".save") 
            {
                saveFiles.push_back(entry.path().filename().string());
            }
        }
    } 
    catch (const filesystem::filesystem_error& e) 
    {
        cout << "Error accessing save files: " << e.what() << endl;
    }
    return saveFiles;
}