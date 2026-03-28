#ifndef COMMAND_H
#define COMMAND_H

#include <memory>
#include <string>

class Game;

// Базовый класс команды
class Command {
public:
	virtual ~Command() = default;
	virtual void execute(Game& game) = 0;
	virtual bool isValid() const { return true; }
};

// Команда движения вверх
class MoveUpCommand : public Command {
public:
	void execute(Game& game) override;
};

// Команда движения вниз
class MoveDownCommand : public Command {
public:
	void execute(Game& game) override;
};

// Команда движения влево
class MoveLeftCommand : public Command {
public:
	void execute(Game& game) override;
};

// Команда движения вправо
class MoveRightCommand : public Command {
public:
	void execute(Game& game) override;
};

// Команда переключения на ближний бой
class SwitchMeleeCommand : public Command {
public:
	void execute(Game& game) override;
};

// Команда переключения на дальний бой
class SwitchRangedCommand : public Command {
public:
	void execute(Game& game) override;
};

// Команда использования заклинания
class UseSpellCommand : public Command {
private:
	int index_;
	int tx_;
	int ty_;
public:
	UseSpellCommand(int index, int tx, int ty) : index_(index), tx_(tx), ty_(ty) {}
	void execute(Game& game) override;
	bool isValid() const override { return index_ >= 0 && tx_ >= 0 && ty_ >= 0; }
};

// Команда покупки заклинания
class BuySpellCommand : public Command {
public:
	void execute(Game& game) override;
};

// Команда сохранения игры
class SaveCommand : public Command {
private:
	std::string filename_;
public:
	SaveCommand(const std::string& filename) : filename_(filename) {}
	void execute(Game& game) override;
};

// Команда загрузки игры
class LoadCommand : public Command {
private:
	std::string filename_;
public:
	LoadCommand(const std::string& filename) : filename_(filename) {}
	void execute(Game& game) override;
};

// Команда выхода в меню
class MenuCommand : public Command {
public:
	void execute(Game& game) override;
};

// Команда выхода из программы
class ExitCommand : public Command {
public:
	void execute(Game& game) override;
};

// Команда помощи
class HelpCommand : public Command {
public:
	void execute(Game& game) override;
};

// Команда дальней атаки
class AttackCommand : public Command {
private:
	char direction_;
public:
	AttackCommand(char direction) : direction_(direction) {}
	void execute(Game& game) override;
};

// Пустая команда 
class NoCommand : public Command {
public:
	void execute(Game& game) override;
};

#endif 

