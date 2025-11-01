cmake_minimum_required(VERSION 4.0)
project(WindowGame C)


set(CMAKE_C_STANDARD 11)

file(COPY ${CMAKE_SOURCE_DIR}/Assets
        DESTINATION ${CMAKE_BINARY_DIR})

find_package(OpenGL REQUIRED)
find_package(SDL2 CONFIG REQUIRED)
find_package(SDL2_image CONFIG REQUIRED)
find_package(SDL2_ttf CONFIG REQUIRED)

include_directories(glad/include)

add_executable(WindowGame Scripts/main.c
        Scripts/Infrastructure/init.c
        Scripts/Render/draw.c
        Scripts/Render/Headers/draw.h
        Scripts/General/Headers/structs.h
        Scripts/Input/input.c
        Scripts/Input/Headers/input.h
        Scripts/DataStructures/List.c
        Scripts/DataStructures/Headers/List.h
        Scripts/DataStructures/Dictionary.c
        Scripts/DataStructures/Headers/Dictionary.h
        Scripts/Tests/DictionaryTests.c
        Scripts/Tests/Headers/DictionaryTests.h
        Scripts/General/player.c
        Scripts/General/Headers/player.h
        Scripts/Utils/MathUtils.c
        Scripts/Utils/Headers/mathUtils.h
        Scripts/Utils/ViewPortUtils.c
        Scripts/Utils/Headers/viewPortUtils.h
        Scripts/Infrastructure/window.c
        Scripts/Infrastructure/Headers/window.h
        Scripts/Infrastructure/Headers/app.h
        Scripts/General/levelTarget.c
        Scripts/General/Headers/levelTarget.h
        Scripts/Services/winService.c
        Scripts/Services/Headers/winService.h
        Scripts/Infrastructure/app.c
        Scripts/UI/ui.c
        Scripts/UI/Headers/ui.h
        Scripts/Tween/tweener.c
        Scripts/Tween/Headers/tweener.h
        Scripts/Services/levelService.c
        Scripts/Services/Headers/levelService.h
        Scripts/General/menu.c
        Scripts/General/Headers/menu.h
        Scripts/Infrastructure/stateMachine.c
        Scripts/Infrastructure/Headers/stateMachine.h
        Scripts/Infrastructure/update.c
        Scripts/Infrastructure/Headers/update.h
        glad/src/glad.c
        Scripts/Render/openGL.c
        Scripts/Render/Headers/openGL.h
        Scripts/Render/textures.c
        Scripts/Render/Headers/textures.h
)


target_link_libraries(WindowGame PRIVATE SDL2::SDL2
        SDL2_image::SDL2_image
        SDL2_ttf::SDL2_ttf
        OpenGL::GL)

target_compile_definitions(WindowGame PRIVATE SDL_MAIN_HANDLED)

