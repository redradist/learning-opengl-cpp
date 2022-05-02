cmake_minimum_required(VERSION 3.21)
project(LearnOpenGL)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_BINARY_DIR})

set(CMAKE_CXX_STANDARD 17)

option(OPENGL_WRAPPERS "Enables C++ wrappers" OFF)

if (OPENGL_WRAPPERS)
    add_definitions(-DOPENGL_WRAPPERS)
endif()

if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
    message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
    file(DOWNLOAD "https://raw.githubusercontent.com/conan-io/cmake-conan/0.18.1/conan.cmake"
         "${CMAKE_BINARY_DIR}/conan.cmake"
         TLS_VERIFY ON)
endif()

include(${CMAKE_BINARY_DIR}/conan.cmake)

set(CONAN_COMMAND conan)
conan_cmake_autodetect(settings)
conan_cmake_install(PATH_OR_REFERENCE ${CMAKE_CURRENT_LIST_DIR}
                    BUILD missing
                    REMOTE conancenter
                    SETTINGS ${settings})

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)

add_executable(LearnOpenGLStarter main.cpp)
conan_target_link_libraries(LearnOpenGLStarter)
