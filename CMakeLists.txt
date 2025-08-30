cmake_minimum_required(VERSION 4.0)
project(WindowGame C)

set(CMAKE_C_STANDARD 11)


find_package(SDL2 CONFIG REQUIRED)
find_package(SDL2_image CONFIG REQUIRED)
find_package(SDL2_ttf CONFIG REQUIRED)

add_executable(WindowGame Scripts/main.c
        Scripts/initSDL2.c
        Scripts/draw.c
        Scripts/Headers/draw.h
        Scripts/Headers/structs.h
        Scripts/input.c
        Scripts/Headers/input.h
        Scripts/DataStructures/List.c
        Scripts/DataStructures/Headers/List.h
        Scripts/DataStructures/Dictionary.c
        Scripts/DataStructures/Headers/Dictionary.h
        Scripts/Tests/DictionaryTests.c
        Scripts/Tests/DictionaryTests.h
        Scripts/player.c
        Scripts/Headers/player.h
        Scripts/Utils/MathUtils.c
        Scripts/Utils/MathUtils.h
        Scripts/Utils/ViewPortUtils.c
        Scripts/Utils/ViewPortUtils.h
        Scripts/Window.c
        Scripts/Headers/Window.h
        Scripts/Headers/app.h
        Scripts/levelTarget.c
        Scripts/Headers/levelTarget.h
        Scripts/winService.c
        Scripts/Headers/winService.h
        Scripts/app.c
        Scripts/ui.c
        Scripts/Headers/ui.h)


target_link_libraries(WindowGame PRIVATE SDL2::SDL2
        SDL2_image::SDL2_image
        SDL2_ttf::SDL2_ttf)

target_compile_definitions(WindowGame PRIVATE SDL_MAIN_HANDLED)

