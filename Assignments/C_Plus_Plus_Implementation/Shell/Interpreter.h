#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <optional>
#include <filesystem>
#include "Shell.h"
#include "ShellMemory.h"

constexpr size_t MAX_ARGS_SIZE = 3;

int help();
int quit();
int source(const std::string& script);
int print(const std::string& variable);
int set(const std::string& variable, std::string value);

// new funtions
int echo(const std::string& input);
int my_ls();
int my_mkdir(const std::string& dirname);
int my_touch(const std::string& filename);
int my_cd(const std::string& dirname);
int run(const std::string& command_with_args);
