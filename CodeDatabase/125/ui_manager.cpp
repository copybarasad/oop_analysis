#include "ui_manager.h"
#include "console_renderer.h"
#include <iostream>
#include <limits>

bool UIManager::ShowSpellShop(Player& player) {
    std::cout << "\n========================= МАГАЗИН ЗАКЛИНАНИЙ =========================\n";
    std::cout << "У вас очков: " << player.GetScore() << "\n";
    std::cout << "Стоимость заклинания: 10 очков\n";
    std::cout << "Свободных ячеек: " << (player.GetHand().GetMaxSize() - player.GetHand().GetSize()) 
              << "/" << player.GetHand().GetMaxSize() << "\n\n";
    
    std::cout << "1. Исцеление - восстанавливает 30 HP\n";
    std::cout << "2. Огненный шар - урон 20 по области вокруг вас\n";
    std::cout << "3. Телепортация - случайная телепортация по полю\n";
    std::cout << "4. Магический удар - точный урон 40 по выбранной цели\n";
    std::cout << "5. Взрыв - урон 25 по области 2x2\n";
    std::cout << "6. Ловушка - устанавливает ловушку (30 урона)\n";
    std::cout << "0. Выйти из магазина\n\n";
    
    std::cout << "Выберите заклинание (0-6): ";
    
    int choice;
    if (!(std::cin >> choice)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Неверный ввод!\n";
        return false;
    }
    
    if (choice == 0) {
        std::cout << "Выход из магазина.\n";
        return false;
    }
    
    if (choice < 1 || choice > 6) {
        std::cout << "Неверный выбор!\n";
        return false;
    }

    SpellType spellType = static_cast<SpellType>(choice - 1);
    
    if (player.BuySpell(spellType)) {
        std::cout << "Заклинание успешно куплено!\n";
        std::cout << "Осталось очков: " << player.GetScore() << "\n";
        return true;
    } else {
        if (player.GetScore() < 10) {
            std::cout << "Недостаточно очков! Нужно 10, у вас: " << player.GetScore() << "\n";
        } else {
            std::cout << "Рука заклинаний полна! Максимум: " << player.GetHand().GetMaxSize() << " заклинаний\n";
        }
        return false;
    }
}
