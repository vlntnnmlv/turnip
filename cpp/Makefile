CC := clang++
SRC := src/*.cpp

LIBDIR := /usr/local/lib/

INCLUDEDIR := /usr/local/include
INCLUDE_RAYLIB := /raylib
INCLUDE_RAYLIB_CPP := /raylib-cpp

LIB = -L ${LIBDIR} -lraylib -framework OpenGL -framework Cocoa -framework IOKit -framework CoreFoundation -framework CoreAudio -framework CoreVideo -framework AudioToolbox
INCLUDE = -I${INCLUDEDIR}${INCLUDE_RAYLIB} -I${INCLUDEDIR}${INCLUDE_RAYLIB_CPP}

all:
	$(CC) $(SRC) ${LIB} ${INCLUDE} -o turnip 