#include "Game.h"
#include "InputHandler.h"
#include "Renderer.h"
#include <algorithm>
#include <fstream>
#include <sstream>
#include <iostream>
#include <limits>
#include <cstdlib>
#include <ctime>

using namespace std;

Game::Game() : field(nullptr), spellHand(5), isRunning(false), currentLevel(1) {
    srand(time(0));
}

Game::~Game() 
{   
    if (field) 
    {
        delete field;
        field = nullptr;
    }
}

// Game.cpp - ИЗМЕНИТЬ метод StartGame():
void Game::StartGame() 
{
    isRunning = true;
    Renderer::RenderMainMenu();
    
    int choice;
    cin >> choice;
    
    GameSaveManager saveManager(this);
    
    if (choice == 2) 
    {
        if (saveManager.ShowLoadMenuAtStart()) 
        {
            // Только инициализация
        } 
        else 
        {
            currentLevel = 1;
            StartLevel(currentLevel);
        }
    } 
    else 
    {
        currentLevel = 1;
        StartLevel(currentLevel);
    }
    
    // НЕ вызывать GameLoop() здесь!
}

void Game::InitializePlayer() 
{
    string name;
    Renderer::RenderPlayerNameInput();
    cin >> name;

    if (name.empty()) {
        name = "Steve";
    }
    player.SetNickname_player(name);
    
    int combat_range = GetCombatRange();
    player.SetBattle(combat_range);
    
    player.SetMana(100);
}

void Game::StartLevel(int level) 
{
    currentLevel = level;
    isRunning = true;
    Renderer::RenderLevelStart(level);
    
    if (level == 1) {
        InitializePlayer();
    }
    
    int baseSize;
    int barriers;
    
    if (level == 1) {
        baseSize = GetFieldSize();
        barriers = GetBarriersCount();
    } else {
        baseSize = 10 + (level - 1) * 2;
        if (baseSize > 25) baseSize = 25;
        barriers = 5 + (level - 1) * 5; 
    }
    
    if (field) {
        delete field;
    }
    field = new PlayingField(baseSize, baseSize, barriers);
    
    player.SetHp_player(100);
    player.SetMana(100);
    
    enemy = Enemy();
    if (level > 1) 
    {
        int bonusHp = (level - 1) * 20;
        int bonusDamage = (level - 1) * 5;
        
        enemy.SetMaxHp_enemy(enemy.GetMaxHp_enemy() + bonusHp);
        enemy.SetHp_enemy(bonusHp);
        
        enemy.SetDamage_enemy(enemy.GetDamage_enemy() + bonusDamage);
    }
    enemy.SetField(field);
    player.SetField(field);

    player.SetX_player(1);  
    player.SetY_player(1); 
    
    Renderer::DisplayGameInfo(*this);
}

int Game::GetFieldSize() 
{
    Renderer::RenderFieldSizeInput();
    int width, height;
    
    if (!(cin >> width)) 
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        Renderer::RenderInvalidWidthInput();
        width = 10;
        height = 10;
    }
    else if (!(cin >> height)) 
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        Renderer::RenderInvalidHeightInput();
        width = 10;
        height = 10;
    } 

    if (width < 10 || width > 25 || height < 10 || height > 25) 
    {
        Renderer::RenderInvalidFieldSize();
        width = 10;
        height = 10;
    }
    
    return width;
}

int Game::GetBarriersCount() 
{
    Renderer::RenderBarriersCountInput();
    int number_of_barriers;
    cin >> number_of_barriers;
    
    if (number_of_barriers > 10) 
    {
        Renderer::RenderTooManyBarriers();
        number_of_barriers = 5;
    }

    if (number_of_barriers < 0) 
    {
        Renderer::RenderNegativeBarriers();
        number_of_barriers = 5;
    }
    
    return number_of_barriers;
}

int Game::GetCombatRange() 
{
    Renderer::RenderCombatRangeInput();
    int combat_range;
    cin >> combat_range;

    if (combat_range != 1 && combat_range != 0) 
    {
        Renderer::RenderInvalidCombatRange();
        combat_range = 0;
    }
    
    return combat_range;
}

void Game::GameLoop() 
{
    char input;
    do 
    {
        Renderer::DisplayGameState(*this);
        
        if (!player.CanMakeMove()) 
        {
            HandleGameOver();
            return;
        }

        input = GetPlayerInput();
        ProcessPlayerInput(input);
        
        if (CheckPlayerDefeat()) 
        {
            HandleGameOver();
            return;
        }

        if (!enemy.IsAlive_enemy()) 
        {
            HandleLevelCompletion();
            return; 
        }

    } while (input != 'E' && input != 'e' && isRunning);

    Renderer::RenderExitMessage();
}

