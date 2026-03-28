#include "Field.h"

Field::Field() : Field(10, 10) {};

Field::Field(int width, int height, int count_impassable, int count_slowing)
    : width_((width >= 10 && width <= 25) ? width : 10),
      height_((height >= 10 && height <= 25) ? height : 10) {

    count_impassable = count_impassable + count_slowing > (width_ * height_) / 2 ? (width_ * height_) / 10  : count_impassable;

	count_slowing = count_impassable + count_slowing > (width_ * height_) / 2 ? (width_ * height_) / 10 : count_slowing;

    CreateField(count_slowing, count_impassable);
}

Field::Field(int width, int height) : Field(width, height, width*height, width*height) {}


Field::Field(const Field& other)
    : width_(other.width_), height_(other.height_), count_spawn_cells_(other.count_spawn_cells_) {
    field_.resize(height_);
    for (int i = 0; i < height_; ++i) {
        field_[i].resize(width_);
        for (int j = 0; j < width_; ++j) {
            field_[i][j] = other.field_[i][j]->cloneStructure();

            if (!other.field_[i][j]->IsEmpty()) {
                auto entity = other.field_[i][j]->GetEntity();
                if (entity) {
                    field_[i][j]->SpawnEntity(entity);
                }
            }
        }
    }
}

Field::Field(Field&& other)
    : width_(other.width_), height_(other.height_),
      count_spawn_cells_(other.count_spawn_cells_),
      field_(std::move(other.field_)) {
    other.width_ = 0;
    other.height_ = 0;
    other.count_spawn_cells_ = 0;
    other.field_.clear();
}

Field& Field::operator=(const Field& other) {
    if (this != &other) {
        width_ = other.width_;
        height_ = other.height_;
        count_spawn_cells_ = other.count_spawn_cells_;
        field_.clear();
        field_.resize(height_);
        for (int i = 0; i < height_; ++i) {
            field_[i].resize(width_);
            for (int j = 0; j < width_; ++j) {
                field_[i][j] = other.field_[i][j]->cloneStructure();

                if (!other.field_[i][j]->IsEmpty()) {
                    auto entity = other.field_[i][j]->GetEntity();
                    if (entity) {
                        field_[i][j]->SpawnEntity(entity);
                    }
                }
            }
        }
    }
    return *this;
}

Field& Field::operator=(Field&& other) {
    if (this != &other) {
        width_ = other.width_;
        height_ = other.height_;
        count_spawn_cells_ = other.count_spawn_cells_;
        field_ = std::move(other.field_);
        other.width_ = 0;
        other.height_ = 0;
        other.count_spawn_cells_ = 0;
        other.field_.clear();
    }
    return *this;
}

void Field::CreateField(int count_slowing, int count_impassable) {

    this->field_ = std::vector<std::vector<std::shared_ptr<Cell>>>(height_, std::vector<std::shared_ptr<Cell>>(width_));

    std::random_device rd;
    std::mt19937 gen(rd());

    int total_cells = width_ * height_;

    this->count_spawn_cells_ = total_cells - count_impassable;

    int count_cell = total_cells - count_impassable - count_slowing;

    std::uniform_int_distribution<> dist(0, count_cell + count_impassable + count_slowing - 1);

    for (int i = 0; i < height_; i++) {
        for (int j = 0; j < width_; j++) {

            if (i == 0 && j == 0)
                continue;

            int x = dist(gen);

            if (x < count_cell) {
                field_[i][j] = std::make_shared<Cell>();
                count_cell--;
            } else if (x < count_cell + count_impassable) {
                field_[i][j] = std::make_shared<ImpassableCell>();
                count_impassable--;
            } else {
                field_[i][j] = std::make_shared<SlowingCell>();
                count_slowing--;
            }

            if (count_cell + count_impassable + count_slowing > 0)
                dist = std::uniform_int_distribution<>(0, count_cell + count_impassable + count_slowing - 1);
        }
    }

    // Start Cell is always safety.
    field_[0][0] = std::make_shared<Cell>();
}

