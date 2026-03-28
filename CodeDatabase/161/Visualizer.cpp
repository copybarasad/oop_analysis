#include <iostream>
#include <iomanip>

#include "../map/cells/Events/TrapEvent.h"
#include "Visualizer.h"

// ANSI цвета
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define BLUE    "\033[34m"
#define DARKBLUE "\033[38;5;18m"
#define YELLOW  "\033[33m"
#define ORANGE  "\033[38;5;208m"
#define GRAY    "\033[37m"
#define BLACK   "\033[30m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define BORDEAUX "\033[38;5;52m"

void Visualizer::PrintFrame(const std::vector<std::vector<std::string>>& frame,
                            int width, int height, int player_score,
                            int player_hp)
{
    std::cout << "\033[H";

    std::cout << MAGENTA << "+--------+\n";
    std::cout << "| HP " << std::setw(3) << std::left << player_hp << "|\n";
    std::cout << "+--------+" << RESET << "\n";

    std::cout << "Score: " << player_score << "\n";


	std::cout << "   ";
	for (int x = 0; x < width; ++x) {
		std::cout << std::setw(3) << x;
	}
	std::cout << "\n";


	std::cout << CYAN << "   "; // место под вертикальную линию слева
	for (int x = 0; x < width; ++x) {
		std::cout << "───"; // точно 3 символа под каждую клетку
	}
	std::cout << RESET << "\n";

	// Вывод поля с вертикальной линией слева
	for (int y = 0; y < height; ++y) {
		std::cout << CYAN << std::setw(2) << y << "│" << RESET; // цифра строки + вертикальная линия без пробела
		for (int x = 0; x < width; ++x) {
			const std::string& cell = frame[y][x];
			char symbol = cell.empty() ? '.' : cell[0];
			std::string color = RESET;

			switch (symbol) {
				case 'P': color = GREEN; break;
				case 'A': color = BLUE; break;
				case 'E': color = RED; break;
				case 'T': color = ORANGE; break;
				case 'B': color = YELLOW; break;
				case '*': color = BORDEAUX; break; // ловушка
				case 'C': color = GRAY; break;     // обычная клетка
				case 'I': color = BLACK; break;    // непроходимая
				case 'S': color = DARKBLUE; break; // замедляющая
				default: color = RESET; break;
			}

			std::cout << color << std::setw(3) << symbol << RESET;
		}
		std::cout << "\n";
	}


    std::cout << "\nLegend: "
              << GREEN << "P=Player " << RESET
              << BLUE << "A=Ally " << RESET
              << RED << "E=Enemy " << RESET
              << ORANGE << "T=Tower " << RESET
              << YELLOW << "B=Building " << RESET
              << BORDEAUX << "*=Trap/Event " << RESET
              << GRAY << "C=Cell " << RESET
              << BLACK << "I=Impassable " << RESET
              << DARKBLUE << "S=Slowing" << RESET
              << "\n\n";
}

void Visualizer::Draw(Field& field, World& world, const std::shared_ptr<Player>& player) {
    int width, height;
    field.GetSize(width, height);

    std::vector<std::vector<std::string>> frame(height, std::vector<std::string>(width, "C")); // по умолчанию обычная клетка

    // Заполняем клетки и события
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            auto cell = field.GetCell(x, y);
            if (!cell) continue;

            std::string type_full = field.GetCellType(x, y);
            frame[y][x] = type_full.substr(0,1);
            if (cell->GetEvent()) frame[y][x] = "*";
        }
    }

    auto draw_entities = [&](auto& entities, char symbol) {
        for (const auto& e : entities) {
            if (auto ent = e.lock()) {
                int x, y;
                if (field.GetPosEntity(ent, x, y)) {
                    frame[y][x] = symbol; // HP не отображаем
                }
            }
        }
    };

    draw_entities(world.Enemies().GetEntities(), 'E');
    draw_entities(world.Allyes().GetEntities(), 'A');
    draw_entities(world.EnemiesTowers().GetEnemiesTowers(), 'T');
    draw_entities(world.EnemiesBuildings().GetEnemiesBuildings(), 'B');

    // Игрок поверх всего
    int px, py;
    if (field.GetPosEntity(player, px, py)) {
        frame[py][px] = "P";
    }

    PrintFrame(frame, width, height, player->GetScore(), player->GetHP());
}
