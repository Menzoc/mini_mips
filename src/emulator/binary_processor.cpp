
#include "binary_processor.h"


/*
 * BinaryInstructionProcessor class definition
 */

/* PUBLIC DEFINITION */
BinaryInstructionProcessor::BinaryInstructionProcessor(std::uint32_t address, std::uint32_t instruction, std::array<int32_t, REGISTER_MEMORY_SIZE> &register_mem, MemoryProcessor &cache_processor) : address(address), instruction(instruction), register_mem(register_mem), cache_processor(cache_processor) {

    if (instruction < 0x00000000) {
        LOG(LOG_ERROR) << "Error : instruction " << std::hex << instruction << " value is smaller than the minimum authorized value (" << 0x00000000 << ").";
        throw std::invalid_argument("Too small instruction!");
    }
    if (instruction > 0xFFFFFFFF) {
        LOG(LOG_ERROR) << "Error : instruction " << std::hex << instruction << " value is bigger than the maximum authorized value (" << 0xFFFFFFFF << ").";
        throw std::invalid_argument("Too long instruction!");
    }
    this->latency = 0;

    this->code_op = -1;
    this->code_op_e = OperatorCode::UNKNOWN;

    // use for add, mult,sub
    this->reg_alpha = -1;
    this->reg_0 = -1;
    this->reg_beta = -1;
    this->imm = -1;

    // use for braz branz
    this->a = -1;

    // use for jmp
    this->reg_0_jmp = -1;

    // use for scall
    this->n = -1;

    //  use to fixe the next instructions
    this->current_addr = address;

    decode();
}

std::uint32_t BinaryInstructionProcessor::get_instruction() const {
    return instruction;
}

std::uint32_t BinaryInstructionProcessor::get_code_op() const {
    return code_op;
}

std::uint32_t BinaryInstructionProcessor::get_reg_alpha() const {
    return reg_alpha;
}

std::int32_t BinaryInstructionProcessor::get_reg_0() const {
    return reg_0;
}

std::uint32_t BinaryInstructionProcessor::get_reg_beta() const {
    return reg_beta;
}

std::int32_t BinaryInstructionProcessor::get_imm() const {
    return imm;
}

std::uint32_t BinaryInstructionProcessor::get_a() const {
    return a;
}

std::int32_t BinaryInstructionProcessor::get_reg_0_jmp() const {
    return reg_0_jmp;
}

std::int32_t BinaryInstructionProcessor::get_n() const {
    return n;
}

std::uint32_t BinaryInstructionProcessor::get_current_addr() const {
    return current_addr;
}

std::array<std::int32_t, REGISTER_MEMORY_SIZE> BinaryInstructionProcessor::get_R() const {
    return register_mem;
}

std::uint32_t BinaryInstructionProcessor::get_latency() const {
    return latency;
}


