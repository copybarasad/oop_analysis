#pragma once

#include "Renderer.h"
#include <memory>
#include <string>


template<typename RendererType>
class GameVisualizer {
private:
    std::unique_ptr<RendererType> renderer;
    
public:
    // Конструкторы
    GameVisualizer() : renderer(std::make_unique<RendererType>()) {}
    
    explicit GameVisualizer(std::unique_ptr<RendererType> customRenderer) 
        : renderer(std::move(customRenderer)) {}
    
    // Деструктор
    ~GameVisualizer() = default;
    
    // Получение доступа к рендереру
    RendererType* getRenderer() const { return renderer.get(); }
    

    void visualizeLevelStart(int level) const {
        renderer->renderLevelStart(level);
    }

    void visualizeGameField(const GameField& field) const {
        renderer->renderGameField(field);
    }
    

    void visualizePlayerStats(const Player& player, int level) const {
        renderer->renderPlayerStats(player, level);
    }
    

    void visualizePlayerSpells(const Player& player) const {
        renderer->renderPlayerSpellInfo(player);
    }
    

    void visualizeSpellInfo(const Player& player) const {
        renderer->renderSpellInfo(player);
    }
    
 
    void visualizePlayerAttack(int damage, int enemyCurrentHealth, int enemyMaxHealth) const {
        renderer->renderPlayerAttackEnemy(damage, enemyCurrentHealth, enemyMaxHealth);
    }
    

    void visualizeEnemyAttack(const Enemy& enemy, int damage) const {
        renderer->renderEnemyAttacksPlayer(enemy, damage);
    }
    

    void visualizeEnemyDefeated(int scoreBonus = 10) const {
        renderer->renderEnemyDefeated(scoreBonus);
    }
    

    void visualizeSpellEffect(const std::string& spellType, 
                             const std::string& result,
                             int x = 0, int y = 0, int damage = 0) const {
        renderer->renderSpellDamageEffect(spellType, result, x, y, damage);
    }
    
   
    void visualizeSpellCastSuccess(int remainingMana) const {
        renderer->renderSpellCastSuccess(remainingMana);
    }
    

    void visualizeSpellTargetIssue(const std::string& issueType,
                                  int distance = 0, int range = 0) const {
        renderer->renderSpellTargetIssue(issueType, distance, range);
    }
    

    void visualizeLevelTransition(int level, int width, int height, int nextLevelScore) const {
        renderer->renderLevelTransition(level, width, height, nextLevelScore);
    }
    

    void visualizeSpellLoss(int spellsLost) const {
        renderer->renderSpellLossInfo(spellsLost);
    }
    

    void visualizeEnemiesSpawned(int count, int level) const {
        renderer->renderEnemiesSpawned(count, level);
    }
    

    void visualizeLevelUpMenu(const Player& player) const {
        renderer->renderLevelUpMenu(player);
    }
    

    void visualizeSpellUpgradeMenu(const Player& player) const {
        renderer->renderSpellUpgradeMenu(player);
    }
    

    void visualizeSpellUpgradeOptions(const Spell* spell) const {
        renderer->renderSpellUpgradeOptions(spell);
    }
    
    void visualizeUpgradeResult(const std::string& targetType,
                               const std::string& upgradeType,
                               const std::string& name = "",
                               int newValue = 0) const {
        renderer->renderUpgradeResult(targetType, upgradeType, name, newValue);
    }
    
  
    void visualizeSpellShop(int playerScore, int spellsInHand, int maxSpells) const {
        renderer->renderSpellShop(playerScore, spellsInHand, maxSpells);
    }
    

    void visualizeSpellPurchased() const {
        renderer->renderSpellPurchased();
    }
    
    
    void visualizeSpellHandFull() const {
        renderer->renderSpellHandFull();
    }
    

    void visualizeInsufficientResource(const std::string& resourceType,
                                      int needed, int have) const {
        renderer->renderInsufficientResource(resourceType, needed, have);
    }
    

    void visualizeError(const std::string& context, const std::string& message) const {
        renderer->renderError(context, message);
    }
    

    void visualizeOperationResult(const std::string& operation,
                                 const std::string& result,
                                 const std::string& details = "") const {
        renderer->renderOperationResult(operation, result, details);
    }
    

    void visualizeMainMenu() const {
        renderer->renderMainMenu();
    }
    

    void visualizeGameOverMenu() const {
        renderer->renderGameOverMenu();
    }
    

    void visualizeExtendedMenu() const {
        renderer->renderExtendedMenu();
    }
    

    void visualizeSizeInputPrompt() const {
        renderer->renderSizeInputPrompt();
    }
    

    void visualizeControlsHelp(const std::string& controlsHelp) const {
        renderer->renderControlsHelp(controlsHelp);
    }
    

    void visualizeSpellSelectionPrompt(int spellCount) const {
        renderer->renderSpellSelectionPrompt(spellCount);
    }
    

    void visualizeCoordinatesInputPrompt() const {
        renderer->renderCoordinatesInputPrompt();
    }
    

    void visualizeConfirmationPrompt(const std::string& message) const {
        renderer->renderConfirmationPrompt(message);
    }
    

    void visualizeGameOver(int finalScore) const {
        renderer->renderGameOver(finalScore);
    }
    

    void visualizeGameStateOperation(const std::string& operation, int level) const {
        renderer->renderGameStateOperation(operation, level);
    }
    

    void visualizeSpellUpgradeDetails(const Spell* spell) const {
        renderer->renderSpellUpgradeDetails(spell);
    }
    

    void visualizeSpellSpecificUpgradeOptions(const Spell* spell) const {
        renderer->renderSpellSpecificUpgradeOptions(spell);
    }
    
   
    void visualizeNoSpellsToUpgrade() const {
        renderer->renderNoSpellsToUpgrade();
    }
    

    void visualizeUnknownSpellType() const {
        renderer->renderUnknownSpellType();
    }


    void visualizeSaveFileInfo(const std::string& saveInfo) const {
        renderer->renderSaveFileInfo(saveInfo);
    }
};

// Алиас для стандартного рендерера (консольный вывод)
using ConsoleGameVisualizer = GameVisualizer<GameRenderer>;
