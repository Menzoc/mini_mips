
#include "assembly_converter.h"

/*
 * AssemblyFileConverter class definition
 */

/* PUBLIC DEFINITION */
AssemblyFileConverter::AssemblyFileConverter(std::filesystem::path assembly_file_path) : assembly_file_path(std::move(assembly_file_path)) {
    this->instructions.clear();
    this->labels.clear();
    this->unparsed_line.clear();
}

const std::filesystem::path &AssemblyFileConverter::get_assembly_file_path() const {
    return assembly_file_path;
}


const std::map<std::uint32_t, std::uint32_t> &AssemblyFileConverter::get_instructions() const {
    return instructions;
}

std::ostream &operator<<(std::ostream &os, const AssemblyFileConverter &converter) {
    os << "Original assembly file " << converter.get_assembly_file_path().string() << " contain " << converter.get_instructions().size() << " instructions :" << std::endl;
    for (auto pair: converter.get_instructions()) {
        os << "0x" << std::hex << std::setfill('0') << std::setw(8) << pair.first << "\t0x" << std::setfill('0') << std::setw(8) << pair.second << std::dec << std::endl;
    }
    return os;
}

void AssemblyFileConverter::convert_file_to_binary(const std::filesystem::path &binary_file_path) {
    // Converting assembly file parameters to binary
    LOG(LOG_DEBUG) << "Converting file " << this->assembly_file_path.string() << " content to binary...";
    this->convert_to_binary();

    // Writing the new binary file
    std::filesystem::path file_path;
    if (binary_file_path.empty()) {
        file_path = this->assembly_file_path;
        file_path.replace_extension(".o");
    } else {
        file_path = binary_file_path;
    }

    LOG(LOG_DEBUG) << "Writing binary converted file to " << file_path.string() << "...";
    this->write_binary_file(file_path);
}

/* PRIVATE DEFINITION */
void AssemblyFileConverter::convert_to_binary() {
    std::string line = {};

    // Check if assembly file can be opened
    std::ifstream input_file(this->assembly_file_path);
    if (!input_file.is_open()) {
        LOG(LOG_ERROR) << "Error : file with path " + this->assembly_file_path.string() + "  not found.";
        throw std::filesystem::filesystem_error("Can't open file!", std::make_error_code(std::errc::no_such_file_or_directory));
    }

    // Parse all file lines
    std::uint32_t instruction_index = 0;
    while (std::getline(input_file, line)) {
        LOG(LOG_DEBUG) << "Parsing line " << line;
        AssemblyInstructionConverter asm_ins_c(line);

        // Check if a new label as been declared
        if (!asm_ins_c.get_label().empty()) {
            if (this->labels.contains(asm_ins_c.get_label())) {
                LOG(LOG_ERROR) << "Error : got a new label definition of an already existing label (" << asm_ins_c.get_label() << ").";
                throw std::invalid_argument("Label redefinition!");
            } else {
                this->labels.insert(std::make_pair(asm_ins_c.get_label(), instruction_index));
                LOG(LOG_DEBUG) << "new label added (" << asm_ins_c.get_label() << ") for instruction at index: " << std::hex << instruction_index << std::dec;
            }
        }

        try {
            std::uint32_t converted_instruction = asm_ins_c.convert_to_binary(this->labels);
            LOG(LOG_DEBUG) << "assembly instruction: " << asm_ins_c << ", converted instruction: " << std::hex << converted_instruction << std::dec;
            this->instructions.insert(std::make_pair(instruction_index, converted_instruction));
            instruction_index++;
        } catch (const UnknownLabelException &e) {
            LOG(LOG_DEBUG) << "assembly instruction using currently unknown label, saving it to parse later";
            this->unparsed_line.insert(std::make_pair(instruction_index, asm_ins_c));
            instruction_index++;
        } catch (const EmptyAssemblyLineException &e) {
            LOG(LOG_DEBUG) << "no assembly instruction on this line";
            // Do nothing
        }
    }

    // Parse again unparsed lines now that all labels should have been seen
    for (std::pair<std::uint32_t, AssemblyInstructionConverter> unparsed_pair: this->unparsed_line) {
        std::uint32_t converted_instruction = unparsed_pair.second.convert_to_binary(this->labels);
        this->instructions.insert(std::make_pair(unparsed_pair.first, converted_instruction));
        LOG(LOG_DEBUG) << "assembly instruction: " << unparsed_pair.second << ", converted instruction: " << std::hex << converted_instruction << std::dec;
    }

    input_file.close();
}

