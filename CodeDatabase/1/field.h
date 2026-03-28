#pragma once
#include "vampire_tower.h"
#include <vector>
#include "position.h"
#include "cell_type.h"
#include "library.h"

class Character;

class Field {
private:
    int width_;
    int height_;
    std::vector<std::vector<CellType>> grid_;

    std::vector<std::vector<int>> trapDamage_;
    std::vector<VampireTower> towers_;
    std::vector<Library> libraries_; 

    void validateDimensions(int width, int height) const;
    CellType getCell(const Position& position) const;

public:
    Field(int width, int height);
    
    // Конструктор копирования
    Field(const Field& other);
    
    // Конструктор перемещения
    Field(Field&& other) noexcept;
    
    // Оператор присваивания с копированием
    Field& operator=(const Field& other);
    
    // Оператор присваивания с перемещением
    Field& operator=(Field&& other) noexcept;
    
    // Деструктор
    ~Field() = default;    
    
    int getWidth() const;
    int getHeight() const;
    bool isValidPosition(const Position& position) const;
    bool isPassable(const Position& position) const;
    CellType getCellType(const Position& position) const;


    void placeWall(const Position& position);
    void placeChapel(const Position& position);
    void placeVampireLair(const Position& position);
    void placeArmory(const Position& position);

    void applyCellEffect(Character& character);  

    bool placeTrap(const Position& position, int damage);
    bool removeTrap(const Position& position);
    bool hasTrap(const Position& position) const;
    int triggerTrap(const Position& position);

    void addTower(const VampireTower& tower);
    const std::vector<VampireTower>& getTowers() const;
    bool hasTowerAt(const Position& pos) const;
    
    
    void addLibrary(const Library& library);
    const std::vector<Library>& getLibraries() const;
    const Library* getLibraryAt(const Position& pos) const;
    Library* getLibraryAt(const Position& pos); 

    void setCell(const Position& position, CellType cell_type);
    
    //lb3
    // НОВЫЙ СЕТТЕР для GameSaver
    void setCellType(const Position& position, CellType cell_type) {
        setCell(position, cell_type);
    }
    // НОВЫЕ МЕТОДЫ ДЛЯ СИСТЕМЫ УРОВНЕЙ
    void generateTowers(int count);
    void clearTowers() { towers_.clear(); }
    void clearLibraries() { libraries_.clear(); }
    void placeDraculaTomb(const Position& position);
};