char Game::GetPlayerInput() 
{
    char input;
    if (!(cin >> input)) 
    {
        cin.clear();
        cin.ignore(1000, '\n');
        Renderer::RenderInvalidInput();
        return ' ';
    }
    return input;
}

void Game::ProcessPlayerInput(char input) 
{
    bool playerMoved = false;
    bool spellCasted = false;

    InputHandler inputHandler;
    auto result = inputHandler.handleInput(input);

    switch(result.type) {
        case InputHandler::CommandType::MOVE_UP:
            player.MoveUp(); 
            playerMoved = true;
            break;
        case InputHandler::CommandType::MOVE_DOWN:
            player.MoveDown(); 
            playerMoved = true;
            break;
        case InputHandler::CommandType::MOVE_LEFT:
            player.MoveLeft(); 
            playerMoved = true;
            break;
        case InputHandler::CommandType::MOVE_RIGHT:
            player.MoveRight(); 
            playerMoved = true;
            break;
        case InputHandler::CommandType::SHOW_STATS:
            Renderer::DisplayStatistics(*this);
            break;
        case InputHandler::CommandType::SHOW_INSTRUCTIONS:
            player.Instruction();
            break;
        case InputHandler::CommandType::SHOW_SPELLS:
            spellHand.DisplaySpells(player, enemy);
            break;
        case InputHandler::CommandType::BUY_SPELLS:
            HandleSpellPurchase();
            break;
        case InputHandler::CommandType::CAST_SPELL:
            spellCasted = HandleSpellCasting(result.spellIndex);
            break;
        case InputHandler::CommandType::CHANGE_COMBAT_RANGE:
            HandleCombatRangeChange(result.combatRange);  
            break;
        case InputHandler::CommandType::SAVE_GAME: {
            GameSaveManager saveManager(this);
            saveManager.ShowSaveMenu();
            break;
        }
        case InputHandler::CommandType::LOAD_GAME: {
            GameSaveManager saveManager(this);
            saveManager.ShowLoadMenu();
            break;
        }
        case InputHandler::CommandType::EXIT:
            break;
        case InputHandler::CommandType::INVALID:
            Renderer::RenderCommandMessage(result.message);
            break;
    }

    if (playerMoved || spellCasted) {
        ExecuteGameTurn(playerMoved, spellCasted);
    }
}

void Game::HandleSpellPurchase() 
{
    Renderer::RenderSpellShop(player);
    
    int spellChoice;
    cin >> spellChoice;
    
    if (spellChoice == 1) 
    {
        if (player.GetMana() >= 15) 
        {
            auto fireball = make_shared<DirectDamageSpell>("Fireball", 15, 15, 3);
            if (spellHand.AddSpell(fireball)) 
            {
                player.SetMana(player.GetMana() - 15);
                Renderer::RenderSpellPurchaseResult(true, "Fireball", 15, player.GetMana());
            }
        } 
        else 
        {
            Renderer::RenderSpellPurchaseResult(false, "Fireball", 15, player.GetMana());
        }
    } 
    else if (spellChoice == 2) 
    {
        if (player.GetMana() >= 18) 
        {
            auto iceNova = make_shared<AreaDamageSpell>("Bomb", 12, 18, 2);
            if (spellHand.AddSpell(iceNova)) 
            {
                player.SetMana(player.GetMana() - 18);
                Renderer::RenderSpellPurchaseResult(true, "Bomb", 18, player.GetMana());
            }
        } 
        else 
        {
            Renderer::RenderSpellPurchaseResult(false, "Bomb", 18, player.GetMana());
        }
    } 
    else if (spellChoice == 3) 
    {
        if (player.GetMana() >= 10) 
        {
            auto bearTrap = make_shared<TrapSpell>("Bear Trap", 10, 10);
            if (spellHand.AddSpell(bearTrap)) 
            {
                player.SetMana(player.GetMana() - 10);
                Renderer::RenderSpellPurchaseResult(true, "Bear Trap", 10, player.GetMana());
            }
        } 
        else 
        {
            Renderer::RenderSpellPurchaseResult(false, "Bear Trap", 10, player.GetMana());
        }
    } 
    else if (spellChoice != -1) 
    {
        Renderer::RenderInvalidSpellChoice();
    }
}

