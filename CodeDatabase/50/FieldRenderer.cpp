#include "FieldRenderer.h"

void FieldRenderer::print(const Field& field, int hp, int turns, bool isAttRange, int mana) {

    int width = field.getWidth();
    int height = field.getHeight();

    std::cout << "     ";
    for (int j = 0; j < width; ++j) {
        if (j < 10) std::cout << j << "  ";
        else std::cout << j << " ";
    }
    std::cout << "\n";

    std::cout << "   /";
    for (int j = 0; j < width * 3; ++j) std::cout << "-";
    std::cout << "\\" << "\n";

    for (int i = 0; i < height; ++i) {
        if (i < 10) std::cout << " " << i << " | ";
        else std::cout << i << " | ";

        for (int j = 0; j < width; ++j) {
            GameEnt* ent = field.getCellEntity(j, i);

            if (ent != nullptr) {
                switch (ent->getObjType()) {
                    case GameEntType::Player:
                        std::cout << (j + 1 == width ? "P " : "P  ");
                        break;
                    case GameEntType::Enemy:
                        std::cout << (j + 1 == width ? "E " : "E  ");
                        break;
                    case GameEntType::Tower:
                        std::cout << (j + 1 == width ? "T " : "T  ");
                        break;
                }
            } else {
                switch (field.getCellType(j, i)) {
                    case CellType::Empty:
                        std::cout << (j + 1 == width ? ". " : ".  ");
                        break;
                    case CellType::Wall:
                        std::cout << (j + 1 == width ? "# " : "#  ");
                        break;
                    case CellType::Trap:
                        std::cout << (j + 1 == width ? "* " : "*  ");
                        break;
                }
            }
        }
        std::cout << "| " << i << "\n";
    }

    std::cout << "   \\";
    for (int j = 0; j < width * 3; ++j) std::cout << "-";
    std::cout << "/" << "\n";

    std::cout << "     ";
    for (int j = 0; j < width; ++j) {
        if (j < 10) std::cout << j << "  ";
        else std::cout << j << " ";
    }
    std::cout << "\n\n";

    std::cout << "Ход: " << turns
              << " || Здоровье игрока: " << hp
              << " || Мана игрока: " << mana
              << " || Тип атаки: " << (isAttRange ? "Дальний" : "Ближний")
              << std::endl;
}
