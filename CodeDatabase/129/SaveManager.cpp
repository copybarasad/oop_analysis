#include "SaveManager.h"

/*
работаем с ,бинарным файлом .dat
представляем все данные как char* (так как это универсальный
указатель на байты) с помощью (char*)
*/

/*
"исключительные" ситуации
-не получилось открыть файл для сохранения
-ошибка во время записи данных в файл
-нет файла для загрузки сохранения или не получилось его открыть
-ошибка во время загрузки сохранения = записи данных в структуру
*/

void SaveManager::copy(){
    std::ifstream src_file(tempsave_filename, std::ios::binary);
    std::ofstream dest_file(save_filename, std::ios::binary);

    std::string line;
    while(std::getline(src_file, line)){
	    dest_file << line <<'\n';
    }
}


const char* SaveManager::save_game_ans(const Saving& game_data){
    try{
	//попробуй сохранить во временный файл
	    save_game(game_data); //если здесь не вылетели -> копируем в основное сохранение
	    copy();
	    return "Saved";
    } catch(const std::exception& e) { //если вылетели -> старое сохранение останется
	    return e.what(); //возвращаем сообщение ошибки
    }
}

const char* SaveManager::load_game_ans(Saving& game_data){
    try{
	    load_game(game_data);
	    return "Loaded";
    } catch(const std::exception& e) {
	    return e.what(); //возвращаем сообщение ошибки
    }
}


//итоговое сохранение и загрузка
void SaveManager::save_game(const Saving& game_data) {
    //тут сохраняем во временный файл
    std::ofstream temp_file(tempsave_filename, std::ios::binary);
    if (!temp_file) { 
	throw std::runtime_error("file-error");
    }

    save_saving_data(temp_file, game_data);
}

void SaveManager::load_game(Saving& game_data) {
    //загружаем из основного файла
    std::ifstream file(save_filename, std::ios::binary);
    if (!file) {
        throw std::runtime_error("file-error");
    }
        
    load_saving_data(file, game_data);
}
    

//сохранение и загрузка полей GManager = Saving - Manager и GField
void SaveManager::save_saving_data(std::ofstream& file, const Saving& data) {
    if(!save_manager_data(file, data.manager)) throw std::runtime_error("Not Saved: data_error");
}
    
void SaveManager::load_saving_data(std::ifstream& file, Saving& data) {
    if(!load_manager_data(file, data.manager)) throw std::runtime_error("Not Loaded: data error");
}
    

//сохранение ManagerData
//нам нужно записать ВСЕ поля ВСЕХ полей
bool SaveManager::save_manager_data(std::ofstream& file, const Saving::ManagerData& data) {
	// уровень
    	file.write((const char*)(&data.level), sizeof(data.level));
        
    	// PlayerData
    	file.write((const char*)(&data.player.lifes), sizeof(data.player.lifes));
    	file.write((const char*)(&data.player.damage), sizeof(data.player.damage));
    	file.write((const char*)(&data.player.x), sizeof(data.player.x));
    	file.write((const char*)(&data.player.y), sizeof(data.player.y));
    	file.write((const char*)(&data.player.close_combat), sizeof(data.player.close_combat));
        
    	// FieldData
    	file.write((const char*)(&data.field.height), sizeof(data.field.height));
    	file.write((const char*)(&data.field.width), sizeof(data.field.width));
        
    	// cохраняем каждую из клеток поля
    	for (int i = 0; i < data.field.height; ++i) {
            for (int j = 0; j < data.field.width; ++j) {
            const auto& cell = data.field.cells[i][j];
            file.write((const char*)(&cell.impassible), sizeof(cell.impassible));
            file.write((const char*)(&cell.is_there_player), sizeof(cell.is_there_player));
            file.write((const char*)(&cell.is_there_enemy), sizeof(cell.is_there_enemy));
            file.write((const char*)(&cell.trap), sizeof(cell.trap));
            file.write((const char*)(&cell.enemy_tower), sizeof(cell.enemy_tower));
            }
    	}
        
    	// враги
    	int enemy_count = data.enemies.size();
    	file.write((const char*)(&enemy_count), sizeof(enemy_count));
    	for (const auto& enemy : data.enemies) {
            file.write((const char*)(&enemy.x), sizeof(enemy.x));
            file.write((const char*)(&enemy.y), sizeof(enemy.y));
            file.write((const char*)(&enemy.lifes), sizeof(enemy.lifes));
            file.write((const char*)(&enemy.damage), sizeof(enemy.damage));
    	}
        
    	// killed
    	int kill_count = data.killed.size();
    	file.write((const char*)(&kill_count), sizeof(kill_count));
    	for (const auto& [enemy_type, count] : data.killed) {
            file.write((const char*)(&enemy_type), sizeof(enemy_type));
            file.write((const char*)(&count), sizeof(count));
    	}
        
    	// HandData
    	save_string_vector(file, data.hand.acceptable);
    	save_string_vector(file, data.hand.spells);
    	file.write((const char*)(&data.hand.last_spell), sizeof(data.hand.last_spell));
    	file.write((const char*)(&data.hand.max_count), sizeof(data.hand.max_count));
    	return !file.fail();
}


