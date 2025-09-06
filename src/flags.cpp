#include "flags.hpp"

#include <cstdlib>
#include <cstring>
#include <print>
#include <string_view>

void print_help(std::string_view prog) {
    std::print("Usage: {} <options> <.banksinfo file>\n", prog);
    std::print("Options:\n"
               "\t-b:\tDon't show banks\n"
               "\t-l:\tDon't show lookups\n"
               "\t-d:\tShow metadata\n"
               "\t-D:\tOnly show metadata (equivalent to -bld)\n"
               "\t-u:\tShow used bytes of the file\n"
               "\t-U:\tOnly show used bytes of the file (equivalent to -blu)\n"
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
                    case 'U': {
                        banks = false;
                        lookups = false;
                    }
                    case 'u': {
                        unused = true;
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
