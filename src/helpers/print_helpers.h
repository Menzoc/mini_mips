#ifndef PRINT_HELPERS_H
#define PRINT_HELPERS_H

#include "globals.h"
#include <filesystem>
#include <vector>
#include <array>

template<typename T>
inline std::ostream &operator<<(std::ostream &os, const std::vector<T> &v) {
    os << "[";
    for (typename std::vector<T>::const_iterator i = v.begin(); i != v.end(); ++i) {
        if (i == v.end() - 1) {
            os << *i;
        } else {
            os << *i << ", ";
        }
    }
    os << "]";
    return os;
}

template<typename T, std::size_t SIZE>
inline std::ostream &operator<<(std::ostream &os, const std::array<T, SIZE> &a) {
    os << "(";
    for (typename std::array<T, SIZE>::const_iterator i = a.begin(); i != a.end(); ++i) {
        if (i == a.end() - 1) {
            os << *i;
        } else {
            os << *i << ", ";
        }
    }
    os << ")";
    return os;
}

inline std::ostream &operator<<(std::ostream &os, const ParameterType &parameter_type) {
    switch (parameter_type) {
        case ParameterType::REGISTER : {
            os << "REGISTER";
            break;
        }
        case ParameterType::VALUE : {
            os << "VALUE";
            break;
        }
        case ParameterType::KNOWN_LABEL : {
            os << "KNOWN_LABEL";
            break;
        }
        case ParameterType::UNKNOWN_LABEL : {
            os << "UNKNOWN_LABEL";
            break;
        }

        case ParameterType::UNKNOWN: {
            os << "UNKNOWN";
            break;
        }
        case ParameterType::LABEL: {
            os << "LABEL";
            break;
        }
    }
    return os;
}

inline std::ostream &operator<<(std::ostream &os, const OperatorCode &operator_code) {
    switch (operator_code) {
        case OperatorCode::STOP : {
            os << "STOP";
            break;
        }
        case OperatorCode::ADD : {
            os << "ADD";
            break;
        }
        case OperatorCode::SUB : {
            os << "SUB";
            break;
        }
        case OperatorCode::MULT : {
            os << "MULT";
            break;
        }
        case OperatorCode::DIV : {
            os << "DIV";
            break;
        }
        case OperatorCode::AND : {
            os << "AND";
            break;
        }
        case OperatorCode::OR : {
            os << "OR";
            break;
        }
        case OperatorCode::XOR : {
            os << "XOR";
            break;
        }
        case OperatorCode::SHL : {
            os << "SHL";
            break;
        }
        case OperatorCode::SHR : {
            os << "SHR";
            break;
        }
        case OperatorCode::SLT : {
            os << "SLT";
            break;
        }
        case OperatorCode::SLE : {
            os << "SLE";
            break;
        }
        case OperatorCode::SEQ : {
            os << "SEQ";
            break;
        }
        case OperatorCode::LOAD : {
            os << "LOAD";
            break;
        }
        case OperatorCode::STORE : {
            os << "STORE";
            break;
        }
        case OperatorCode::JMP : {
            os << "JMP";
            break;
        }
        case OperatorCode::BRAZ : {
            os << "BRAZ";
            break;
        }
        case OperatorCode::BRANZ : {
            os << "BRANZ";
            break;
        }
        case OperatorCode::SCALL : {
            os << "SCALL";
            break;
        }
        default: {
            os << "UNKNOWN";
            break;
        }
    }
    return os;
}


#endif //PRINT_HELPERS_H