void AssemblyFileConverter::write_binary_file(const std::filesystem::path &binary_file_path) {
    // Open the binary file and write all parameters index and parameters values in 32 bits blocks
    std::ofstream output_file(binary_file_path, std::ios::binary);
    for (auto pair: this->instructions) {
        constexpr bool isBigEndian = (std::endian::native == std::endian::big);
        uint32_t instruction_index = pair.first;
        // Convert to network endianness (big endian) if needed then write the file
        if (!isBigEndian) {
            uint32_t b0, b1, b2, b3;
            b0 = (instruction_index & 0x000000ff) << 24u;
            b1 = (instruction_index & 0x0000ff00) << 8u;
            b2 = (instruction_index & 0x00ff0000) >> 8u;
            b3 = (instruction_index & 0xff000000) >> 24u;
            instruction_index = b0 | b1 | b2 | b3;
        }
        std::vector<char> instruction_buf_index;
        instruction_buf_index.push_back((instruction_index) & 0xFF);
        instruction_buf_index.push_back((instruction_index >> 8) & 0xFF);
        instruction_buf_index.push_back((instruction_index >> 16) & 0xFF);
        instruction_buf_index.push_back((instruction_index >> 24) & 0xFF);
        output_file.write(instruction_buf_index.data(), (long) instruction_buf_index.size());

        uint32_t instruction = pair.second;
        // Convert to network endianness (big endian) if needed then write the file
        if (!isBigEndian) {
            uint32_t b0, b1, b2, b3;
            b0 = (instruction & 0x000000ff) << 24u;
            b1 = (instruction & 0x0000ff00) << 8u;
            b2 = (instruction & 0x00ff0000) >> 8u;
            b3 = (instruction & 0xff000000) >> 24u;
            instruction = b0 | b1 | b2 | b3;
        }
        std::vector<char> instruction_buf;
        instruction_buf.push_back((instruction) & 0xFF);
        instruction_buf.push_back((instruction >> 8) & 0xFF);
        instruction_buf.push_back((instruction >> 16) & 0xFF);
        instruction_buf.push_back((instruction >> 24) & 0xFF);
        output_file.write(instruction_buf.data(), (long) instruction_buf.size());
    }

    output_file.close();
}


/*
 * AssemblyInstructionConverter class definition
 */

/* PUBLIC DEFINITION */
AssemblyInstructionConverter::AssemblyInstructionConverter(const std::string &assembly_line) {
    this->opcode = OperatorCode::UNKNOWN;
    this->label = "";
    this->parameters.clear();
    this->parse_line(assembly_line);
}

OperatorCode AssemblyInstructionConverter::get_opcode() const {
    return opcode;
}

const std::vector<std::string> &AssemblyInstructionConverter::get_parameters() const {
    return parameters;
}

const std::string &AssemblyInstructionConverter::get_label() const {
    return label;
}

std::ostream &operator<<(std::ostream &os, const AssemblyInstructionConverter &converter) {
    os << "opcode=" << converter.get_opcode() << ", label= " << converter.get_label() << ", parameters[" << converter.get_parameters().size() << "]=(";
    for (std::size_t i = 0; i < converter.get_parameters().size(); i++) {
        if (i < converter.get_parameters().size() - 1) {
            os << converter.get_parameters().at(i) << ", ";
        } else {
            os << converter.get_parameters().at(i) << ")";
        }
    }
    return os;
}

bool AssemblyInstructionConverter::is_empty() {
    return (this->opcode == OperatorCode::UNKNOWN);
}

