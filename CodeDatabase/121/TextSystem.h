#ifndef TEXT_SYSTEM_H
#define TEXT_SYSTEM_H

#include <string>
#include <memory>

// Централизованная система текстовых сообщений игры
// Содержит все текстовые сообщения для единообразного вывода.
// Используется всеми системами игры для локализации текстов.

class TextSystem {
public:
    // Основное меню и системные сообщения
    std::string getMainMenu() const;
    std::string getNewGameMessage(int levelNumber) const;
    
    // Игровой процесс и интерфейс
    std::string getGameStatus(int levelNumber, int turnCount, int health, int score) const;
    std::string getBattleInfo(const std::string& weapon, int damage, double range) const;
    std::string getLegend() const;
    std::string getCommands() const;
    
    // Движение и действия игрока
    std::string getOutOfBoundsMessage() const;
    std::string getBlockedCellMessage() const;
    std::string getPlayerDamageMessage(int damage, int remainingHealth) const;
    std::string getMoveMessage(const std::string& from, const std::string& to) const;
    std::string getNoTargetsMessage() const;
    std::string getInvalidChoice() const;
    
    // Эффекты статусов
    std::string getPlayerFrozenMessage() const;
    std::string getPlayerSlowedMessage() const;
    
    // Боевая система
    std::string getBattleModeMessage(bool isCloseCombat) const;
    std::string getAttackSuccessMessage() const;
    
    // Система заклинаний
    std::string getSpellHandTitle() const;
    std::string getEmptySpellHand() const;
    std::string getSpellSlotInfo(int freeSlots, int maxSlots) const;
    std::string getSpellCastMessage(const std::string& spellName, bool success) const;
    std::string getSpellObtainedMessage(const std::string& spellName) const;
    std::string getNoSpellsMessage() const;
    std::string getAvailableSpellsRange(int maxSpells) const;
    
    // Магазин заклинаний
    std::string getShopTitle() const;
    std::string getPlayerScoreInfo(int score) const;
    std::string getSpellSlotsInfo(int freeSlots, int maxSlots) const;
    std::string getSpellShopItem(int index, const std::string& spellName, 
                                const std::string& spellDescription, int cost) const;
    std::string getShopExitOption() const;
    std::string getShopDivider() const;
    std::string getShopPrompt() const;
    std::string getNotEnoughPoints() const;
    std::string getNoSpellSlots() const;
    std::string getSpellPurchasedMessage(const std::string& spellName, int cost) const;

    // Система улучшений персонажа
    std::string getUpgradeMenuTitle() const;
    std::string getAvailablePoints(int points) const;
    std::string getUpgradeOption(int number, const std::string& upgradeName, int cost) const;
    std::string getSelectOption() const;
    std::string getContinueOption(int nextLevel) const;
    std::string getUpgradeAppliedMessage(const std::string& upgradeName) const;
    std::string getNotEnoughPointsMessage() const;
    std::string getHealthUpgradeMessage(int newHealth) const;
    std::string getChooseDifferentUpgrade() const;
    
    // Завершение игры и прогресс
    std::string getLevelCompleteMessage(int levelNumber) const;
    std::string getGameOver(bool victory, int score) const;
    std::string getGameOverMenu() const;
    
    // Система сохранения и загрузки
    std::string getSaveSuccessMessage() const;
    std::string getSaveErrorMessage(const std::string& error) const;
    std::string getLoadSuccessMessage() const;
    std::string getLoadErrorMessage(const std::string& error) const;

    // Боевые события и уничтожение врагов
    std::string getEnemyKilledMessage() const;
    std::string getBuildingDestroyedMessage() const;
    std::string getTowerDestroyedMessage() const;
    std::string getEnemyKilledByTrapMessage() const;
    std::string getEnemyTrapActivatedMessage(int damage) const;
    std::string getEnemyAttackMessage(int damage) const;

    // Заголовки для системы исключений
    std::string getSaveLoadErrorTitle() const;
    std::string getLevelGenerationErrorTitle() const;
    std::string getSpellErrorTitle() const;
    std::string getFieldErrorTitle() const;
    std::string getAIErrorTitle() const;
    std::string getPlayerErrorTitle() const;
    std::string getCombatErrorTitle() const;
    std::string getConfigErrorTitle() const;

    // Конкретные сообщения об ошибках для исключений
    std::string getCannotOpenForSaveError(const std::string& filename) const;
    std::string getCannotOpenForLoadError(const std::string& filename) const;
    std::string getWriteError() const;
    std::string getReadError() const;
    std::string getCorruptedFileError() const;
    std::string getSaveFailedError(const std::string& details) const;
    std::string getLoadFailedError(const std::string& details) const;

    // Ошибки генерации уровней
    std::string getInvalidPositionError(int attempts) const;
    std::string getPlayerPlacementFailedError(const std::string& pos) const;
    std::string getEnemyPlacementFailedError(const std::string& pos) const;
    std::string getBuildingPlacementFailedError(const std::string& pos) const;
    std::string getTowerPlacementFailedError(const std::string& pos) const;
    std::string getLevelInitFailedError(int levelNumber, const std::string& details) const;
    std::string getLevelCopyFailedError(const std::string& details) const;

    // Ошибки заклинаний
    std::string getUnknownSpellTypeError(int type) const;
    std::string getSpellCreationFailedError(const std::string& details) const;
    std::string getSpellCastFailedError(const std::string& details) const;

    // Ошибки игрового поля
    std::string getInvalidDimensionsError(int width, int height) const;
    std::string getTooLargeDimensionsError(int width, int height) const;
    std::string getInvalidPositionError(int x, int y) const;
    std::string getPlacementFailedError(int x, int y, const std::string& details) const;

    // Системные сообщения для контроллера
    std::string getAIWarningMessage(const std::string& details) const;
    std::string getFieldCriticalErrorMessage(const std::string& details) const;
    std::string getCombatErrorMessage(const std::string& details) const;
    std::string getLevelRestorationError() const;
    std::string getCreatingNewLevelMessage() const;
    std::string getLevelCreatedMessage() const;
    std::string getCriticalLevelError() const;
    std::string getNextLevelError(const std::string& details) const;
    std::string getTryAgainMessage() const;
    std::string getRestartGameMessage() const;
    std::string getCriticalErrorTitle() const;
    std::string getStandardErrorTitle() const;
    std::string getUnknownErrorTitle() const;
    std::string getPressEnterToExit() const;
};

#endif