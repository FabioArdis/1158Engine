cmake_minimum_required(VERSION 3.10)

project(1158engine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(Freetype REQUIRED)
find_package(glfw3 REQUIRED)

include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${FREETYPE_INCLUDE_DIRS})

file(GLOB_RECURSE SRC_FILES ${PROJECT_SOURCE_DIR}/src/*.cpp ${PROJECT_SOURCE_DIR}/src/*.c)

#[[ Add these for warnings and code tidiness, but I still haven't figured out how to exclude some files from the analysis.
add_compile_options(-Wall -Wextra -Wpedantic)

set(CMAKE_CXX_CLANG_TIDY 
    "clang-tidy;-checks=clang-analyzer-*,modernize-*,readability-*,bugprone-*,performance-*,cppcoreguidelines-*,portability-*,musc-*,google-*"
    "-header-filter=.*"
    "-system-headers=false")

file (GLOB IMGUI_FILES ${PROJECT_SOURCE_DIR}/src/imgui*.cpp ${PROJECT_SOURCE_DIR}/include/imgui*.h ${PROJECT_SOURCE_DIR}/include/imstb*.h)
set (GLAD_FILE ${PROJECT_SOURCE_DIR}/src/glad.c ${PROJECT_SOURCE_DIR}/include/glad/glad.h)

set (LINT_FILES ${IMGUI_FILES} ${GLAD_FILE})

set_source_files_properties(${LINT_FILES} PROPERTIES SKIP_LINTING ON)
]]

add_executable(1158engine ${SRC_FILES})

set_target_properties(1158engine PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR})

target_link_libraries(1158engine ${FREETYPE_LIBRARIES} glfw)