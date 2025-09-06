#include "banksinfo.hpp"
#include "flags.hpp"

#include <cstdlib>
#include <print>

bool banks{ true };
bool lookups{ true };
bool debug{ false };
bool unused{ false };
std::string file{};

int main(int argc, char **argv) {
    if (argc > 1) {
        get_flags(argc, argv);
        if (file.empty()) {
            print_help(argv[0]);
            std::exit(EXIT_FAILURE);
        }

        std::print("Building bank info...\n");
        AllBanksInfo b{ std::move(get_banks_info(file)) };
        if (lookups) {
            std::print("\nSoundLookups:\n");
            for (const auto [k, v] : b.soundlookups) {
                std::print("\t{:<16} {:<32} {:>16}\n", k, std::get<0>(v),
                           std::get<1>(v));
            }
        }

        if (banks) {
            std::print("\nSoundbanks:\n");
            for (const auto &s : b.soundbanks) {
                std::print("\t{}\n", s);
            }
        }

        if (unused) {
            std::print("\nUsed bytes:\n");
            for (const auto &p : visited) {
                std::print("\t{:#08x}, {:#08x}\n", std::get<0>(p),
                           std::get<1>(p));
            }
            std::print("\n\tEOF: {:#08x}\n", b.metadata.file_size);
        }

        if (debug) {
            std::print("\nMetadata:\n"
                       "\tfile_size: {} bytes\n"
                       "\tnum_banks: {}\n"
                       "\tbanks_offset: {}\n"
                       "\tnum_sounds: {}\n"
                       "\tsounds_offset: {}\n"
                       "\tnum_sound_lookups: {}\n"
                       "\tsound_lookups_offset: {}\n",
                       b.metadata.file_size, b.metadata.num_banks,
                       b.metadata.banks_offset, b.metadata.num_sounds,
                       b.metadata.sounds_offset, b.metadata.num_sound_lookups,
                       b.metadata.sound_lookups_offset);
        }
    } else {
        print_help(argv[0]);
    }
}
