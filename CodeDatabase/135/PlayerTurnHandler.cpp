#include "PlayerTurnHandler.h"
#include "Player.h"
#include "IPlayerCharacter.h"
#include "SpellContext.h"
#include "SpellHand.h"
#include "SpellEnhancer.h"
#include "PlayerResources.h"
#include "GameConstants.h"
#include <limits>
#include <iostream>

PlayerTurnHandler::PlayerTurnHandler(GameField& game_field, EntityManager& em, int& player_idx,
                                       SpellFactory& factory, RandomGenerator& random_gen, ScoreSystem& score_sys)
	: field(game_field),
	  entity_manager(em),
	  player_index_ref(player_idx),
	  spell_factory(factory),
	  rng(random_gen),
	  score_system(score_sys),
	  movement_helper(game_field, em),
	  spell_shop(game_field, em, player_idx, factory) {}

PlayerTurnResult PlayerTurnHandler::processPlayerTurn() {
	int& player_index = player_index_ref.get();
	PlayerTurnResult result;
	result.action = TurnAction::NONE;
	
	if (player_index < 0 || !entity_manager.hasEntity(player_index)) {
		return result;
	}
	
	auto* entity = entity_manager.getEntity(player_index);
	if (!entity) return result;
	auto* player_char = dynamic_cast<IPlayerCharacter*>(entity);
	if (!player_char) return result;
	
	field.indexToCoord(player_index, result.player_x, result.player_y);
	result.player_slowed = player_char->isSlowed();
	
	if (player_char->isSlowed()) {
		result.action = TurnAction::SLOWED;
		player_char->setSlow(false);
		return result;
	}
	
	char command;
	if (!(std::cin >> command)) {
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		return result;
	}
	
	if (command == 'e') {
		player_char->setWeapon();
		result.action = TurnAction::WEAPON_CHANGE;
		result.weapon.is_melee = player_char->melee();
		result.weapon.damage = player_char->getDamage();
		return result;
	}
	
	if (command == 'c') {
		result.action = TurnAction::SPELL_CAST;
		result.spell = processSpellCasting();
		return result;
	}
	
	if (command == 'b') {
		spell_shop.openShop();
		result.action = TurnAction::SHOP_VISIT;
		return result;
	}
	
	result.move = processPlayerMovement(command);
	if (result.move.success) {
		result.action = TurnAction::MOVE;
		return result;
	}
	
	if (result.move.error_message.empty()) {
		int target_x = result.player_x;
		int target_y = result.player_y;
		
		switch (command) {
			case 'w': target_y--; break;
			case 's': target_y++; break;
			case 'a': target_x--; break;
			case 'd': target_x++; break;
			default: return result;
		}
		
		if (field.isValidPosition(target_x, target_y)) {
			int target_index = field.coordToIndex(target_x, target_y);
			if (entity_manager.hasEntity(target_index)) {
				result.attack = processPlayerAttack(target_index);
				if (result.attack.success) {
					result.action = TurnAction::ATTACK;
				}
			}
		}
	}
	
	return result;
}

MoveResult PlayerTurnHandler::processPlayerMovement(char command) {
	int& player_index = player_index_ref.get();
	MoveResult result;
	result.success = false;
	
	field.indexToCoord(player_index, result.old_x, result.old_y);
	
	int new_x = result.old_x;
	int new_y = result.old_y;
	
	switch (command) {
		case 'w': new_y--; result.direction = "вверх ↑"; break;
		case 's': new_y++; result.direction = "вниз ↓"; break;
		case 'a': new_x--; result.direction = "влево ←"; break;
		case 'd': new_x++; result.direction = "вправо →"; break;
		default:
			result.error_message = "неверная команда!";
			return result;
	}
	
	if (!field.isValidPosition(new_x, new_y)) {
		result.error_message = "вы не можете выйти за пределы поля!";
		return result;
	}
	
	int new_index = field.coordToIndex(new_x, new_y);
	
	if (!field.getCell(new_index).isPassable()) {
		result.error_message = "эта клетка непроходима (стена)!";
		return result;
	}
	
	if (entity_manager.hasEntity(new_index)) {
		return result;  
	}
	
	entity_manager.moveEntity(player_index, new_index);
	player_index = new_index;
	
	result.new_x = new_x;
	result.new_y = new_y;
	result.success = true;
	result.stepped_on_slow_cell = field.getCell(new_index).isSlowing();
	
	if (result.stepped_on_slow_cell) {
		auto* entity = entity_manager.getEntity(player_index);
		if (entity) {
			auto* slowable = dynamic_cast<ISlowable*>(entity);
			if (slowable) {
				slowable->setSlow(true);
			}
		}
	}
	
	result.trap_triggered = false;
	
	return result;
}

