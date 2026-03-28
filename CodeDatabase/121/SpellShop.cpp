#include "SpellShop.h"
#include <iostream>


// Реализация магазина заклинаний
// Содержит логику покупки заклинаний, использует конфигурацию
// для определения цен и автоматического формирования ассортимента.

// Конструктор магазина - принимает конфигурацию по ссылке
SpellShop::SpellShop(std::shared_ptr<TextSystem> textSystem, const GameConfig& config) 
    : textSystem(textSystem), config(config) {
    initializeSpells();
}

// Инициализирует список доступных заклинаний из конфигурации
void SpellShop::initializeSpells() {
    availableSpells.clear();
    
    // Получаем все доступные типы заклинаний из фабрики
    std::vector<SpellType> allSpellTypes = SpellFactory::getAvailableSpellTypes();
    
    // Для каждого типа заклинания получаем цену из конфигурации
    for (SpellType type : allSpellTypes) {
        const SpellConfig& spellConfig = config.getSpellConfig(type);
        availableSpells.emplace_back(type, spellConfig.cost);
    }
}

// Показывает интерфейс магазина игроку
void SpellShop::showShop(const Player& player) {
    // Используем TextSystem для всех выводов
    
    // Заголовок магазина
    std::cout << textSystem->getShopTitle() << std::endl;
    
    // Информация об очках игрока
    std::cout << textSystem->getPlayerScoreInfo(player.getScore()) << std::endl;
    
    // Информация о свободных слотах
    int freeSlots = player.getHand().getMaxSize() - player.getHand().getSpellCount();
    std::cout << textSystem->getSpellSlotsInfo(freeSlots, player.getHand().getMaxSize()) << std::endl;
    
    // Разделительная линия
    std::cout << textSystem->getShopDivider() << std::endl;
    
    // Список доступных заклинаний
    for (size_t i = 0; i < availableSpells.size(); i++) {
        const auto& [spellType, cost] = availableSpells[i];
        
        // Получаем конфигурацию заклинания для отображения информации
        const SpellConfig& spellConfig = config.getSpellConfig(spellType);
        
        // Отображаем информацию о заклинании через TextSystem
        std::cout << textSystem->getSpellShopItem(i + 1, spellConfig.name, spellConfig.description, cost) << std::endl;
        std::cout << std::endl;  // Пустая строка между заклинаниями
    }
    
    // Опция выхода
    std::cout << textSystem->getShopExitOption() << std::endl;
    
    // Нижняя разделительная линия
    std::cout << textSystem->getShopDivider() << std::endl;
    
    // Приглашение для выбора
    std::cout << textSystem->getShopPrompt();
}

// Покупает выбранное заклинание для игрока
bool SpellShop::purchaseSpell(Player& player, int spellIndex) {
    // Проверяем валидность индекса
    if (spellIndex < 0 || static_cast<size_t>(spellIndex) >= availableSpells.size()) {
        std::cout << textSystem->getInvalidChoice() << std::endl;
        return false;
    }
    
    try {
        const auto& [spellType, cost] = availableSpells[spellIndex];
        auto& hand = player.getHand();
        
        // Проверяем достаточно ли очков у игрока
        if (player.getScore() < cost) {
            std::cout << textSystem->getNotEnoughPoints() << std::endl;
            return false;
        }
        
        // Проверяем есть ли свободные слоты для заклинаний
        if (!hand.hasSpace()) {
            std::cout << textSystem->getNoSpellSlots() << std::endl;
            return false;
        }
        
        // Создаем заклинание через фабрику
        auto spell = SpellFactory::createSpell(spellType);
        
        // Списываем очки и добавляем заклинание в руку
        player.setScore(player.getScore() - cost);
        hand.addSpell(spell);
        
        // Получаем название заклинания из конфигурации для сообщения
        const SpellConfig& spellConfig = config.getSpellConfig(spellType);
        
        // Показываем сообщение об успешной покупке
        std::cout << textSystem->getSpellPurchasedMessage(spellConfig.name, cost) << std::endl;
        return true;
        
    } catch (const std::exception& e) {
        std::cout << "Ошибка при покупке заклинания: " << e.what() << std::endl;
        return false;
    }
}