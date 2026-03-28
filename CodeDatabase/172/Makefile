APP			:= game
SRCDIR		:= src
OBJDIR		:= obj
TESTDIR		:= tests
TESTBINDIR	:= tests_bin

CXX			:= g++
CXXFLAGS	:= -std=c++17 -Wall -Wextra

SRC			:= $(wildcard $(SRCDIR)/*.cpp)
SRC_NO_MAIN := $(filter-out $(SRCDIR)/main.cpp, $(SRC))


OBJ			:= $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SRC))

TEST		:= $(wildcard $(TESTDIR)/*.cpp)
TESTEXES	:= $(patsubst $(TESTDIR)/%.cpp,$(TESTBINDIR)/%,$(TEST))

.PHONY: all test run clean debug release
.INTERMEDIATE: $(OBJ)

all: $(APP)
$(APP): $(OBJ)
	$(CXX) $(OBJ) -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJDIR):
	mkdir -p $@

debug: CXXFLAGS += -g -O0
debug: clean all

release: CXXFLAGS += -O2
release: clean all

test: $(TESTEXES)
	@set -e; \
	for t in $(TESTEXES); do \
		echo "== $$t =="; \
		$$t; \
	done;
	echo "All tests passed."

$(TESTBINDIR)/%: $(TESTDIR)/%.cpp $(SRC_NO_MAIN) | $(TESTBINDIR)
	$(CXX) $(CXXFLAGS) -DTESTING -Isrc $^ -o $@

$(TESTBINDIR):
	mkdir -p $@

run: all
	./$(APP)

clean:
	rm -rf $(OBJDIR) $(APP) $(TESTBINDIR)