//загружаем ВСЕ поля ВСЕХ полей Manager
bool SaveManager::load_manager_data(std::ifstream& file, Saving::ManagerData& data) {
    	// уровень
    	file.read((char*)(&data.level), sizeof(data.level));
        
    	// PlayerData
    	file.read((char*)(&data.player.lifes), sizeof(data.player.lifes));
    	file.read((char*)(&data.player.damage), sizeof(data.player.damage));
    	file.read((char*)(&data.player.x), sizeof(data.player.x));
    	file.read((char*)(&data.player.y), sizeof(data.player.y));
    	file.read((char*)(&data.player.close_combat), sizeof(data.player.close_combat));
        
    	// FieldData
    	file.read((char*)(&data.field.height), sizeof(data.field.height));
    	file.read((char*)(&data.field.width), sizeof(data.field.width));
        
    	// загружаем все клетки поля
    	data.field.cells.resize(data.field.height, std::vector<Saving::CellData>(data.field.width));
    	for (int i = 0; i < data.field.height; ++i) {
            for (int j = 0; j < data.field.width; ++j) {
            	auto& cell = data.field.cells[i][j];
            	file.read((char*)(&cell.impassible), sizeof(cell.impassible));
            	file.read((char*)(&cell.is_there_player), sizeof(cell.is_there_player));
            	file.read((char*)(&cell.is_there_enemy), sizeof(cell.is_there_enemy));
            	file.read((char*)(&cell.trap), sizeof(cell.trap));
            	file.read((char*)(&cell.enemy_tower), sizeof(cell.enemy_tower));
            }
    	}
        
    	// враги
    	int enemy_count;
    	file.read((char*)(&enemy_count), sizeof(enemy_count));
    	data.enemies.resize(enemy_count);
    	for (int i = 0; i < enemy_count; ++i) {
            auto& enemy = data.enemies[i];
            file.read((char*)(&enemy.x), sizeof(enemy.x));
            file.read((char*)(&enemy.y), sizeof(enemy.y));
            file.read((char*)(&enemy.lifes), sizeof(enemy.lifes));
            file.read((char*)(&enemy.damage), sizeof(enemy.damage));
    	}
        
    	// killed
    	int kill_count;
    	file.read((char*)(&kill_count), sizeof(kill_count));
    	for (int i = 0; i < kill_count; ++i) {
            char enemy_type;
            int count;
            file.read((char*)(&enemy_type), sizeof(enemy_type));
            file.read((char*)(&count), sizeof(count));
            data.killed[enemy_type] = count;
    	}
        
    	// HandData
    	data.hand.acceptable = load_string_vector(file);
    	data.hand.spells = load_string_vector(file);
    	file.read((char*)(&data.hand.last_spell), sizeof(data.hand.last_spell));
    	file.read((char*)(&data.hand.max_count), sizeof(data.hand.max_count));   
    	return !file.fail();
}


//вспомогательные для работы с массивами
void SaveManager::save_string_vector(std::ofstream& file, const std::vector<std::string>& vec) {
    int size = vec.size();
    file.write((const char*)(&size), sizeof(size));
    for (const auto& str : vec) {
        int str_size = str.size();
        file.write((const char*)(&str_size), sizeof(str_size));
        file.write(str.c_str(), str_size);
    }
}
    
std::vector<std::string> SaveManager::load_string_vector(std::ifstream& file) {
    int size;
    file.read((char*)(&size), sizeof(size));
    std::vector<std::string> result(size);
        
    for (int i = 0; i < size; ++i) {
        int str_size;
        file.read((char*)(&str_size), sizeof(str_size));
            
        std::vector<char> buffer(str_size);
        file.read(buffer.data(), str_size);
        result[i] = std::string(buffer.data(), str_size);
    }
        
    return result;
}

