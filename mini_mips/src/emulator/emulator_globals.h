#ifndef EMULATOR_GLOBALS_H
#define EMULATOR_GLOBALS_H

#include <array>
#include <string>

static const std::uint32_t REGISTER_MEMORY_SIZE = 32;
static const std::uint32_t RAM_MEMORY_BITMAP_SCREEN_WIDTH = 100;
static const std::uint32_t RAM_MEMORY_BITMAP_SCREEN_HEIGHT = 100;
static const std::uint32_t RAM_MEMORY_SIZE = RAM_MEMORY_BITMAP_SCREEN_WIDTH * RAM_MEMORY_BITMAP_SCREEN_HEIGHT + 1000;

// Ram data memory
static std::array<std::int32_t, RAM_MEMORY_SIZE> data_memory{};
static std::array<std::int32_t, REGISTER_MEMORY_SIZE> register_memory{};

#endif //EMULATOR_GLOBALS_H
