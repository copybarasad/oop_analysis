CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I. -I./models -I./models/entities -I./models/spells -I./models/interfaces -I./models/commands -I./models/serializers -I./models/events -I./controllers -I./controllers/input -I./input -I./views -I./third_party
DEBUG_FLAGS = -g -O0
RELEASE_FLAGS = -O3 -DNDEBUG

# Директория для объектных файлов
OBJDIR = obj

# Поиск всех исходных файлов
CPP_SOURCES = $(wildcard models/*.cpp) \
              $(wildcard models/entities/*.cpp) \
              $(wildcard models/spells/*.cpp) \
              $(wildcard models/serializers/*.cpp) \
              $(wildcard models/events/*.cpp) \
              $(wildcard controllers/*.cpp) \
              $(wildcard controllers/input/*.cpp) \
              $(wildcard input/*.cpp) \
              $(wildcard models/commands/*.cpp) \
              $(wildcard views/*.cpp) \
              main.cpp

TEST_CPP_SOURCES = $(wildcard models/*.cpp) \
              $(wildcard models/entities/*.cpp) \
              $(wildcard models/spells/*.cpp) \
              $(wildcard models/serializers/*.cpp) \
              $(wildcard models/events/*.cpp) \
              $(wildcard controllers/*.cpp) \
              $(wildcard controllers/input/*.cpp) \
              $(wildcard input/*.cpp) \
              $(wildcard models/commands/*.cpp) \
              $(wildcard views/*.cpp)

TEST_FILES = tests/view_tests.cpp \
             tests/spell_tests.cpp \
             tests/tests.cpp

# Преобразование .cpp в .o с путями в OBJDIR
OBJECTS = $(patsubst %.cpp,$(OBJDIR)/%.o,$(CPP_SOURCES))
TEST_OBJECTS = $(patsubst %.cpp,$(OBJDIR)/%.o,$(TEST_CPP_SOURCES))
TEST_FILE_OBJECTS = $(patsubst %.cpp,$(OBJDIR)/%.o,$(TEST_FILES))

# Автоматическое определение зависимостей
DEPFILES = $(OBJECTS:.o=.d) $(TEST_OBJECTS:.o=.d) $(TEST_FILE_OBJECTS:.o=.d)

# Флаги для генерации зависимостей
DEPFLAGS = -MT $@ -MMD -MP -MF $(OBJDIR)/$*.d

# Параллельная сборка
MAKEFLAGS += --jobs=$(shell nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 4)

.PHONY: all game debug release test clean

all: release

# Основная цель сборки
game: $(OBJECTS)
	$(CXX) $(CXXFLAGS) $^ -o $@

debug: CXXFLAGS += $(DEBUG_FLAGS)
debug: game

release: CXXFLAGS += $(RELEASE_FLAGS)
release: game

# Правило компиляции с зависимостями
$(OBJDIR)/%.o: %.cpp | $(OBJDIR)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(DEPFLAGS) -c $< -o $@

# Создание директорий для объектных файлов
$(OBJDIR):
	@mkdir -p $(OBJDIR)

# Включение файлов зависимостей
-include $(DEPFILES)

# Тесты
test: $(TEST_OBJECTS) $(TEST_FILE_OBJECTS)
	$(CXX) $(CXXFLAGS) $^ -lgtest -lgtest_main -pthread -o TESTS_EXEC
	./TESTS_EXEC

test-views: $(TEST_OBJECTS) $(OBJDIR)/tests/view_tests.o
	$(CXX) $(CXXFLAGS) $^ -lgtest -lgtest_main -pthread -o TESTS_VIEWS_EXEC
	./TESTS_VIEWS_EXEC

test-spell: $(TEST_OBJECTS) $(OBJDIR)/tests/spell_tests.o
	$(CXX) $(CXXFLAGS) $^ -lgtest -lgtest_main -pthread -o TESTS_SPELL_EXEC
	./TESTS_SPELL_EXEC

clean:
	rm -rf $(OBJDIR) game TESTS_EXEC TESTS_SPELL_EXEC TESTS_VIEWS_EXEC