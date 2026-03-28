#include "Field.h"
#include <ctime>


void Field::Cell::setSlow(bool value)
{
	slow = value;
	empty = !slow && obstruction;
}


void Field::Cell::setObstruction(bool value)
{
	obstruction = value;
	empty = !slow && obstruction;
}


bool Field::Cell::isEmpty() const
{
	return !slow && obstruction;
}


bool Field::Cell::isSlow() const
{
	return slow;
}


bool Field::Cell::isObstruction() const
{
	return obstruction;
}


Field::Field(int width, int height, int cellSize)
{
	if (width > 25)
		width = 25;

	if (width < 10)
		width = 10;

	if (height > 25)
		height = 25;

	if (height < 10)
		height = 10;

	this->width = width;
	this->height = height;
	this->cellSize = cellSize;

	field.resize(height);
	for (int i = 0; i < height; ++i)
	{
		field[i].reserve(width);
		for (int j = 0; j < width; ++j)
			field[i].emplace_back();
	}
}


Field::Field(const Field& other) : width(other.width), height(other.height), cellSize(other.cellSize), field(other.field) {}


Field::Field(Field&& other) noexcept
{
	std::swap(width, other.width);
	std::swap(height, other.height);
	std::swap(cellSize, other.cellSize);
	std::swap(field, other.field);
}


void Field::to_json(nlohmann::json& js) const
{
	js["width"] = width;
	js["height"] = height;
	js["cellSize"] = cellSize;

	js["cells"] = nlohmann::json::array();

	for (const auto& row : field) {
		nlohmann::json jRow = nlohmann::json::array();
		for (const auto& cell : row) {
			jRow.push_back({
				{"slow", cell.isSlow()},
				{"obstruction", cell.isObstruction()}
				});
		}
		js["cells"].push_back(jRow);
	}
}


void Field::from_json(const nlohmann::json& js)
{
	if (js.contains("width"))
		width = js["width"];
	else
		throw std::runtime_error(u8"Ошибка считывания width");

	if (js.contains("height"))
		height = js["height"];
	else
		throw std::runtime_error(u8"Ошибка считывания height");

	if (js.contains("cellSize"))
		cellSize = js["cellSize"];
	else
		throw std::runtime_error(u8"Ошибка считывания cellSize");

	// Проверка наличия ячеек
	if (!js.contains("cells") || !js["cells"].is_array()) {
		throw std::runtime_error(u8"Отсутствует поле cells, либо оно некорректно");
	}

	// Пересоздаём поле нужного размера
	field.clear();
	field.resize(height, std::vector<Cell>(width));

	const auto& jCells = js["cells"];

	// Загружаем построчно
	for (int y = 0; y < height && y < (int)jCells.size(); ++y)
	{
		const auto& jRow = jCells[y];
		if (!jRow.is_array())
			continue;

		for (int x = 0; x < width && x < (int)jRow.size(); ++x)
		{
			const auto& jCell = jRow[x];

			if (!jCell.is_object())
				throw std::runtime_error(u8"Неверный формат cell");

			// Устанавливаем свойства
			if (jCell.contains("slow"))
				field[y][x].setSlow(jCell["slow"]);
			else
				throw std::runtime_error(u8"Ошибка считывания slow для cell");

			if (jCell.contains("obstruction"))
				field[y][x].setObstruction(jCell["obstruction"]);
			else
				throw std::runtime_error(u8"Ошибка считывания obstruction для cell");
		}
	}
}


Field& Field::operator=(const Field& other)
{
	if (this != &other)
	{
		width = other.width;
		height = other.height;
		cellSize = other.cellSize;
		field = other.field;
	}
	return *this;
}


Field& Field::operator=(Field&& other) noexcept
{
	if (this != &other)
	{
		field = {};
		std::swap(field, other.field);
	}
	return *this;
}


int Field::getWidth(bool realSize) const
{
	return realSize ? width * cellSize : width;
}


int Field::getHeight(bool realSize) const
{
	return realSize ? height * cellSize : height;
}


const Field::Cell& Field::getCell(const Point& point) const
{
	return field[point.Y()][point.X()];
}


int Field::getCellSize() const
{
	return cellSize;
}


const std::vector<std::vector<Field::Cell>>& Field::getField() const
{
	return field;
}


void Field::changeObstruction(const Point& point)
{
	auto& cell = field[point.Y()][point.X()];
	cell.setObstruction(!cell.isObstruction());
}


std::set<Point> Field::generatePositions(const Point& center, int  n, int radius)
{
	std::set<Point> points;
	srand((unsigned int)time(0));

	int x_start, y_start, x_end, y_end;

	int count = 0;
	size_t lastSize = 0;

	while (points.size() < n)
	{
		if (count > (n  < 10 ? 20 : n * 3))
			break;

		Point point;

		x_start = center.X() - radius > 0 ? center.X() - radius : 0;
		x_end = center.X() + radius >= width ? width - 1 : center.X() + radius;
		y_start = center.Y() - radius > 0 ? center.Y() - radius : 0;
		y_end = center.Y() + radius >= height ? height - 1: center.Y() + radius;

		point.setX(rand() % (x_end - x_start + 1) + x_start);
		point.setY(rand() % (y_end - y_start + 1) + y_start);
		if (center != point and field[point.Y()][point.X()].isEmpty()) 
		{
			lastSize = points.size();
			points.insert(point);
		}

		if (lastSize != points.size())
			count = 0;

		++count;
	}

	return points;
}


std::set<Point> Field::generatePositions(int n, bool escapeNotEmpty)
{
	std::set<Point> points;
	srand((unsigned int)time(0));

	if (n == -1)
		n = rand() % 30 + (int)(width * height * 0.1);
	else
		n = (float)n / (width * height) <= 0.3 ? n : (int)(width * height * 0.3);

	while (points.size() < n)
	{
		Point point;

		point.setX(rand() % width);
		point.setY(rand() % height);
		if (escapeNotEmpty)
		{
			int tries = 0;
			while (!field[point.Y()][point.X()].isEmpty())
			{
				if (tries > 100)
					break;
				point.setX(rand() % width);
				point.setY(rand() % height);
				++tries;
			}
		}

		points.insert(point);
	}

	return points;
}

void Field::generateSlowCells(int n)
{
	auto positions = generatePositions(n, true);
	for (const auto& point : positions)
		field[point.Y()][point.X()].setSlow(true);
}


void Field::generateObstructionCells(int n)
{
	auto positions = generatePositions(n);
	for (const auto& point: positions)
		field[point.Y()][point.X()].setObstruction(false);
}