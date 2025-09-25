set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Engine library
add_library(turnip STATIC
app.cpp
turnip.cpp
assetManager.cpp
renderer.cpp
# Add all source files here
)

set(VENDOR_DIR ${CMAKE_SOURCE_DIR}/vendor)

# Engine headers
target_include_directories(turnip
    PUBLIC ${CMAKE_SOURCE_DIR}/include/turnip
    PUBLIC ${BGFX_DIR}/include
    PUBLIC ${BX_DIR}/include
    PUBLIC ${BIMG_DIR}/include
    PUBLIC ${VENDOR_DIR}/SDL/include
    PUBLIC ${VENDOR_DIR}/stb/include
)

# Dependencies
target_link_libraries(turnip
    PRIVATE
        bgfx
        bimg
        bx
        "-framework Foundation"
        "-framework Cocoa"
        "-framework QuartzCore"
        "-framework Metal"
        "-framework MetalKit"
        SDL3::SDL3
)
