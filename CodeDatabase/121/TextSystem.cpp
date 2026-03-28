#include "TextSystem.h"
#include <sstream>
#include <iomanip>


// Реализация всех текстовых сообщений игры
// Содержит конкретные тексты для всех ситуаций в игре.
// Централизованное хранение позволяет легко изменять локализацию.

// Основное меню и системные сообщения
std::string TextSystem::getMainMenu() const {
    return "=== SUPERGAME ===\n"
           "1. Новая игра\n"
           "2. Выйти\n"
           "3. Загрузить игру\n"
           "4. Сохранить игру\n"
           "Выберите действие: ";
}

std::string TextSystem::getNewGameMessage(int levelNumber) const {
    return "Начинается уровень " + std::to_string(levelNumber) + "!";
}

// Игровой процесс и интерфейс
std::string TextSystem::getGameStatus(int levelNumber, int turnCount, int health, int score) const {
    std::ostringstream oss;
    oss << "Уровень: " << levelNumber 
        << " | Ход: " << turnCount 
        << " | Здоровье: " << health 
        << " | Очки: " << score;
    return oss.str();
}

std::string TextSystem::getBattleInfo(const std::string& weapon, int damage, double range) const {
    std::ostringstream oss;
    oss << "Оружие: " << weapon 
        << " | Урон: " << damage 
        << " | Дистанция: " << std::fixed << std::setprecision(1) << range;
    return oss.str();
}

std::string TextSystem::getLegend() const {
    return "Легенда: P-Игрок, E-Враг, B-Здание, T-Башня, #-Стена, *-Замедление, J-Ловушка";
}

std::string TextSystem::getCommands() const {
    return "Команды: W-A-S-D(движение), F(атака), M(смена оружия), 1-2-3(заклинания), I(показать заклинания), B(магазин), Q(выход)";
}

// Движение и действия игрока
std::string TextSystem::getOutOfBoundsMessage() const {
    return "Невозможно выйти за границы поля!";
}

std::string TextSystem::getBlockedCellMessage() const {
    return "Клетка заблокирована!";
}

std::string TextSystem::getPlayerDamageMessage(int damage, int remainingHealth) const {
    return "Игрок получил " + std::to_string(damage) + " урона! Здоровье: " + std::to_string(remainingHealth);
}

std::string TextSystem::getMoveMessage(const std::string& from, const std::string& to) const {
    return "Перемещение из " + from + " в " + to;
}

std::string TextSystem::getNoTargetsMessage() const {
    return "Нет целей для атаки в радиусе действия!";
}

std::string TextSystem::getInvalidChoice() const {
    return "Неверный выбор!";
}

// Эффекты статусов
std::string TextSystem::getPlayerFrozenMessage() const {
    return "Игрок заморожен и пропускает ход!";
}

std::string TextSystem::getPlayerSlowedMessage() const {
    return "Игрок замедлен! Следующий ход будет пропущен.";
}

// Боевая система
std::string TextSystem::getBattleModeMessage(bool isCloseCombat) const {
    return isCloseCombat ? "Режим боя: Ближний бой (меч)" : "Режим боя: Дальний бой (лук)";
}

std::string TextSystem::getAttackSuccessMessage() const {
    return "Атака успешна!";
}

// Система заклинаний
std::string TextSystem::getSpellHandTitle() const {
    return "=== ЗАКЛИНАНИЯ ===";
}

std::string TextSystem::getEmptySpellHand() const {
    return "Нет заклинаний";
}

std::string TextSystem::getSpellSlotInfo(int freeSlots, int maxSlots) const {
    return "Свободно слотов: " + std::to_string(freeSlots) + "/" + std::to_string(maxSlots);
}

std::string TextSystem::getSpellCastMessage(const std::string& spellName, bool success) const {
    if (success) {
        return "Заклинание " + spellName + " успешно применено!";
    } else {
        return "Не удалось применить заклинание " + spellName;
    }
}

std::string TextSystem::getSpellObtainedMessage(const std::string& spellName) const {
    return "Получено новое заклинание: " + spellName;
}

std::string TextSystem::getNoSpellsMessage() const {
    return "У вас нет заклинаний! Используйте магазин (B) чтобы купить.";
}

std::string TextSystem::getAvailableSpellsRange(int maxSpells) const {
    return "Доступные заклинания: 1-" + std::to_string(maxSpells);
}

// Магазин заклинаний
std::string TextSystem::getShopTitle() const {
    return "=== МАГАЗИН ЗАКЛИНАНИЙ ===";
}

std::string TextSystem::getPlayerScoreInfo(int score) const {
    return "Ваши очки: " + std::to_string(score);
}

std::string TextSystem::getSpellSlotsInfo(int freeSlots, int maxSlots) const {
    return "Свободных слотов: " + std::to_string(freeSlots) + "/" + std::to_string(maxSlots);
}

std::string TextSystem::getSpellShopItem(int index, const std::string& spellName, 
                                        const std::string& spellDescription, int cost) const {
    std::ostringstream oss;
    oss << index << ". " << spellName << " - " << spellDescription << "\n"
        << "   Стоимость: " << cost << " очков";
    return oss.str();
}

