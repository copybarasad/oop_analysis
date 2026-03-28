    #include "Renderer.h"

    using namespace std;

    void Renderer::DisplayGameInfo(const Game& game) 
    {
        cout << endl;
        cout << "The game started" << endl;
        if (game.GetField()) 
        {
            cout << "Field size: " << game.GetField()->GetWidth() << "x" << game.GetField()->GetHeight() << endl;
        }
        cout << "Player position: (" << game.GetPlayer().GetX_player() << ", " << game.GetPlayer().GetY_player() << ")" << endl;
        cout << "Enemy spawned: " << game.GetEnemy().GetNickname_enemy() << " at position (" << game.GetEnemy().GetX_enemy() << ", " << game.GetEnemy().GetY_enemy() << ")" << endl;
        cout << "Your combat range: " << game.GetPlayer().GetBattle() << endl;
        cout << "Spell hand size: " << game.GetSpellHand().GetSpellCount() << "/" << 5 << endl;
        cout << "Control: WASD - movement, E - exit, I - instruction manual, Z - statistics, " << endl;
        cout << "B - changing the damage range, C - cast spell, H - show spells, P - purchase spells" << endl;
        cout << "S - save game, L - load game" << endl;
    }

    void Renderer::DisplayGameState(const Game& game) 
    {
        ClearScreen();
        
        DisplayStatistics(game);
        
        cout << endl;
        
        cout << "=== GAME FIELD ===" << endl;
        if (game.GetField()) 
        {
            game.GetField()->DisplayField(game.GetPlayer(), game.GetEnemy());
            cout << endl;
        }
        
        cout << endl;
        cout << "Enter the command: " << endl;
    }

    void Renderer::DisplayStatistics(const Game& game) 
    {
        cout << endl;
        
        if (game.GetPlayer().IsAlive_player()) 
        {
            game.GetPlayer().Statistics_player();
        } 
        else 
        {
            cout << "PLAYER: DEFEATED" << endl;
        }
        
        cout << endl;

        if (game.GetEnemy().IsAlive_enemy()) 
        {
            game.GetEnemy().Statistics_enemy();
        } 
        else 
        {
            cout << "ENEMY: DEFEATED" << endl;
        }
        
        cout << endl;
        cout << "Spell hand: " << game.GetSpellHand().GetSpellCount() << "/5 spells" << endl;
        cout << "Current level: " << game.GetCurrentLevel() << endl;
    }

    void Renderer::RenderMainMenu() 
    {
        cout << "=== Main Menu ===" << endl;
        cout << "1. New Game" << endl;
        cout << "2. Load Game" << endl;
        cout << "Choose: ";
    }

    void Renderer::RenderPlayerNameInput() 
    {
        cout << "Enter your player's name: ";
    }

    void Renderer::RenderLevelStart(int level) 
    {
        cout << "=== Starting Level " << level << " ===" << endl;
    }

    void Renderer::RenderFieldSizeInput() 
    {
        cout << "Enter the size of the playing field (enter width first and then height): ";
    }

    void Renderer::RenderBarriersCountInput() 
    {
        int maxPossibleBarriers = 10;
        cout << "Enter the number of barriers (maximum number = 10): " << endl;
    }

    void Renderer::RenderCombatRangeInput() 
    {
        cout << "Enter the combat range (0 - close combat, 1 - long combat): " << endl;
    }

    void Renderer::RenderSpellShop(const Player& player) 
    {
        cout << "=== Spell Shop ===" << endl;
        cout << "Your mana: " << player.GetMana() << "/100" << endl;
        cout << "Available spells:" << endl;
        cout << "1. Fireball - 15 mana (Deals 15 damage, range 3)" << endl;
        cout << "2. Bomb - 18 mana (Deals 12 damage in 2 radius)" << endl;
        cout << "3. Bear Trap - 10 mana (Places a trap, deals 10 damage)" << endl;
        cout << "Enter spell number to buy (or -1 to cancel): ";
    }

    void Renderer::RenderSpellPurchaseResult(bool success, const std::string& spellName, int manaCost, int currentMana) 
    {
        if (success) 
        {
            cout << "Purchased " << spellName << " for " << manaCost << " mana!" << endl;
        } 
        else 
        {
            cout << "Not enough mana! Need " << manaCost << ", have " << currentMana << endl;
        }
    }

    void Renderer::RenderInvalidSpellChoice() 
    {
        cout << "Invalid spell choice!" << endl;
    }

    void Renderer::RenderNoSpellsMessage() 
    {
        cout << "No spells in hand!" << endl;
    }

    void Renderer::RenderSpellTargetSelection(const std::string& spellName, const std::string& enemyName) 
    {
        cout << "Select target for " << spellName << ":" << endl;
        cout << "1. Enemy (" << enemyName << ")" << endl;
        cout << "2. Tower" << endl;
        cout << "Enter target number (or -1 to cancel): ";
    }

    void Renderer::RenderCannotCastOnEnemy() 
    {
        cout << "Cannot cast this spell on enemy!" << endl;
    }

    void Renderer::RenderCannotCastOnTower() 
    {
        cout << "Cannot cast this spell on tower - target out of range or not enough mana!" << endl;
    }

    void Renderer::RenderNoTowersAvailable() 
    {
        cout << "No towers available to target!" << endl;
    }

    void Renderer::RenderInvalidTargetChoice() 
    {
        cout << "Invalid target choice! Spell not used." << endl;
    }

    void Renderer::RenderSpellCastingCancelled() {

        cout << "Spell casting cancelled." << endl;
    }

    void Renderer::RenderSpellCastOnTower(const std::string& playerName, const std::string& spellName, int towerX, int towerY, int towerHp, int towerMaxHp) 
    {
        cout << playerName << " casts " << spellName << " on tower at (" << towerX << ", " << towerY << ")!" << endl;
        cout << "Dealt 25 damage to tower!" << endl;
        cout << "Tower HP: " << towerHp << "/" << towerMaxHp << endl;
    }

    void Renderer::RenderInvalidSpellIndex() 
    {
        cout << "Invalid spell index!" << endl;
    }

    void Renderer::RenderCannotCastSpell() 
    {
        cout << "Cannot cast this spell!" << endl;
    }

    void Renderer::RenderGameOver() 
    {
        cout << "Game over! You have not got hp(" << endl;
    }

    void Renderer::RenderEnemyDefeated() 
    {
        cout << "Enemy defeated!" << endl;
    }

    void Renderer::RenderLevelCompleted(int level) 
    {
        cout << endl;
        cout << "=== Level " << level << " Completed! ===" << endl;
        cout << "Congratulations! Moving to next level..." << endl;
    }

    void Renderer::RenderRestartPrompt() 
    {
        cout << "Would you like to restart the game? (y/n): ";
    }

    void Renderer::RenderExitMessage() 
    {
        cout << "You are out of the game" << endl;
    }

    void Renderer::RenderInvalidInput() 
    {
        cout << "Invalid input. Please enter a character." << endl;
    }

    void Renderer::RenderInvalidWidthInput() 
    {
        cout << "Invalid input for width. Using default size 10x10." << endl;
    }

    void Renderer::RenderInvalidHeightInput() 
    {
        cout << "Invalid input for height. Using default size 10x10." << endl;
    }

    void Renderer::RenderInvalidFieldSize() 
    {
        cout << "You entered the wrong coordinates. The default field dimensions are 10*10" << endl;
    }

    void Renderer::RenderTooManyBarriers() 
    {
        cout << "Too many barriers. The default number of barriers is 5" << endl;
    }

    void Renderer::RenderNegativeBarriers() 
    {
        cout << "Number of barriers cannot be negative. The default number of barriers is 5" << endl;
    }

    void Renderer::RenderInvalidCombatRange() 
    {
        cout << "You have entered an incorrect range value, the default range value is 0" << endl;
    }

    void Renderer::RenderRandomSpellFound() 
    {
        cout << "You found a random spell!" << endl;
    }

    void Renderer::RenderSpellsRemoved(int count) 
    {
        cout << "Removed " << count << " random spells from your hand." << endl;
    }

    void Renderer::RenderDebugFieldState(const Game& game) 
    {
        if (!game.GetField()) 
        {
            cout << "No field initialized" << endl;
            return;
        }
        
        cout << "=== DEBUG FIELD STATE ===" << endl;
        cout << "Field size: " << game.GetField()->GetWidth() << "x" << game.GetField()->GetHeight() << endl;
        
        const vector<Cell*>& barriers = game.GetField()->GetBarriers();
        cout << "Barriers via GetBarriers(): " << barriers.size() << endl;
        
        for (const auto& barrier : barriers) 
        {
            cout << "Barrier at (" << barrier->GetX() << ", " << barrier->GetY() << ")" << endl;
        }
        
        cout << "Player position: (" << game.GetPlayer().GetX_player() << ", " << game.GetPlayer().GetY_player() << ")" << endl;
        cout << "Enemy position: (" << game.GetEnemy().GetX_enemy() << ", " << game.GetEnemy().GetY_enemy() << ")" << endl;
    }

    void Renderer::RenderCommandMessage(const std::string& message) 
    {
        cout << message << endl;
    }

    void Renderer::ClearScreen() 
    {
        #ifdef _WIN32
            system("cls");
        #else
            system("clear");
        #endif
    }

    void Renderer::RenderSeparator() 
    {
        std::cout << "----------------------------------------" << std::endl;
    }

    