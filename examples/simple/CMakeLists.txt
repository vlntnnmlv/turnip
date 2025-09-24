set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Create executable
add_executable(ExampleApp
    main.cpp
)

# Include headers
target_include_directories(ExampleApp PRIVATE
    ${CMAKE_SOURCE_DIR}/src
)

# Link against engine
target_link_libraries(ExampleApp
    PRIVATE turnip
)