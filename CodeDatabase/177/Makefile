# Компилятор и флаги
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2 -Wno-reorder
DEBUG_FLAGS = -g -DDEBUG

# Директории
SRC_DIR = .
BUILD_DIR = build
BIN_DIR = bin

# Исходные файлы
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)

# Удалить файлы для логирования и настраиваемого управления
SOURCES := $(filter-out $(SRC_DIR)/game_engine.cpp, $(SOURCES))
SOURCES := $(filter-out $(SRC_DIR)/game_event_logger.cpp, $(SOURCES))
SOURCES := $(filter-out $(SRC_DIR)/console_logger.cpp, $(SOURCES))
SOURCES := $(filter-out $(SRC_DIR)/file_logger.cpp, $(SOURCES))
SOURCES := $(filter-out $(SRC_DIR)/key_bindings.cpp, $(SOURCES))
SOURCES := $(filter-out $(SRC_DIR)/key_bindings_loader.cpp, $(SOURCES))

# ПРОВЕРИТЬ: если continue_game_command.cpp существует, он ДОЛЖЕН компилироваться
# УБЕРИТЕ эту строку если она есть:
# SOURCES := $(filter-out $(SRC_DIR)/continue_game_command.cpp, $(SOURCES))

OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

# Исключаем main из объектных файлов для библиотеки
LIB_OBJECTS = $(filter-out $(BUILD_DIR)/main.o, $(OBJECTS))

# Имя исполняемого файла
TARGET = $(BIN_DIR)/game

# Правило по умолчанию
all: $(TARGET)

# Создание исполняемого файла
$(TARGET): $(OBJECTS) | $(BIN_DIR)
	$(CXX) $(OBJECTS) -o $(TARGET)

# Компиляция объектных файлов
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Создание директорий
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# Отладочная сборка
debug: CXXFLAGS += $(DEBUG_FLAGS)
debug: $(TARGET)

# Очистка
clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

# Пересборка
rebuild: clean all

# Запуск программы
run: $(TARGET)
	./$(TARGET)

# Отладочный запуск
debug-run: debug
	./$(TARGET)

# Создание статической библиотеки
libgame.a: $(LIB_OBJECTS)
	ar rcs $@ $(LIB_OBJECTS)

# Обновленные зависимости (добавлена зависимость от continue_game_command.h)

$(BUILD_DIR)/main.o: $(SRC_DIR)/main.cpp \
                     $(SRC_DIR)/game_controller.h \
                     $(SRC_DIR)/game_utils.h \
                     $(SRC_DIR)/command_reader.h \
                     $(SRC_DIR)/game_renderer.h \
                     $(SRC_DIR)/game_command_manager.h \
                     $(SRC_DIR)/game_visualization_manager.h \
                     $(SRC_DIR)/game_save_manager.h \
                     $(SRC_DIR)/command_processor.h

$(BUILD_DIR)/game_controller.o: $(SRC_DIR)/game_controller.cpp \
                                $(SRC_DIR)/game_controller.h \
                                $(SRC_DIR)/player.h \
                                $(SRC_DIR)/game_field.h \
                                $(SRC_DIR)/game_enums.h \
                                $(SRC_DIR)/enemy_manager.h \
                                $(SRC_DIR)/building_manager.h \
                                $(SRC_DIR)/combat_manager.h \
                                $(SRC_DIR)/player_controller.h \
                                $(SRC_DIR)/game_initializer.h \
                                $(SRC_DIR)/turn_processor.h \
                                $(SRC_DIR)/tower.h

$(BUILD_DIR)/game_command_manager.o: $(SRC_DIR)/game_command_manager.cpp \
                                     $(SRC_DIR)/game_command_manager.h \
                                     $(SRC_DIR)/command_input_interface.h \
                                     $(SRC_DIR)/command_processor.h \
                                     $(SRC_DIR)/command_interface.h \
                                     $(SRC_DIR)/game_controller.h

