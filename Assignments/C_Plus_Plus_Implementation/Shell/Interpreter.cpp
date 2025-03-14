#include "Interpreter.h"

using namespace std;
using namespace filesystem;

static inline int failed_operation(const optional<exception>& e)
{
	cerr << " - Error occured while executing operation: ";
	if (e.has_value()) cerr << e.value().what();
    cerr << "\n";
	return EXIT_FAILURE;
}

static inline int file_not_found()
{
	cerr << "Bad command : File not found\n";
	return EXIT_FAILURE;
}

// Interpreter Commands
int quit()
{
	cout << "Thank you for using my Shell Program :)\n";
	exit(EXIT_SUCCESS);
}

int help()
{
	string help_string =
		"------------------------------------------------------------------------------\n"
		"COMMAND\t\t\tDESCRIPTION\n"
		"------------------------------------------------------------------------------\n"
		"help\t\t\tDisplays all the commands.\n"
		"quit\t\t\tExits / terminates the shell.\n"
		"set <VAR> <VALUE>\tAssigns a value to shell memory.\n"
		"print <VAR>\t\tDisplays the VALUE assigned to VAR.\n"
		"source <SCRIPT>.TXT\tExecutes the file SCRIPT.TXT.\n"
		"echo <INPUT>\t\tDisplay strings passed as arguments.\n"
		"my_ls \t\t\tLists all the files present in the current directory.\n"
		"my_mkdir <DIRNAME>\tCreates a new directory in the current directory.\n"
		"my_touch <FILENAME>\tCreates a new empty file inside the current directory.\n"
		"my_cd <DIRNAME>\tChanges current directory to directory DIRNAME.\n"
        "run <COMMAND>\tForks the shell to execute the given COMMAND.\n"
		"------------------------------------------------------------------------------\n";
	clog << help_string;
	return EXIT_SUCCESS;
}

int print(const string& variable)
{
	optional<string> opt = get_variable_value(variable);
	if (opt.has_value())
	{
		cout << opt.value() << endl;
		return EXIT_SUCCESS;
	}
	else
	{
		cerr << "Variable not found\n";
		return EXIT_FAILURE;
	}
}

// TODO: Revisit this later
int set(const string& variable, string value)
{
	string link; // supposed to be " " 

	// Hint: If "value" contains multiple tokens, you'll need to loop through them, 
	// concatenate each token to the buffer, and handle spacing appropriately. 
	// Investigate how `strcat` works and how you can use it effectively here.

	OperationStatus set_status = set_variable_value(variable, value);
	if (set_status.operation_success == false) 
		return failed_operation(set_status.error_message);

	return EXIT_SUCCESS;
}

int source(const string& script)
{
    try 
    {
        ifstream file(script);
        if (!file) return file_not_found();

        string line; 
		int i = 1, return_code;
        while (getline(file, line)) 
		{
			cout << i << " " << line << "\n";
			return_code = parse_input(line);
			cout << "[Status " << return_code << "]";
			if (return_code != 0) failed_operation(nullopt);
			cout << "\n\n";
			i++;
        }
        return EXIT_SUCCESS;
    }
	catch (const exception& e)
	{
		return failed_operation(optional<exception>(e));
	}
}

int echo(const string& input)
{
	if (input[0] != '$') cout << input << "\n";
	else
	{
		optional<string> opt = get_variable_value(input.substr(1));
		string output = opt.has_value() ? opt.value() : "";
		cout << output << "\n";
	}
	return EXIT_SUCCESS;
}

int my_ls()
{
    try
    {
		vector<path> entries;
        path currentPath = current_path();
        entries.push_back(".");
        entries.push_back("..");
            
        for (const auto& entry : directory_iterator(currentPath)) entries.push_back(entry.path().filename());  
        sort(entries.begin(), entries.end(), [](const path& a, const path& b) 
			{	// Uses lexicographical comparison
                return a.string() < b.string();
            }
		); 
        for (const auto& entry : entries) cout << entry.string() << '\n'; 
        return EXIT_SUCCESS;
    }
    catch (const filesystem_error& e)
    {
        return failed_operation(optional<exception>(e));
    }
}

int my_mkdir(const string& dirname)
{
    try
    {
		string actual_dirname;
        if (dirname[0] == '$')
        {
            optional<string> opt = get_variable_value(dirname.substr(1));
            if (!opt.has_value())
            {
                cerr << "Bad command: my_mkdir\n";
                return EXIT_FAILURE;
            }
            actual_dirname = opt.value();
        }
        else actual_dirname = dirname;
        

        // Verify that dirname is alphanumeric
        if (!all_of(actual_dirname.begin(), actual_dirname.end(), [](char c)
            { return isalnum(c); }))
        {
            cerr << "Bad command: my_mkdir\n";
            return EXIT_FAILURE;
        }
        if (!create_directory(actual_dirname))
        {
            cerr << "Failed to create directory: " << actual_dirname << '\n';
            return EXIT_FAILURE;
        }
		cout << "Successfully created new Directory " << actual_dirname << "\n";
        return EXIT_SUCCESS;
    }
    catch (const filesystem_error& e)
    {
        return failed_operation(optional<exception>(e));
    }
}