std::uint32_t AssemblyInstructionConverter::convert_to_binary(const std::map<std::string, uint32_t> &existing_labels) {

    if (this->is_empty()) {
        throw EmptyAssemblyLineException("Empty assembly line!");
    }

    std::uint32_t instruction = 0;

    if ((std::uint32_t) this->opcode > this->MAX_OPCODE_LEN) {
        LOG(LOG_ERROR) << "Error : got a too long operator code (" << this->opcode << ").";
        throw std::invalid_argument("Too long operator code!");
    }
    // Convert opcode
    instruction += (int) this->opcode << 27;

    // Convert parameters
    switch (this->opcode) {
        case OperatorCode::STOP  : {
            if (!this->parameters.empty()) {
                LOG(LOG_ERROR) << "Error : instruction with opcode " << this->opcode << " is expected to have 0 parameters but got " << this->parameters.size() << ".";
                throw std::invalid_argument("Wrong number of instructions arguments!");
            }

            // Do nothing
            break;
        }
        case OperatorCode::ADD ... OperatorCode::STORE : {
            // Check number of arguments
            if (this->parameters.size() != 3) {
                LOG(LOG_ERROR) << "Error : instruction with opcode " << this->opcode << " is expected to have 3 parameters but got " << this->parameters.size() << ".";
                throw std::invalid_argument("Wrong number of instructions arguments!");
            }

            // r1 is always a register index
            std::pair<ParameterType, std::uint32_t> r1 = AssemblyInstructionConverter::parse_parameter(this->parameters.at(0), existing_labels);
            switch (r1.first) {
                case ParameterType::REGISTER: {
                    if (r1.second > this->MAX_R_LEN) {
                        LOG(LOG_ERROR) << "Error : got a too long register (" << r1.second << ") max value is " << this->MAX_R_LEN << " where parameter value is " << r1.second << ".";
                        throw std::invalid_argument("Too long register!");
                    }
                    instruction += r1.second << 22;
                    break;
                }
                default: {
                    LOG(LOG_ERROR) << "Error : got a r1 instruction parameter of type " << r1.first << " but should be of type " << ParameterType::REGISTER << ".";
                    throw std::invalid_argument("Wrong instruction parameter type!");
                    break;
                }
            }

            // o is either a register index or a value
            std::pair<ParameterType, std::uint32_t> o = AssemblyInstructionConverter::parse_parameter(this->parameters.at(1), existing_labels);
            switch (o.first) {
                case ParameterType::REGISTER: {
                    if (o.second > this->MAX_R_LEN) {
                        LOG(LOG_ERROR) << "Error : got a too long register (" << o.second << ") max value is " << this->MAX_R_LEN << " where parameter value is " << o.second << ".";
                        throw std::invalid_argument("Too long register!");
                    }
                    instruction += 0 << 21;
                    break;
                }
                case ParameterType::VALUE: {
                    if (o.second > this->MAX_SHORT_O_LEN) {
                        LOG(LOG_ERROR) << "Error : got a too long value (" << o.second << ") max value is " << this->MAX_SHORT_O_LEN << " where parameter value is " << o.second << ".";
                        throw std::invalid_argument("Too long value!");
                    }
                    instruction += 1 << 21;
                    break;
                }
                default: {
                    LOG(LOG_ERROR) << "Error : got an o instruction parameter of type " << o.first << " but should be of type " << ParameterType::REGISTER << " or " << ParameterType::VALUE << ".";
                    throw std::invalid_argument("Wrong instruction parameter type!");
                    break;
                }

            }
            instruction += o.second << 5;

            // r2 is always a register index
            std::pair<ParameterType, std::uint32_t> r2 = AssemblyInstructionConverter::parse_parameter(this->parameters.at(2), existing_labels);
            switch (r2.first) {
                case ParameterType::REGISTER: {
                    if (r2.second > this->MAX_R_LEN) {
                        LOG(LOG_ERROR) << "Error : got a too long register (" << r2.second << ") max value is " << this->MAX_R_LEN << " where parameter value is " << r2.second << ".";
                        throw std::invalid_argument("Too long register!");
                    }
                    instruction += r2.second;
                    break;
                }
                default: {
                    LOG(LOG_ERROR) << "Error : got a r2 instruction parameter of type " << r2.first << " but should be of type " << ParameterType::REGISTER << ".";
                    throw std::invalid_argument("Wrong instruction parameter type!");
                    break;
                }
            }

            break;
        }
        case OperatorCode::JMP : {
            // Check number of arguments
            if (this->parameters.size() != 2) {
                LOG(LOG_ERROR) << "Error : instruction with opcode " << this->opcode << " is expected to have 2 parameters but got " << this->parameters.size() << ".";
                throw std::invalid_argument("Wrong number of instructions arguments!");
            }

            // o is either a register index, a value or a label (currently known or not)
            std::pair<ParameterType, std::uint32_t> o = AssemblyInstructionConverter::parse_parameter(this->parameters.at(0), existing_labels);
            switch (o.first) {
                case ParameterType::REGISTER: {
                    if (o.second > this->MAX_R_LEN) {
                        LOG(LOG_ERROR) << "Error : got a too long register (" << o.second << ") max value is " << this->MAX_R_LEN << " where parameter value is " << o.second << ".";
                        throw std::invalid_argument("Too long register!");
                    }
                    instruction += 0 << 26;
                    break;
                }
                case ParameterType::VALUE: {
                    if (o.second > this->MAX_LONG_O_LEN) {
                        LOG(LOG_ERROR) << "Error : got a too long value (" << o.second << ") max value is " << this->MAX_LONG_O_LEN << " where parameter value is " << o.second << ".";
                        throw std::invalid_argument("Too long value!");
                    }
                    instruction += 1 << 26;
                    break;
                }
                case ParameterType::KNOWN_LABEL: {
                    if (o.second > this->MAX_LONG_O_LEN) {
                        LOG(LOG_ERROR) << "Error : got a too long value (" << o.second << ") max value is " << this->MAX_LONG_O_LEN << " where parameter value is " << o.second << ".";
                        throw std::invalid_argument("Too long value!");
                    }
                    instruction += 1 << 26;
                    break;
                }
                case ParameterType::UNKNOWN_LABEL: {
                    throw UnknownLabelException("Unknown label!");
                    break;
                }
                default: {
                    LOG(LOG_ERROR) << "Error : got an o instruction of type " << o.first << " but should be of type " << ParameterType::REGISTER << ", " << ParameterType::VALUE << ", " << ParameterType::KNOWN_LABEL << " or " << ParameterType::UNKNOWN_LABEL << ".";
                    throw std::invalid_argument("Wrong instruction type!");
                    break;
                }
            }
            instruction += o.second << 5;
            // r is always a register index
            std::pair<ParameterType, std::uint32_t> r = AssemblyInstructionConverter::parse_parameter(this->parameters.at(1), existing_labels);
            switch (r.first) {
                case ParameterType::REGISTER: {
                    if (r.second > this->MAX_R_LEN) {
                        LOG(LOG_ERROR) << "Error : got a too long register (" << r.second << ") max value is " << this->MAX_R_LEN << " where parameter value is " << r.second << ".";
                        throw std::invalid_argument("Too long register!");
                    }
                    instruction += r.second;
                    break;
                }
                default: {
                    LOG(LOG_ERROR) << "Error : got a r instruction of type " << r.first << " but should be of type " << ParameterType::REGISTER << ".";
                    throw std::invalid_argument("Wrong instruction type!");
                    break;
                }
            }

            break;
        }
        case OperatorCode::BRAZ :
        case OperatorCode::BRANZ : {
            // Check number of arguments
            if (this->parameters.size() != 2) {
                LOG(LOG_ERROR) << "Error : instruction with opcode " << this->opcode << " is expected to have 2 parameters but got " << this->parameters.size() << ".";
                throw std::invalid_argument("Wrong number of instructions arguments!");
            }

            // r is always a register index
            std::pair<ParameterType, std::uint32_t> r = AssemblyInstructionConverter::parse_parameter(this->parameters.at(0), existing_labels);
            switch (r.first) {
                case ParameterType::REGISTER: {
                    if (r.second > this->MAX_R_LEN) {
                        LOG(LOG_ERROR) << "Error : got a too long register (" << r.second << ") max value is " << this->MAX_R_LEN << " where parameter value is " << r.second << ".";
                        throw std::invalid_argument("Too long register!");
                    }
                    instruction += r.second << 22;
                    break;
                }
                default: {
                    LOG(LOG_ERROR) << "Error : got a r instruction of type " << r.first << " but should be of type " << ParameterType::REGISTER << ".";
                    throw std::invalid_argument("Wrong instruction type!");
                    break;
                }
            }

            // a is either a value or a label (currently known or not)
            std::pair<ParameterType, std::uint32_t> a = AssemblyInstructionConverter::parse_parameter(this->parameters.at(1), existing_labels);
            switch (a.first) {
                case ParameterType::VALUE: {
                    if (a.second > this->MAX_A_LEN) {
                        LOG(LOG_ERROR) << "Error : got a too long value (" << a.second << ") max value is " << this->MAX_A_LEN << " where parameter value is " << a.second << ".";
                        throw std::invalid_argument("Too long value!");
                    }
                    instruction += a.second;
                    break;
                }
                case ParameterType::KNOWN_LABEL: {
                    if (a.second > this->MAX_A_LEN) {
                        LOG(LOG_ERROR) << "Error : got a too long value (" << a.second << ") max value is " << this->MAX_A_LEN << " where parameter value is " << a.second << ".";
                        throw std::invalid_argument("Too long value!");
                    }
                    instruction += a.second;
                    break;
                }
                case ParameterType::UNKNOWN_LABEL: {
                    throw UnknownLabelException("Unknown label!");
                    break;
                }
                default: {
                    LOG(LOG_ERROR) << "Error : got an a instruction of type " << a.first << " but should be of type " << ParameterType::VALUE << ", " << ParameterType::KNOWN_LABEL << " or " << ParameterType::UNKNOWN_LABEL << ".";
                    throw std::invalid_argument("Wrong instruction type!");
                    break;
                }
            }
            break;
        }
        case OperatorCode::SCALL : {
            // Check number of arguments
            if (this->parameters.size() != 1) {
                LOG(LOG_ERROR) << "Error : instruction with opcode " << this->opcode << " is expected to have 1 parameters but got " << this->parameters.size() << ".";
                throw std::invalid_argument("Wrong number of instructions arguments!");
            }

            // n is always a register index
            std::pair<ParameterType, std::uint32_t> n = AssemblyInstructionConverter::parse_parameter(this->parameters.at(0), existing_labels);
            switch (n.first) {
                case ParameterType::VALUE: {
                    if (n.second > this->MAX_N_LEN) {
                        LOG(LOG_ERROR) << "Error : got a too long value (" << n.second << ") max value is " << this->MAX_N_LEN << " where parameter value is " << n.second << ".";
                        throw std::invalid_argument("Too long value!");
                    }
                    instruction += n.second;
                    break;
                }
                default: {
                    LOG(LOG_ERROR) << "Error : got a n instruction of type " << n.first << " but should be of type " << ParameterType::VALUE << ".";
                    throw std::invalid_argument("Wrong instruction type!");
                    break;
                }
            }

            break;
        }
        default: {
            LOG(LOG_ERROR) << "Error : the operator code value usage is not defined in the project.";
            throw std::domain_error("Unsupported opcode!");
            break;
        }
    }

    return instruction;
}


