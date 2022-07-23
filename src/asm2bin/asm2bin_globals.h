#ifndef ASM2BIN_GLOBALS_H
#define ASM2BIN_GLOBALS_H

#include <string>

// assembly line regex
static const std::string ASSEMBLY_LINE_REGEX = R"((?:[\s\t]*([a-zA-Z0-9_]*)[\s\t]*:)?[\s\t]*(?:([A-Za-z]+)(?:[\s\t]+([a-zA-Z0-9_]+))?(?:[\s\t]*,[\s\t]*([a-zA-Z0-9_]+))?(?:[\s\t]*,[\s\t]*([a-zA-Z0-9_]+))?)?.*)";

#endif //ASM2BIN_GLOBALS_H
