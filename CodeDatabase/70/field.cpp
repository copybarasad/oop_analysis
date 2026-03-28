#include "field.h"
Field::Field() : height(10), width(10)
{
	fieldInit();
}

Field::Field(int height, int width) : height(height), width(width)
{
	if (height>25 or height<10 or width>25 or width<10)
	{
		throw std::invalid_argument("Size of field must be 10x10 - 25x25.");
	}
	fieldInit();
}

void Field::fieldInit()
{
	field = new Cell*[width];
	for(int i = 0; i < width; ++i)
	{
		field[i] = new Cell[height];
	}
}

Field::~Field()
{
	clearField();
}

void Field::clearField()
{
	if (field != nullptr) {
        for(int i = 0; i < width; ++i) {
            delete[] field[i];
        }
        delete[] field;
        field = nullptr;
    }
}

Field::Field(const Field& other) : height(other.height), width(other.width)
{
	clearField();
    field = new Cell*[other.width];
    for(int i = 0; i < width; ++i)
	{
		field[i] = new Cell[other.height];
		memcpy(field[i], other.field[i], sizeof(Cell)*other.height);
	}
}
Field& Field::operator=(const Field& other) 
{
    if (this != &other) 
    {
    	clearField();
        height = other.height;
        width = other.width;
        field = new Cell*[other.width];
        for(int i = 0; i < width; ++i)
		{
			field[i] = new Cell[other.height];
			memcpy(field[i], other.field[i], sizeof(Cell)*other.height);
		}
    }
    return *this;
}

Field::Field(Field&& other) noexcept : height(other.height), width(other.width)
{
	field = other.field;
    other.height = 0;
    other.width = 0;
}

Field& Field::operator=(Field&& other) noexcept 
{
	if (this != &other) {
	    clearField();

	    height = other.height;
	    width = other.width;
	    field = other.field;
	    other.field = nullptr;
	    other.height = 0;
	    other.width = 0;
	}
	return *this;
}

int Field::getHeight() const
{
	return height;
}

int Field::getWidth() const
{
	return width;
}

Cell** Field::getField() const
{
	return field;
}

void Field::setUnpassableCell(int x, int y)
{
	if (x<0 or x>(width-1) or y<0 or y>(height-1))
	{
		throw std::invalid_argument("Coordinates must be non-negative and less than sizes of field.");
	}

	field[x][y].setPassable(false);
}

bool Field::isCellPassable(int x, int y) const
{
	return field[x][y].getPassable();
}
//TRAP
void Field::setTrappedCell(int x, int y, int damage)
{
	field[x][y].setTrapped(true);
	field[x][y].setDamage(damage);
}

bool Field::isCellTrapped(int x, int y) const
{
	return field[x][y].getTrapped();
}

int Field::useTrap(int x, int y)
{
	int damage = field[x][y].getDamage();
	field[x][y].setDamage(0);
	field[x][y].setTrapped(false);
	return damage;
}

void Field::setField(Cell** field, int width, int height)
{
	clearField();
	this->field = field;
	this->width = width;
	this->height = height;
}

std::ostream& operator << (std::ostream &os, const Cell &cell);
std::istream& operator >> (std::istream& in, Cell& cell);

std::ostream& operator << (std::ostream &os, const Field &field)
{
	os << field.getWidth() << " " << field.getHeight() << " ";
	for(size_t i = 0; i < field.getWidth(); ++i)
	{
		for(size_t j = 0; j < field.getHeight(); ++j)
		{
			os << field.getField()[i][j] << " ";
		}
	}
	return os;
}

std::istream& operator >> (std::istream& in, Field& field)
{
	Cell** newField;
	int width;
	int height;
    in >> width >> height;
    if (!in.good()) throw std::runtime_error("Failed to read field sizes");
    if(width <= 0 or width > 25 or height <= 0 or height > 25)
    {
    	throw std::invalid_argument("Wrong arguments for Field!");
    }
    newField = new Cell*[width];
    for(size_t i = 0; i < width; ++i)
	{
		newField[i] = new Cell[height];
		for(size_t j = 0; j < height; ++j)
		{
			in >> newField[i][j];
			if (!in.good()) throw std::runtime_error("Failed to read field sizes");
		}
	}
	field.setField(newField, width, height);
    return in;
}