/* PRIVATE DEFINITION */
void AssemblyInstructionConverter::parse_line(const std::string &assembly_line) {
    // Extraction of several sub-matches
    const std::regex assembly_line_regex(ASSEMBLY_LINE_REGEX);
    std::smatch matches;
    // Apply regex to the line
    if (std::regex_match(assembly_line, matches, assembly_line_regex)) {
        // do not process match at index 0 (whole line)

        // process match at index 1 (label)
        if (matches[1].matched) {
            if (AssemblyInstructionConverter::test_parameter_type(matches[1].str()) != ParameterType::LABEL) {
                LOG(LOG_ERROR) << "Error :  label " << matches[1].str() << " is ill formatted and may contain unauthorized characters.";
                throw std::invalid_argument("Ill formatted label!");
            }
            this->label = matches[1].str();
        } else {
            this->label = "";
        }

        // process match at index 2 (operator code)
        if (matches[2].matched) {
            // Transform opcode to uppercase
            std::string opcode_str = matches[2].str();
            std::transform(opcode_str.begin(), opcode_str.end(), opcode_str.begin(), [](unsigned char c) { return std::toupper(c); });
            // Check if the found opcode is supported
            if (OPERATOR_CODE_STR.contains(opcode_str)) {
                this->opcode = OPERATOR_CODE_STR.at(opcode_str);
            } else {
                this->opcode = OperatorCode::UNKNOWN;
            }
        }

        // process match at index 3 to 5 (parameters 1 to 3)
        for (std::size_t i = 3; i <= 5; i++) {
            if (matches[i].matched) {
                std::string parameter_str = matches[i].str();
                // Transform parameter to uppercase only if it is not a label
                if (AssemblyInstructionConverter::test_parameter_type(matches[i].str()) != ParameterType::LABEL) {
                    std::transform(parameter_str.begin(), parameter_str.end(), parameter_str.begin(), [](unsigned char c) { return std::toupper(c); });
                }
                this->parameters.push_back(parameter_str);
            }
        }
    }
}

