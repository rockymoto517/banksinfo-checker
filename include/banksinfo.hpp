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
};

AllBanksInfo get_banks_info(const std::string &);