int my_touch(const string& filename)
{
    try
    {
        string actual_filename;
        if (filename[0] == '$')
        {
            optional<string> opt = get_variable_value(filename.substr(1));
            if (!opt.has_value())
            {
                cerr << "Bad command: my_touch\n";
                return EXIT_FAILURE;
            }
            actual_filename = opt.value();
        }
        else actual_filename = filename;

        // Verify that filename is alphanumeric
        if (!all_of(actual_filename.begin(), actual_filename.end(), [](char c) { return isalnum(c); }))
        {
            cerr << "Bad command: my_touch\n";
            return EXIT_FAILURE;
        }
        ofstream file(actual_filename);
        if (!file)
        {
            cerr << "Failed to create file: " << actual_filename << '\n';
            return EXIT_FAILURE;
        }
        file.close();
        cout << "Successfully created new file " << actual_filename << "\n";
        return EXIT_SUCCESS;
    }
    catch (const filesystem_error& e)
    {
        return failed_operation(optional<exception>(e));
    }
}

int my_cd(const string& dirname)
{
    try
    {
        // Handle special case for parent directory
        if (dirname == "..")
        {
            current_path(current_path().parent_path());
            cout << "Changed directory to: " << current_path().string() << "\n";
            return EXIT_SUCCESS;
        }

        // Verify that dirname exists and is a directory
        path target_path = current_path() / dirname;
        if (!exists(target_path) || !is_directory(target_path))
        {
            cerr << "Directory does not exist: " << dirname << "\n";
            return EXIT_FAILURE;
        }
        current_path(target_path);
        cout << "Changed directory to: " << current_path().string() << "\n";
        return EXIT_SUCCESS;
    }
    catch (const filesystem_error& e)
    {
        return failed_operation(optional<exception>(e));
    }
}

int run(const string& command_with_args)
{
#ifdef _WIN32
    try
    {
        STARTUPINFO si;
        PROCESS_INFORMATION pi;

        ZeroMemory(&si, sizeof(si));
        si.cb = sizeof(si);
        ZeroMemory(&pi, sizeof(pi));

        // Create mutable command string (CreateProcess requires modifiable buffer)
        string cmd = command_with_args;

        // Create process
        if (!CreateProcess(
            nullptr,               // No module name (use command line)
            cmd.data(),            // Command line
            nullptr,               // Process handle not inheritable
            nullptr,               // Thread handle not inheritable
            FALSE,                 // Set handle inheritance to FALSE
            0,                     // No creation flags
            nullptr,               // Use parent's environment block
            nullptr,               // Use parent's starting directory 
            &si,                   // Pointer to STARTUPINFO structure
            &pi                    // Pointer to PROCESS_INFORMATION structure
        ))
        {
            cerr << "CreateProcess failed: " << GetLastError() << "\n";
            return EXIT_FAILURE;
        }

        // Wait for the process to complete
        WaitForSingleObject(pi.hProcess, INFINITE);

        // Get the exit code
        DWORD exitCode;
        GetExitCodeProcess(pi.hProcess, &exitCode);

        // Clean up process and thread handles
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);

        return static_cast<int>(exitCode);
    }
    catch (const exception& e)
    {
        return failed_operation(optional<exception>(e));
    }
#else
    try
    {
        pid_t pid = fork();

        if (pid == -1)
        {
            cerr << "Fork failed\n";
            return EXIT_FAILURE;
        }

        if (pid == 0)  // Child process
        {
            vector<string> args;
            istringstream iss(command_with_args);
            string token;

            while (iss >> token)
                args.push_back(token);

            if (args.empty())
            {
                cerr << "No command specified\n";
                exit(EXIT_FAILURE);
            }

            vector<char*> c_args;
            for (auto& arg : args)
                c_args.push_back(arg.data());
            c_args.push_back(nullptr);

            execvp(c_args[0], c_args.data());

            cerr << "Failed to execute command: " << strerror(errno) << "\n";
            exit(EXIT_FAILURE);
        }
        else  // Parent process
        {
            int status;
            waitpid(pid, &status, 0);

            if (WIFEXITED(status))
                return WEXITSTATUS(status);
            else
                return EXIT_FAILURE;
        }
    }
    catch (const exception& e)
    {
        return failed_operation(optional<exception>(e));
    }
#endif
}
