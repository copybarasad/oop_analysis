CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -g -Iheaders
SRCDIR = src
OBJDIR = obj
TARGET = game.exe

# Определяем все подпапки внутри src
SUBDIRS = commands gamelogic render spells objects field managers ui

# Формируем список всех исходных файлов
SOURCES = \
	$(SRCDIR)/Main.cpp \
	$(foreach dir,$(SUBDIRS),$(wildcard $(SRCDIR)/$(dir)/*.cpp))

# Преобразуем пути исходных файлов в пути объектных файлов
OBJECTS = $(SOURCES:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)

# Основное правило сборки
$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET)

# Правило компиляции для каждого исходного файла
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@if not exist "$(dir $@)" mkdir "$(dir $@)"
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Правило для очистки
clean:
	@if exist "$(OBJDIR)" rmdir /S /Q "$(OBJDIR)"
	@if exist "$(TARGET)" del "$(TARGET)"

.PHONY: clean