std::uint32_t BinaryInstructionProcessor::execute(BitmapScreenRenderer &bitmap_renderer) {
    // Convention Imm vu avec prof 1→ valeur immédiates
    switch (code_op_e) {
        case OperatorCode::ADD: {
            if (imm == 0) {
                register_mem.at(reg_beta) = register_mem.at(reg_alpha) + register_mem.at(reg_0);
            } else if (imm == 1) {
                register_mem.at(reg_beta) = register_mem.at(reg_alpha) + reg_0;
            }
            latency = 1;
            break;
        }
        case OperatorCode::SUB: {
            if (imm == 0) {
                register_mem.at(reg_beta) = register_mem.at(reg_alpha) - register_mem.at(reg_0);
            } else if (imm == 1) {
                register_mem.at(reg_beta) = register_mem.at(reg_alpha) - reg_0;
            }
            latency = 1;
            break;
        }
        case OperatorCode::MULT: {
            if (imm == 0) {
                register_mem.at(reg_beta) = register_mem.at(reg_alpha) * register_mem.at(reg_0);
            } else if (imm == 1) {
                register_mem.at(reg_beta) = register_mem.at(reg_alpha) * reg_0;
            }
            latency = 2;
            break;
        }
        case OperatorCode::DIV: {
            if (imm == 0) {
                register_mem.at(reg_beta) = register_mem.at(reg_alpha) / register_mem.at(reg_0);
            } else if (imm == 1) {
                register_mem.at(reg_beta) = register_mem.at(reg_alpha) / reg_0;
            }
            latency = 2;
            break;
        }
        case OperatorCode::AND: {
            if (imm == 0) {
                register_mem.at(reg_beta) = register_mem.at(reg_alpha) & register_mem.at(reg_0);
            } else if (imm == 1) {
                register_mem.at(reg_beta) = register_mem.at(reg_alpha) & reg_0;
            }
            latency = 1;
            break;
        }
        case OperatorCode::OR: {
            if (imm == 0) {
                register_mem.at(reg_beta) = register_mem.at(reg_alpha) | register_mem.at(reg_0);
            } else if (imm == 1) {
                register_mem.at(reg_beta) = register_mem.at(reg_alpha) | reg_0;
            }
            latency = 1;
            break;
        }
        case OperatorCode::XOR: {
            if (imm == 0) {
                register_mem.at(reg_beta) = register_mem.at(reg_alpha) ^ register_mem.at(reg_0);
            } else if (imm == 1) {
                register_mem.at(reg_beta) = register_mem.at(reg_alpha) ^ reg_0;
            }
            latency = 1;
            break;
        }
        case OperatorCode::SHL: {
            if (imm == 0) {
                register_mem.at(reg_beta) = register_mem.at(reg_alpha) << register_mem.at(reg_0);
            } else if (imm == 1) {
                register_mem.at(reg_beta) = register_mem.at(reg_alpha) << reg_0;
            }
            latency = 1;
            break;
        }
        case OperatorCode::SHR: {
            if (imm == 0) {
                register_mem.at(reg_beta) = register_mem.at(reg_alpha) >> register_mem.at(reg_0);
            } else if (imm == 1) {
                register_mem.at(reg_beta) = register_mem.at(reg_alpha) >> reg_0;
            }
            latency = 1;
            break;
        }
        case OperatorCode::SLT: {
            if (imm == 0) {
                if (register_mem.at(reg_alpha) < register_mem.at(reg_0)) {
                    register_mem.at(reg_beta) = 1;
                }
            } else if (imm == 1) {
                if (register_mem.at(reg_alpha) < reg_0) {
                    register_mem.at(reg_beta) = 1;
                }
            } else {
                register_mem.at(reg_beta) = 0;
            }
            latency = 1;
            break;
        }
        case OperatorCode::SLE: {
            if (imm == 0) {
                if (register_mem.at(reg_alpha) <= register_mem.at(reg_0)) {
                    register_mem.at(reg_beta) = 1;
                }
            } else if (imm == 1) {
                if (register_mem.at(reg_alpha) <= reg_0) {
                    register_mem.at(reg_beta) = 1;
                }
            } else {
                register_mem.at(reg_beta) = 0;
            }
            latency = 1;
            break;
        }
        case OperatorCode::SEQ: {
            if (imm == 0) {
                if (register_mem.at(reg_alpha) == register_mem.at(reg_0)) {
                    register_mem.at(reg_beta) = 1;
                }
            } else if (imm == 1 and register_mem.at(reg_alpha) == reg_0) {
                register_mem.at(reg_beta) = 1;
            } else {
                register_mem.at(reg_beta) = 0;
            }
            latency = 1;
            break;
        }
        case OperatorCode::LOAD: {
            if (imm == 0) {
                register_mem.at(reg_beta) = cache_processor.read_data((std::uint32_t) register_mem.at(reg_alpha) + register_mem.at(reg_0));
            } else if (imm == 1) {
                register_mem.at(reg_beta) = cache_processor.read_data(register_mem.at(reg_alpha) + reg_0);
            }
            latency = cache_processor.get_latency();
            break;
        }
        case OperatorCode::STORE: {
            std::size_t data_i;
            if (imm == 0) {
                data_i = register_mem.at(reg_alpha) + register_mem.at(reg_0);
            } else {
                data_i = register_mem.at(reg_alpha) + reg_0;
            }
            // Save data
            cache_processor.write_data(data_i, register_mem.at(reg_beta));
            // If data is in the bitmap screen, draw the pixel
            uint32_t yi = data_i / RAM_MEMORY_BITMAP_SCREEN_WIDTH;
            uint32_t xi = data_i % RAM_MEMORY_BITMAP_SCREEN_WIDTH;
            bitmap_renderer.draw_pixel((int32_t) xi, (int32_t) yi, this->cache_processor.get_data().at(data_i));
            latency = cache_processor.get_latency();
            break;
        }
        case OperatorCode::JMP: {
            register_mem.at(reg_beta) = (std::int32_t) address;
            if (imm == 0) {
                current_addr = register_mem.at(reg_0_jmp);
            } else if (imm == 1) {
                current_addr = reg_0_jmp;
            }
            latency = 2;
            break;
        }
        case OperatorCode::BRAZ: {
            if (register_mem.at(reg_alpha) == (std::int32_t) 0) {
                current_addr = (std::int32_t) a;
            }
            latency = 2;
            break;
        }
        case OperatorCode::BRANZ: {
            if (register_mem.at(reg_alpha) != (std::int32_t) 0) {
                current_addr = (std::int32_t) a;
            }
            latency = 2;
            break;
        }
        case OperatorCode::SCALL: {
            if (n == 0) {
                std::cout << "Number is : " << register_mem.at(1);
            } else if (n == 1) {
                std::string temp;
                std::cout << "Enter a number: ";
                std::cin >> temp;
                latency = cache_processor.get_latency();
                register_mem.at(1) = (std::int32_t) stoi(temp);
            } else if (n == 2) {
                bitmap_renderer.render_screen();
            } else {
                LOG(LOG_ERROR) << "Error : scall instruction parameter " << n << " usage is not defined in the project.";
                throw std::invalid_argument("Bad SCALL instruction parameter !");
            }
            latency = 100;
            break;
        }
        default: {
            LOG(LOG_ERROR) << "Error : the operator code " << std::dec << code_op << " value usage is not defined in the project.";
            throw std::domain_error("Unsupported  operator code!");
        }
    }

    // Display instructions details
    LOG(LOG_DEBUG) << std::dec << "code_op           : 0b" << std::bitset<5>(code_op) << " / " << code_op << " -> " << code_op_e;
    LOG(LOG_DEBUG) << std::dec << "reg_alpha         : 0b" << std::bitset<5>(reg_alpha) << " / " << reg_alpha;
    LOG(LOG_DEBUG) << std::dec << "imm               : 0b" << std::bitset<1>(imm) << " / " << imm;
    LOG(LOG_DEBUG) << std::dec << "reg_0             : 0b" << std::bitset<32>(reg_0) << " / " << reg_0;
    LOG(LOG_DEBUG) << std::dec << "reg_beta          : 0b" << std::bitset<20>(reg_beta) << " / " << reg_beta;
    LOG(LOG_DEBUG) << std::dec << "a                 : 0b" << std::bitset<27>(a) << " / " << a;
    LOG(LOG_DEBUG) << std::dec << "reg_0_jmp         : 0b" << std::bitset<32>(reg_0_jmp) << " / " << reg_0_jmp;
    LOG(LOG_DEBUG) << std::dec << "n                 : 0b" << std::bitset<32>(n) << " / " << n;

    // Affichage des valeurs de registres
    LOG(LOG_DEBUG) << "Register Values: ";
    for (std::size_t i = 0; i < register_mem.size(); i++) {
        LOG(LOG_DEBUG) << "\t- register_memory[" << i << "]: \t0b" << std::bitset<REGISTER_MEMORY_SIZE>(register_mem[i]) << " / " << register_mem[i];
    }

    if (current_addr == address) {
        current_addr++;
    }
    return current_addr;
}

