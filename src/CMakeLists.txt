set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Engine library
add_library(turnip STATIC
    app.cpp
    turnip.cpp
    # Add all source files here
)

# Engine headers
target_include_directories(turnip
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
    PUBLIC ${BGFX_DIR}/include
    PUBLIC ${BX_DIR}/include
    PUBLIC ${CMAKE_SOURCE_DIR}/vendor/SDL/include
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
