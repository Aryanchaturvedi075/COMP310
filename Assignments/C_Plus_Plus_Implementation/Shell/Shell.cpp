// Shell.cpp : Defines the entry point for the application.
#include "Shell.h"

using namespace std;

using CommandFn = function<int(const vector<string>&)>;

static inline bool is_interactive_mode()
{
#ifdef _WIN32
    return _isatty(_fileno(stdin)) != 0;
#else
    return isatty(STDIN_FILENO) != 0;
#endif
}

static inline int bad_command()
{
    cerr << "Unknown Command\n";
    return EXIT_FAILURE;
}

static const unordered_map<string_view, pair<size_t, CommandFn>> commands{
    {"help",		{1, [](const auto& args) { return help(); }}},
    {"quit",		{1, [](const auto& args) { return quit(); }}},
    {"set",			{3, [](const auto& args) { return set(args[1], args[2]); }}},
    {"print",		{2, [](const auto& args) { return print(args[1]); }}},
    {"source",		{2, [](const auto& args) { return source(args[1]); }}},
	{"echo",		{2, [](const auto& args) { return echo(args[1]); }}},
	{"my_ls",		{1, [](const auto& args) { return my_ls(); }}},
	{"my_mkdir",	{2, [](const auto& args) { return my_mkdir(args[1]); }}},
	{"my_touch",	{2, [](const auto& args) { return my_touch(args[1]); }}},
	{"my_cd",		{2, [](const auto& args) { return my_cd(args[1]); }}},
    {"run",         {0, [](const auto& args) 
    {
        // Special case: run command needs at least 2 tokens
        if (args.size() < 2) return bad_command();

        // Reconstruct command string without "run"
        string cmd_str;
        for (size_t i = 1; i < args.size(); ++i)
        {
            if (i > 1) cmd_str += " ";
            cmd_str += args[i];
        }
        return run(cmd_str);
    }}}
};

int parse_input(string& input)
{
	vector<string> tokens;
	istringstream tokenStream(input);
	string token;
	while (tokenStream >> token) tokens.push_back(token);
	return interpreter(tokens);
}

int parse_input_chain(string& user_input)
{
    // Split input by semicolons using views
    for (const auto& command_view : user_input | views::split(';'))
    {
        string command{ command_view.begin(), command_view.end() };
        if (!command.empty())
        {
            int return_code = parse_input(command);
            if (return_code == -1) return -1; // Propagate critical error
        }
    }

    return EXIT_SUCCESS;
}

int interpreter(const vector<string>& command_args) {
    if (command_args.empty() || command_args.size() > MAX_ARGS_SIZE) return bad_command();

    // Sanitize inputs once using views
    for (const auto& arg : command_args) {
        if (const auto pos = arg.find_first_of("\r\n"); pos != string::npos) {
            string clean_arg = arg;
            clean_arg.erase(pos);
        }
    }

    // Command lookup and validation
    if (const auto it = commands.find(command_args[0]); it != commands.end()) {
        const auto& [expected_size, handler] = it->second;
        return command_args.size() == expected_size ? handler(command_args) : bad_command();
    }

    return bad_command();
}

int main()
{
	clog << "Shell version 2.0 created March 2025\n";
	initialise_memory();
	help();
	string user_input;

	while (true)
	{
        if (is_interactive_mode()) cout << "$ ";
		if (!getline(cin, user_input)) break;
		if (parse_input_chain(user_input) == -1) exit(99);		// critical error
	}
}