std::string TextSystem::getShopExitOption() const {
    return "0. Выйти из магазина";
}

std::string TextSystem::getShopDivider() const {
    return "=========================================";
}

std::string TextSystem::getShopPrompt() const {
    return "Выберите заклинание для покупки (0 - выйти): ";
}

std::string TextSystem::getNotEnoughPoints() const {
    return "Недостаточно очков для покупки!";
}

std::string TextSystem::getNoSpellSlots() const {
    return "Нет свободных слотов для заклинаний!";
}

std::string TextSystem::getSpellPurchasedMessage(const std::string& spellName, int cost) const {
    return "Вы купили заклинание '" + spellName + "' за " + std::to_string(cost) + " очков!";
}

// Система улучшений персонажа
std::string TextSystem::getUpgradeMenuTitle() const {
    return "=== УЛУЧШЕНИЯ ПЕРСОНАЖА ===";
}

std::string TextSystem::getAvailablePoints(int points) const {
    return "Доступно очков улучшений: " + std::to_string(points);
}

std::string TextSystem::getUpgradeOption(int number, const std::string& upgradeName, int cost) const {
    return std::to_string(number) + ". " + upgradeName + " (" + std::to_string(cost) + " очков)";
}

std::string TextSystem::getSelectOption() const {
    return "Выберите улучшение: ";
}

std::string TextSystem::getContinueOption(int nextLevel) const {
    return "0. Продолжить на уровень " + std::to_string(nextLevel);
}

std::string TextSystem::getUpgradeAppliedMessage(const std::string& upgradeName) const {
    return "Улучшение '" + upgradeName + "' применено!";
}

std::string TextSystem::getNotEnoughPointsMessage() const {
    return "Недостаточно очков для этого улучшения!";
}

std::string TextSystem::getHealthUpgradeMessage(int newHealth) const {
    return "Теперь у вас " + std::to_string(newHealth) + " HP!";
}

std::string TextSystem::getChooseDifferentUpgrade() const {
    return "Выберите другое улучшение или 0 чтобы продолжить";
}

// Завершение игры и прогресс
std::string TextSystem::getLevelCompleteMessage(int levelNumber) const {
    return "Уровень " + std::to_string(levelNumber) + " пройден!";
}

std::string TextSystem::getGameOver(bool victory, int score) const {
    if (victory) {
        return "ПОБЕДА! Ваш счет: " + std::to_string(score);
    } else {
        return "ИГРА ОКОНЧЕНА. Ваш счет: " + std::to_string(score);
    }
}

std::string TextSystem::getGameOverMenu() const {
    return "1. Новая игра\n2. Выйти\n3. Загрузить игру\nВыберите действие: ";
}

// Система сохранения и загрузки
std::string TextSystem::getSaveSuccessMessage() const {
    return "Игра успешно сохранена!";
}

std::string TextSystem::getSaveErrorMessage(const std::string& error) const {
    return "Ошибка сохранения: " + error;
}

std::string TextSystem::getLoadSuccessMessage() const {
    return "Игра успешно загружена!";
}

std::string TextSystem::getLoadErrorMessage(const std::string& error) const {
    return "Ошибка загрузки: " + error;
}

// Боевые события и уничтожение врагов
std::string TextSystem::getEnemyKilledMessage() const {
    return "Враг уничтожен! +10 очков";
}

std::string TextSystem::getBuildingDestroyedMessage() const {
    return "Здание разрушено! +25 очков";
}

std::string TextSystem::getTowerDestroyedMessage() const {
    return "Башня разрушена! +35 очков";
}

std::string TextSystem::getEnemyKilledByTrapMessage() const {
    return "Враг погиб от ловушки!";
}

std::string TextSystem::getEnemyTrapActivatedMessage(int damage) const {
    return "Враг активировал ловушку и получил " + std::to_string(damage) + " урона!";
}

std::string TextSystem::getEnemyAttackMessage(int damage) const {
    return "Враг атаковал игрока и нанес " + std::to_string(damage) + " урона!";
}

// Заголовки для системы исключений
std::string TextSystem::getSaveLoadErrorTitle() const {
    return "СОХРАНЕНИЕ/ЗАГРУЗКА";
}

std::string TextSystem::getLevelGenerationErrorTitle() const {
    return "ГЕНЕРАЦИЯ УРОВНЯ";
}

std::string TextSystem::getSpellErrorTitle() const {
    return "ЗАКЛИНАНИЯ";
}

std::string TextSystem::getFieldErrorTitle() const {
    return "ИГРОВОЕ ПОЛЕ";
}

std::string TextSystem::getAIErrorTitle() const {
    return "ИСКУССТВЕННЫЙ ИНТЕЛЛЕКТ";
}

std::string TextSystem::getPlayerErrorTitle() const {
    return "ИГРОК";
}

std::string TextSystem::getCombatErrorTitle() const {
    return "БОЕВАЯ СИСТЕМА";
}

