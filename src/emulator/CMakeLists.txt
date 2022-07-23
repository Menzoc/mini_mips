add_executable(emulator emulator.cpp binary_processor.cpp memory_processor.cpp bitmap_screen_renderer.cpp)
target_link_libraries(emulator ${SDL2_LIBRARIES})