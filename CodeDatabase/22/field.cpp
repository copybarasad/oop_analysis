#include "field/field.h"
#include <stddef.h> 
#include <iostream> 
#include <random>   
#include <string>   
#include <utility>

Field::Field(int height) : height_(height) { generateField(); }
Field::Field() : height_(10) { generateField(); }

Field &Field::copy(const Field &other)
{
    if (this == &other)
    {
        return *this;
    }

    height_ = other.height_;
    field_.clear();
    field_.resize(other.field_.size());
    for (size_t y = 0; y < other.field_.size(); ++y)
    {
        field_[y].reserve(other.field_[y].size());
        for (const auto &cell : other.field_[y])
        {
            field_[y].push_back(cell);
        }
    }

    return *this; 
}

Field::Field(const Field &other)
{
    copy(other);
}

Field &Field::operator=(const Field &other)
{
    return copy(other); 
}

Field &Field::move(Field &&other)
{
    if (this == &other)
    {
        return *this;
    }

    height_ = other.height_;
    field_ = std::move(other.field_);

    other.height_ = 0;
    other.field_.clear();

    return *this; 
}

Field::Field(Field &&other)  noexcept
{
    move(std::move(other)); 
}

Field &Field::operator=(Field &&other)  noexcept
{
    return move(std::move(other)); 
}


void Field::generateField()
{    std::random_device rd;
    std::mt19937 gen(rd());
    std::vector<double> weights = {1.0, 1.0, 5.0};
    std::discrete_distribution<> d(weights.begin(), weights.end());

    for (int y = 0; y < height_; y++)
    {
        std::vector<Cell> line;
        for (int x = 0; x < height_; x++)
        {
            if (x == 1 && y == 1){
                line.push_back(Cell(SPACE, NOOCUP));
            }
            else{
                Type opts = static_cast<Type>(d(gen));

                line.push_back(Cell(opts, NOOCUP));
            }
        }

        field_.push_back(line);
    }
}

void Field::addPlayer(Position position)
{
    auto pos = position;
    if (pos.x < 0 || pos.y < 0 || pos.y >= field_.size() || pos.x >= field_[pos.y].size())
    {
        std::cerr << "[ERROR] Player out of bounds: (" << pos.x << ", " << pos.y << ")\n";
        return;
    }
    field_[pos.y][pos.x].setOcupant(PLAYER);
}

void Field::addEnemy(Position position)
{
    auto pos = position;
    if (pos.x < 0 || pos.y < 0 || pos.y >= field_.size() || pos.x >= field_[pos.y].size())
    {
        std::cerr << "[ERROR] Enemy out of bounds: (" << pos.x << ", " << pos.y << ")\n";
        return;
    }
    field_[pos.y][pos.x].setOcupant(ENEMY);
}

void Field::addEnemyHut(Position position)
{
    auto pos = position;
    if (pos.x < 0 || pos.y < 0 || pos.y >= field_.size() || pos.x >= field_[pos.y].size())
    {
        std::cerr << "[ERROR] EnemyHut out of bounds: (" << pos.x << ", " << pos.y << ")\n";
        return;
    }
    field_[pos.y][pos.x].setOcupant(ENEMYHUT);
}

void Field::deliteCharacter(Position position)
{
    auto pos = position;
    field_[pos.y][pos.x].setOcupant(NOOCUP);
}


const std::vector<std::vector<Cell>> &Field::getField() const { return field_; }
int Field::getHeight() { return height_; }

bool Field::isFree(Position position)
{
    int x = position.x;
    int y = position.y;

    if (y < 0 || y >= height_)
        return false;
    if (x < 0 || x >= height_)
        return false;

    return field_[y][x].getOcupant() == NOOCUP &&
           field_[y][x].getType() != WALL;
}

bool Field::isSlow(Position position){
    int x = position.x;
    int y = position.y;

    if (y < 0 || y >= height_)
        return false;
    if (x < 0 || x >= height_)
        return false;

    return field_[y][x].getType() == SLOW;
}

Position Field::playerInZone(Position position, int radius){

    int dx = position.x;
    int dy = position.y;

    for(int x = dx-radius; x <= dx+radius; x++){
        for(int y = dy - radius; y <= dy+radius; y++){
            if (x < 0 || x >= height_ || y < 0 || y >= height_) continue;
            if(field_[y][x].getOcupant() == PLAYER){
                return {x,y};
            }
        }
    }

    return {-1,-1};
}


Position Field::enemyInRadius(Position position, int radius){
    int dx = position.x;
    int dy = position.y;

    for (int x = dx - radius; x < dx + radius; x++){
        for (int y = dy - radius; y < dy + radius; y++)
        {
            if (x < 0 || x >= height_ || y < 0 || y >= height_) continue;
            Ocupant oc = field_[y][x].getOcupant();
            if (oc == ENEMY)
            {
                return {x,y};
            }
        }
    }

    return {-1, -1};
}

std::vector<Position> Field::enHInRadius(Position position, int radius)
{
    int dx = position.x;
    int dy = position.y;

    std::vector<Position> result;

    for (int x = dx - radius; x < dx + radius; x++)
    {
        for (int y = dy - radius; y < dy + radius; y++)
        {
            if (x < 0 || x >= height_ || y < 0 || y >= height_)
                continue;
            Ocupant oc = field_[y][x].getOcupant();
            if (oc == ENEMY || oc == ENEMYHUT)
            {
                result.push_back({x,y});
            }
        }
    }

    return result;
}

Position Field::getPlayer(){

     for (int y = 0; y < field_.size(); y++)
    {
        for (int x = 0; x < field_.size(); x++)
        {
            if(field_[y][x].getOcupant() == PLAYER){
                return {x, y};
            }
        }
    } 
    return {-1, -1};
}

void Field::addTrap(Position position){
    field_[position.y][position.x].setType(TRAP);
}

void Field::deliteTrap(Position position){
    field_[position.y][position.x].setType(SPACE);
}

void Field::addTower(Position position)
{
    field_[position.y][position.x].setOcupant(TOWER);
}

void Field::addAlly(Position position)
{
    field_[position.y][position.x].setOcupant(ALLY);
}

bool Field::isTrap(Position position){
    return field_[position.y][position.x].getType() == TRAP;
}

void Field::buildNewField(std::vector<std::vector<int>> newField){
    height_ = newField[0].size();

    field_.clear();

    for (int y = 0; y < height_; y++){

        std::vector<Cell> line;
        for (int x = 0; x < height_; x++){

            Type opts = static_cast<Type>(newField[y][x]);

            line.push_back(Cell(opts, NOOCUP));
        }
        
        field_.push_back(line);
    }
}

void Field::markRegion(Position center, int radius, bool value)
{
    for (int y = center.y - radius; y <= center.y + radius; ++y)
    {
        for (int x = center.x - radius; x <= center.x + radius; ++x)
        {
            if (y < 0 || y >= height_ || x < 0 || x >= height_)
                continue;
            field_[y][x].setReadyToUse(value);
        }
    }
}

void Field::clearReady()
{
    for (auto &row : field_)
        for (auto &cell : row)
            cell.setReadyToUse(false);
}
