#include "SaveManager.h"
#include "Game.h" 


void SaveManager::saveGame(const Game& game, const std::string& filename) {
    std::ofstream out(filename);
    if (!out) {
        throw SaveFileOpenException(filename);
    }

    out << "SAVE1\n";
    out << game.getCurrentLevel() << ' ' << game.getTurns() << '\n';

    game.getPlayer().save(out);
    game.getField().save(out);
    game.getEntityManager().save(out);
    game.getHand().save(out);

    if (!out) {
        throw GameException("Ошибка при записи файла сохранения");
    }
}

void SaveManager::loadGame(Game& game, const std::string& filename) {
    std::ifstream in(filename);
    if (!in) {
        throw LoadFileOpenException(filename);
    }

    std::string magic;
    std::getline(in, magic);
    if (magic != "SAVE1") {
        throw InvalidSaveDataException("Неверная сигнатура файла");
    }

    int lvl, turns;
    in >> lvl >> turns;
    if (!in) {
        throw InvalidSaveDataException("Ошибка чтения заголовка сохранения");
    }

    game.setCurrentLevel(lvl);
    game.setTurns(turns);

    game.getPlayer().load(in);
    game.getField().load(in);

    // очистить внутренние сущности перед загрузкой
    game.getField().clearAllEntities();

    game.getEntityManager().load(in, game.getField());
    game.getHand().load(in);

    // игрока надо поставить на поле
    game.getField().setCellEntity(&game.getPlayer(), game.getPlayer().getX(), game.getPlayer().getY());
}
