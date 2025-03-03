#include "ShellMemory.h"

using namespace std;

static unordered_map<string, string> shell_memory;

void initialise_memory() {
	shell_memory.reserve(MEMORY_SIZE);
}

optional<string> get_variable_value(const string& variable)
{
	optional<string> value = nullopt;
	auto it = shell_memory.find(variable);
	if (it != shell_memory.end()) value = it->second;
	return value;
}

OperationStatus set_variable_value(const string& variable, string value)
{
	OperationStatus set_status;
	try
	{
		// check if variable not set, and max capacity reached
		bool variable_set = get_variable_value(variable).has_value();
		bool full_capacity = shell_memory.size() == MEMORY_SIZE;

		if (!variable_set && full_capacity) throw out_of_range("Shell Memory exceeded.");

		shell_memory.insert_or_assign(variable, move(value));
		cout << "Successfully updated variable " << variable << " : " << shell_memory[variable] << endl;
	}
	catch (exception error)
	{
		set_status.operation_success = false;
		set_status.error_message = error;
	}
	return set_status;
}
