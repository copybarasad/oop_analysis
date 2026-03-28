CXX := g++
CXXFLAGS := -std=c++20 -g

OBJ_DIR := obj
SRCS := $(shell find . -name "*.cpp" -not -path "./obj/*")
OBJS := $(addprefix $(OBJ_DIR)/, $(notdir $(SRCS:.cpp=.o)))

TARGET := game

all: $(TARGET)

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

$(TARGET): $(OBJS) | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

define compile_rule
$(OBJ_DIR)/$(notdir $(1:.cpp=.o)): $(1) | $(OBJ_DIR)
	@mkdir -p $$(OBJ_DIR)
	$$(CXX) $$(CXXFLAGS) -c $$< -o $$@
endef

$(foreach src,$(SRCS),$(eval $(call compile_rule,$(src))))

clean:
	rm -rf $(OBJ_DIR) $(TARGET)

rebuild: clean all

.PHONY: all clean rebuild