std::pair<ParameterType, uint32_t> AssemblyInstructionConverter::parse_parameter(const std::string &parameter, const std::map<std::string, uint32_t> &existing_labels) {
    std::pair<ParameterType, std::uint32_t> instruction;
    ParameterType parameter_type = AssemblyInstructionConverter::test_parameter_type(parameter, existing_labels);
    switch (parameter_type) {
        case ParameterType::REGISTER: {
            instruction = std::make_pair(ParameterType::REGISTER, (std::uint32_t) std::stoi(parameter.substr(1, parameter.size())));
            break;
        }
        case ParameterType::VALUE: {
            instruction = std::make_pair(ParameterType::VALUE, (std::uint32_t) std::stoi(parameter));
            break;
        }
        case ParameterType::KNOWN_LABEL: {
            instruction = std::make_pair(ParameterType::KNOWN_LABEL, existing_labels.at(parameter));
            break;
        }
        case ParameterType::UNKNOWN_LABEL: {
            instruction = std::make_pair(ParameterType::UNKNOWN_LABEL, 0);
            break;
        }
        default: {
            LOG(LOG_ERROR) << "Error : parameter " << parameter << " of type " << parameter_type << " is not supported.";
            throw std::invalid_argument("Unsupported parameter!");
            break;
        }
    }

    LOG(LOG_DEBUG) << "Parsed instruction is a " << parameter_type << " (" << parameter << ") : (hex=" << std::hex << instruction.second << ", bin=" << std::bitset<32>(instruction.second) << ", dec=" << std::dec << instruction.second << ")";

    return instruction;
}

