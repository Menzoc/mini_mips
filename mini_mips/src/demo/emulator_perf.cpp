
#include "../helpers/log.h"
#include "../emulator/binary_processor.h"
#include "../asm2bin/assembly_converter.h"

#include <filesystem>
#include <chrono>

log_struct LOGGING_CONFIG = {};

int main() {
    LOGGING_CONFIG.level = LOG_ERROR;

    std::int32_t value = 5;
    data_memory.fill(value);

    std::filesystem::path asm_file_1 = "../../assembly_files/assembly_file_1.o";
    std::filesystem::path asm_file_2 = "../../assembly_files/assembly_file_2.o";

    AssemblyFileConverter asm2bin_asm1("../../assembly_files/assembly_file_1.asm");
    AssemblyFileConverter asm2bin_asm2("../../assembly_files/assembly_file_2.asm");

    std::uint32_t instructions_asm1_rawmem = 0;
    std::uint32_t instructions_asm1_cahmem = 0;
    std::uint32_t instructions_asm2_rawmem = 0;

    // FIRST TEST - ASM_1 - WITHOUT CACHE
    MemoryProcessor asm_file_1_memory(false, data_memory);
    BinaryFileProcessor asm_file_1_mem_processor(asm_file_1, asm_file_1_memory, register_memory);

    auto end = std::chrono::steady_clock::now();
    auto start = std::chrono::steady_clock::now();
    while (std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() < 1000000) {
        asm_file_1_mem_processor.execute_prog();
        instructions_asm1_rawmem += 5;
        end = std::chrono::steady_clock::now();
    }

    //SECOND TEST - ASM_1 - WITH CACHE
    MemoryProcessor asm_file_1_cache(true, data_memory);
    BinaryFileProcessor asm_file_1_cache_processor(asm_file_1, asm_file_1_cache, register_memory);

    end = std::chrono::steady_clock::now();
    start = std::chrono::steady_clock::now();
    while (std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() < 1000000) {
        asm_file_1_cache_processor.execute_prog();
        instructions_asm1_cahmem += 5;
        end = std::chrono::steady_clock::now();
    }

    //THIRD TEST - ASM_2 - WITH ACCESS TO MEMORY
    MemoryProcessor asm_file_2_memory(false, data_memory);
    BinaryFileProcessor asm_file_2_mem_processor(asm_file_2, asm_file_2_memory, register_memory);

    end = std::chrono::steady_clock::now();
    start = std::chrono::steady_clock::now();
    while (std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() < 1000000) {
        asm_file_2_mem_processor.execute_prog();
        instructions_asm2_rawmem += 5;
        end = std::chrono::steady_clock::now();
    }

    LOG(LOG_INFO) << "Nombre d'instruction en 1 secondes du fichier asm_1 sans cache: " << std::dec << instructions_asm1_rawmem;
    LOG(LOG_INFO) << "Nombre d'instruction en 1 secondes du fichier asm_1 avec cache: " << instructions_asm1_cahmem;
    LOG(LOG_INFO) << "Nombre d'instruction en 1 secondes du fichier asm_2 sans mem  : " << instructions_asm2_rawmem;

}