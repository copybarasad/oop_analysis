#include <SFML/Graphics.hpp>
#include "Hero.hpp"
#include <iostream>
#include "./Systems/GameLogger.hpp"
#include <sstream> 
#include "./Spells/SpellHand.hpp"

Hero::Hero(int startX, int startY, int offsetX, int offsetY, int level ) : Entity::Entity("./entity/JinVu.png",
      startX, startY, offsetX, offsetY, 75, 500, "Hero", friends, level) {
            spellHand = new SpellHand();
      } 
 
Hero::~Hero() {
    delete spellHand;
}

void Hero::Switching_weapons(InterfaceGameLogger& logger_consol) {
      if(weapon == melee){
            weapon = ranged; 
            std::cout << "Weapons have been changed from melee to ranged!" << std::endl;
            logger_consol.log("Weapons have been changed from melee to ranged!\n"); 
      } 
      else {
            weapon = melee;
            std::cout << "Weapons have been changed from ranged to melee!" << std::endl;
            logger_consol.log("Weapons have been changed from ranged to melee!\n");
      }  
}
void Hero::hero_attack_range(std::vector<std::vector<Cell*>>& cells, Vector_range_attack vector, InterfaceGameLogger& logger_consol) {
      std::ostringstream oss;
      

      if(weapon == melee) return;
      int hero_x = this->get_location().x;
      int hero_y = this->get_location().y;
      int width = int(cells.size());
      int height = int(cells[0].size());
      
      Entity* find_entity = nullptr;
      
      switch(vector){
            case forward:
                  for(int y = hero_y - 1; y >= hero_y - RADIUS_RANG_ATTACK; y--) {
                  if(y < 0) break;
                  if(cells[hero_x][y]->Cell_get_use()) {
                        find_entity = cells[hero_x][y]->Cell_get_entity();
                        break;
                  } 
                  }
                  break;
            case backward:
                  for(int y = hero_y + 1; y <= hero_y + RADIUS_RANG_ATTACK; y++) {
                  if(y >= height) break;
                  if(cells[hero_x][y]->Cell_get_use()) {
                        find_entity = cells[hero_x][y]->Cell_get_entity();
                        break;
                  }
                  }
                  break;
            case left:
                  for(int x = hero_x - 1; x >= hero_x - RADIUS_RANG_ATTACK; x--) {
                  if(x < 0) break;
                  if(cells[x][hero_y]->Cell_get_use()) {
                        find_entity = cells[x][hero_y]->Cell_get_entity();
                        break;
                  }
                  }
                  break;
            case right:
                  for(int x = hero_x + 1; x <= hero_x + RADIUS_RANG_ATTACK; x++) {
                  if(x >= width) break;
                  if(cells[x][hero_y]->Cell_get_use()) {
                        find_entity = cells[x][hero_y]->Cell_get_entity();
                        break;
                  }
                  }
                  break;
      }


      if(find_entity == nullptr) {
            std::cout << "You didn't hit anyone!" << std::endl;
            logger_consol.log("You didn't hit anyone!\n");
            return;
      }
 

      
      std::cout << "-----------------------------------------------------" << std::endl;
      logger_consol.log("-----------------------------------------------------\n");

      std::cout << "Hero"  << " shoots "  << find_entity->get_name() << " and deals " << DAMAGE_RANG_ATTACK << " damage." << std::endl;
      oss << "Hero"  << " shoots "  << find_entity->get_name() << " and deals " << DAMAGE_RANG_ATTACK << " damage." << std::endl;
      logger_consol.log(oss.str());
      oss.str("");

      std::cout << find_entity->get_name() << " has " << find_entity->get_helpth() - DAMAGE_RANG_ATTACK << " health remaining." << std::endl;
      oss << find_entity->get_name() << " has " << find_entity->get_helpth() - DAMAGE_RANG_ATTACK << " health remaining." << std::endl;
      logger_consol.log(oss.str());
      oss.str("");
      find_entity->change_helpth(DAMAGE_RANG_ATTACK, logger_consol, cells);


      std::cout << "-----------------------------------------------------" << std::endl;
      oss << "-----------------------------------------------------" << std::endl;
      logger_consol.log(oss.str());
      
}


void Hero::onEnemyKilled(InterfaceGameLogger& logger, int count_kill) {
      if(count_kill == 0) return;
      const int MAX_COUNT_SPELLS = 5;
      
      logger.log("Enemy defeated! Total kills: " + std::to_string(this->total_kill + count_kill) + "\n");

      for(int i = 1; i < count_kill + 1; i++) {
            this->total_kill += i;
            if (this->total_kill % 5 == 0 && spellHand->getCount() != MAX_COUNT_SPELLS) {
                  spellHand->addRandomSpell();
                  logger.log("You've unlocked a new spell!\n");
            }
      }
      
}
void Hero::drawSpellHand(sf::RenderWindow& window) {
      if (spellHand) {
            spellHand->draw(window);
      }
}

void Hero::UpgradeEnhancementSpellHero(int new_level_spells) {
      this->spellHand->UpgradeEnhancementSpell(new_level_spells);
}

int  Hero::getLevelEnhancementSpellHero() {return spellHand->getLevelEnhancementSpell(); }