cmake_minimum_required(VERSION 4.0)
project(WindowGame C)


set(CMAKE_C_STANDARD 11)

file(COPY ${CMAKE_SOURCE_DIR}/Assets
        DESTINATION ${CMAKE_BINARY_DIR})

find_package(OpenGL REQUIRED)
find_package(SDL2 CONFIG REQUIRED)
find_package(SDL2_image CONFIG REQUIRED)
find_package(SDL2_ttf CONFIG REQUIRED)


file(GLOB_RECURSE PROJECT_SOURCES "${CMAKE_SOURCE_DIR}/Scripts/*.c")
file(GLOB_RECURSE PROJECT_HEADERS "${CMAKE_SOURCE_DIR}/Scripts/*.h")


add_executable(WindowGame
        ${PROJECT_SOURCES}
        glad/src/glad.c
        CCerealLib/meta_data/metaData.c
)


target_include_directories(WindowGame PRIVATE
        ${CMAKE_SOURCE_DIR}/Scripts
        ${CMAKE_SOURCE_DIR}/glad/include
        ${CMAKE_SOURCE_DIR}/CCerealLib/include)


target_link_libraries(WindowGame PRIVATE SDL2::SDL2
        SDL2_image::SDL2_image
        SDL2_ttf::SDL2_ttf
        OpenGL::GL
        ${CMAKE_SOURCE_DIR}/CCerealLib/lib/libCCereal.a)

target_compile_definitions(WindowGame PRIVATE SDL_MAIN_HANDLED)