/* PRIVATE DEFINITION */
void BinaryInstructionProcessor::decode() {
    code_op = ((std::uint32_t) instruction) >> 27;
    code_op_e = static_cast<OperatorCode>(code_op);
    switch (code_op_e) {
        case OperatorCode::ADD ... OperatorCode::STORE: { // ADD+SUB+MULT+DIV+AND+OR+XOR+SHl+SHR+SLT+SLE+SEQ+LOAD+STORE
            reg_alpha = ((std::int32_t) instruction & 0x07FFFFFF) >> 22;
            imm = ((std::int32_t) instruction & 0x003FFFFF) >> 21;
            reg_0 = ((std::int32_t) instruction & 0x001FFFFF) >> 5;
            reg_beta = (((std::int32_t) instruction | 0x00000020) & 0x0000001F);
            break;
        }
        case OperatorCode::JMP: { // JMP
            reg_0_jmp = ((std::int32_t) instruction & 0x03FFFFFF) >> 5;
            imm = ((std::int32_t) instruction & 0x7FFFFFF) >> 26;
            reg_beta = (((std::int32_t) instruction | 0x00000020) & 0x0000001F);
            break;
        }
        case OperatorCode::BRAZ ... OperatorCode::BRANZ: { // BRAZ+BRANZ
            reg_alpha = ((std::int32_t) instruction & 0x07FFFFFF) >> 22;
            a = ((std::int32_t) instruction & 0x003FFFFF);
            break;
        }
        case OperatorCode::SCALL: { // SCALL
            n = (std::int32_t) instruction & 0x07FFFFFF;
            break;
        }
        case OperatorCode::STOP: { // STOP
            exit(0);
        }
        default : { // Unsupported instruction
            LOG(LOG_ERROR) << "Error : the operator code " << std::dec << code_op << " value usage is not defined in the project.";
            throw std::domain_error("Unsupported  operator code!");
        }
    }
}


