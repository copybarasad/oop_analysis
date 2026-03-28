#pragma once

#include <string> 

class Game;
class Player;
class GameField;
class AreaDamageSpell;
class Spell;
class Enemy;


class GameRenderer {
public:
     // Метод для отрисовки информации о заклинаниях игрока
    void renderPlayerSpellInfo(const Player& player) const;

    //для SpellCasting:
    void renderSpellCastSuccess(int remainingMana) const;
    void renderSpellInfo(const Player& player) const;
    void renderSpellPurchased() const;
    void renderSpellHandFull() const;

    // для CombatManager:
    void renderPlayerAttackEnemy(int damage, int currentHealth, int maxHealth) const;
    void renderEnemyDefeated(int scoreBonus = 10) const;
    void renderEnemyAttacksPlayer(const Enemy& enemy, int damage) const;

    //  LevelManager:
    void renderLevelTransition(int level, int width, int height, int nextLevelScore) const;
    void renderSpellLossInfo(int spellsLost) const;
    void renderLevelUpMenu(const Player& player) const;

    
    void renderSpellUpgradeMenu(const Player& player) const;
    void renderSpellUpgradeOptions(const Spell* spell) const;
    
    void renderPlayerStats(const Player& player, int level) const;
    void renderEnemiesSpawned(int count, int level) const;

    // для меню spell
    void renderSpellUpgradeDetails(const Spell* spell) const;
    void renderSpellSpecificUpgradeOptions(const Spell* spell) const;

    //
    void renderNoSpellsToUpgrade() const;
    void renderUnknownSpellType() const;

    void renderSpellDamageEffect(const std::string& spellType, 
                                         const std::string& result, 
                                         int x, int y, int damage = 0) const;

    void renderSpellTargetIssue(const std::string& issueType, 
                                        int distance = 0, int range = 0) const;

    void renderOperationResult(const std::string &operation, const std::string &result, const std::string &details = "") const;

    void renderInsufficientResource(const std::string &resourceType, int needed, int have) const;

    void renderUpgradeResult(const std::string &targetType, const std::string &upgradeType, const std::string &name = "", int newValue = 0) const;

    void renderGameStateOperation(const std::string& operation, int level) const;


    void renderGameField(const GameField& field) const;
    void renderLevelStart(int level) const;
    void renderGameOver(int finalScore) const;
    void renderError(const std::string& context, const std::string& message) const;
    void renderSpellShop(int playerScore, int spellsInHand, int maxSpells) const;
    void renderMainMenu() const;
    void renderSizeInputPrompt() const;
    void renderControlsHelp(const std::string &controlsHelp) const;
    void renderExtendedMenu() const;
    void renderSpellSelectionPrompt(int spellCount) const;
    void renderCoordinatesInputPrompt() const;
    void renderConfirmationPrompt(const std::string& message) const;
    void renderGameOverMenu() const;
    void renderSaveFileInfo(const std::string& saveInfo) const;
};