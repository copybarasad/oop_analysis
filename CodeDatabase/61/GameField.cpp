#include "GameField.h"
#include <iostream>
#include <random>
#include <algorithm>
#include <iomanip>
#include <sstream>

GameField::GameField(int fieldWidth, int fieldHeight) 
    : width(fieldWidth), height(fieldHeight) {
    
    // Validate field size
    if (width < 10 || width > 25 || height < 10 || height > 25) {
        throw std::invalid_argument("Field size must be between 10x10 and 25x25");
    }
    
    initializeField();
    placeObstacles();
    placeSlowingCells();
}

GameField::GameField(const GameField& other) 
    : width(other.width), height(other.height), field(other.field) {}

GameField::GameField(GameField&& other) noexcept 
    : width(other.width), height(other.height), field(std::move(other.field)) {
    other.width = 0;
    other.height = 0;
}

GameField& GameField::operator=(const GameField& other) {
    if (this != &other) {
        width = other.width;
        height = other.height;
        field = other.field;
    }
    return *this;
}

GameField& GameField::operator=(GameField&& other) noexcept {
    if (this != &other) {
        width = other.width;
        height = other.height;
        field = std::move(other.field);
        other.width = 0;
        other.height = 0;
    }
    return *this;
}

void GameField::initializeField() {
    field.resize(height);
    for (int i = 0; i < height; ++i) {
        field[i].resize(width);
        for (int j = 0; j < width; ++j) {
            field[i][j] = Cell(CellType::EMPTY);
        }
    }
}

void GameField::placeObstacles() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> disWidth(0, width - 1);
    std::uniform_int_distribution<> disHeight(0, height - 1);
    
    // Place obstacles on approximately 15% of cells
    int obstacleCount = (width * height) / 7;
    
    for (int i = 0; i < obstacleCount; ++i) {
        int x = disWidth(gen);
        int y = disHeight(gen);
        field[y][x].setType(CellType::OBSTACLE);
    }
}

void GameField::placeSlowingCells() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> disWidth(0, width - 1);
    std::uniform_int_distribution<> disHeight(0, height - 1);
    
    // Place slowing cells on approximately 10% of cells
    int slowingCount = (width * height) / 10;
    
    for (int i = 0; i < slowingCount; ++i) {
        int x = disWidth(gen);
        int y = disHeight(gen);
        // Only place on empty cells
        if (field[y][x].getType() == CellType::EMPTY) {
            field[y][x].setType(CellType::SLOWING);
        }
    }
}

bool GameField::isValidPosition(int x, int y) const {
    return x >= 0 && x < width && y >= 0 && y < height;
}

int GameField::getWidth() const {
    return width;
}

int GameField::getHeight() const {
    return height;
}

Cell& GameField::getCell(int x, int y) {
    if (!isValidPosition(x, y)) {
        throw std::out_of_range("Position out of field bounds");
    }
    return field[y][x];
}

const Cell& GameField::getCell(int x, int y) const {
    if (!isValidPosition(x, y)) {
        throw std::out_of_range("Position out of field bounds");
    }
    return field[y][x];
}

bool GameField::canMoveTo(int x, int y) const {
    if (!isValidPosition(x, y)) {
        return false;
    }
    const Cell& cell = getCell(x, y);
    return cell.isPassable() && cell.isEmpty();
}

void GameField::setPlayerPosition(int x, int y) {
    if (isValidPosition(x, y)) {
        getCell(x, y).setPlayerPresence(true);
    }
}

void GameField::setEnemyPosition(int x, int y) {
    if (isValidPosition(x, y)) {
        getCell(x, y).setEnemyPresence(true);
    }
}

void GameField::setBuildingPosition(int x, int y) {
    if (isValidPosition(x, y)) {
        getCell(x, y).setBuildingPresence(true);
    }
}

void GameField::setAllyPosition(int x, int y) {
    if (isValidPosition(x, y)) {
        getCell(x, y).setAllyPresence(true);
    }
}

void GameField::setTrap(int x, int y, int damage) {
    if (isValidPosition(x, y)) {
        getCell(x, y).setTrap(true, damage);
    }
}

void GameField::clearPosition(int x, int y) {
    if (isValidPosition(x, y)) {
        getCell(x, y).clearOccupants();
    }
}

