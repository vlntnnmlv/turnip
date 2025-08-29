# Global include directory
INCLUDEDIR := /usr/local/include

# Raylib include directories
I_RAYLIB := /raylib
I_RAYLIB_CPP := /raylib-cpp

# Local include directory
I := ./include/

# Final include flags
INCLUDE = -I$(INCLUDEDIR)$(I_RAYLIB) -I$(INCLUDEDIR)$(I_RAYLIB_CPP) -I$(I)

CXX = clang++
CXXFLAGS = --std=c++20 -Wall -Wextra -Werror -MMD -MP

BUILDDIR := build

SRC_DIRS := \
src \
src/interpolated \
src/events \
src/ecs \
src/ecs/components \
src/ecs/engines \
src/ecs/systems

SRCS := $(foreach D,$(SRC_DIRS),$(wildcard $(D)/*.cpp))
OBJS := $(patsubst src/%.cpp,$(BUILDDIR)/%.o,$(SRCS))

DEPS := $(OBJS:.o=.d)

TARGET := turnip.a

all: $(TARGET)

$(TARGET) : $(OBJS)
	ar rvs $@ $^

$(BUILDDIR)/%.o: src/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $< -o $@

-include $(DEPS)

clean:
	@rm -rf $(BUILDDIR)
	@echo Successfully cleaned build!