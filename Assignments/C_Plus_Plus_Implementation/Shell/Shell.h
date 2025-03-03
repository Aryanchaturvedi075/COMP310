#pragma once
#include <iostream>
#include <sstream>
#include <functional>
#include <ranges>
#include "Interpreter.h"
#include "ShellMemory.h"
#ifdef _WIN32
#include <windows.h>
#include <io.h>
#include <fcntl.h>
#else
#include <sys/wait.h>
#include <unistd.h>
#endif

int parse_input(std::string& input);
int interpreter(const std::vector<std::string>& command_args);
