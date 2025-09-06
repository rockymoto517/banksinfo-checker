#include "banksinfo.hpp"

#include <format>
#include <fstream>
#include <iostream>

std::vector<std::pair<std::uint64_t, std::uint64_t>> visited{};
static std::uint64_t last_pos{ 0 };
static std::uint64_t travelled{ 0 };

static inline std::string get_string(std::ifstream &f) {
    std::string s{};
    char c = 1;
    while (c != 0) {
        f >> c;
        s += c;
    }

    return s;
}

static inline Idstring get_hash(std::uint64_t h) {
    return std::format("{:#016x}", h);
}

void read(std::integral auto &in, std::ifstream &f) {
    f.read(reinterpret_cast<char *>(&in), sizeof(in));
}

AllBanksInfo get_banks_info(const std::string &path) {
    std::vector<std::string> s{};

    std::ifstream f{ path, std::ios::binary };

    std::uint32_t bnk_count, bnk_offset;
    ::read(bnk_count, f);
    f.seekg(4, std::ios_base::cur);
    ::read(bnk_offset, f);
    f.seekg(8, std::ios_base::cur);
    travelled += 20;

    std::uint32_t sound_count, sound_offset;
    ::read(sound_count, f);
    f.seekg(4, std::ios_base::cur);
    ::read(sound_offset, f);
    f.seekg(12, std::ios_base::cur);
    travelled += 24;

    std::uint32_t u_count, u_offset;
    ::read(u_count, f);
    f.seekg(4, std::ios_base::cur);
    ::read(u_offset, f);
    travelled += 12;

    visited.emplace_back(std::pair<std::uint64_t, std::uint64_t>{
        last_pos, last_pos + travelled });
    f.seekg(bnk_offset);
    last_pos = bnk_offset;
    travelled = 0;
    std::streampos prev_pos{};
    std::uint64_t bnk_start{ 0 }, bnk_travelled{ 0 };
    for (int i = 0; i < bnk_count; ++i) {
        f.seekg(4, std::ios_base::cur);
        std::uint32_t position;
        ::read(position, f);
        travelled += 8;

        if (bnk_start == 0)
            bnk_start = position;
        prev_pos = f.tellg();
        f.seekg(position);
        s.push_back(::get_string(f));
        bnk_travelled += s.front().size();
        f.seekg(prev_pos);
    }
    visited.emplace_back(std::pair<std::uint64_t, std::uint64_t>{
        bnk_start, bnk_start + bnk_travelled });

    visited.emplace_back(std::pair<std::uint64_t, std::uint64_t>{
        last_pos, last_pos + travelled });
    f.seekg(sound_offset);
    last_pos = sound_offset;
    travelled = 0;
    std::unordered_map<std::uint64_t, std::uint64_t> sound_lookups{};
    for (int i = 0; i < sound_count; ++i) {
        std::uint64_t id, hash;
        ::read(id, f);
        ::read(hash, f);
        travelled += 16;
        if (sound_lookups.contains(hash)) {
            continue;
        }
        sound_lookups[hash] = id;
    }

    visited.emplace_back(std::pair<std::uint64_t, std::uint64_t>{
        last_pos, last_pos + travelled });
    f.seekg(u_offset);
    last_pos = u_offset;
    travelled = 0;
    std::uint64_t lookup_start{ 0 }, lookup_travelled{ 0 };
    SoundLookups sl{};
    for (int i = 0; i < u_count; ++i) {
        std::uint64_t hash;
        std::uint32_t string_pos;
        ::read(hash, f);
        f.seekg(4, std::ios_base::cur);
        ::read(string_pos, f);
        travelled += 16;

        if (lookup_start == 0)
            lookup_start = string_pos;
        prev_pos = f.tellg();
        f.seekg(string_pos);
        std::string s{ std::move(::get_string(f)) };
        f.seekg(prev_pos);
        lookup_travelled += s.size();

        if (!sound_lookups.contains(hash)) {
            continue;
        }

        std::uint64_t id = sound_lookups.at(hash);
        if (sl.contains(id)) {
            continue;
        }

        Idstring ids{ std::move(::get_hash(hash)) };
        sl[id] = std::pair<std::string, Idstring>{
            std::to_string(id) != s ? s : "", ids
        };
    }
    visited.emplace_back(std::pair<std::uint64_t, std::uint64_t>{
        lookup_start, lookup_start + lookup_travelled });

    visited.emplace_back(std::pair<std::uint64_t, std::uint64_t>{
        last_pos, last_pos + travelled });

    for (int i = 0; i < visited.size(); ++i) {
        for (int j = i + 1; j < visited.size(); ++j) {
            if (std::get<1>(visited.at(i)) == std::get<0>(visited.at(j))) {
                std::get<1>(visited.at(i)) = std::get<1>(visited.at(j));
                visited.erase(visited.begin() + j);
            }
        }
    }

    f.seekg(0, std::ios_base::end);
    return AllBanksInfo{ sl,
                         s,
                         {
                             static_cast<std::uint64_t>(f.tellg()),
                             bnk_count,
                             bnk_offset,
                             sound_count,
                             sound_offset,
                             u_count,
                             u_offset,
                         } };
}
