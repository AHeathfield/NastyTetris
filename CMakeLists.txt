cmake_minimum_required(VERSION 3.16)
project(NastyTetris)

# set the output directory for built objects.
# This makes sure that the dynamic library goes into the build directory automatically.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

# Use vendored libs
set(SDLTTF_VENDORED ON)

# This assumes the SDL source is available in vendored/SDL
add_subdirectory(vendored/SDL EXCLUDE_FROM_ALL)

# This assumes the SDL_image source is available in vendored/SDL_image
add_subdirectory(vendored/SDL_image EXCLUDE_FROM_ALL)

# This assumes the SDL_ttf source is available in vendored/SDL_ttf
add_subdirectory(vendored/SDL_ttf EXCLUDE_FROM_ALL)

# This assumes the SDL_mixer source is available in vendored/SDL_mixer
add_subdirectory(vendored/SDL_mixer EXCLUDE_FROM_ALL)


# This creates a var called sources which will contain all the cpp files we are compiling
set(SRCS 
    game.cpp
    src/Systems/RenderSystem.cpp
    src/Systems/MouseButtonSystem.cpp
    src/Systems/PhysicsSystem.cpp
    src/Systems/CollisionSystem.cpp
    src/Systems/PlayerEventSystem.cpp
    src/Systems/TextEventSystem.cpp
    src/Systems/ShapeSystem.cpp
    src/Systems/PlayShapeSystem.cpp
    src/Systems/HoldSystem.cpp
    src/Systems/RowSystem.cpp
    src/Systems/ScoreSystem.cpp
    src/Systems/AudioSystem.cpp
    src/States/TitleState.cpp
    src/States/PlayState.cpp
    src/States/ScoreState.cpp
    src/Components/Buttons/StateChangerButton.cpp
    src/Shapes/LShape.cpp
    src/Shapes/ReverseLShape.cpp
    src/Shapes/TShape.cpp
    src/Shapes/ZShape.cpp
    src/Shapes/ReverseZShape.cpp
    src/Shapes/LineShape.cpp
    src/Shapes/SquareShape.cpp
)


# Create your game executable target as usual
if (APPLE)
    # create exe for MacOS
    add_executable(game MACOSX_BUNDLE ${SRCS})
else()
    # WIN32 is needed for windows, it is ignored for linux
    add_executable(game WIN32 ${SRCS})
endif()

# Link to the actual SDL3 library.
target_link_libraries(game PRIVATE SDL3_mixer::SDL3_mixer SDL3_ttf::SDL3_ttf SDL3_image::SDL3_image SDL3::SDL3)
