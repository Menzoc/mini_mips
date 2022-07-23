
#include "../helpers/globals.h"
#include "../helpers/log.h"
#include "../asm2bin/assembly_converter.h"

#include <execution>
#include <bitset>

log_struct LOGGING_CONFIG = {};

int main() {
    LOGGING_CONFIG.level = LOG_DEBUG;

    std::filesystem::path fileName = "../../assembly_files/space_invader_animation.asm";
    //std::filesystem::path fileName = "../../assembly_files/screen_test.asm";
    //std::filesystem::path fileName = "../../assembly_files/syracuse.asm";
    //std::filesystem::path fileName = "../../assembly_files/chenillard.asm";
    //std::filesystem::path fileName = "../../assembly_files/matrix_3x3.asm";
    //std::filesystem::path fileName = "../../assembly_files/bressenham_line.asm";

    AssemblyFileConverter asm2bin(fileName);
    asm2bin.convert_file_to_binary();
    LOG(LOG_INFO) << asm2bin;

    return 0;
}

