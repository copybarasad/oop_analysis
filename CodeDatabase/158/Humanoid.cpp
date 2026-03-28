#include "Humanoid.h"

Humanoid::Humanoid(bool IsPlayer, bool IsAlive, ATKtype type, std::string Body, int HP, int ATK, int X_coord, int Y_coord, int Speed) 
{
    this->IsPlayer=IsPlayer;
    this->IsAlive=true;
    this->type=ATKtype::melee;
    this->Body=Body;
    this->HP=HP;
    this->maxHP=HP;
    this->ATK=ATK;
    this->Speed=Speed;
    this->XY_coord = Coordinates(X_coord, Y_coord);
    this->where = Direction::KNOWHERE;
}

int Humanoid::getX_coord() { return XY_coord.getX_coord(); }
int Humanoid::getY_coord() { return XY_coord.getY_coord(); }
void Humanoid::setXY_coord(int new_X_coord, int new_Y_coord) { XY_coord.setXY_coord(new_X_coord, new_Y_coord); }
void Humanoid::take_damage(int damage) { this->HP = HP - damage; if(HP<=0) IsAlive = false; }
int Humanoid::getSpeed() const { return Speed; }
void Humanoid::setSpeed(int newSpeed) { this->Speed = newSpeed; }
bool Humanoid::checkPulse() const { return IsAlive; }
int Humanoid::getHP() const { return HP; }
void Humanoid::setHP(int newHP) { this->HP = newHP; if(this->HP > maxHP) this->HP = maxHP; }
int Humanoid::getMaxHP() const { return maxHP; }
void Humanoid::setMaxHP(int newMaxHP) { maxHP = newMaxHP; }
void Humanoid::healFull() { HP = maxHP; }
int Humanoid::getAttackDamage() const { return ATK; }
Coordinates Humanoid::getCoordinates() const { return XY_coord; }

std::string Humanoid::serialize() const 
{
    std::stringstream ss;
    ss << IsPlayer << " " << IsAlive << " " << static_cast<int>(type) << " "
       << Body << " " << HP << " " << maxHP << " " << ATK << " " << Speed << " "
       << XY_coord.getX_coord() << " " << XY_coord.getY_coord() << " "
       << SpeedCD << " " << ATKCD;
    return ss.str();
}

void Humanoid::deserialize(const std::string& data) 
{
    std::stringstream ss(data);
    int typeInt;
    ss >> IsPlayer >> IsAlive >> typeInt >> Body >> HP >> maxHP >> ATK >> Speed;
    int x, y;
    ss >> x >> y >> SpeedCD >> ATKCD;
    XY_coord.setXY_coord(x, y);
    type = static_cast<ATKtype>(typeInt);
}