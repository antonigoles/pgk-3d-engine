cmake_minimum_required(VERSION 3.15)
project(Labirynt3D LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# IDE integration
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# FetchContent will download external libs at configure time
include(FetchContent)

# --- GLFW ---
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG latest
)
FetchContent_MakeAvailable(glfw)

# --- GLAD ---
FetchContent_Declare(
    glad
    GIT_REPOSITORY https://github.com/Dav1dde/glad.git
    GIT_TAG master
)
FetchContent_MakeAvailable(glad)

# --- Project sources ---
file(GLOB_RECURSE PROJECT_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/**/*.cpp"
)

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(Labirynt3D PRIVATE glfw glad)

#  Copy shaders to build directory
set(SHADER_DIR "${CMAKE_CURRENT_SOURCE_DIR}/shaders")

#  Copy meshes to build directory
set(MESH_DIR "${CMAKE_CURRENT_SOURCE_DIR}/meshes")

file(COPY ${SHADER_DIR} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

file(COPY ${MESH_DIR} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})