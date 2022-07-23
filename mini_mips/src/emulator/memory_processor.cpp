//
// Created by mathis on 05/02/2022.
//

#include "memory_processor.h"


/*
 * BinaryInstructionProcessor class definition
 */

/* PUBLIC DEFINITION */
MemoryProcessor::MemoryProcessor(bool use_cache, std::array<std::int32_t, RAM_MEMORY_SIZE> &raw_memory): use_cache(use_cache), raw_memory(raw_memory) {
    // Create Cache
    this->nb_set = 256;
    this->byte_per_line = 16;
    if (use_cache){
        this->cache = build_cache();
    }
    this->latency = 0;

}

std::ostream &operator<<(std::ostream &os, const MemoryProcessor &memory_processor) {
    if (memory_processor.use_cache) {
        os << "\n";
        os << "\t\t\t\t" << "Structure of the cache:\n";
        std::uint32_t i = 0;
        for (const auto &set: memory_processor.get_cache()) {
            os << "\t\t\t\t" << "set " << i << ": \n";
            i++;
            os << "\t\t\t\t";
            for (const auto &byte: set) {
                os << byte << " ";
            }
            os << "\n";
        }
    }
    else{
        os << "The emulator do not use a cache, only RAW memory";
    }
    return os;
}

std::vector<std::vector<std::uint32_t>> MemoryProcessor::get_cache()const {
    return cache;
}

std::array<std::int32_t, RAM_MEMORY_SIZE> MemoryProcessor::get_data() const {
    return this->raw_memory;
}

std::uint32_t MemoryProcessor::get_latency() const{
    return latency;
}


std::uint32_t MemoryProcessor::read_data(std::uint32_t address){
    std::uint32_t tag = ((std::uint32_t) address) >> 12;
    std::uint32_t index= ((std::int32_t) address & 0x00000FFF) >> 4;
    std::uint32_t offset= (((std::int32_t) address | 0x00000020) & 0x0000000F);

    LOG(LOG_DEBUG) << "Read data";
    LOG(LOG_DEBUG) << "address " << std::bitset<32>(address);
    LOG(LOG_DEBUG) << "tag " << std::bitset<20>(tag) << " index " << std::bitset<8>(index) << " offset " << std::bitset<4>(offset);

    if (use_cache) {
        LOG(LOG_DEBUG) << "Cache will be used";
        if(data_is_in_cache(index, tag)){
            LOG(LOG_DEBUG) << "CACHE HIT";
            this->latency = 1;
            LOG(LOG_DEBUG) << cache.at(index).at(offset+2);
            return cache.at(index).at(offset+2);
        }else{
            LOG(LOG_DEBUG) << "CACHE MISS";
            // Charger la donnée de l'offset demander, mais aussi charger les autres données
            for(std::size_t offset_i = 0; offset_i < byte_per_line; offset_i++) {
                LOG(LOG_DEBUG) << address << " " << offset << " " << offset_i;
                if ( offset_i == offset) {
                    LOG(LOG_DEBUG) << "Data" << raw_memory.at(address);
                    write_data(address, raw_memory.at(address));
                }
                else if (offset_i < offset){
                    LOG(LOG_DEBUG) << address - (offset - offset_i);
                    LOG(LOG_DEBUG) << "Data" << raw_memory.at(address - (offset - offset_i));
                    write_data(address - (offset - offset_i), raw_memory.at(address - (offset - offset_i)));
                }
                else if (offset_i > offset){
                    LOG(LOG_DEBUG) << address + (offset_i - offset);
                    LOG(LOG_DEBUG) << "Data" << raw_memory.at(address + (offset_i - offset));

                    write_data(address + (offset_i - offset), raw_memory.at(address + (offset_i - offset)));
                }
            }
            cache.at(index).at(0) = 1;
            cache.at(index).at(1) = tag;
            this->latency = 10;
            return raw_memory.at(address);
        }
    }else{
        LOG(LOG_DEBUG) << "Raw memory will be used";
        this->latency = 10;
        return raw_memory.at(address);
    }
}

void MemoryProcessor::write_data(std::uint32_t address, std::int32_t data) {

    std::uint32_t tag = ((std::uint32_t) address) >> 12;
    std::uint32_t index = ((std::int32_t) address & 0x00000FFF) >> 4;
    std::uint32_t offset = (((std::int32_t) address | 0x00000020) & 0x0000000F);

    LOG(LOG_DEBUG) << "Write data";
    LOG(LOG_DEBUG) << "address " << std::bitset<32>(address) << " data " << std::bitset<32>(data);
    LOG(LOG_DEBUG) << "tag " << std::bitset<20>(tag) << " index " << std::bitset<8>(index) << " offset " << std::bitset<4>(offset);

    if (use_cache) {
        LOG(LOG_DEBUG) << "Cache will be used";

        cache.at(index).at(offset + 2) = data;
        raw_memory.at(address) = data;
    }else{
        LOG(LOG_DEBUG) << "Raw memory will be used";
        raw_memory.at(address) = data;
    }
    this->latency = 1;
}


/* PRIVATE DEFINITION */
std::vector<std::vector<std::uint32_t>> MemoryProcessor::build_cache() const{
    std::uint32_t each_set;
    std::uint32_t each_byte;
    std::vector<std::vector<std::uint32_t>> cache_struct;
    for(each_set = 0; each_set<nb_set; each_set++){
        std::vector<std::uint32_t> mots;
        mots.push_back(0); //ValidBit
        mots.push_back(0); //tag
        for(each_byte = 0; each_byte < byte_per_line; each_byte++){
            mots.push_back(0);
        }
        cache_struct.push_back(mots);
    }

    return cache_struct;
}

bool MemoryProcessor::data_is_in_cache(std::uint32_t index, std::uint32_t tag) {

    std::uint32_t valid_bit = cache.at(index).at(0);
    std::uint32_t tag_cache = cache.at(index).at(1);
    LOG(LOG_DEBUG) << "Cache read:  ValidBit-" << valid_bit << " ValidBeatWanted-" << 1 << " Tag-" << tag_cache << " TageWanted-" << tag << " index-" << index;
    if (valid_bit == 1 and tag == tag_cache) {
        return true;
    }

    return false;
}
