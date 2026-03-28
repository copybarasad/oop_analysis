CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -O2
TARGET := game


BUILD_DIR := build


SRCS := $(wildcard *.cpp)

OBJS := $(patsubst %.cpp,$(BUILD_DIR)/%.o,$(SRCS))


DEPS := $(OBJS:.o=.d)


INCLUDES := -I.

.PHONY: all clean run

all: $(TARGET)


$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $@


$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -MMD -MP -c $< -o $@


-include $(DEPS)

run: all
	./$(TARGET)

clean:
	rm -rf $(BUILD_DIR) $(TARGET)