#include "../include/Tower.h"



//constructors and destructors

TeslaTower::TeslaTower(int hp, int dmg, int x, int y, int range){
    this->hp = hp;
    this->setDamage(dmg);
    this->x_pos = x;
    this->y_pos = y;
    this->range = range;

    if (!this->texture.loadFromFile("../sprites/tesla_tower.png")){
        std::cerr<<"file loading error";
    }
    float scaleX = static_cast<float>(CELL_SIZE) / this->texture.getSize().x;
    float scaleY = static_cast<float>(CELL_SIZE) / this->texture.getSize().y;
    
    this->sprite.setTexture(texture);
    this->sprite.setScale(scaleX,scaleY);
    
    syncSprite();
}


TeslaTower::TeslaTower(const TeslaTower& other)
    : Enemy(other.hp, other.getDamage(), other.x_pos, other.y_pos),
      range(other.range)
{
    if (!this->texture.loadFromFile("../sprites/tesla_tower.png")) {
        std::cerr << "file loading error in copy constructor\n";
    }
    this->sprite.setTexture(this->texture);
    float scaleX = static_cast<float>(CELL_SIZE) / this->texture.getSize().x;
    float scaleY = static_cast<float>(CELL_SIZE) / this->texture.getSize().y;
    this->sprite.setScale(scaleX, scaleY);

    syncSprite();
}


TeslaTower& TeslaTower::operator=(const TeslaTower& other)
{
    if (this == &other)
        return *this;

    this->hp = other.hp;
    this->setDamage(other.getDamage());
    this->x_pos = other.x_pos;
    this->y_pos = other.y_pos;
    this->range = other.range;

    if (!this->texture.loadFromFile("../sprites/tesla_tower.png")) {
        std::cerr << "file loading error in assignment operator\n";
    }

    this->sprite.setTexture(this->texture);

    float scaleX = static_cast<float>(CELL_SIZE) / this->texture.getSize().x;
    float scaleY = static_cast<float>(CELL_SIZE) / this->texture.getSize().y;

    this->sprite.setScale(scaleX, scaleY);

    syncSprite();

    return *this;
}


//functions

void TeslaTower::move(const std::string& direction, Gamefield* gf) {
    std::cout << "TeslaTower at (" << x_pos << ", " << y_pos << ") does not move.\n";
}



void TeslaTower::attack(Gamefield* gf)
{
    attackCooldown++;

    if (attackCooldown < 2)
        return;

    attackCooldown = 0; 

    int tx = getX();
    int ty = getY();

    std::unordered_set<void*> alreadyHit;

    std::cout << "[TOWER] Tower at (" << tx << "," << ty << ") fires. damage=" << this->getDamage() << "\n";

    attackLine(tx, ty,  0, -1, gf, alreadyHit);
    attackLine(tx, ty,  0,  1, gf, alreadyHit);
    attackLine(tx, ty, -1, 0, gf, alreadyHit); 
    attackLine(tx, ty,  1, 0, gf, alreadyHit); 
}

void TeslaTower::attackLine(int x, int y, int dx, int dy, Gamefield* gf, std::unordered_set<void*>& alreadyHit){
    int cx = x;
    int cy = y;

    for (int i = 1; i <= range; i++) {
        cx += dx;
        cy += dy;

        Cell* c = gf->getCell(cx, cy);
        if (!c) {
            std::cout << "[TOWER] beam stopped at ("<<cx<<","<<cy<<") - out of bounds\n";
            break;
        }
        if (!c->is_passable()) {
            std::cout << "[TOWER] beam stopped at ("<<cx<<","<<cy<<") - blocked\n";
            break;
        }

        if (c->is_occupied()) {
            Entity* ent = c->is_occupied_by();
            std::cout << "[TOWER] beam hits occupied cell ("<<cx<<","<<cy<<") ent="<<ent<<"\n";

            if (!ent) {
                std::cout << "[TOWER] WARNING: occupied_by == nullptr, clearing flag\n";
                c->set_occupied(false);
                continue;
            }

            if (alreadyHit.find((void*)ent) != alreadyHit.end()) {
                std::cout << "[TOWER] already hit this entity, skipping\n";
                continue;
            }

            alreadyHit.insert((void*)ent);

            int dmg = this->getDamage();
            std::cout << "[TOWER] dealing " << dmg << " damage to entity at ("<<cx<<","<<cy<<")\n";
            ent->get_damaged(dmg);

            std::cout << "[TOWER] entity HP now " << ent->getHP() << "\n";

            if (!ent->isAlive()) {
                c->set_occupied(false);
                c->set_occupied_by(nullptr);
                std::cout << "[TOWER] entity died at ("<<cx<<","<<cy<<"), cell cleared\n";
            }
        }
    }
}





void TeslaTower::cast_spell( Gamefield *gf){
    Lightning_bolt spell;
    for (const std::string& direction : {"up", "down", "left", "right"}){
        spell.cast(direction, gf, x_pos, y_pos);
    }
    
}