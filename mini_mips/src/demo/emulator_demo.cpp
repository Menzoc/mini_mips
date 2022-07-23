
#include "../helpers/log.h"
#include "../emulator/binary_processor.h"

#include <filesystem>
#include <bitset>

log_struct LOGGING_CONFIG = {};

int main() {
    LOGGING_CONFIG.level = LOG_DEBUG;

    // Fill all memory values with a value
    std::int32_t value = 5;
    data_memory.fill(value);

    std::filesystem::path fileName = "../../assembly_files/space_invader_animation.o";
    //std::filesystem::path fileName = "../../assembly_files/screen_test.o";
    //std::filesystem::path fileName = "../../assembly_files/syracuse.o";
    //std::filesystem::path fileName = "../../assembly_files/chenillard.o";
    //std::filesystem::path fileName = "../../assembly_files/matrix_3x3.o";
    //std::filesystem::path fileName = "../../assembly_files/bressenham_line.o";

    // Test the program given in fileName with a direct mapped cache
    MemoryProcessor mem_processor(true, data_memory);
    LOG(LOG_INFO) << mem_processor;

    // Parse and execute program
    BinaryFileProcessor binary_file_proc_cache(fileName, mem_processor, register_memory);
    binary_file_proc_cache.execute_prog();

    std::uint32_t index = 0;
    for (std::int32_t each_data: data_memory) {
        index++;
        if (each_data != value) {
            LOG(LOG_INFO) << "Data modified-> data_memory[" << index << "] :" << each_data;
        }
    }

    // Print latency comparaison
    std::uint32_t latency_cache = binary_file_proc_cache.get_latency();
    LOG(LOG_INFO) << "Latency with cache memory: " << latency_cache;

    return 0;
}