bool Game::HandleSpellCasting(int spellIndex) 
{
    if (spellIndex == -1) 
    {
        if (spellHand.GetSpellCount() == 0) 
        {
            Renderer::RenderNoSpellsMessage();
            return false;
        }

        spellHand.DisplaySpells(player, enemy);
        cout << "Enter spell number to cast (or -1 to cancel): ";
        cin >> spellIndex;
    }
    
    if (spellIndex >= 0 && spellIndex < spellHand.GetSpellCount()) 
    {
        auto spell = spellHand.GetSpell(spellIndex);
        string spellName = spell->GetName();
        
        if (spellName == "Fireball") 
        {
            Renderer::RenderSpellTargetSelection(spellName, enemy.GetNickname_enemy());
            int targetChoice;
            cin >> targetChoice;
            
            if (targetChoice == 1) 
            {
                if (spellHand.CanCastSpell(spellIndex, player, enemy)) 
                {
                    spellHand.CastSpell(spellIndex, player, enemy);
                    spellHand.RemoveSpell(spellIndex);
                    return true;
                } 
                else 
                {
                    Renderer::RenderCannotCastOnEnemy();
                }
            } 
            else if (targetChoice == 2) 
            {
                EnemyTower* targetTower = field->GetNearestTower(player.GetX_player(), player.GetY_player());
                if (targetTower != nullptr) 
                {
                    int distance = abs(targetTower->GetX() - player.GetX_player()) + abs(targetTower->GetY() - player.GetY_player());
                    
                    if (player.GetMana() >= spell->GetManaCost() && distance <= 3) 
                    {
                        Renderer::RenderSpellCastOnTower(
                            player.GetNickname_player(), 
                            spellName, 
                            targetTower->GetX(), 
                            targetTower->GetY(),
                            targetTower->GetHp(),
                            targetTower->GetMaxHp()
                        );
                        targetTower->TakeDamage(25);
                        player.SetMana(player.GetMana() - spell->GetManaCost());
                        spellHand.RemoveSpell(spellIndex);
                        return true;
                    }
                    else 
                    {
                        Renderer::RenderCannotCastOnTower();
                    }
                } 
                else 
                {
                    Renderer::RenderNoTowersAvailable();
                }
            } 
            else if (targetChoice != -1) 
            {
                Renderer::RenderInvalidTargetChoice();
            }
            else 
            {
                Renderer::RenderSpellCastingCancelled();
            }
        }
        else 
        {
            if (spellHand.CanCastSpell(spellIndex, player, enemy)) 
            {
                spellHand.CastSpell(spellIndex, player, enemy);
                spellHand.RemoveSpell(spellIndex);
                return true;
            } 
            else 
            {
                Renderer::RenderCannotCastSpell();
            }
        }
    }
    else if (spellIndex != -1)
    {
        Renderer::RenderInvalidSpellIndex();
    }
    return false;
}

void Game::HandleCombatRangeChange(int combatRange) 
{
    player.SetBattle(combatRange);  
    ExecuteGameTurn(false, false);
}

void Game::ExecuteGameTurn(bool playerMoved, bool spellCasted) 
{
    if (playerMoved) 
    {
        player.DealDamage_player(player.GetDamage_player(), enemy);
    }

    TrapSpell::CheckTraps(*field, enemy);

    field->UpdateTowers();
    field->CheckTowerAttacks(player);

    if (enemy.IsAlive_enemy()) 
    {
        if (enemy.CanAttackPlayer(player)) 
        {
            enemy.AttackPlayer(player);
        } 
        else 
        {
            enemy.AI_MakeMove(player);
        }
        
        if (!enemy.IsAlive_enemy()) 
        {
            HandleEnemyDefeat();
        }
    }
    
    if (rand() % 5 == 0 && !spellHand.IsFull()) 
    {
        Renderer::RenderRandomSpellFound();
        spellHand.AddRandomSpell();
    }
}

bool Game::CheckPlayerDefeat() 
{
    return !player.IsAlive_player();
}

void Game::HandleGameOver() 
{
    Renderer::RenderGameOver();
    if (OfferRestart()) 
    {
        StartGame();
    } 
    else 
    {
        isRunning = false;
    }
}

bool Game::OfferRestart() 
{
    Renderer::RenderRestartPrompt();
    char choice;
    cin >> choice;
    return tolower(choice) == 'y';
}

void Game::HandleEnemyDefeat() 
{
    Renderer::RenderEnemyDefeated();
    HandleLevelCompletion();
}

void Game::HandleLevelCompletion() 
{
    Renderer::RenderLevelCompleted(currentLevel);

    int spellsToRemove = spellHand.GetSpellCount() / 2;
    if (spellsToRemove > 0) 
    {
        RemoveRandomSpells(spellsToRemove);
    }
    
    currentLevel++;
    StartLevel(currentLevel);
    // GameLoop(); 

    return;
}

void Game::RemoveRandomSpells(int count) 
{
    for (int i = 0; i < count && spellHand.GetSpellCount() > 0; i++) 
    {
        int randomIndex = rand() % spellHand.GetSpellCount();
        spellHand.RemoveSpell(randomIndex);
    }
    Renderer::RenderSpellsRemoved(count);
}

void Game::DebugFieldState() 
{
    Renderer::RenderDebugFieldState(*this);
}

