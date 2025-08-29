# --- Includes ---
# Global include directory
INCLUDEDIR := /usr/local/include

# Raylib include directories
I_RAYLIB := /raylib
I_RAYLIB_CPP := /raylib-cpp

# Local include directory
I := ./include/

# Final include flags
INCLUDE = -I$(INCLUDEDIR)$(I_RAYLIB) -I$(INCLUDEDIR)$(I_RAYLIB_CPP) -I$(I)

# --- Libraries ---
# Global lib directory
LIBDIR := /usr/local/lib/

LIB = -L $(LIBDIR) -lraylib

# --- Frameworks ---
FRAMEWORKS = -framework OpenGL -framework Cocoa -framework IOKit -framework CoreFoundation -framework CoreAudio -framework CoreVideo -framework AudioToolbox

CXX = clang++
CXXFLAGS = --std=c++20 -Wall -Wextra -Werror -MMD -MP
LDFLAGS = $(INCLUDE) $(LIB) $(FRAMEWORKS)

SRCS = $(wildcard src/*.cpp) $(wildcard src/interpolated/*.cpp) $(wildcard src/events/*.cpp) $(wildcard src/ecs/*.cpp) $(wildcard src/ecs/components/*.cpp) $(wildcard src/ecs/engines/*.cpp) $(wildcard src/ecs/systems/*.cpp)
OBJS = $(SRCS:.cpp=.o)
DEPS = $(OBJS:.o=.d)

LIB_TARGET = turnip.a
TARGET = turnipEngine

all: $(LIB_TARGET) $(TARGET)

# build static library
$(LIB_TARGET) : $(OBJS)
	ar rvs turnip.a $(OBJS)
# 	$(CXX) $(OBJS) -o $(TARGET) $(LDFLAGS)

# build app
$(TARGET):
	$(CXX) $(CXXFLAGS) main.cpp $(LIB_TARGET) $(LDFLAGS) -o $(TARGET)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $< -o $@

-include $(DEPS)

clean:
	rm -f $(OBJS) $(DEPS) $(TARGET) $(LIB_TARGET)