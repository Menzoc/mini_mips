#ifndef GLOBALS_H
#define GLOBALS_H

#include <string>
#include <unordered_map>

// operator code enumeration
enum class OperatorCode : std::int8_t {
    STOP = 0, /** stop(r1,o,r2) **/
    ADD = 1, /** add(r1,o,r2) **/
    SUB = 2, /** sub(r1,o,r2) **/
    MULT = 3, /** mult(r1,o,r2) **/
    DIV = 4, /** div(r1,o,r2) **/
    AND = 5, /** and(r1,o,r2) **/
    OR = 6, /** or(r1,o,r2) **/
    XOR = 7, /** xor(r1,o,r2) **/
    SHL = 8, /** shl(r1,o,r2) **/
    SHR = 9, /** shr(r1,o,r2) **/
    SLT = 10, /** slt(r1,o,r2) **/
    SLE = 11, /** sle(r1,o,r2) **/
    SEQ = 12, /** seq(r1,o,r2) **/
    LOAD = 13, /** load(r1,o,r2) **/
    STORE = 14, /** store(r1,o,r2) **/
    JMP = 15, /** jmp(o,r) **/
    BRAZ = 16, /** braz(r,a) **/
    BRANZ = 17, /** branz(r,a) **/
    SCALL = 28, /** scall(n) **/
    UNKNOWN = 100 /** unsupported operator code **/
};

// map of operator code using their string name as key
static std::unordered_map<std::string, OperatorCode> const OPERATOR_CODE_STR = {
        {"STOP",  OperatorCode::STOP},
        {"ADD",   OperatorCode::ADD},
        {"SUB",   OperatorCode::SUB},
        {"MULT",  OperatorCode::MULT},
        {"DIV",   OperatorCode::DIV},
        {"AND",   OperatorCode::AND},
        {"OR",    OperatorCode::OR},
        {"XOR",   OperatorCode::XOR},
        {"SHL",   OperatorCode::SHL},
        {"SHR",   OperatorCode::SHR},
        {"SLT",   OperatorCode::SLT},
        {"SLE",   OperatorCode::SLE},
        {"SEQ",   OperatorCode::SEQ},
        {"LOAD",  OperatorCode::LOAD},
        {"STORE", OperatorCode::STORE},
        {"JMP",  OperatorCode::JMP},
        {"BRAZ",  OperatorCode::BRAZ},
        {"BRANZ", OperatorCode::BRANZ},
        {"SCALL", OperatorCode::SCALL}};

// instruction parameters types enumeration
enum class ParameterType : std::int8_t {
    UNKNOWN, /** parameter type is unsupported **/
    REGISTER, /** parameters is a register **/
    VALUE, /** parameters is a value **/
    LABEL, /** parameter is a label, more tests have to be done to see if it is a known or unknown label **/
    KNOWN_LABEL, /** parameters is a label where the instruction index is already known **/
    UNKNOWN_LABEL /** parameters is a label where the instruction index is currently unknown **/
};

#endif //GLOBALS_H