std::string TextSystem::getConfigErrorTitle() const {
    return "КОНФИГУРАЦИЯ";
}

// Конкретные сообщения об ошибках для исключений
std::string TextSystem::getCannotOpenForSaveError(const std::string& filename) const {
    return "Не удалось открыть файл для сохранения: " + filename;
}

std::string TextSystem::getCannotOpenForLoadError(const std::string& filename) const {
    return "Не удалось открыть файл для загрузки: " + filename;
}

std::string TextSystem::getWriteError() const {
    return "Ошибка записи в файл сохранения";
}

std::string TextSystem::getReadError() const {
    return "Ошибка чтения файла сохранения";
}

std::string TextSystem::getCorruptedFileError() const {
    return "Файл сохранения поврежден или неполный";
}

std::string TextSystem::getSaveFailedError(const std::string& details) const {
    return "Не удалось сохранить игру: " + details;
}

std::string TextSystem::getLoadFailedError(const std::string& details) const {
    return "Не удалось загрузить игру: " + details;
}

// Ошибки генерации уровней
std::string TextSystem::getInvalidPositionError(int attempts) const {
    return "Не удалось найти валидную позицию после " + std::to_string(attempts) + " попыток";
}

std::string TextSystem::getPlayerPlacementFailedError(const std::string& pos) const {
    return "Не удалось разместить игрока на позиции " + pos;
}

std::string TextSystem::getEnemyPlacementFailedError(const std::string& pos) const {
    return "Не удалось разместить врага на позиции " + pos;
}

std::string TextSystem::getBuildingPlacementFailedError(const std::string& pos) const {
    return "Не удалось разместить здание на позиции " + pos;
}

std::string TextSystem::getTowerPlacementFailedError(const std::string& pos) const {
    return "Не удалось разместить башню на позиции " + pos;
}

std::string TextSystem::getLevelInitFailedError(int levelNumber, const std::string& details) const {
    return "Уровень " + std::to_string(levelNumber) + ": ошибка инициализации - " + details;
}

std::string TextSystem::getLevelCopyFailedError(const std::string& details) const {
    return "Ошибка копирования уровня: " + details;
}

// Ошибки заклинаний
std::string TextSystem::getUnknownSpellTypeError(int type) const {
    return "Неизвестный тип заклинания: " + std::to_string(type);
}

std::string TextSystem::getSpellCreationFailedError(const std::string& details) const {
    return "Ошибка создания заклинания: " + details;
}

std::string TextSystem::getSpellCastFailedError(const std::string& details) const {
    return "Ошибка применения заклинания: " + details;
}

// Ошибки игрового поля
std::string TextSystem::getInvalidDimensionsError(int width, int height) const {
    return "Неверные размеры поля: " + std::to_string(width) + "x" + std::to_string(height);
}

std::string TextSystem::getTooLargeDimensionsError(int width, int height) const {
    return "Слишком большие размеры поля: " + std::to_string(width) + "x" + std::to_string(height);
}

std::string TextSystem::getInvalidPositionError(int x, int y) const {
    return "Неверная позиция: (" + std::to_string(x) + ", " + std::to_string(y) + ")";
}

std::string TextSystem::getPlacementFailedError(int x, int y, const std::string& details) const {
    return "Не удалось разместить объект на позиции (" + std::to_string(x) + ", " + std::to_string(y) + "): " + details;
}

// Системные сообщения для контроллера
std::string TextSystem::getAIWarningMessage(const std::string& details) const {
    return "Предупреждение ИИ: " + details;
}

std::string TextSystem::getFieldCriticalErrorMessage(const std::string& details) const {
    return "Критическая ошибка поля: " + details;
}

std::string TextSystem::getCombatErrorMessage(const std::string& details) const {
    return "Ошибка боевой системы: " + details;
}

std::string TextSystem::getLevelRestorationError() const {
    return "Ошибка восстановления уровня из сохранения";
}

std::string TextSystem::getCreatingNewLevelMessage() const {
    return "Создание нового уровня";
}

std::string TextSystem::getLevelCreatedMessage() const {
    return "Уровень создан";
}

std::string TextSystem::getCriticalLevelError() const {
    return "Критическая ошибка: не удалось создать уровень. Игра завершена";
}

std::string TextSystem::getNextLevelError(const std::string& details) const {
    return "Ошибка создания следующего уровня: " + details;
}

std::string TextSystem::getTryAgainMessage() const {
    return "Попробуйте начать игру заново";
}

std::string TextSystem::getRestartGameMessage() const {
    return "Перезапустите игру";
}

std::string TextSystem::getCriticalErrorTitle() const {
    return "КРИТИЧЕСКАЯ ОШИБКА ИГРЫ";
}

std::string TextSystem::getStandardErrorTitle() const {
    return "СТАНДАРТНАЯ ОШИБКА";
}

std::string TextSystem::getUnknownErrorTitle() const {
    return "НЕИЗВЕСТНАЯ ОШИБКА";
}

std::string TextSystem::getPressEnterToExit() const {
    return "Нажмите Enter для выхода...";
}