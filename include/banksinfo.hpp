#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

using Idstring = std::string;
using SoundLookups
    = std::unordered_map<std::uint64_t, std::pair<std::string, Idstring>>;
struct AllBanksInfo {
    SoundLookups soundlookups;
    std::vector<std::string> soundbanks;
    struct {
        std::uint64_t file_size;
        std::uint32_t num_banks;
        std::uint32_t banks_offset;
        std::uint32_t num_sounds;
        std::uint32_t sounds_offset;
        std::uint32_t num_sound_lookups;
        std::uint32_t sound_lookups_offset;
    } metadata;
};

extern std::vector<std::pair<std::uint64_t, std::uint64_t>> visited;

AllBanksInfo get_banks_info(const std::string &);
