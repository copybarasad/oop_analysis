#include "Map.h"

Map::Map(int x, int y):X_size(x),Y_size(y) 
{
    pos.resize(Y_size, std::vector<Cell>(X_size));
    for (int i = 0; i < Y_size; i++) 
    {
        for (int j = 0; j < X_size; j++) 
        {
            if (i == 0 || i == Y_size-1 || j == 0 || j == X_size-1) pos[i][j] = Cell(CellType::wall);
            else pos[i][j] = Cell(CellType::empty);
        }
    }
    pos[Y_size-2][X_size-2] = Cell(CellType::exit_cell);
    
    int wallCount = (x * y) / 10;
    for (int i = 0; i < wallCount; i++) 
    {
        int wallX = rand() % (X_size-2) + 1;
        int wallY = rand() % (Y_size-2) + 1;
        if (!(wallX == X_size-2 && wallY == Y_size-2) && !(wallX == 1 && wallY == 1)) 
        {
            pos[wallY][wallX] = Cell(CellType::wall);
        }
    }
    
    int swampCount = (x * y) / 20;
    for (int i = 0; i < swampCount; i++) 
    {
        int swampX = rand() % (X_size-2) + 1;
        int swampY = rand() % (Y_size-2) + 1;
        if (pos[swampY][swampX].CellGetType() == CellType::empty) 
        {
            pos[swampY][swampX] = Cell(CellType::swamp);
        }
    }
}

Map::Map(const Map& other) : X_size(other.X_size), Y_size(other.Y_size) 
{
    pos.resize(Y_size, std::vector<Cell>(X_size));
    for(int i = 0; i < Y_size; i++)
        for(int j = 0; j < X_size; j++)
            pos[i][j] = other.pos[i][j];
}

Map& Map::operator=(const Map& other) 
{
    if (this != &other) 
    {
        X_size = other.X_size;
        Y_size = other.Y_size;
        pos = other.pos;
    }
    return *this;
}

Map::Map(Map&& other) noexcept : X_size(other.X_size), Y_size(other.Y_size), pos(std::move(other.pos)) 
{
    other.X_size = 0;
    other.Y_size = 0;
}

Map& Map::operator=(Map&& other) noexcept 
{
    if (this != &other) 
    {
        X_size = other.X_size;
        Y_size = other.Y_size;
        pos = std::move(other.pos);
        other.X_size = 0;
        other.Y_size = 0;
    }
    return *this;
}

Cell Map::getCell(int x, int y) const { return pos[y][x]; }
Cell& Map::getCell(int x, int y) { return pos[y][x]; }
bool Map::iswithinlimits(int x, int y) { return x >= 0 && x < X_size && y >= 0 && y < Y_size; }

void Map::draw() 
{
    for (int i = 0; i < Y_size; i++) 
    {
        for (int j = 0; j < X_size; j++) 
        {
            pos[i][j].CellPlot();
        }
        std::cout << "\n";
    }
}

int Map::getXSize() const { return X_size; }
int Map::getYSize() const { return Y_size; }

std::string Map::serialize() const 
{
    std::stringstream ss;
    ss << X_size << " " << Y_size << "\n";
    for (int i = 0; i < Y_size; i++) 
    {
        for (int j = 0; j < X_size; j++) 
        {
            ss << static_cast<int>(pos[i][j].CellGetType()) << " ";
        }
        ss << "\n";
    }
    return ss.str();
}

void Map::deserialize(const std::string& data) 
{
    std::stringstream ss(data);
    ss >> X_size >> Y_size;
    pos.resize(Y_size, std::vector<Cell>(X_size));
    for (int i = 0; i < Y_size; i++) 
    {
        for (int j = 0; j < X_size; j++) 
        {
            int cellType;
            ss >> cellType;
            pos[i][j] = Cell(static_cast<CellType>(cellType));
        }
    }
}