#include "Command.h"
#include "Game.h"
#include "ControlConfig.h"
#include <iostream>

void MoveUpCommand::execute(Game& game) {
	game.processMovementOrMeleeAttack(0, -1);
}

void MoveDownCommand::execute(Game& game) {
	game.processMovementOrMeleeAttack(0, 1); 
}

void MoveLeftCommand::execute(Game& game) {
	game.processMovementOrMeleeAttack(-1, 0);
}

void MoveRightCommand::execute(Game& game) {
	game.processMovementOrMeleeAttack(1, 0);
}

void SwitchMeleeCommand::execute(Game& game) {
	game.setPlayerAttackMode(Player::AttackMode::Melee);
}

void SwitchRangedCommand::execute(Game& game) {
	game.setPlayerAttackMode(Player::AttackMode::Ranged);
}

void UseSpellCommand::execute(Game& game) {
	game.useSpell(index_, tx_, ty_);
}

void BuySpellCommand::execute(Game& game) {
	game.buyRandomSpell();
}

void SaveCommand::execute(Game& game) {
	try {
		game.saveToFile(filename_);
	}
	catch (const std::exception& e) {
		std::cerr << "Не удалось сохранить игру: " << e.what() << "\n";
	}
}

void LoadCommand::execute(Game& game) {
	try {
		game.loadFromFile(filename_);
	}
	catch (const std::exception& e) {
		std::cerr << "Не удалось загрузить игру: " << e.what() << "\n";
	}
}

void MenuCommand::execute(Game& game) {
	game.requestMenuExit();
}

void ExitCommand::execute(Game& game) {
	game.requestProgramExit();
}

void HelpCommand::execute(Game& game) {
	ControlConfig* config = game.getControlConfig();
	
	if (config != nullptr) {
		auto bindings = config->getAllBindings();
		char moveUp = bindings.count("move_up") ? bindings.at("move_up") : 'w';
		char moveDown = bindings.count("move_down") ? bindings.at("move_down") : 's';
		char moveLeft = bindings.count("move_left") ? bindings.at("move_left") : 'a';
		char moveRight = bindings.count("move_right") ? bindings.at("move_right") : 'd';
		char attack = bindings.count("attack") ? bindings.at("attack") : 'f';
		char melee = bindings.count("switch_melee") ? bindings.at("switch_melee") : 'm';
		char ranged = bindings.count("switch_ranged") ? bindings.at("switch_ranged") : 'r';
		char useSpell = bindings.count("use_spell") ? bindings.at("use_spell") : 'u';
		char buySpell = bindings.count("buy_spell") ? bindings.at("buy_spell") : 'b';
		char save = bindings.count("save") ? bindings.at("save") : 'z';
		char load = bindings.count("load") ? bindings.at("load") : 'l';
		char menu = bindings.count("menu") ? bindings.at("menu") : 'q';
		char exit = bindings.count("exit") ? bindings.at("exit") : 'x';
		char help = bindings.count("help") ? bindings.at("help") : 'h';
		
		std::cout << "Команды управления:\n";
		std::cout << "  Движение: " << moveUp << "/" << moveLeft << "/" << moveDown << "/" << moveRight << "\n";
		std::cout << "  Атака: " << attack << "+направление - дальняя атака\n";
		std::cout << "  Режимы: " << melee << " - ближний бой, " << ranged << " - дальний бой\n";
		std::cout << "  Заклинания: " << useSpell << " <index> <x> <y> - использовать, " << buySpell << " - купить\n";
		std::cout << "  Система: " << save << " - сохранить, " << load << " - загрузить, " 
			<< menu << " - меню, " << exit << " - выход, " << help << " - помощь\n";
	} else {
		std::cout << "Команды: w/a/s/d - движение; f+направление - дальняя атака; "
			"m/r - сменить режим; u <index> <x> <y> - использовать заклинание; "
			"save <file>; load <file>; menu - вернуться в меню; exit - выйти из программы.\n";
	}
}

void AttackCommand::execute(Game& game) {
	game.performRangedAttack(direction_);
}

void NoCommand::execute(Game& game) {
	ControlConfig* config = game.getControlConfig();
	
	if (config != nullptr) {
		auto bindings = config->getAllBindings();
		char moveUp = bindings.count("move_up") ? bindings.at("move_up") : 'w';
		char moveDown = bindings.count("move_down") ? bindings.at("move_down") : 's';
		char moveLeft = bindings.count("move_left") ? bindings.at("move_left") : 'a';
		char moveRight = bindings.count("move_right") ? bindings.at("move_right") : 'd';
		char attack = bindings.count("attack") ? bindings.at("attack") : 'f';
		char melee = bindings.count("switch_melee") ? bindings.at("switch_melee") : 'm';
		char ranged = bindings.count("switch_ranged") ? bindings.at("switch_ranged") : 'r';
		char menu = bindings.count("menu") ? bindings.at("menu") : 'q';
		
		std::cout << "Неизвестная команда. Используйте " << moveUp << "/" << moveLeft << "/" << moveDown << "/" << moveRight 
			<< " для хода/атаки, " << attack << " для выстрела (в Ranged), " 
			<< melee << "/" << ranged << " для переключения режима, " << menu << " для выхода в меню.\n";
	} else {
		std::cout << "Неизвестная команда. Используйте w/a/s/d для хода/атаки, f для выстрела (в Ranged), m/r для переключения режима, q для выхода в меню.\n";
	}
}

