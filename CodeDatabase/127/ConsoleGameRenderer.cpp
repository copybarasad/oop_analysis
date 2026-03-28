#include "ConsoleGameRenderer.h"
#include "Game.h"
#include "Cell.h"
#include "Player.h"
#include "ControlConfig.h"
#include <iostream>

void ConsoleGameRenderer::render(const Game& game) {
	const auto& field = game.getField();
	const auto& player = game.getPlayer();
	const auto& hand = game.getHand();
	const auto& enemies = game.getEnemies();
	const auto& allies = game.getAllies();
	const auto& towers = game.getTowers();
	const auto& traps = game.getTraps();
	int turn = game.getTurn();
	int level = game.getLevel();
	int maxLevels = game.getMaxLevels();
	int separatorLines = game.getSeparatorLines();
	auto* controlConfig = game.getControlConfig();

	// Отрисовка координатной сетки
	std::cout << "   ";
	for (int x = 0; x < field.getWidth(); ++x) {
		std::cout << (x % 10);
		if (x + 1 < field.getWidth()) std::cout << ' ';
	}
	std::cout << '\n';

	// Отрисовка поля
	for (int y = 0; y < field.getHeight(); ++y) {
		std::cout << (y % 10) << "  ";
		for (int x = 0; x < field.getWidth(); ++x) {
			char cellChar = (field.getCellType(x, y) == Cell::Type::Impassable) ? '#' : '.';

			// Игрок
			if (player.getX() == x && player.getY() == y) {
				cellChar = 'P';
			}

			// Союзники
			for (const auto& a : allies) {
				if (a.isAlive() && a.getX() == x && a.getY() == y) {
					cellChar = 'A';
					break;
				}
			}

			// Враги
			if (game.findEnemyIndexAt(x, y) != -1) {
				cellChar = 'E';
			}

			// Башни
			for (const auto& t : towers) {
				if (t.getX() == x && t.getY() == y) {
					cellChar = 'T';
					break;
				}
			}

			// Ловушки
			for (const auto& tr : traps) {
				if (tr.getX() == x && tr.getY() == y) {
					if (cellChar == '.') cellChar = 'x';
					break;
				}
			}

			std::cout << cellChar;
			if (x + 1 < field.getWidth()) std::cout << ' ';
		}
		std::cout << '\n';
	}

	// Информация о состоянии игры
	std::cout << "Ход: " << turn
		<< "  HP: " << player.getHP()
		<< "   Очки: " << player.getScore()
		<< "   Уровень: " << level << " / " << maxLevels
		<< "   Режим: " << (player.getAttackMode() == Player::AttackMode::Melee ? "Ближний" : "Дальний")
		<< "   Дальность: " << player.getRangedRange()
		<< "\n";

	// Карты в руке
	hand.printSpellsWithDescription();

	if (controlConfig != nullptr) {
		auto bindings = controlConfig->getAllBindings();
		char moveUp = bindings.count("move_up") ? bindings.at("move_up") : 'w';
		char moveDown = bindings.count("move_down") ? bindings.at("move_down") : 's';
		char moveLeft = bindings.count("move_left") ? bindings.at("move_left") : 'a';
		char moveRight = bindings.count("move_right") ? bindings.at("move_right") : 'd';
		char attack = bindings.count("attack") ? bindings.at("attack") : 'f';
		char melee = bindings.count("switch_melee") ? bindings.at("switch_melee") : 'm';
		char ranged = bindings.count("switch_ranged") ? bindings.at("switch_ranged") : 'r';
		char menu = bindings.count("menu") ? bindings.at("menu") : 'q';
		char useSpell = bindings.count("use_spell") ? bindings.at("use_spell") : 'u';
		char buySpell = bindings.count("buy_spell") ? bindings.at("buy_spell") : 'b';
		char save = bindings.count("save") ? bindings.at("save") : 'z';
		char load = bindings.count("load") ? bindings.at("load") : 'l';
		char help = bindings.count("help") ? bindings.at("help") : 'h';

		std::cout << "(Управление: " << moveUp << "/" << moveLeft << "/" << moveDown << "/" << moveRight
			<< " - ход/атака, " << attack << " - выстрел (в Ranged), "
			<< melee << " - ближний, " << ranged << " - дальний, " << menu << " - выход в меню)\n";
		std::cout << "(Использовать карту: " << useSpell << " <index> <tx> <ty>)\n";
		std::cout << "(Купить карту: " << buySpell << "  |  Сохранить: " << save
			<< "  |  Загрузить: " << load << "  |  Помощь: " << help << ")\n";
	}
	else {
		std::cout << "(Управление: w/a/s/d - ход/атака, f - выстрел (в Ranged), m - ближний, r - дальний, q - выход в меню)\n";
		std::cout << "(Использовать карту: u <index> <tx> <ty>)\n";
		std::cout << "(Купить карту: b  |  Сохранить: save <filename>  |  Загрузить: load <filename>)\n";
	}
	std::cout << "Примечания: ловушки (x) нельзя занять. '#' - непроходимая клетка.\n";

	// Разделительные строки
	for (int k = 0; k < separatorLines; ++k) std::cout << '\n';
	std::cout << "----------------------------------------\n";

	// Информация о башнях
	if (!towers.empty()) {
		std::cout << "Башни на поле:\n";
		int t_idx = 0;
		for (const auto& t : towers) {
			std::cout << "  Башня " << t_idx++ << " на (" << t.getX() << ", " << t.getY() << "): "
				<< "Урон: " << t.getDamage() << ", Радиус: " << t.getRange() << ", HP: " << t.getHP() << "\n";
		}
		std::cout << "----------------------------------------\n";
	}

	std::string errorMsg = game.getLastErrorMessage();
	if (!errorMsg.empty()) {
		std::cout << "ОШИБКА: " << errorMsg << "\n";
	}

	// Подсказка для ввода команды 
	if (controlConfig != nullptr) {
		auto bindings = controlConfig->getAllBindings();
		char help = bindings.count("help") ? bindings.at("help") : 'h';
		char save = bindings.count("save") ? bindings.at("save") : 'z';
		char load = bindings.count("load") ? bindings.at("load") : 'l';
		char menu = bindings.count("menu") ? bindings.at("menu") : 'q';
		char exit = bindings.count("exit") ? bindings.at("exit") : 'x';
		std::cout << "Введите команду (" << help << "/" << save << "/" << load << "/" << menu << "/" << exit << "): " << std::flush;
	} else {
		std::cout << "Введите команду (help/save/load/menu/exit): " << std::flush;
	}
}

