#pragma once
#include <iostream>
#include <optional>
#include <unordered_map>

constexpr size_t MEMORY_SIZE = 1000;

struct OperationStatus
{
	// Assume Success by default
	bool operation_success = true;
	std::optional<std::exception> error_message = std::nullopt;			// null by default;
};

void initialise_memory();
std::optional<std::string> get_variable_value(const std::string& variable);
OperationStatus set_variable_value(const std::string& variable, std::string value);
