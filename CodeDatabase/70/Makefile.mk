CXX = g++
CXXFLAGS = -std=c++20 -g

TARGET = game.exe
GDB_TARGET = gdbgame.exe

SOURCES = $(shell find src -name '*.cpp')
OBJECTS = $(patsubst src/%.cpp, build/%.o, $(filter src/%.cpp, $(SOURCES)))

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(LDFLAGS) $^ -o $@

#g++ -std=c++20 -g -c src/spellsFactory.cpp -o build/spellsFactory.o
build/%.o: src/%.cpp
	@mkdir -p build
	@mkdir -p build/Menus
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(TARGET) $(OBJECTS)

rebuild: clean all

gdb: $(GDB_TARGET)

$(GDB_TARGET): $(OBJECTS)
	$(CXX) $(LDFLAGS) $^ -o $@ -g