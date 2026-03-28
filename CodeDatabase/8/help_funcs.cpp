#include "help_funcs.h"
#include "command_mapper.h"
using namespace std;
void displayMenu(const CommandMapper& mapper) {
    cout << "\n=== МЕНЮ ДЕЙСТВИЙ ===" << endl;
    cout << mapper.getFormattedCommand("UP") << " - Движение вверх" << endl;
    cout << mapper.getFormattedCommand("DOWN") << " - Движение вниз" << endl;
    cout << mapper.getFormattedCommand("LEFT") << " - Движение влево" << endl;
    cout << mapper.getFormattedCommand("RIGHT") << " - Движение вправо" << endl;
    cout << mapper.getFormattedCommand("ATTACK") << " - Атаковать" << endl;
    cout << mapper.getFormattedCommand("CHANGE_TYPE") << " - Сменить тип боя" << endl;
    cout << mapper.getFormattedCommand("SHOW_STATUS") << " - Показать статистику" << endl;
    cout << mapper.getFormattedCommand("SHOW_MENU") << " - Показать меню" << endl;
    cout << mapper.getFormattedCommand("USE_SPELL") << " - использовать заклинание" << endl;
    cout << mapper.getFormattedCommand("BUY_SPELL") << " - купить заклинание" << endl;
    cout << mapper.getFormattedCommand("SHOW_SPELLS") << " - показать заклинания в руке" << endl; 
    cout << mapper.getFormattedCommand("SAVE") << " - сохранить состояние" << endl; 
    cout << mapper.getFormattedCommand("LOAD") << " - загрузить из файла" << endl; 
    cout << mapper.getFormattedCommand("QUIT") << " - Выйти из игры" << endl;
}
void showStatsPlayer(Player& player) {
        cout << "\n=== СТАТИСТИКА ===" << endl;
        cout << "Имя: " << player.getname() << endl;
        cout << "Жизни: " << player.getLives() << endl;
        cout << "Урон: " << player.getDamage() << endl;
        cout << "Очки: " << player.getPoints() << endl;
        cout << "Тип боя: " << (player.getCombatType() == Player::CombatType::NEAR ? "Ближний" : "Дальний") << endl;
        cout << "Сила атаки: " << player.getPower() << endl;
        cout << "Радиус атаки: " << player.getRadius() << endl;
        cout << "Позиция: (" << player.getX() << "," << player.getY() << ")" << endl;
        cout << "Заклинаний в руке: " << player.getHand().getSpellCount() << "/" << player.getHand().getMaxSize() << endl;
        cout << "========================\n" << endl;
    }
void showStatsEnemy(Enemy& enemy) {
        cout << "\n=== СТАТИСТИКА ===" << endl;
        cout << "Имя: " << enemy.getname() << endl;
        cout << "Жизни: " << enemy.getLives() << endl;
        cout << "Урон: " << enemy.getDamage() << endl;
        cout << "Позиция: (" << enemy.getX() << "," << enemy.getY() << ")" << endl;
        cout << "========================\n" << endl;
    }
void renderSpellShop(int playerPoints) {
    cout << "=== МАГАЗИН ЗАКЛИНАНИЙ ===" << endl;
    cout << "0. Огненный шар - урон: 4, стоимость: 10, радиус: 3" << endl;
    cout << "1. Ледяная стрела - урон: 3, стоимость: 8, радиус: 5" << endl;
    cout << "2. Огненный шторм - урон: 5, стоимость: 20, радиус: 5" << endl;
    cout << "3. Ледяная буря - урон: 6, стоимость: 15, радиус: 4" << endl;
    cout << "4. Ядовитая ловушка - урон: 4, стоимость: 6, радиус: 3" << endl;
    cout << "==========================" << endl;
    cout << "Ваши очки: " << playerPoints << endl;
}