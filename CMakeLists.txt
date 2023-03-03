cmake_minimum_required(VERSION 3.21)
project(Music_Visualizer)

set(CMAKE_CXX_STANDARD 17)

add_executable(Music_Visualizer main.cpp src/view/keys.cpp src/view/keys.hpp src/shader.cpp src/shader.hpp src/model/keyboard.hpp src/model/keyboard.cpp src/view/notes.hpp src/view/notes.cpp src/view/graph.hpp src/view/graph.cpp src/view/view.hpp src/view/view.cpp src/input/dp.hpp src/input/dp.cpp)

target_link_libraries(Music_Visualizer
        # OpenGL
        "-framework OpenGL"
        "-framework Cocoa"
        "-framework IOKit"
        # PortMidi
        "-framework CoreServices"
        "-framework CoreFoundation"
        "-framework AudioUnit"
        "-framework AudioToolbox"
        "-framework CoreAudio"
        "-framework CoreMIDI"
        )

target_include_directories(Music_Visualizer PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_directories(Music_Visualizer PRIVATE ${CMAKE_SOURCE_DIR}/lib)

target_compile_definitions(Music_Visualizer PRIVATE GLEW_STATIC)

target_link_libraries(Music_Visualizer glfw3)
target_link_libraries(Music_Visualizer GLEW)
target_link_libraries(Music_Visualizer portmidi)
