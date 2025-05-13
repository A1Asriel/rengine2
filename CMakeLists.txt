cmake_minimum_required(VERSION 3.11)
project(REngine)

# Add LaTeX documentation support
find_package(LATEX REQUIRED)

# Add documentation target
add_custom_target(
    latex
    COMMAND xelatex -interaction=nonstopmode -synctex=1 main.tex
    COMMAND xelatex -interaction=nonstopmode -synctex=1 main.tex
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/docs
    COMMENT "Building LaTeX"
    DEPENDS ${CMAKE_SOURCE_DIR}/docs/main.tex
)

# Add clean target for documentation
add_custom_target(
    latex-clean
    COMMAND ${CMAKE_COMMAND} -E remove
        ${CMAKE_SOURCE_DIR}/docs/main.pdf
        ${CMAKE_SOURCE_DIR}/docs/main.aux
        ${CMAKE_SOURCE_DIR}/docs/main.log
        ${CMAKE_SOURCE_DIR}/docs/main.out
        ${CMAKE_SOURCE_DIR}/docs/main.toc
        ${CMAKE_SOURCE_DIR}/docs/main.lop
        ${CMAKE_SOURCE_DIR}/docs/main.lpc
        ${CMAKE_SOURCE_DIR}/docs/main.synctex.gz
    COMMENT "Cleaning LaTeX"
)

include(FetchContent)

FetchContent_Declare(
    glm
    GIT_REPOSITORY    https://github.com/g-truc/glm.git
    GIT_TAG    bf71a834948186f4097caa076cd2663c69a10e1e #refs/tags/1.0.1
)
FetchContent_MakeAvailable(glm)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY    https://github.com/google/googletest.git
    GIT_TAG    52eb8108c5bdec04579160ae17225d66034bd723
)
FetchContent_MakeAvailable(googletest)

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
    src/glad.c
    src/Shader.cpp
    src/CubeMesh.cpp
    src/SphereMesh.cpp
    src/RE_Window.cpp
    src/SceneLoader.cpp
    src/Camera.cpp
    src/Texture.cpp
)

# Set include directories
include_directories(
    include
    ${SDL2_INCLUDE_DIRS}
    ${OPENGL_INCLUDE_DIRS}
)

# Create executable
add_executable(rengine src/main.cpp ${SOURCES})

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

enable_testing()

add_executable(rengine_tests tests/test_main.cpp ${SOURCES})
if(WIN32 OR MINGW)
    set_target_properties(rengine_tests PROPERTIES LINK_FLAGS "-mconsole")
endif()
target_link_libraries(rengine_tests
    gtest
    SDL2::SDL2main
    SDL2::SDL2-static
    glm::glm
    ${EXTRA_LIBS}
    "-static-libgcc" "-static-libstdc++"
)
add_test(NAME rengine_tests COMMAND rengine_tests)

option(COVERAGE "Enable coverage" OFF)
if(COVERAGE)
    find_program(GCOVR gcovr)
    if(GCOVR)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage -g -O0")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} --coverage -g -O0")
        add_custom_target(clean_coverage
            COMMAND ${CMAKE_COMMAND} -E remove_directory "${CMAKE_BINARY_DIR}/CMakeFiles/rengine.dir/src/"
            COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/CMakeFiles/rengine.dir/src/"
        )

        add_custom_target(coverage
            DEPENDS clean_coverage
            DEPENDS rengine_tests
            COMMAND ${CMAKE_COMMAND} -E chdir ${CMAKE_BINARY_DIR} ${CMAKE_CTEST_COMMAND} --output-on-failure
            COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/coverage"
            COMMAND ${GCOVR} -r ${CMAKE_SOURCE_DIR} --html --html-details --exclude "${CMAKE_SOURCE_DIR}/build" --exclude "${CMAKE_SOURCE_DIR}/src/glad.c" -o ${CMAKE_BINARY_DIR}/coverage/index.html
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        )
    endif()
endif()

option(PROFILING "Enable profiling" OFF)
if(PROFILING)
    find_program(GPROF gprof)
    if(GPROF)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg -O2 -no-pie")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pg -O2 -no-pie")
        add_custom_target(profile
            DEPENDS rengine
            COMMAND ${CMAKE_BINARY_DIR}/rengine${CMAKE_EXECUTABLE_SUFFIX}
            COMMAND gprof ${CMAKE_BINARY_DIR}/rengine${CMAKE_EXECUTABLE_SUFFIX} ${CMAKE_BINARY_DIR}/gmon.out > ${CMAKE_BINARY_DIR}/profiling.txt
            COMMAND ${CMAKE_COMMAND} -E rm ${CMAKE_BINARY_DIR}/gmon.out
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        )
    endif()
endif()
