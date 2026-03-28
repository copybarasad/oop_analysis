#include "logger.h"
#include "enums.h"
#include <sstream>
#include <iostream>

std::unique_ptr<Logger> Logger::logger_instance = nullptr;

Logger::Logger(const std::string& file_name, Log_output mode) 
    : output_mode(mode), log_file_name(file_name) {
    
    if (mode != Log_output::CONSOLE_ONLY) {
        log_file.open(file_name, std::ios::app);
        if (!log_file.is_open()) {
            std::cerr << "Не удалось открыть файл лога: " << file_name << std::endl;
        }
    }
    
    auto& event_system = Event_system::get_instance();
    event_system.subscribe(Event_type::PLAYER_MOVED, this);
    event_system.subscribe(Event_type::PLAYER_ATTACKED, this);
    event_system.subscribe(Event_type::PLAYER_CAST_SPELL, this);
    event_system.subscribe(Event_type::PLAYER_DAMAGED, this);
    event_system.subscribe(Event_type::PLAYER_HEALED, this);
    event_system.subscribe(Event_type::ENEMY_KILLED, this);
    event_system.subscribe(Event_type::ENEMY_SPAWNED, this);
    event_system.subscribe(Event_type::BUILDING_DESTROYED, this);
    event_system.subscribe(Event_type::SPELL_LEARNED, this);
    event_system.subscribe(Event_type::GAME_SAVED, this);
    event_system.subscribe(Event_type::GAME_LOADED, this);
    event_system.subscribe(Event_type::LEVEL_CHANGED, this);
    event_system.subscribe(Event_type::TRAP_TRIGGERED, this);
}

Logger::~Logger() {
    if (log_file.is_open()) {
        log_file.close();
    }
}

std::string Logger::get_timestamp() {
    std::time_t now = std::time(nullptr);
    if (now == -1) {
        return "[TIME_ERROR]";
    }
    
    std::tm* timeinfo = std::localtime(&now);
    if (!timeinfo) {
        return "[TIME_ERROR]";
    }
    
    std::stringstream ss;
    ss << std::put_time(timeinfo, "%Y-%m-%d %H:%M:%S");
    return ss.str();
}


std::string Logger::level_to_string(Log_level level) {
    switch(level) {
        case Log_level::INFO: return "INFO";
        case Log_level::WARNING: return "WARNING";
        case Log_level::ERROR: return "ERROR";
        case Log_level::DEBUG: return "DEBUG";
        default: return "UNKNOWN";
    }
}

void Logger::write_log(Log_level level, const std::string& message) {
    std::lock_guard<std::mutex> lock(log_mutex);
    
    std::string log_entry = "[" + get_timestamp() + "] [" + level_to_string(level) + "] " + message;
    
    if ((output_mode == Log_output::FILE_ONLY || output_mode == Log_output::BOTH) && 
        log_file.is_open()) {
        log_file << log_entry << std::endl;
        log_file.flush();
    }
    
    if (output_mode == Log_output::CONSOLE_ONLY || output_mode == Log_output::BOTH) {
        std::cout << log_entry << std::endl;
    }
}

void Logger::on_event(const Game_event& event) {
    std::string log_message;
    
    switch(event.type) {
        case Event_type::PLAYER_MOVED:
            log_message = event.source + " переместился на (" + std::to_string(event.x) + "," + std::to_string(event.y) + ")";
            break;
            
        case Event_type::PLAYER_ATTACKED:
            log_message = event.source + " атаковал " + event.target + " и нанес " + std::to_string(event.value) + " урона";
            break;
            
        case Event_type::PLAYER_CAST_SPELL:
            log_message = event.source + " применил заклинание " + event.target + " на координаты (" + std::to_string(event.x) + "," + std::to_string(event.y) + ")";
            break;
            
        case Event_type::PLAYER_DAMAGED:
            log_message = event.source + " получил " + std::to_string(event.value) + " урона от " + event.target;
            break;
            
        case Event_type::PLAYER_HEALED:
            log_message = event.source + " восстановил " + std::to_string(event.value) + " здоровья";
            break;
            
        case Event_type::ENEMY_KILLED:
            log_message = "Враг " + event.source + " убит в (" + std::to_string(event.x) + "," + std::to_string(event.y) + ")";
            break;
            
        case Event_type::ENEMY_SPAWNED:
            log_message = "Враг " + event.source + " появился в (" + std::to_string(event.x) + "," + std::to_string(event.y) + ")";
            break;
            
        case Event_type::BUILDING_DESTROYED:
            log_message = "Здание " + event.source + " разрушено в (" + std::to_string(event.x) + "," + std::to_string(event.y) + ")";
            break;
            
        case Event_type::SPELL_LEARNED:
            log_message = event.source + " выучил новое заклинание: " + event.target;
            break;
            
        case Event_type::GAME_SAVED:
            log_message = "Игра сохранена в файл: " + event.target;
            break;
            
        case Event_type::GAME_LOADED:
            log_message = "Игра загружена из файла: " + event.target;
            break;
            
        case Event_type::LEVEL_CHANGED:
            log_message = "Переход на уровень " + std::to_string(event.value);
            break;
            
        case Event_type::TRAP_TRIGGERED:
            log_message = "Ловушка " + event.source + " сработала на " + event.target + " в (" + std::to_string(event.x) + "," + std::to_string(event.y) + ")";
            break;
            
        default:
            log_message = event.description;
    }
    
    write_log(Log_level::INFO, log_message);
}

void Logger::log(Log_level level, const std::string& message) {
    write_log(level, message);
}

void Logger::info(const std::string& message) {
    log(Log_level::INFO, message);
}

void Logger::warning(const std::string& message) {
    log(Log_level::WARNING, message);
}

void Logger::error(const std::string& message) {
    log(Log_level::ERROR, message);
}

void Logger::set_output_mode(Log_output mode) {
    std::lock_guard<std::mutex> lock(log_mutex);
    output_mode = mode;
}

void Logger::change_log_file(const std::string& file_name) {
    std::lock_guard<std::mutex> lock(log_mutex);
    
    if (log_file.is_open()) {
        log_file.close();
    }
    
    log_file_name = file_name;
    log_file.open(file_name, std::ios::app);
}

void Logger::init(const std::string& file_name, Log_output mode) {
    if (!logger_instance) {
        logger_instance = std::make_unique<Logger>(file_name, mode);
    }
}

Logger& Logger::get_instance() {
    if (!logger_instance) {
        logger_instance = std::make_unique<Logger>();
    }
    return *logger_instance;
}