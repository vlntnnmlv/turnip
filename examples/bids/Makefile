# --- Includes ---
# Global include directory
INCLUDEDIR := /usr/local/include

# Raylib include directories
I_RAYLIB := /raylib
I_RAYLIB_CPP := /raylib-cpp

# Local include directory
I := ../../include/

# Final include flags
INCLUDE = -I$(INCLUDEDIR)$(I_RAYLIB) -I$(INCLUDEDIR)$(I_RAYLIB_CPP) -I$(I)

# --- Libraries ---
# Global lib directory
LIBDIR := /usr/local/lib/

LIB = -L $(LIBDIR) -lraylib

# --- Frameworks ---
FRAMEWORKS = -framework OpenGL -framework Cocoa -framework IOKit -framework CoreFoundation -framework CoreAudio -framework CoreVideo -framework AudioToolbox

CXX = clang++
CXXFLAGS = --std=c++20 -Wall -Wextra -Werror
LDFLAGS = $(INCLUDE) $(LIB) $(FRAMEWORKS)

# SRCS = $(wildcard src/*.cpp) $(wildcard src/interpolated/*.cpp) $(wildcard src/events/*.cpp) $(wildcard src/ecs/*.cpp) $(wildcard src/ecs/components/*.cpp) $(wildcard src/ecs/engines/*.cpp) $(wildcard src/ecs/systems/*.cpp)
SRCS = $(wildcard *.cpp)
OBJS = $(SRCS:.cpp=.o)

TARGET = bids

all: $(TARGET)

$(TARGET) : $(OBJS)
	$(CXX) $(OBJS) ../../turnip.a -o $(TARGET) $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)