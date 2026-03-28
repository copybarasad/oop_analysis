#include "field.h"
#include "character.h"
#include <stdexcept>
#include <algorithm>
#include <utility>
#include <cstdlib>
#include <ctime>

void Field::validateDimensions(int width, int height) const {
    if (width < 10 || width > 25 || height < 10 || height > 25) {
        throw std::invalid_argument("Field size must be between 10 and 25");
    }
}

void Field::setCell(const Position& position, CellType cell_type) {
    if (isValidPosition(position)) {
        grid_[position.getY()][position.getX()] = cell_type;
    }
}

CellType Field::getCell(const Position& position) const {
    if (!isValidPosition(position)) {
        throw std::out_of_range("Position is outside field bounds");
    }
    return grid_[position.getY()][position.getX()];
}

Field::Field(int width, int height) : width_(width), height_(height) {
    validateDimensions(width, height);
    grid_.resize(height_, std::vector<CellType>(width_, CellType::EMPTY));
    trapDamage_.resize(height_, std::vector<int>(width_, 0));
    
    // Внешние стены
    for (int x = 0; x < width_; ++x) {
        setCell(Position(x, 0), CellType::WALL);
        setCell(Position(x, height_ - 1), CellType::WALL);
    }
    for (int y = 0; y < height_; ++y) {
        setCell(Position(0, y), CellType::WALL);
        setCell(Position(width_ - 1, y), CellType::WALL);
    }

    // Внутренние стены - лабиринт
    for (int x = 3; x < width_ - 3; x += 4) {
        for (int y = 2; y < height_ - 2; y += 2) {
            if (y % 4 != 0) {
                setCell(Position(x, y), CellType::WALL);
            }
        }
    }
    
    for (int y = 4; y < height_ - 4; y += 4) {
        for (int x = 2; x < width_ - 2; x += 2) {
            if (x % 4 != 0) {
                setCell(Position(x, y), CellType::WALL);
            }
        }
    }
    
    // Специальные клетки
    setCell(Position(width_/2, height_/2), CellType::ARMORY);                      
    setCell(Position(width_-3, 2), CellType::CHAPEL);                      
    setCell(Position(width_-3, height_-3), CellType::VAMPIRE_LAIR);  
    
    
    // Библиотеки - только как объекты (не как клетки в grid_)
    addLibrary(Library(Position(4, 4), SpellType::LIGHT_BEAM));
    addLibrary(Library(Position(2, height_-3), SpellType::SACRED_GLOW));
    addLibrary(Library(Position(width_/2, 2), SpellType::HOLY_WATER));
}



// Конструктор копирования
Field::Field(const Field& other)
    : width_(other.width_), height_(other.height_),
      grid_(other.grid_),
      trapDamage_(other.trapDamage_),
      towers_(other.towers_),
      libraries_(other.libraries_)
{}

// Конструктор перемещения
Field::Field(Field&& other) noexcept
    : width_(other.width_), height_(other.height_), 
      grid_(std::move(other.grid_)),
      trapDamage_(std::move(other.trapDamage_)),
      towers_(std::move(other.towers_)),
      libraries_(std::move(other.libraries_))
{
    other.width_ = 0;
    other.height_ = 0;
}

// Оператор присваивания с копированием
Field& Field::operator=(const Field& other) {
    if (this != &other) {
        width_ = other.width_;
        height_ = other.height_;
        grid_ = other.grid_;
        trapDamage_ = other.trapDamage_;
        towers_ = other.towers_;
        libraries_ = other.libraries_;
    }
    return *this;
}

// Оператор присваивания с перемещением
Field& Field::operator=(Field&& other) noexcept {
    if (this != &other) {
        width_ = other.width_;
        height_ = other.height_;
        grid_ = std::move(other.grid_);
        trapDamage_ = std::move(other.trapDamage_);
        towers_ = std::move(other.towers_);
        libraries_ = std::move(other.libraries_);
        
        other.width_ = 0;
        other.height_ = 0;
    }
    return *this;
}

int Field::getWidth() const { return width_; }

int Field::getHeight() const { return height_; }

bool Field::isValidPosition(const Position& position) const {
    return position.getX() >= 0 && position.getX() < width_ &&
           position.getY() >= 0 && position.getY() < height_;
}

bool Field::isPassable(const Position& position) const {
    if (!isValidPosition(position)) return false;
    
    if (hasTowerAt(position)) {
        return false;
    }
    
    return ::isPassable(getCell(position));
}

CellType Field::getCellType(const Position& position) const {
    return getCell(position);
}

void Field::placeWall(const Position& position) { 
    setCell(position, CellType::WALL); 
}

void Field::placeChapel(const Position& position) { 
    setCell(position, CellType::CHAPEL); 
}

void Field::placeVampireLair(const Position& position) { 
    setCell(position, CellType::VAMPIRE_LAIR); 
}

void Field::placeArmory(const Position& position) { 
    setCell(position, CellType::ARMORY); 
}

void Field::applyCellEffect(Character& character) {
    Position position = character.getPosition();
    
    if (!isValidPosition(position)) return;
       
    CellType cell_type = getCell(position);
    character.applyCellEffect(cell_type);  
}

bool Field::placeTrap(const Position& position, int damage) {
    if (!isValidPosition(position) || !isPassable(position) || damage <= 0) {
        return false;
    }
    
    if (hasTrap(position)) {
        return false;
    }
    
    trapDamage_[position.getY()][position.getX()] = damage;
    return true;
}

bool Field::removeTrap(const Position& position) {
    if (!isValidPosition(position)) return false;
    
    if (hasTrap(position)) {
        trapDamage_[position.getY()][position.getX()] = 0;
        return true;
    }
    return false;
}

bool Field::hasTrap(const Position& position) const {
    if (!isValidPosition(position)) return false;
    return trapDamage_[position.getY()][position.getX()] > 0;
}

int Field::triggerTrap(const Position& position) {
    if (!isValidPosition(position)) return 0;
    
    if (hasTrap(position)) {
        int damage = trapDamage_[position.getY()][position.getX()];
        trapDamage_[position.getY()][position.getX()] = 0;
        return damage;
    }
    return 0;
}

void Field::addTower(const VampireTower& tower) {
    towers_.push_back(tower);
}

const std::vector<VampireTower>& Field::getTowers() const {
    return towers_;
}

void Field::addLibrary(const Library& library) {
    libraries_.push_back(library);
}

const std::vector<Library>& Field::getLibraries() const {
    return libraries_;
}

const Library* Field::getLibraryAt(const Position& pos) const {
    for (const Library& library : libraries_) {
        if (library.getPosition() == pos) {
            return &library;
        }
    }
    return nullptr;
}

Library* Field::getLibraryAt(const Position& pos) {
    for (Library& library : libraries_) {
        if (library.getPosition() == pos) {
            return &library;
        }
    }
    return nullptr;
}

bool Field::hasTowerAt(const Position& pos) const {
    for (const VampireTower& tower : towers_) {
        if (tower.getPosition() == pos && tower.isAlive()) {
            return true;
        }
    }
    return false;
}

//  НОВЫЙ МЕТОД ДЛЯ СИСТЕМЫ УРОВНЕЙ
void Field::generateTowers(int count) {
    clearTowers();
    
    if (count >= 1) {
        addTower(VampireTower(Position(3, 3)));
    }
    if (count >= 2) {
        addTower(VampireTower(Position(width_-4, height_-4)));
    }
    if (count >= 3) {
        addTower(VampireTower(Position(width_/2, 3)));
    }
}

void Field::placeDraculaTomb(const Position& position) { 
    setCell(position, CellType::DRACULA_TOMB); 
}