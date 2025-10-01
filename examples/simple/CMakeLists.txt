set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_compile_definitions(BX_CONFIG_DEBUG=1)

# Create executable
add_executable(simple
    main.cpp
)

# Include headers
target_include_directories(simple PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

# Link against engine
target_link_libraries(simple
    PRIVATE feyerverx
)