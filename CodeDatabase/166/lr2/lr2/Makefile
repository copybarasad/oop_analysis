# компилятор и флаги
CXX       := g++
CXXFLAGS  := -std=c++20 -Wall -Wextra -Wpedantic -O2 -MMD -MP -I. -Ispells
LDFLAGS   :=

TARGET    := lab2

# исходники: корень + подкаталог spells
SRC_ROOT  := $(wildcard *.cpp)
SRC_SPN   := $(wildcard spells/*.cpp)

# тут перечисляем файлы, которые сейчас НЕ надо собирать
EXCLUDE_SRC := spells/heal_spell.cpp spells/tower_spell.cpp

# общий список исходников минус исключения
SRC       := $(filter-out $(EXCLUDE_SRC), $(SRC_ROOT) $(SRC_SPN))

OBJDIR    := build
OBJ       := $(patsubst %.cpp,$(OBJDIR)/%.o,$(SRC))
DEP       := $(OBJ:.o=.d)

all: $(TARGET)

$(TARGET): $(OBJ)
	@echo "link $(TARGET)"
	$(CXX) $(OBJ) -o $@ $(LDFLAGS)

$(OBJDIR)/%.o: %.cpp
	@mkdir -p $(@D)
	@echo "c++ $<"
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

debug: CXXFLAGS := -std=c++20 -Wall -Wextra -Wpedantic -O0 -g -fsanitize=address,undefined -MMD -MP -I. -Ispells
debug: clean $(TARGET)

clean:
	@echo "clean"
	@rm -rf $(OBJDIR) $(TARGET)

-include $(DEP)

.PHONY: all run debug clean
