cmake_minimum_required(VERSION 3.28)
project(CMakeSFMLProject LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

#----------------------------------------------------------------

file(GLOB_RECURSE PROJECT_SOURCES CONFIGURE_DEPENDS
    "src/*.cpp"
)
file(GLOB_RECURSE PROJECT_HEADERS CONFIGURE_DEPENDS
    "include/*.hpp"
)

#---------------------------------------------------------------

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.1.0
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

#---------------------------------------------------------------

find_package(OpenCL REQUIRED)

#---------------------------------------------------------------

add_executable(main
    src/2D_lensing.cpp
)

target_compile_definitions(main PRIVATE
    PROJECT_SOURCE_DIR="${CMAKE_SOURCE_DIR}"
)

target_compile_features(main PRIVATE cxx_std_23)

target_include_directories(main PRIVATE
    include
)

target_link_libraries(main PRIVATE
    OpenCL::OpenCL
    SFML::Graphics
)

#---------------------------------------------------------------

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    PROPERTY VS_STARTUP_PROJECT main)