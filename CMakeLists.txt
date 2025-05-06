cmake_minimum_required(VERSION 3.11)
project(REngine)

include(FetchContent)

FetchContent_Declare(
    glm
    GIT_REPOSITORY    https://github.com/g-truc/glm.git
    GIT_TAG    bf71a834948186f4097caa076cd2663c69a10e1e #refs/tags/1.0.1
)
FetchContent_MakeAvailable(glm)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(BUILD_SHARED_LIBRARIES OFF)

if(WIN32 OR MINGW)
    set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
    set(CMAKE_EXE_LINKER_FLAGS "-static")
    set(EXTRA_LIBS opengl32 glu32 gdi32)
    set(SDL2_DIR "${CMAKE_CURRENT_LIST_DIR}/sdl2-mingw/cmake")
endif()

# Find required packages
find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)

# Set source files
set(SOURCES
    src/main.cpp
    src/glad.c
    src/Shader.cpp
    src/CubeMesh.cpp
    src/SphereMesh.cpp
    src/RE_Window.cpp
    src/SceneLoader.cpp
    src/Camera.cpp
)

# Set include directories
include_directories(
    include
    ${SDL2_INCLUDE_DIRS}
    ${OPENGL_INCLUDE_DIRS}
)

# Create executable
add_executable(rengine ${SOURCES})

if(WIN32 OR MINGW)
    set_target_properties(rengine PROPERTIES LINK_FLAGS "-mconsole")
endif()

# Link libraries statically
target_link_libraries(rengine
    SDL2::SDL2main
    SDL2::SDL2-static
    glm::glm
    ${EXTRA_LIBS}
    "-static-libgcc" "-static-libstdc++"
)
