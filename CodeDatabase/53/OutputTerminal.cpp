#include "OutputTerminal.h"
#include "Game.h"




void OutputTerminal::displayField(GameField& field) {
    int width = field.getWidth();
    int height = field.getHeight();
    if(height < MIN_FIELD_SIZE or height > MAX_FIELD_SIZE or width > MAX_FIELD_SIZE or width < MIN_FIELD_SIZE){
        std::cout << "Поле не инициализировано т.к его длина ширина не соответствует (" 
        << MIN_FIELD_SIZE << ":" << MAX_FIELD_SIZE << ")" << std::endl;
        return;
    }
    std::cout << "=== ИГРОВОЕ ПОЛЕ ===" << std::endl;
    std::cout << "   ";
    for (int x = 0; x < width; ++x) {
        std::cout << char('A' + x) << " ";
    }
    std::cout << std::endl;
    
    for (int y = 0; y < height; ++y) {
        if(y < 10) std::cout << y << "  ";
        else std::cout << y << " ";
        for (int x = 0; x < width; ++x) {
            std::cout << field.getCell(x,y).getSymbol() << " ";
        }
        std::cout << std::endl;
    }
    
    std::cout << "==========================================================" << std::endl;
    std::cout << "Легенда: P-Игрок, E-Враг, O-Яма, B-Барак," << std::endl;
    std::cout << "T-Башня, (+)-Ловушка, J-Союзник, S-Медленная зона " << std::endl;
    std::cout << "==========================================================" << std::endl;
}

void OutputTerminal::displayPlayer(Player& player) {
    
    std::cout << "=== ИНФОРМАЦИЯ ОБ ИГРОКЕ ===" << std::endl;
    std::cout << "Имя: " << player.getName() << std::endl;
    std::cout << "Здоровье: " << player.getHealth() << "/" << player.getMaxHealth() << std::endl;
    std::cout << "Оружие: " << player.getAttackTypeString() << " (урон: " << player.getDamage()<< ")" << std::endl;
    std::cout << "Уровень: " << player.getLevel() << " (" << player.getXp() << " XP)" << std::endl;
    std::cout << "Монеты: " << player.getCoins() << std::endl;
    std::cout << "============================" << std::endl;
}

void OutputTerminal::displaySpells(Player& player) {
    std::cout << "=== ЗАКЛИНАНИЯ ИГРОКА ===" << std::endl;
    int cntMegaShotgun = player.getHandSpell()->getCntMegaShotgun();
    int cntBulletRain = player.getHandSpell()->getCntBulletRain();
    int cntCheapWhiskey = player.getHandSpell()->getCntCheapWhiskey();
    int cntJohnnysGhost = player.getHandSpell()->getCntJohnnysGhost();
    int cntUpdateSpell = player.getHandSpell()->getCntUpdateSpell();
    int cntTotalSpells = player.getHandSpell()->getCntTotalSpells();
    int maxSizeHandSpell = player.getHandSpell()->getMaxSizeHandSpell();

    cout << "Уровень всех заклинаний: (" << player.getHandSpell()->getUpdateSpell()->getLevel() << ")" << endl; 
    cout << "MegaShotgun: (" << cntMegaShotgun << "), BulletRain: (" << cntBulletRain
         << "), CheapWhiskey: (" << cntCheapWhiskey << "), JohnnysGhost: (" << cntJohnnysGhost
         << "), UpdateSpell: ("<< cntUpdateSpell <<  ") Всего заклинаний: (" << cntTotalSpells << "/" << maxSizeHandSpell << ")" << endl;
    std::cout << "=========================" << std::endl;
}

void OutputTerminal::displayVictory(Game& game) {

    std::cout << "\nПОЛНАЯ ПОБЕДА!" << std::endl;
    std::cout << " ██╗   ██╗██╗ ██████╗████████╗ ██████╗ ██████╗ ██╗   ██╗\n";
    std::cout << " ██║   ██║██║██╔════╝╚══██╔══╝██╔═══██╗██╔══██╗╚██╗ ██╔╝\n";
    std::cout << " ██║   ██║██║██║        ██║   ██║   ██║██████╔╝ ╚████╔╝ \n";
    std::cout << " ╚██╗ ██╔╝██║██║        ██║   ██║   ██║██╔══██╗  ╚██╔╝  \n";
    std::cout << "  ╚████╔╝ ██║╚██████╗   ██║   ╚██████╔╝██║  ██║   ██║   \n";
    std::cout << "   ╚═══╝  ╚═╝ ╚═════╝   ╚═╝    ╚═════╝ ╚═╝  ╚═╝   ╚═╝   \n";
    std::cout << "══════════════════════════════════════════════════════════════════════════════\n";
    std::cout << "Вы прошли все три уровня и стали легендой Дикого Запада!" << std::endl;
    std::cout << "Поздравляем, " << game.getPlayer().getName() << "!" << std::endl;
    std::cout << "══════════════════════════════════════════════════════════════════════════════\n";
}

void OutputTerminal::displayDefeat(Game& game){
    auto& playerRef = game.getPlayerRef();
    std::cout << "\n\n";
    std::cout << "  ██████   █████  ███    ███ ███████      ██████  ██    ██ ███████ ██████  \n";
    std::cout << " ██       ██   ██ ████  ████ ██          ██    ██ ██    ██ ██      ██   ██ \n";
    std::cout << " ██   ███ ███████ ██ ████ ██ █████       ██    ██ ██    ██ █████   ██████  \n";
    std::cout << " ██    ██ ██   ██ ██  ██  ██ ██          ██    ██  ██  ██  ██      ██   ██ \n";
    std::cout << "  ██████  ██   ██ ██      ██ ███████      ██████    ████   ███████ ██   ██ \n";
    std::cout << "\n";
    std::cout << "══════════════════════════════════════════════════════════════════════════════\n";
    std::cout << "                          ИГРОК ПОГИБ В БОЮ!\n";
    std::cout << "══════════════════════════════════════════════════════════════════════════════\n";
    std::cout << "\n";
    std::cout << "                     Заработано монет: " << playerRef.getCoins() << "\n";
    std::cout << "\n";
    std::cout << "              Но легенда о ковбое " << playerRef.getName() << " будет жить вечно!\n";
    std::cout << "\n";
}

void OutputTerminal::displayFinishLevel(Game& game) {
    auto& playerRef = game.getPlayerRef();
    
    int upgradesAvailable = MAX_UPGRADES_PER_LEVEL;
    
    std::cout << "\n=== УЛУЧШЕНИЯ ХАРАКТЕРИСТИК ===" << std::endl;
    std::cout << "Поздравляем с прохождением уровня!" << std::endl;
    std::cout << "Вы можете улучшить " << upgradesAvailable << " характеристики:" << std::endl;
}

void OutputTerminal::displayUpgradeChoice(Game& game) {
    auto& playerRef = game.getPlayerRef();
    
    std::cout << "\nТекущие характеристики:" << std::endl;
    std::cout << "1. Урон в ближнем бою: " << playerRef.getMeleeDamage() << std::endl;
    std::cout << "2. Урон в дальнем бою: " << playerRef.getRangedDamage() << std::endl;
    std::cout << "3. Дальность атаки: " << playerRef.getAttackRange() << std::endl;
}