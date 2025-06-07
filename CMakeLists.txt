cmake_minimum_required(VERSION 3.17)
project(REngine VERSION 1.0)

include(FetchContent)

# Fetch glm (header-only)
FetchContent_Declare(
    glm
    GIT_REPOSITORY    https://github.com/g-truc/glm.git
    GIT_TAG    bf71a834948186f4097caa076cd2663c69a10e1e
)
FetchContent_MakeAvailable(glm)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(BUILD_SHARED_LIBRARIES ON)

if(WIN32 OR MINGW)
    set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
    set(SDL2_DIR "${CMAKE_CURRENT_LIST_DIR}/sdl2-mingw/cmake")
    # set(EXTRA_LIBS opengl32 glu32 gdi32)
endif()

# find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)

set(SOURCES
    src/glad.c
    src/Shader.cpp
    src/Mesh.cpp
    src/Renderer.cpp
    src/InputHandler.cpp
    src/Camera.cpp
    src/Texture.cpp
    src/Engine.cpp
    src/Volume.cpp
)

# Create library
add_library(rengine)
add_library(REngine::rengine ALIAS rengine)

target_sources(rengine PRIVATE ${SOURCES})

# Include glm headers and your own headers
target_include_directories(
    rengine
    PRIVATE src
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${glm_SOURCE_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

# Link libraries (remove glm::glm)
target_link_libraries(rengine
    PUBLIC
    ${EXTRA_LIBS}
    SDL2::SDL2-static SDL2::SDL2main
)

include(GNUInstallDirs)

# Install your library and headers
install(
    TARGETS rengine
    EXPORT rengineTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# Install your headers and glm's headers
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(DIRECTORY ${glm_SOURCE_DIR}/glm DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# Generate and install config files
include(CMakePackageConfigHelpers)
configure_package_config_file(
    "${CMAKE_CURRENT_LIST_DIR}/cmake/REngineConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/REngineConfig.cmake"
    INSTALL_DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/cmake/rengine
)

write_basic_package_version_file(
    REngineConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

install(EXPORT rengineTargets
    FILE REngineTargets.cmake
    NAMESPACE REngine::
    DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/cmake/rengine
)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/REngineConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/REngineConfigVersion.cmake
    DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/cmake/rengine
)

# enable_testing()

# add_executable(rengine_tests tests/test_main.cpp ${SOURCES})
# if(WIN32 OR MINGW)
#     set_target_properties(rengine_tests PROPERTIES LINK_FLAGS "-mconsole")
# endif()
# target_link_libraries(rengine_tests
#     gtest
#     # SDL2::SDL2main
#     # SDL2::SDL2-static
#     glm::glm
#     ${EXTRA_LIBS}
#     "-static-libgcc" "-static-libstdc++"
# )
# add_test(NAME rengine_tests COMMAND rengine_tests)

# option(COVERAGE "Enable coverage" OFF)
# if(COVERAGE)
#     find_program(GCOVR gcovr)
#     if(GCOVR)
#         set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage -g -O0")
# enable_testing()

# add_executable(rengine_tests tests/test_main.cpp ${SOURCES})
# if(WIN32 OR MINGW)
#     set_target_properties(rengine_tests PROPERTIES LINK_FLAGS "-mconsole")
# endif()
# target_link_libraries(rengine_tests
#     gtest
#     # SDL2::SDL2main
#     # SDL2::SDL2-static
#     glm::glm
#     ${EXTRA_LIBS}
#     "-static-libgcc" "-static-libstdc++"
# )
# add_test(NAME rengine_tests COMMAND rengine_tests)

# option(COVERAGE "Enable coverage" OFF)
# if(COVERAGE)
#     find_program(GCOVR gcovr)
#     if(GCOVR)
#         set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage -g -O0")
#         set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} --coverage -g -O0")
#         add_custom_target(clean_coverage
#             COMMAND ${CMAKE_COMMAND} -E remove_directory "${CMAKE_BINARY_DIR}/coverage/"
#             COMMAND ${CMAKE_COMMAND} -E remove_directory "${CMAKE_BINARY_DIR}/CMakeFiles/rengine.dir/src/"
#             COMMAND ${CMAKE_COMMAND} -E remove_directory "${CMAKE_BINARY_DIR}/CMakeFiles/rengine_tests.dir/src/"
#             COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/CMakeFiles/rengine.dir/src/"
#             COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/CMakeFiles/rengine_tests.dir/src/"
#         )

#         add_custom_target(coverage
#             # DEPENDS clean_coverage
#             # DEPENDS rengine
#             DEPENDS rengine_tests
#             # COMMAND ${CMAKE_BINARY_DIR}/rengine${CMAKE_EXECUTABLE_SUFFIX}
#             COMMAND ${CMAKE_COMMAND} -E chdir ${CMAKE_BINARY_DIR} ${CMAKE_CTEST_COMMAND} --output-on-failure
#             COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/coverage"
#             COMMAND ${GCOVR} -r ${CMAKE_SOURCE_DIR} --html --html-details --exclude "${CMAKE_SOURCE_DIR}/build" --exclude "${CMAKE_SOURCE_DIR}/src/glad.c" -o ${CMAKE_BINARY_DIR}/coverage/index.html
#             WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
#         )
#     endif()
# endif()

# option(PROFILING "Enable profiling" OFF)
# if(PROFILING)
#     find_program(GPROF gprof)
#     if(GPROF)
#         set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg -O2 -no-pie")
#         set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pg -O2 -no-pie")
#         add_custom_target(profile
#             DEPENDS rengine
#             COMMAND ${CMAKE_BINARY_DIR}/rengine${CMAKE_EXECUTABLE_SUFFIX}
#             COMMAND gprof ${CMAKE_BINARY_DIR}/rengine${CMAKE_EXECUTABLE_SUFFIX} ${CMAKE_BINARY_DIR}/gmon.out > ${CMAKE_BINARY_DIR}/profiling.txt
#             COMMAND ${CMAKE_COMMAND} -E rm ${CMAKE_BINARY_DIR}/gmon.out
#             WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
#         )
#     endif()
# endif()

find_package(LATEX)

if(${LATEX_FOUND})
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
endif()