/*
 * BinaryFileProcessor class definition
 */

/* PUBLIC DEFINITION */

BinaryFileProcessor::BinaryFileProcessor(const std::filesystem::path &fileName, MemoryProcessor &cache_processor, std::array<std::int32_t, REGISTER_MEMORY_SIZE> &register_mem) : fileName(fileName), bitmap_renderer(8), cache_processor(cache_processor), register_mem(register_mem) {
    this->current_addr = 0;
    this->instructions = fetch();
    this->latency = 0;
}

BinaryFileProcessor::BinaryFileProcessor(const std::filesystem::path &fileName, MemoryProcessor &cache_processor, std::array<std::int32_t, REGISTER_MEMORY_SIZE> &register_mem, std::int32_t pixel_ratio) : fileName(fileName), bitmap_renderer(pixel_ratio), cache_processor(cache_processor), register_mem(register_mem) {
    this->current_addr = 0;
    this->register_mem = std::array<std::int32_t, REGISTER_MEMORY_SIZE>{};
    this->instructions = this->fetch();
}

std::array<std::int32_t, REGISTER_MEMORY_SIZE> BinaryFileProcessor::get_R() const {
    return this->register_mem;
}

std::map<std::uint32_t, std::uint32_t> BinaryFileProcessor::get_list_inst() const {
    return this->instructions;
}

std::uint32_t BinaryFileProcessor::get_current_add() const {
    return this->current_addr;
}

std::array<std::int32_t, RAM_MEMORY_SIZE> BinaryFileProcessor::get_data() const {
    return cache_processor.get_data();
}

std::filesystem::path BinaryFileProcessor::get_filename() const {
    return this->fileName;
}

std::uint32_t BinaryFileProcessor::get_latency() const {
    return latency;
}


/* PRIVATE DEFINITION */
std::map<uint32_t, uint32_t> BinaryFileProcessor::fetch() {
    std::ifstream read_file;
    std::map<std::uint32_t, std::uint32_t> list_inst;
    std::uint32_t bytes, address_uint32, instruction_uint32;

    //OPEN FILE
    read_file.open(fileName, std::ios::binary);
    if (!read_file.is_open()) {
        LOG(LOG_ERROR) << "Error : file with path " + fileName.string() + "  not found.";
        throw std::filesystem::filesystem_error("Can't open file!", std::make_error_code(std::errc::no_such_file_or_directory));
    }

    LOG(LOG_INFO) << "Fetching file instructions and address...";
    int i = 0;
    while ((i < 5000) or (list_inst.end()->second != 0 and not list_inst.empty())) {
        i++;

        // GET ADDRESS
        read_file.read(reinterpret_cast<char *>(&bytes), sizeof(bytes));
        address_uint32 = (((uint32_t) bytes) & 0xFF) << 24 |
                         (((uint32_t) bytes >> 8) & 0xFF) << 16 |
                         (((uint32_t) bytes >> 16) & 0xFF) << 8 |
                         (((uint32_t) bytes >> 24) & 0xFF);

        // GET INSTRUCTIONS
        read_file.read(reinterpret_cast<char *>(&bytes), sizeof(bytes));
        instruction_uint32 = (((uint32_t) bytes) & 0xFF) << 24 |
                             (((uint32_t) bytes >> 8) & 0xFF) << 16 |
                             (((uint32_t) bytes >> 16) & 0xFF) << 8 |
                             (((uint32_t) bytes >> 24) & 0xFF);

        if (address_uint32 == instruction_uint32) {
            break;
        }
        LOG(LOG_INFO) << "address: " << address_uint32 << ", instruction: 0x" << std::hex << instruction_uint32;
        list_inst.insert(std::make_pair(address_uint32, instruction_uint32));
    }
    read_file.close();

    return list_inst;
}

void BinaryFileProcessor::execute_prog() {
    for (this->current_addr = 0; this->current_addr < this->instructions.size();) {
        LOG(LOG_INFO) << "Executing instruction[" << std::hex << this->current_addr << "]: 0x" << this->instructions[this->current_addr] << std::dec << " / 0b" << std::bitset<REGISTER_MEMORY_SIZE>(this->instructions[this->current_addr]) << " ...";
        BinaryInstructionProcessor instruction_processor(current_addr, instructions[current_addr], register_mem, cache_processor);
        current_addr = instruction_processor.execute(this->bitmap_renderer);
        register_mem = instruction_processor.get_R();
        latency += instruction_processor.get_latency();
        LOG(LOG_DEBUG) << cache_processor.get_cache();
    }
}
