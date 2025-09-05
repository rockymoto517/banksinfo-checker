#include "banksinfo.hpp"

#include <cstdlib>
#include <cstring>
#include <print>
#include <string_view>

static bool banks = true;
static bool lookups = true;
static bool debug = false;
static std::string file{};

void print_help(std::string_view prog) {
    std::print("Usage: {} <options> <.banksinfo file>\n", prog);
    std::print("Options:\n"
               "\t-b:\tDon't show banks\n"
               "\t-l:\tDon't show lookups\n"
               "\t-d:\tShow metadata\n"
               "\t-D:\tOnly show metadata (equivalent to -bld)\n"
               "\t-h:\tShow this help message again\n");
}

void get_flags(int argc, char **argv) {
    for (int i = 1; i < argc; ++i) {
        if (argv[i][0] == '-') {
            for (int j = 1; j < strlen(argv[i]); ++j) {
                switch (argv[i][j]) {
                    case 'b': {
                        banks = false;
                        break;
                    }
                    case 'l': {
                        lookups = false;
                        break;
                    }
                    case 'D': {
                        banks = false;
                        lookups = false;
                    }
                    case 'd': {
                        debug = true;
                        break;
                    }
                    case 'h':
                    default: {
                        print_help(argv[0]);
                        std::exit(EXIT_SUCCESS);
                    }
                }
            }
        } else {
            file = argv[i];
        }
    }
}

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