MoveResult Field::MoveEntity(MoveType move, std::shared_ptr<Entity> entity) {
    int x_curr, y_curr;
    if (!GetPosEntity(entity, x_curr, y_curr))
        return MoveResult::BlockedByWall;

    int x_new = x_curr, y_new = y_curr;
    switch (move) {
        case MoveType::Forward: y_new++; break;
        case MoveType::Back:    y_new--; break;
        case MoveType::Left:    x_new--; break;
        case MoveType::Right:   x_new++; break;
        default: return MoveResult::BlockedByWall;
    }

    if (!CorrectPosition(x_new, y_new))
        return MoveResult::BlockedByWall;

    MoveResult res = field_[y_new][x_new]->EnterEntity(entity);
    if (res == MoveResult::Moved) {
        field_[y_curr][x_curr]->RemoveEntity(entity);
    }
    return res;
}

bool Field::SetEntity(std::shared_ptr<Entity> entity, int x, int y) {
    if (!CorrectPosition(x, y))
        return false;

    if (!field_[y][x]->IsEmpty())
        return false;

    if (std::dynamic_pointer_cast<ImpassableCell>(field_[y][x]))
        return false;

    return field_[y][x]->SpawnEntity(entity);
}

int Field::GetCountSpawnCells(){
    return count_spawn_cells_;
}

void Field::GetSize(int& w, int& h){
    w = width_; h = height_;
}

bool Field::RemoteEntity(std::shared_ptr<Entity> entity){
    int x_curr, y_curr;
    if (!GetPosEntity(entity, x_curr, y_curr))
        return false;

    return field_[y_curr][x_curr]->RemoveEntity(entity);
}

bool Field::GetPosEntity(std::shared_ptr<Entity> entity, int& x, int& y){
    for (int i = 0; i < height_; i++){
        for (int j = 0; j < width_; j++){
            if (field_[i][j]->EqualEntites(entity)){
                x = j;
                y = i;
                return true;
            }
        }
    }
    return false;
}

bool Field::CorrectPosition(int x, int y) const{
    return (x < width_ && y < height_ && x >= 0 && y >= 0);
}

std::shared_ptr<Entity> Field::GetEntity(int x, int y) const {
	if (CorrectPosition(x, y))
		return field_[y][x]->GetEntity();

	return nullptr;
}

std::shared_ptr<Cell> Field::GetCell(int x, int y) {
	return field_[y][x];
}

std::string Field::GetCellType(int x, int y) const {
	return field_[y][x]->GetName();
}

int Field::getNotFriendlyCount() {
	int res = 0;

	for (int i = 0; i < height_; i++){
		for (int j = 0; j < width_; j++){
			if (!field_[i][j]->IsEmpty() && !field_[i][j]->GetEntity()->IsFriendly()){
				res++;
			}
		}
	}
	return res;
}

bool Field::ReplaceCellWithType(int x, int y, const std::string& type) {
	if (!CorrectPosition(x, y))
		return false;

	// создаём нужный объект клетки
	std::shared_ptr<Cell> newCell;
	if (type == "Cell") {
		newCell = std::make_shared<Cell>();
	} else if (type == "Slowing") {
		newCell = std::make_shared<SlowingCell>();
	} else if (type == "Impassable") {
		newCell = std::make_shared<ImpassableCell>();
	} else {
		newCell = std::make_shared<Cell>(); // fallback
	}

	// Заменяем структуру клетки (удаляется старая структура/ссылка)
	field_[y][x] = newCell;

	// Пересчитаем count_spawn_cells_ — простая и надёжная логика
	int spawn = 0;
	for (int i = 0; i < height_; ++i) {
		for (int j = 0; j < width_; ++j) {
			if (!std::dynamic_pointer_cast<ImpassableCell>(field_[i][j])) {
				spawn++;
			}
		}
	}
	count_spawn_cells_ = spawn;

	return true;
}
