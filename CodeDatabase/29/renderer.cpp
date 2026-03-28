#include "renderer.hpp"
#include "game.hpp"
#include "spell.hpp"
#include <iostream>

void TerminalRenderer::Draw(const Game& game) {
	const Player& player = game.PlayerRef();
	std::cout << "HP: " << player.HP()
	          << "  Mode: " << (player.Mode() == CombatMode::kMelee ? "Melee" : "Ranged")
	          << "  Dmg: " << player.CurrentDamage()
	          << "  Score: " << player.Score() << "\n";

	std::cout << "Spells: ";
	std::size_t count = player.Hand().Size();
	for (std::size_t i = 0; i < count; ++i) {
		if (i) std::cout << ' ';
		const ISpell* sp = player.Hand().Get(i);
		if (sp) {
			std::cout << (i + 1) << ":" << sp->Name() << "[r=" << sp->Range() << "]";
		} else {
			std::cout << (i + 1) << ":(empty)";
		}
	}
	std::cout << "\n";

	const Board& board = game.BoardRef();
	const int W = board.Width();
	const int H = board.Height();

	std::cout << "     ";
	for (int x = 0; x < W; ++x) {
		std::cout << (x / 10) << ' ';
	}
	std::cout << "\n";

	std::cout << "     ";
	for (int x = 0; x < W; ++x) {
		std::cout << (x % 10) << ' ';
	}
	std::cout << "\n";

	for (int y = 0; y < H; ++y) {
		if (y < 10) {
			std::cout << "0" << y << "   ";
		} else {
			std::cout << y << "   ";
		}

		for (int x = 0; x < W; ++x) {
			char ch = '.';
			if (board.GetCellType(x, y) == CellType::kWall) {
				ch = '#';
			}

			if (player.Pos().x == x && player.Pos().y == y) {
				ch = 'P';
			}

			for (const auto& e : game.Enemies()) {
				if (e.IsAlive() && e.Pos().x == x && e.Pos().y == y) {
					ch = 'E';
					break;
				}
			}

			for (const auto& t : game.Towers()) {
				if (t.IsAlive() && t.Pos().x == x && t.Pos().y == y) {
					ch = 'T';
					break;
				}
			}

			for (const auto& tr : game.Traps()) {
				if (tr.X() == x && tr.Y() == y) {
					ch = '^';
					break;
				}
			}

			std::cout << ch << ' ';
		}

		std::cout << "\n";
	}

	std::cout << "Controls: WASD move/attack, U/L/R/V attack, T toggle mode, Q quit, <slot> x y cast\n";
}
