CC := clang++
SRC := src/*.cpp
LOCALINCLUDE := include/

LIBDIR := /usr/local/lib/

LIBINCLUDEDIR := /usr/local/include
INCLUDE_RAYLIB := /raylib
INCLUDE_RAYLIB_CPP := /raylib-cpp


LIB = -L ${LIBDIR} -lraylib -framework OpenGL -framework Cocoa -framework IOKit -framework CoreFoundation -framework CoreAudio -framework CoreVideo -framework AudioToolbox
INCLUDE = -I${LIBINCLUDEDIR}${INCLUDE_RAYLIB} -I${LIBINCLUDEDIR}${INCLUDE_RAYLIB_CPP} -I${LOCALINCLUDE}

all:
	$(CC) --std=c++20 $(SRC) ${LIB} ${INCLUDE} -o turnip

debug:
	$(CC) -g --std=c++20 $(SRC) ${LIB} ${INCLUDE} -o turnip

run: all
	./turnip