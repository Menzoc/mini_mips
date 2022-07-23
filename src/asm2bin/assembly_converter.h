
#ifndef ASSEMBLY_CONVERTER_H
#define ASSEMBLY_CONVERTER_H

#include "asm2bin_globals.h"
#include "../helpers/globals.h"
#include "../helpers/log.h"
#include "../helpers/print_helpers.h"
#include <map>
#include <utility>
#include <fstream>
#include <filesystem>
#include <string>
#include <vector>
#include <math.h>
#include <bitset>
#include <regex>

class AssemblyInstructionConverter {
public:
    explicit AssemblyInstructionConverter(const std::string &assembly_line);

    OperatorCode get_opcode() const;

    const std::vector<std::string> &get_parameters() const;

    const std::string &get_label() const;

    friend std::ostream &operator<<(std::ostream &os, const AssemblyInstructionConverter &converter);

    bool is_empty();

    std::uint32_t convert_to_binary(const std::map<std::string, uint32_t> &existing_labels);

private:
    const std::uint32_t MAX_OPCODE_LEN = pow(2, 5); /* 5 bits */
    const std::uint32_t MAX_R_LEN = pow(2, 5); /* 5 bits */
    const std::uint32_t MAX_SHORT_O_LEN = pow(2, 16); /* 16 bits */
    const std::uint32_t MAX_LONG_O_LEN = pow(2, 21); /* 16 bits */
    const std::uint32_t MAX_A_LEN = pow(2, 22); /* 16 bits */
    const std::uint32_t MAX_N_LEN = pow(2, 27); /* 27 bits */

    std::string label;
    OperatorCode opcode;
    std::vector<std::string> parameters;

    void parse_line(const std::string &assembly_line);

    static std::pair<ParameterType, uint32_t> parse_parameter(const std::string &parameter, const std::map<std::string, uint32_t> &existing_labels);

    static ParameterType test_parameter_type(const std::string& parameter);

    static ParameterType test_parameter_type(const std::string& parameter, const std::map<std::string, uint32_t> &existing_labels);
};


class AssemblyFileConverter {
public:
    explicit AssemblyFileConverter(std::filesystem::path assembly_file_path);

    const std::filesystem::path &get_assembly_file_path() const;

    const std::map<std::uint32_t, std::uint32_t> &get_instructions() const;

    friend std::ostream &operator<<(std::ostream &os, const AssemblyFileConverter &converter);

    void convert_file_to_binary(const std::filesystem::path &binary_file_path = "");

private:
    std::filesystem::path assembly_file_path;
    std::map<std::uint32_t, std::uint32_t> instructions;
    std::map<std::string, std::uint32_t> labels;
    std::map<std::uint32_t, AssemblyInstructionConverter> unparsed_line;

    void convert_to_binary();

    void write_binary_file(const std::filesystem::path &binary_file_path);
};


class UnknownLabelException : public std::logic_error {
    using std::logic_error::logic_error;
};


class EmptyAssemblyLineException : public std::logic_error {
    using std::logic_error::logic_error;
};

#endif //ASSEMBLY_CONVERTER_H
