
#ifndef BINARY_PROCESSOR_H
#define BINARY_PROCESSOR_H

#include "emulator_globals.h"
#include "bitmap_screen_renderer.h"
#include "../helpers/globals.h"
#include "../helpers/log.h"
#include "../helpers/print_helpers.h"
#include "./memory_processor.h"
#include <map>
#include <bitset>
#include <fstream>
#include <filesystem>


class BinaryInstructionProcessor {
public:
    explicit BinaryInstructionProcessor(std::uint32_t address, std::uint32_t instruction, std::array<int32_t, REGISTER_MEMORY_SIZE> &register_mem, MemoryProcessor &cache_processor);

    std::uint32_t get_instruction() const;

    std::uint32_t get_code_op() const;

    std::uint32_t get_reg_alpha() const;

    std::int32_t get_reg_0() const;

    std::uint32_t get_reg_beta() const;

    std::int32_t get_imm() const;

    std::uint32_t get_a() const;

    std::int32_t get_reg_0_jmp() const;

    std::int32_t get_n() const;

    std::uint32_t get_current_addr() const;

    std::array<std::int32_t, REGISTER_MEMORY_SIZE> get_R() const;

    std::uint32_t get_latency() const;

    std::uint32_t execute(BitmapScreenRenderer &bitmap_renderer);

private:
    const std::uint32_t address;
    const std::uint32_t instruction;

    std::array<std::int32_t, REGISTER_MEMORY_SIZE> &register_mem;
    MemoryProcessor &cache_processor;

    std::uint32_t latency;

    std::uint32_t current_addr;
    std::uint32_t code_op;
    OperatorCode code_op_e;
    std::uint32_t reg_alpha;
    std::int32_t reg_0;
    std::uint32_t reg_beta;
    std::int32_t imm;
    std::uint32_t a;
    std::int32_t reg_0_jmp;
    std::int32_t n;

    void decode();
};


class BinaryFileProcessor {
public:
    explicit BinaryFileProcessor(const std::filesystem::path &fileName, MemoryProcessor &cache_processor, std::array<std::int32_t, REGISTER_MEMORY_SIZE> &register_mem);

    explicit BinaryFileProcessor(const std::filesystem::path &fileName, MemoryProcessor &cache_processor, std::array<std::int32_t, REGISTER_MEMORY_SIZE> &register_mem, std::int32_t pixel_ratio);

    std::filesystem::path get_filename() const;

    std::array<std::int32_t, RAM_MEMORY_SIZE> get_data() const;

    std::uint32_t get_current_add() const;

    std::map<std::uint32_t, std::uint32_t> get_list_inst() const;

    std::array<std::int32_t, REGISTER_MEMORY_SIZE> get_R() const;

    std::uint32_t get_latency() const;

    void execute_prog();


private:
    const std::filesystem::path &fileName;
    BitmapScreenRenderer bitmap_renderer;

    MemoryProcessor &cache_processor;
    std::map<std::uint32_t, std::uint32_t> instructions;
    std::array<std::int32_t, REGISTER_MEMORY_SIZE> &register_mem;
    std::uint32_t current_addr;
    std::uint32_t latency;

    std::map<uint32_t, uint32_t> fetch();
};

#endif //BINARY_PROCESSOR_H
