CXX       := g++
CXXFLAGS  := -std=c++20 -Wall -Wextra -Wpedantic -O2 -MMD -MP
LDFLAGS   := 

TARGET    := lab1

SRC       := $(wildcard *.cpp) 
OBJDIR    := build
OBJ       := $(patsubst %.cpp,$(OBJDIR)/%.o,$(SRC))
DEP       := $(OBJ:.o=.d)

all: $(TARGET)

# линк
$(TARGET): $(OBJ)
	@echo "link $@"
	$(CXX) $(OBJ) -o $@ $(LDFLAGS)

# компиляция одного cpp в o
$(OBJDIR)/%.o: %.cpp | $(OBJDIR)
	@echo "c++ $<"
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJDIR):
	@mkdir -p $(OBJDIR)

# автозависимости
-include $(DEP)

run: $(TARGET)
	./$(TARGET)

clean:
	@echo "clean"
	@rm -rf $(OBJDIR) $(TARGET)

.PHONY: all run debug clean