ParameterType AssemblyInstructionConverter::test_parameter_type(const std::string &parameter) {
    const char REGISTER_CHAR = 'R';
    const std::string LETTERS = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    const std::string NUMBERS = "0123456789";
    const std::string LABEL_SPECIFIC_CHARACTERS = "_";

    ParameterType parameter_type;

    // if the parameter string contain only numbers, it is a value
    // else if the parameter string first character is the REGISTER_CHAR and the rest are numbers, it is a register
    // else if the parameter string first character is not a number and the rest are characters allowed in a label definition, it is a label
    // else it is unknown
    if (std::all_of(parameter.cbegin(), parameter.cend(), [&NUMBERS](char character) -> bool { return (NUMBERS.find(character) != std::string::npos); })) {
        parameter_type = ParameterType::VALUE;
    } else if (toupper(parameter.at(0)) == REGISTER_CHAR
               && std::all_of(parameter.cbegin() + 1, parameter.cend(), [&NUMBERS](char character) -> bool { return (NUMBERS.find(character) != std::string::npos); })) {
        parameter_type = ParameterType::REGISTER;
    } else if (!std::any_of(NUMBERS.cbegin(), NUMBERS.cend(), [&parameter](char number) -> bool { return (parameter.at(0) == number); })
               && std::all_of(parameter.cbegin() + 1, parameter.cend(), [&NUMBERS, &LETTERS, &LABEL_SPECIFIC_CHARACTERS](char character) -> bool { return (NUMBERS.find(character) != std::string::npos || LETTERS.find(character) != std::string::npos || LABEL_SPECIFIC_CHARACTERS.find(character) != std::string::npos); })) {
        parameter_type = ParameterType::LABEL;
    } else {
        parameter_type = ParameterType::UNKNOWN;
    }


    return parameter_type;
}

ParameterType AssemblyInstructionConverter::test_parameter_type(const std::string &parameter, const std::map<std::string, uint32_t> &existing_labels) {
    ParameterType parameter_type = AssemblyInstructionConverter::test_parameter_type(parameter);
    if (parameter_type == ParameterType::LABEL) {
        if (existing_labels.contains(parameter)) {
            parameter_type = ParameterType::KNOWN_LABEL;
        } else {
            parameter_type = ParameterType::UNKNOWN_LABEL;
        }
    }

    return parameter_type;
}