void GameField::displayField() const {
	// Calculate maximum coordinate width for proper alignment
	int maxCoordinate = std::max(width - 1, height - 1);
	int coordinateWidth = 1;
	if (maxCoordinate >= 10) {
		coordinateWidth = 2;
	}
	if (maxCoordinate >= 100) {
		coordinateWidth = 3;
	}
	
	// Print header with column numbers (X coordinates)
	std::cout << std::string(coordinateWidth + 1, ' ');
	for (int x = 0; x < width; ++x) {
		std::cout << std::setw(coordinateWidth) << x << " ";
	}
	std::cout << std::endl;
	
	// Print field with row numbers (Y coordinates)
	for (int y = 0; y < height; ++y) {
		// Row number on the left
		std::cout << std::setw(coordinateWidth) << y << " ";
		
		// Cell contents
		for (int x = 0; x < width; ++x) {
			const Cell& cell = getCell(x, y);
			
			char cellCharacter = '.';
			if (cell.hasPlayerOnCell()) {
				cellCharacter = 'P';
			} else if (cell.hasEnemyOnCell()) {
				cellCharacter = 'E';
			} else if (cell.hasBuildingOnCell()) {
				cellCharacter = 'B';
			} else if (cell.hasAllyOnCell()) {
				cellCharacter = 'A';
			} else if (cell.hasTrapOnCell()) {
				cellCharacter = 'T';
			} else {
				switch (cell.getType()) {
					case CellType::OBSTACLE:
						cellCharacter = '#';
						break;
					case CellType::SLOWING:
						cellCharacter = '~';
						break;
					case CellType::EMPTY:
					default:
						cellCharacter = '.';
						break;
				}
			}
			// Align cell characters to match coordinate width
			std::cout << std::setw(coordinateWidth) << cellCharacter << " ";
		}
		std::cout << std::endl;
	}
}

std::string GameField::serialize() const {
	std::ostringstream oss;
	oss << "[GameField]\n";
	oss << "width=" << width << "\n";
	oss << "height=" << height << "\n";
	oss << "[Cells]\n";
	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width; ++x) {
			oss << "cell_" << x << "_" << y << "=" << field[y][x].serialize() << "\n";
		}
	}
	return oss.str();
}

bool GameField::deserialize(const std::string& data) {
	if (data.empty()) {
		return false;
	}
	
	std::istringstream iss(data);
	std::string line;
	std::string currentSection;
	int readWidth = 0;
	int readHeight = 0;
	bool inCellsSection = false;
	bool foundGameFieldSection = false;
	
	// Сначала читаем размеры поля
	while (std::getline(iss, line)) {
		// Удаляем символ возврата каретки, если есть
		if (!line.empty() && line.back() == '\r') {
			line.pop_back();
		}
		
		if (line.empty()) continue;
		
		if (line[0] == '[' && line.back() == ']') {
			currentSection = line.substr(1, line.length() - 2);
			if (currentSection == "GameField") {
				foundGameFieldSection = true;
				continue;
			} else if (currentSection == "Cells") {
				break;
			}
			continue;
		}
		
		if (currentSection == "GameField" && foundGameFieldSection) {
			size_t pos = line.find('=');
			if (pos == std::string::npos) continue;
			
			std::string key = line.substr(0, pos);
			std::string value = line.substr(pos + 1);
			
			// Удаляем пробельные символы из конца значения
			while (!value.empty() && (value.back() == ' ' || value.back() == '\t' || value.back() == '\r')) {
				value.pop_back();
			}
			
			if (key == "width") {
				readWidth = std::stoi(value);
			} else if (key == "height") {
				readHeight = std::stoi(value);
			}
		}
	}
	
	// Инициализируем поле нужного размера
	if (readWidth > 0 && readHeight > 0) {
		width = readWidth;
		height = readHeight;
		// Инициализируем структуру поля - все клетки будут загружены из сохраненных данных
		field.resize(height);
		for (int i = 0; i < height; ++i) {
			field[i].resize(width);
			for (int j = 0; j < width; ++j) {
				field[i][j] = Cell(CellType::EMPTY);
			}
		}
	} else {
		return false;
	}
	
	// Теперь читаем клетки
	iss.clear();
	iss.str(data);
	currentSection = "";
	inCellsSection = false;
	
	while (std::getline(iss, line)) {
		// Удаляем символ возврата каретки, если есть
		if (!line.empty() && line.back() == '\r') {
			line.pop_back();
		}
		
		if (line.empty()) continue;
		
		if (line[0] == '[' && line.back() == ']') {
			currentSection = line.substr(1, line.length() - 2);
			inCellsSection = (currentSection == "Cells");
			continue;
		}
		
		if (inCellsSection) {
			size_t pos = line.find('=');
			if (pos == std::string::npos) continue;
			
			std::string key = line.substr(0, pos);
			std::string value = line.substr(pos + 1);
			
			// Удаляем символ новой строки из конца значения, если он есть
			if (!value.empty() && value.back() == '\n') {
				value.pop_back();
			}
			if (!value.empty() && value.back() == '\r') {
				value.pop_back();
			}
			
			if (key.find("cell_") == 0) {
				size_t xPos = key.find('_', 5);
				if (xPos != std::string::npos) {
					int x = std::stoi(key.substr(5, xPos - 5));
					int y = std::stoi(key.substr(xPos + 1));
					
					if (isValidPosition(x, y)) {
						// Десериализуем клетку - это восстановит тип клетки (OBSTACLE, SLOWING, EMPTY)
						// и все остальные данные (hasPlayer, hasEnemy, etc.)
						if (!field[y][x].deserialize(value)) {
							// Если десериализация не удалась, продолжаем с следующей клеткой
							// Тип клетки уже должен быть установлен, даже если остальные данные не загрузились
							continue;
						}
					}
				}
			}
		}
	}
	
	return true;
}