AttackResult PlayerTurnHandler::processPlayerAttack(int target_index) {
	int& player_index = player_index_ref.get();
	AttackResult result;
	result.success = false;
	
	field.indexToCoord(target_index, result.target_x, result.target_y);
	
	auto target_type = entity_manager.getEntityType(target_index);
	if (!target_type.has_value()) return result;
	
	auto* player_entity = entity_manager.getEntity(player_index);
	if (!player_entity) return result;
	auto* player_char = dynamic_cast<IPlayerCharacter*>(player_entity);
	if (!player_char) return result;
	
	result.damage = player_char->getDamage();
	result.is_melee = player_char->melee();
	result.success = true;
	
	if (target_type == EntityType::ENEMY) {
		result.target_type_name = "ВРАГА";
	} else if (target_type == EntityType::BARRACKS) {
		result.target_type_name = "ВРАЖЕСКОГО ЗДАНИЯ (Казарма)";
	} else if (target_type == EntityType::TOWER) {
		result.target_type_name = "ВРАЖЕСКОЙ БАШНИ";
	}
	
	auto target_health = entity_manager.getEntityHealth(target_index);
	if (target_health.has_value()) {
		auto [hp_before, max_hp] = target_health.value();
		result.target_hp_before = hp_before;
		result.target_max_hp = max_hp;
		
		entity_manager.causeDamageToEntity(target_index, result.damage);
		
		auto [hp_after, _] = entity_manager.getEntityHealth(target_index).value_or(std::make_pair(0, 0));
		result.target_hp_after = hp_after;
		result.target_killed = !entity_manager.isEntityAlive(target_index);
		
		if (result.target_killed) {
			score_system.addScoreForKill(target_type.value());
			result.score_gained = (target_type == EntityType::ENEMY) ? 10 : 
			                      (target_type == EntityType::BARRACKS) ? 50 : 30;
			
			entity_manager.removeEntity(target_index);
			
			PlayerResources* resources = player_char->getResourcesPtr();
			if (resources && target_type == EntityType::ENEMY) {
				resources->addPoints(GameConstants::POINTS_FOR_ENEMY_KILL);
				resources->incrementKills();
				result.resource_points_gained = GameConstants::POINTS_FOR_ENEMY_KILL;
				result.total_kills = resources->getKills();
			} else if (resources) {
				resources->addPoints(GameConstants::POINTS_FOR_BUILDING_KILL);
				result.resource_points_gained = GameConstants::POINTS_FOR_BUILDING_KILL;
			}
		}
	}
	
	return result;
}

SpellCastResult PlayerTurnHandler::processSpellCasting() {
	int& player_index = player_index_ref.get();
	SpellCastResult result;
	result.success = false;
	result.cancelled = false;
	
	auto* entity = entity_manager.getEntity(player_index);
	if (!entity) return result;
	auto* player_char = dynamic_cast<IPlayerCharacter*>(entity);
	if (!player_char) return result;
	
	SpellHand* spell_hand_ptr = player_char->getSpellHandPtr();
	SpellEnhancer* spell_enhancer_ptr = player_char->getSpellEnhancerPtr();
	if (!spell_hand_ptr || !spell_enhancer_ptr) return result;
	
	SpellHand& spell_hand = *spell_hand_ptr;
	SpellEnhancer& spell_enhancer = *spell_enhancer_ptr;
	
	if (spell_hand.isEmpty()) {
		result.error_message = "У вас нет заклинаний в руке!";
		return result;
	}
	
	std::cout << "\nВыберите заклинание (0-" << (spell_hand.getCurrentSize() - 1) 
	          << ") или 'q' для отмены: ";
	char choice;
	if (!(std::cin >> choice)) {
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		result.error_message = "Ошибка ввода!";
		return result;
	}
	
	if (choice == 'q') {
		result.cancelled = true;
		return result;
	}
	
	size_t spell_index = static_cast<size_t>(choice - '0');
	
	if (spell_index >= spell_hand.getCurrentSize()) {
		result.error_message = "Неверный индекс заклинания!";
		return result;
	}
	
	std::cout << "Введите координаты цели (x y): ";
	int target_x, target_y;
	if (!(std::cin >> target_x >> target_y)) {
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		result.error_message = "Ошибка ввода координат!";
		return result;
	}
	
	if (!field.isValidPosition(target_x, target_y)) {
		result.error_message = "Неверные координаты!";
		return result;
	}
	
	SpellContext context(field, entity_manager, spell_enhancer, player_index, target_x, target_y);
	std::unique_ptr<ISpell> spell = spell_hand.useSpell(spell_index);
	
	if (spell) {
		result.success = spell->cast(context);
	}
	
	return result;
}

bool PlayerTurnHandler::isPlayerAlive() const {
	int player_index = player_index_ref.get();
	if (player_index < 0) return false;
	
	return entity_manager.isEntityAlive(player_index);
}