$(BUILD_DIR)/game_visualization_manager.o: $(SRC_DIR)/game_visualization_manager.cpp \
                                           $(SRC_DIR)/game_visualization_manager.h \
                                           $(SRC_DIR)/game_renderer_interface.h \
                                           $(SRC_DIR)/game_controller.h

$(BUILD_DIR)/command_factory.o: $(SRC_DIR)/command_factory.cpp \
                               $(SRC_DIR)/command_factory.h \
                               $(SRC_DIR)/command_interface.h \
                               $(SRC_DIR)/game_controller.h \
                               $(SRC_DIR)/command.h \
                               $(SRC_DIR)/string_utils.h \
                               $(SRC_DIR)/position.h \
                               $(SRC_DIR)/game_enums.h \
                               $(SRC_DIR)/continue_game_command.h  # ДОБАВЛЕНО

# Добавить правило для continue_game_command.o если файл существует
$(BUILD_DIR)/continue_game_command.o: $(SRC_DIR)/continue_game_command.cpp \
                                      $(SRC_DIR)/continue_game_command.h \
                                      $(SRC_DIR)/command_interface.h \
                                      $(SRC_DIR)/game_controller.h \
                                      $(SRC_DIR)/command.h

$(BUILD_DIR)/command_reader.o: $(SRC_DIR)/command_reader.cpp \
                              $(SRC_DIR)/command_reader.h \
                              $(SRC_DIR)/command_input_interface.h \
                              $(SRC_DIR)/command_factory.h \
                              $(SRC_DIR)/game_controller.h

# Остальные зависимости (пример для других файлов)
$(BUILD_DIR)/game_renderer.o: $(SRC_DIR)/game_renderer.cpp \
                             $(SRC_DIR)/game_renderer.h \
                             $(SRC_DIR)/game_renderer_interface.h \
                             $(SRC_DIR)/game_controller.h \
                             $(SRC_DIR)/game_enums.h \
                             $(SRC_DIR)/player.h \
                             $(SRC_DIR)/spell_card.h

$(BUILD_DIR)/player.o: $(SRC_DIR)/player.cpp \
                      $(SRC_DIR)/player.h \
                      $(SRC_DIR)/game_enums.h \
                      $(SRC_DIR)/position.h \
                      $(SRC_DIR)/spell_card.h

$(BUILD_DIR)/game_field.o: $(SRC_DIR)/game_field.cpp \
                          $(SRC_DIR)/game_field.h \
                          $(SRC_DIR)/game_enums.h \
                          $(SRC_DIR)/position.h

$(BUILD_DIR)/position.o: $(SRC_DIR)/position.cpp \
                        $(SRC_DIR)/position.h

$(BUILD_DIR)/string_utils.o: $(SRC_DIR)/string_utils.cpp \
                            $(SRC_DIR)/string_utils.h

# Установка (если нужно)
install: $(TARGET)
	@if [ -d "/usr/local/games" ]; then \
		cp $(TARGET) /usr/local/games/; \
		echo "Game installed to /usr/local/games/"; \
	else \
		echo "Creating /usr/local/games directory..."; \
		sudo mkdir -p /usr/local/games; \
		sudo cp $(TARGET) /usr/local/games/; \
		echo "Game installed to /usr/local/games/"; \
	fi

# Генерация зависимостей (опционально)
depend:
	$(CXX) -MM $(SOURCES) > .depend

-include .depend

# Помощь
help:
	@echo "Доступные цели:"
	@echo "  all       - сборка программы (по умолчанию)"
	@echo "  debug     - сборка с отладочной информацией"
	@echo "  clean     - очистка собранных файлов"
	@echo "  rebuild   - полная пересборка"
	@echo "  run       - сборка и запуск программы"
	@echo "  debug-run - отладочная сборка и запуск"
	@echo "  libgame.a - создание статической библиотеки"
	@echo "  install   - установка программы"
	@echo "  depend    - сгенерировать зависимости"
	@echo "  help      - показать эту справку"

.PHONY: all debug clean rebuild run debug-run install depend help libgame.a