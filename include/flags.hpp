#pragma once

#include <string>
#include <string_view>

void get_flags(int, char **);
void print_help(std::string_view);

extern bool banks;
extern bool lookups;
extern bool debug;
extern bool unused;
extern std::string file;
