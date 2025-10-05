set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(SHADERS_DIR ${CMAKE_SOURCE_DIR}/resources/shaders)

bgfx_compile_shaders(
	TYPE VERTEX
	SHADERS ${SHADERS_DIR}/quad_vs.sc
	VARYING_DEF ${SHADERS_DIR}/varying.def.sc
	OUTPUT_DIR ${CMAKE_BINARY_DIR}/include/generated/shaders
    INCLUDE_DIRS ${SHADERS_DIR}
    AS_HEADERS
)

bgfx_compile_shaders(
	TYPE FRAGMENT
	SHADERS ${SHADERS_DIR}/quad_fs.sc
	VARYING_DEF ${SHADERS_DIR}/varying.def.sc
	OUTPUT_DIR ${CMAKE_BINARY_DIR}/include/generated/shaders
    INCLUDE_DIRS ${SHADERS_DIR}
    AS_HEADERS
)

file(GLOB_RECURSE SOURCE_FILES RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "*.cpp")

# Engine library
add_library(feyerverx STATIC
${SOURCE_FILES}
# shaders
${SHADERS_DIR}/quad_vs.sc
${SHADERS_DIR}/quad_fs.sc
)

set(VENDOR_DIR ${CMAKE_SOURCE_DIR}/vendor)

# Engine headers
target_include_directories(feyerverx
    PUBLIC ${CMAKE_SOURCE_DIR}/include
    PUBLIC ${BGFX_DIR}/include
    PUBLIC ${BX_DIR}/include
    PUBLIC ${BIMG_DIR}/include
    PUBLIC ${VENDOR_DIR}/SDL/include
    PUBLIC ${VENDOR_DIR}/stb/include
    PRIVATE ${CMAKE_BINARY_DIR}/include/generated/shaders

)

# Dependencies
target_link_libraries(feyerverx
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
