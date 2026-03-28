#include "playing_field.h"
#include "direction.h"
#include "cell.h"
#include "player.h"
#include "enemy.h"
#include <vector>
#include <iostream>
#include <utility>
#include <algorithm>
#include <iomanip>

PlayingField::PlayingField(int height, int width) {
	const int MIN_SIZE = 10;
	const int MAX_SIZE = 25;

	int corrected_height = std::max(MIN_SIZE, std::min(height, MAX_SIZE));
	int corrected_width = std::max(MIN_SIZE, std::min(width, MAX_SIZE));

	if (height != corrected_height || width != corrected_width) {
		std::cout << "Предупреждение: Введены некорректные размеры поля." << std::endl;
		std::cout << "Размеры должны быть в диапазоне от " << MIN_SIZE << "x" << MIN_SIZE
			<< " до " << MAX_SIZE << "x" << MAX_SIZE << "." << std::endl;
		std::cout << "Вы ввели: " << height << "x" << width
			<< ". Создано поле размером " << corrected_height << "x" << corrected_width << "." << std::endl;
	}

	height_ = corrected_height;
	width_ = corrected_width;
	field.resize(width_);
	for (int i = 0; i < width_; i++) {
		field[i].resize(height_);
	}
}

PlayingField::PlayingField(const PlayingField& other) {
	std::cout << "Вызван конструктор КОПИРОВАНИЯ для PlayingField." << std::endl;

	height_ = other.height_;
	width_ = other.width_;

	field.resize(width_);
	for (int x = 0; x < width_; x++) {
		field[x].resize(height_);
		for (int y = 0; y < height_; y++) {
			field[x][y] = other.field[x][y];  
		}
	}
}

PlayingField& PlayingField::operator=(const PlayingField& other) {
	std::cout << "Вызван оператор ПРИСВАИВАНИЯ С КОПИРОВАНИЕМ для PlayingField." << std::endl;

	if (this == &other) {
		return *this;
	}

	height_ = other.height_;
	width_ = other.width_;
	field = other.field;

	return *this;
}

PlayingField::PlayingField(PlayingField&& other) noexcept {
	std::cout << "Вызван конструктор ПЕРЕМЕЩЕНИЯ для PlayingField." << std::endl;

	height_ = other.height_;
	width_ = other.width_;
	field = std::move(other.field);

	other.height_ = 0;
	other.width_ = 0;
}

PlayingField& PlayingField::operator=(PlayingField&& other) noexcept {
	std::cout << "Вызван оператор ПРИСВАИВАНИЯ С ПЕРЕМЕЩЕНИЕМ для PlayingField." << std::endl;

	if (this == &other) {
		return *this;
	}

	height_ = other.height_;
	width_ = other.width_;
	field = std::move(other.field);

	other.height_ = 0;
	other.width_ = 0;

	return *this;
}

void PlayingField::placeBattler(int x, int y, BasicBattler* battler) {
	if (y >= 0 && y < height_ && x >= 0 && x < width_) {
		if (field[x][y].isFree()) {
			field[x][y].setBattler(battler);
		}
		else {
			std::cerr << "Ошибка размещения бойца: Клетка (" << x << ", " << y << ") уже занята!" << std::endl;
		}
	}
	else {
		std::cerr << "Ошибка размещения: Координаты (" << x << ", " << y << ") вне поля!" << std::endl;
	}
}

std::pair<int, int> PlayingField::getBattlerCoords(BasicBattler* battler) const {
	for (int x = 0; x < width_; x++) {
		for (int y = 0; y < height_; y++) {
			if (field[x][y].getBattler() == battler) {
				return { x, y };
			}
		}
	}
	return { -1, -1 };
}

BasicBattler* PlayingField::getBattlerAt(int x, int y) const {
	if (y >= 0 && y < height_ && x >= 0 && x < width_) {
		return field[x][y].getBattler();
	}
	return nullptr;
}

MoveResult PlayingField::move(BasicBattler* battler, Direction direction) {

	if (!battler) {
		std::cerr << "Ошибка: Попытка сделать ход несуществующим бойцом." << std::endl;
		return MoveResult::INVALID_BATTLER;
	}

	auto coords = getBattlerCoords(battler);
	int old_x = coords.first;
	int old_y = coords.second;
	if (old_x == -1) {
		std::cerr << "Ошибка: Боец " << battler->getName() << " не найден на поле!" << std::endl;
		return MoveResult::INVALID_BATTLER;
	}

	std::string dirName = getDirectionName(direction);
	int new_x = old_x, new_y = old_y;
	switch (direction) {
		case Direction::LEFT:
			new_x--;
			break;
		case Direction::RIGHT:
			new_x++;
			break;
		case Direction::UP:
			new_y++;
			break;
		case Direction::DOWN:
			new_y--;
			break;
	};

	if (new_x >= 0 && new_x < width_ && new_y >= 0 && new_y < height_) {
		if (field[new_x][new_y].isFree()) {
			field[new_x][new_y].setBattler(battler);
			field[old_x][old_y].clean();
			std::cout << "Боец " << battler->getName() << " передвинулся " << dirName << " с ("
				<< old_x << ", " << old_y << ") на (" << new_x << ", " << new_y << ")." << std::endl;
			return MoveResult::SUCCESS_MOVE;

		}
		else {
			BasicBattler* target = field[new_x][new_y].getBattler();

			if (target) {

				if (battler->isAlly(target)) {
					std::cout << "Боец " << battler->getName() << " видит союзника ("
						<< target->getName() << ") и ищет другой путь." << std::endl;
					return MoveResult::ALLY_IN_THE_WAY;
				}
				else {

					std::cout << "\nБоец " << battler->getName() << " пытается пойти " << dirName
						<< ", но клетка (" << new_x << ", " << new_y << ") занята. Атака!" << std::endl;
					battler->hit(target);

					if (!target->isAlive()) {
						std::cout << target->getName() << " повержен!" << std::endl;
						field[new_x][new_y].setBattler(battler);
						field[old_x][old_y].clean();

						if (target->getFaction() == Faction::PLAYER) {
							return MoveResult::PLAYER_DIED;
						}
						else {
							return MoveResult::TARGET_KILLED;
						}
					}
					else {
						return MoveResult::SUCCESS_ATTACK;
					}
				}
			}
		}
	}
	std::cout << "Боец " << battler->getName() << " не может передвинуться " << dirName
		<< " - граница поля." << std::endl;
	return MoveResult::BOUNDARY_HIT;
}

int PlayingField::getWidth() const {
	return width_;
}

int PlayingField::getHeight() const {
	return height_;
}

MoveResult PlayingField::moveLeft(BasicBattler* battler) {
	return move(battler, Direction::LEFT);
}

MoveResult PlayingField::moveRight(BasicBattler* battler) {
	return move(battler, Direction::RIGHT);
}

MoveResult PlayingField::moveUp(BasicBattler* battler) {
	return move(battler, Direction::UP);
}

MoveResult PlayingField::moveDown(BasicBattler* battler) {
	return move(battler, Direction::DOWN);
}

void PlayingField::clearAllEnemies() {
	for (int y = 0; y < height_; ++y) {
		for (int x = 0; x < width_; ++x) {
			BasicBattler* battler = getBattlerAt(x, y); 
			if (battler && battler->getFaction() == Faction::ENEMY) {
				field[x][y].clean(); 
			}
		}
	}
}