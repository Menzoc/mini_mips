#include "../helpers/option_parser.h"
#include "../helpers/log.h"
#include "../emulator/binary_processor.h"
#include <filesystem>

log_struct LOGGING_CONFIG = {};

int main(int argc, const char *argv[]) {
    optpars::OptionParser options_parser;

    options_parser.overview = "Tool to emulate a minimal MIPS processor using a given binary file";
    options_parser.syntax = "emulator [OPTIONS] binary_file";
    options_parser.example = "emulator my_file.o";

    options_parser.add(
            "", // Default.
            false, // Required?
            0, // Number of args expected.
            0, // Delimiter if expecting multiple args.
            "Display usage instructions.", // Help description.
            "-h",     // Flag token.
            "-help",  // Flag token.
            "--help", // Flag token.
            "--usage" // Flag token.
    );

    options_parser.add(
            "1",
            false,
            1,
            0,
            "Print debug informations.\n{0=ERROR, 1=WARNING (default), 2=INFO, 3=DEBUG}",
            "-v",
            "--verbose"
    );

    options_parser.add(
            "",
            false,
            0,
            0,
            "Do not use a cache.",
            "--no-cache"
    );


    options_parser.parse(argc, argv);

    std::string usage;
    options_parser.getUsage(usage);

    if (options_parser.isSet("-h")) {
        std::cout << usage << std::endl;
        return 1;
    }

    if (options_parser.lastArgs.size() != 1) {
        std::cerr << "ERROR: Expected only one arguments.\n\n";
        std::cout << usage << std::endl;
        return 1;
    }

    int verbose_level;
    if (options_parser.isSet("--verbose")) {
        options_parser.get("--verbose")->getInt(verbose_level);
        switch (verbose_level) {
            case 0: {
                LOGGING_CONFIG.level = LOG_ERROR;
                break;
            }
            case 1: {
                LOGGING_CONFIG.level = LOG_WARNING;
                break;
            }
            case 2: {
                LOGGING_CONFIG.level = LOG_INFO;
                break;
            }
            case 3: {
                LOGGING_CONFIG.level = LOG_DEBUG;
                break;
            }
            default: {
                LOGGING_CONFIG.level = LOG_WARNING;
                break;
            }
        }
    } else {
        LOGGING_CONFIG.level = LOG_WARNING;
    }

    bool use_cache = true;
    if (options_parser.isSet("--no-cache")) {
        use_cache = false;
    }

    std::vector<std::string> badOptions;
    if (!options_parser.gotRequired(badOptions)) {
        for (auto &badOption: badOptions) {
            std::cerr << "ERROR: Missing required option " << badOption << ".\n\n";
        }
        std::cout << usage << std::endl;
        return 1;
    }

    if (!options_parser.gotExpected(badOptions)) {
        for (auto &badOption: badOptions) {
            std::cerr << "ERROR: Got unexpected number of arguments for option " << badOption << ".\n\n";
        }
        std::cout << usage << std::endl;
        return 1;
    }

    std::filesystem::path binary_file_name = {options_parser.lastArgs.at(0)[0]};
    LOG(LOG_DEBUG) << "Executing: " << binary_file_name.string();

    // Initialize memory with 0
    data_memory.fill(0);
    register_memory.fill(0);

    MemoryProcessor cache_processor(use_cache, data_memory);
    LOG(LOG_INFO) << cache_processor;

    // Parse and execute program
    BinaryFileProcessor binary_file_processor(binary_file_name, cache_processor, register_memory);
    binary_file_processor.execute_prog();

    // Print latency comparaison
    std::uint32_t latency_mem = binary_file_processor.get_latency();
    LOG(LOG_INFO) << "Done -- latency: " << latency_mem;

    return 0;
}