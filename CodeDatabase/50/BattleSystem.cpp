#include "BattleSystem.h"

int BattleSystem::playerAttack(Field& field, Player& player, PlayerHand& hand){

    int dx = 0, dy = 0;

    char dirOpt;
    int choice_flag = 1;
    while (choice_flag){
        std::cout << "В каком направлении вы хотите атаковать? (W/A/S/D): ";
        std::cin >> dirOpt;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
        switch (dirOpt){
            case 'W':
                dx = 0;
                dy = -1;
                break;
            case 'A':
                dx = -1;
                dy = 0;
                break;
            case 'S':
                dx = 0;
                dy = 1;
                break;
            case 'D':
                dx = 1;
                dy = 0;
                break;
            default:
                std::cout << "Ошибка: введите одну из опций (W/A/S/D).\n";
                continue;
        }
        choice_flag = 0;
    }

    int px = player.getX();
    int py = player.getY();

    if (!player.attackIsRange()){

        int tx = px + dx;
        int ty = py + dy;

        if (tx < 0 || ty < 0 || tx >= field.getWidth() || ty >= field.getHeight()) {
            std::cout << "Вы атаковали за пределами поля.\n";
            return 0;
        }

        if (field.getCellEntity(tx, ty)!=nullptr){
            if (field.getCellEntity(tx, ty)->getObjType() == GameEntType::Enemy){
                GameEnt* target = field.getCellEntity(tx, ty);
                target->takeDamage(player.getPlayerDamage());
                std::cout << "Вы нанесли " << player.getPlayerDamage() << " урона врагу (" << tx << ", " << ty << ").\n";
                if (!target->isAlive()) {
                    std::cout << "Враг уничтожен!\n";
                    rewardForKill(hand);
                    field.clearCellEnt(tx, ty);
                }
                return 1;
            } else if (field.getCellEntity(tx, ty)->getObjType() == GameEntType::Tower){
                GameEnt* target = field.getCellEntity(tx, ty);
                target->takeDamage(player.getPlayerDamage());
                std::cout << "Вы нанесли " << player.getPlayerDamage() << " урона башне (" << tx << ", " << ty << ").\n";
                if (!target->isAlive()) {
                    std::cout << "Башня уничтожена!\n";
                    rewardForKill(hand);
                    field.clearCellEnt(tx, ty);
                }
                return 1;
            
            }

        }
        
        std::cout << "Ваша атака не достигла цели.\n";

    } else {
        int bulletX = px + dx;
        int bulletY = py + dy;
        while (bulletX>=0 && bulletY>=0 && bulletX<field.getWidth() && bulletY<field.getHeight() && (field.getCellType(bulletX, bulletY) == CellType::Empty || field.getCellType(bulletX, bulletY) == CellType::Trap) && field.getCellEntity(bulletX, bulletY)==nullptr){
            bulletX += dx;
            bulletY += dy;
        }
        if (bulletX < 0 || bulletY < 0 || bulletX >= field.getWidth() || bulletY >= field.getHeight()) {
            std::cout << "Выстрел не попал ни в кого.\n";
            return 0;
        }
        if (field.getCellEntity(bulletX, bulletY)!=nullptr){
            if (field.getCellEntity(bulletX, bulletY)->getObjType() == GameEntType::Enemy){
                GameEnt* target = field.getCellEntity(bulletX, bulletY);
                target->takeDamage(player.getPlayerDamage());
                std::cout << "Вы нанесли " << player.getPlayerDamage() << " урона врагу (" << bulletX << ", " << bulletY << ").\n";
                if (!target->isAlive()) {
                    std::cout << "Враг уничтожен!\n";
                    rewardForKill(hand);
                    field.clearCellEnt(bulletX, bulletY);
                    
                }
                return 1;
            }else if (field.getCellEntity(bulletX, bulletY)->getObjType() == GameEntType::Tower) {
                GameEnt* target = field.getCellEntity(bulletX, bulletY);
                target->takeDamage(player.getPlayerDamage());
                std::cout << "Вы нанесли " << player.getPlayerDamage() << " урона башне (" << bulletX << ", " << bulletY << ").\n";
                if (!target->isAlive()) {
                    std::cout << "Башня уничтожена!\n";
                    rewardForKill(hand);
                    field.clearCellEnt(bulletX, bulletY);
                    
                }
                return 1;
            }
        }

        std::cout << "Выстрел не попал ни в кого.\n";
        
    }
    return 0;
}


int BattleSystem::countAliveEnemies(Field& field) {
    int count = 0;
    for (int y = 0; y < field.getHeight(); ++y) {
        for (int x = 0; x < field.getWidth(); ++x) {
            GameEnt* ent = field.getCellEntity(x, y);
            if (ent && (ent->getObjType() == GameEntType::Enemy || ent->getObjType() == GameEntType::Tower) && ent->isAlive())
                count++;
        }
    }
    return count;
}

int BattleSystem::castSpell(Field& field, Player& player, PlayerHand& hand) {
    if (hand.getCount() == 0) {
        std::cout << "У вас нет заклинаний.\n";
        return 0;
    }

    hand.showSpellList();
   
    int index = -1;
    int choice_flag = 1;
    while (choice_flag) {
        std::cout << "Выберите заклинание (номер): ";

        // Проверка на нечисловой ввод
        if (!(std::cin >> index)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Ошибка: введите число.\n";
            continue;
        }

        // Проверка на выход за диапазон
        if (index < 1 || index > hand.getCount()) {
            std::cout << "Некорректный номер. Введите число от 1 до " << hand.getCount() << ".\n";
            continue;
        }

        // Если всё ок — выходим из цикла
        choice_flag = 0;
    }

    ISpell* spell = hand.getSpell(index - 1);
    if (!spell) {
        std::cout << "Не удалось получить заклинание.\n";
        return 0;
    }

    if (!spell->canCast(field, player)) {
        std::cout << "Заклинание нельзя применить.\n";
        return 0;
    }

    // Считаем врагов до каста
    int before = countAliveEnemies(field);

    // Применяем заклинание
    spell->cast(field, player);
    hand.removeSpell(index - 1);

    // Считаем врагов после
    int after = countAliveEnemies(field);

    

    for(int i = after; i<before;i++){
        std::cout << "Враг уничтожен!\n";
        rewardForKill(hand);
    }

    return 1;
}



void BattleSystem::rewardForKill(PlayerHand& hand) {
    std::cout << "Вы получаете новое заклинание за уничтожение врага!\n";
    hand.addRandomSpell();
}

void BattleSystem::updateTraps(Field& field, PlayerHand& hand){
    for (int y = 0; y < field.getHeight(); ++y) {
        for (int x = 0; x < field.getWidth(); ++x) {

            // Проверяем: есть ли ловушка и враг на одной клетке
            if (field.getCellType(x, y) == CellType::Trap) {
                GameEnt* ent = field.getCellEntity(x, y);
                if (ent && ent->getObjType() == GameEntType::Enemy && ent->isAlive()) {

                    std::cout << "Ловушка сработала на клетке (" << x << ", " << y << ")!\n";

                    // Определяем урон (можно базовый или на основе прототипа TrapSpell)
                    double trapDamage = hand.getTrapPrototype().getDamage();
                    ent->takeDamage(trapDamage);
                    std::cout << "Враг получил " << trapDamage << " урона!\n";

                    if (!ent->isAlive()) {
                        field.clearCellEnt(x, y);
                        std::cout << "Враг уничтожен ловушкой!\n";
                        rewardForKill(hand);
                    }

                    // После срабатывания ловушка исчезает
                    field.setCellType(CellType::Empty, x, y);
                }
            }
        }
    }
}
