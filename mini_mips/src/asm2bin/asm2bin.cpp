#include "../helpers/option_parser.h"
#include "../helpers/log.h"
#include "../asm2bin/assembly_converter.h"
#include <filesystem>

log_struct LOGGING_CONFIG = {};

int main(int argc, const char *argv[]) {
    optpars::OptionParser options_parser;

    options_parser.overview = "Tool to convert a assembly file to a binary file usable by a custom virtual machine";
    options_parser.syntax = "asm2bin [OPTIONS] assembly_file";
    options_parser.example = "asm2bin -o ./dir/output_file.bin my_file.asm";

    options_parser.add(
            "", // Default.
            0, // Required?
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
            0,
            1,
            0,
            "Print debug informations.\n{0=ERROR, 1=WARNING (default), 2=INFO, 3=DEBUG}",
            "-v",
            "--verbose"
    );

    options_parser.add(
            "",
            0,
            1,
            0,
            "Use the given file as output",
            "-o",
            "--output"
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

    std::string output_file_path;
    if (options_parser.isSet("--output")) {
        options_parser.get("--output")->getString(output_file_path);
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

    std::filesystem::path assembly_file_name = {options_parser.lastArgs.at(0)[0]};
    LOG(LOG_DEBUG) << "parsing: " << assembly_file_name.string();
    AssemblyFileConverter asm2bin(assembly_file_name);
    if (!output_file_path.empty()) {
        asm2bin.convert_file_to_binary(output_file_path);
    } else {
        asm2bin.convert_file_to_binary();
    }
    LOG(LOG_INFO) << asm2bin;

    // "../../../assembly_files/assembly_file_1.asm"


    return 0;
}