
#ifndef MEMORY_PROCESSOR_H
#define MEMORY_PROCESSOR_H

#include "emulator_globals.h"
#include "../helpers/globals.h"
#include "../helpers/log.h"
#include "../helpers/print_helpers.h"
#include <map>
#include <bitset>


class MemoryProcessor {
public:

    explicit MemoryProcessor(bool use_cache, std::array<std::int32_t, RAM_MEMORY_SIZE> &raw_memory);

    std::uint32_t read_data(std::uint32_t address);

    void write_data(std::uint32_t address, std::int32_t data);

    friend std::ostream &operator<<(std::ostream &os, const MemoryProcessor &memory_processor);

    std::vector<std::vector<std::uint32_t>> get_cache() const;

    std::array<std::int32_t, RAM_MEMORY_SIZE> get_data() const;

    std::uint32_t get_latency() const;


private:
    const bool use_cache;

    std::uint32_t nb_set;

    std::uint32_t byte_per_line;

    std::uint32_t latency;

    std::array<std::int32_t, RAM_MEMORY_SIZE> &raw_memory;

    std::vector<std::vector<std::uint32_t>> cache;

    std::vector<std::vector<std::uint32_t>> build_cache() const;

    bool data_is_in_cache(std::uint32_t index, std::uint32_t tag);

};


#endif //MEMORY_PROCESSOR_H
