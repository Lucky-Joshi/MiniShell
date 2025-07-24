#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <windows.h>
#include <direct.h>
#include <stdlib.h>
#include <stdio.h>
#include <io.h>
#include <conio.h>
#include <deque>
#include <Lmcons.h>

#define ANSI_RESET "\x1b[0m"
#define ANSI_GREEN "\x1b[32m"
#define ANSI_BLUE  "\x1b[34m"
#define ANSI_RED   "\x1b[31m"

// Store in-memory command history
std::deque<std::string> history;
int historyIndex = -1;

std::vector<std::string> tokenize(const std::string& input, char delimiter = ' ') {
    std::stringstream ss(input);
    std::string token;
    std::vector<std::string> tokens;
    while (std::getline(ss, token, delimiter)) {
        if (!token.empty()) tokens.push_back(token);
    }
    return tokens;
}

void handlePwd() {
    char buffer[MAX_PATH];
    if (_getcwd(buffer, MAX_PATH))
        std::cout << buffer << "\n";
    else
        std::cerr << "Error: Could not get current directory.\n";
}

void handleCd(const std::vector<std::string>& args) {
    if (args.size() < 2) {
        std::cerr << "Usage: cd <directory>\n";
        return;
    }
    if (_chdir(args[1].c_str()) != 0)
        std::cerr << "Error: Could not change directory.\n";
}

void handleMkdir(const std::vector<std::string>& args) {
    if (args.size() < 2) {
        std::cerr << "Usage: mkdir <folder>\n";
        return;
    }
    if (_mkdir(args[1].c_str()) != 0)
        std::cerr << "Error: Could not create directory.\n";
}

void handleClear() {
    system("cls");
}

void handleEcho(const std::vector<std::string>& args) {
    for (size_t i = 1; i < args.size(); ++i)
        std::cout << args[i] << " ";
    std::cout << "\n";
}

void handleHistory() {
    int idx = 1;
    for (const auto& cmd : history) {
        std::cout << idx++ << ": " << cmd << "\n";
    }
}

void handleRmdir(const std::vector<std::string>& args) {
    if (args.size() < 2) {
        std::cerr << "Usage: rmdir <directory>\n";
        return;
    }
    if (_rmdir(args[1].c_str()) != 0)
        std::cerr << "Error: Could not remove directory.\n";
}

void handleCat(const std::vector<std::string>& args) {
    if (args.size() < 2) {
        std::cerr << "Usage: cat <file>\n";
        return;
    }
    FILE* file = fopen(args[1].c_str(), "r");
    if (!file) {
        std::cerr << "Error: Could not open file.\n";
        return;
    }
    char buffer[1024];
    while (fgets(buffer, sizeof(buffer), file)) {
        std::cout << buffer;
    }
    fclose(file);
}

void handleTouch(const std::vector<std::string>& args) {
    if (args.size() < 2) {
        std::cerr << "Usage: touch <file>\n";
        return;
    }
    FILE* file = fopen(args[1].c_str(), "a");
    if (!file) {
        std::cerr << "Error: Could not create file.\n";
        return;
    }
    fclose(file);
}

void handleDate() {
    SYSTEMTIME st;
    GetLocalTime(&st);
    printf("%04d-%02d-%02d %02d:%02d:%02d\n",
        st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
}

void handleWhoami() {
    char username[UNLEN + 1];
    DWORD username_len = UNLEN + 1;
    if (GetUserNameA(username, &username_len))
        std::cout << username << "\n";
    else
        std::cerr << "Error: Could not get username.\n";
}

void handleHelp() {
    std::cout << ANSI_GREEN "MiniShell Commands:\n" ANSI_RESET;
    std::cout << "  cd <dir>        - Change directory\n";
    std::cout << "  pwd             - Print working directory\n";
    std::cout << "  mkdir <name>    - Create folder\n";
    std::cout << "  rmdir <name>    - Remove folder\n";
    std::cout << "  clear           - Clear screen\n";
    std::cout << "  echo <msg>      - Print message\n";
    std::cout << "  ls              - List directory contents\n";
    std::cout << "  cat <file>      - Show file contents\n";
    std::cout << "  touch <file>    - Create empty file\n";
    std::cout << "  date            - Show date and time\n";
    std::cout << "  whoami          - Show current user\n";
    std::cout << "  history         - Show command history\n";
    std::cout << "  exit            - Exit shell\n";
    std::cout << "  help            - Show this help menu\n";
    std::cout << "  command &       - Run in background\n";
    std::cout << "  cmd1 && cmd2    - Run multiple commands\n";
}

void handleExternalCommand(const std::vector<std::string>& args, bool runInBackground) {
    std::string commandLine;
    for (const auto& arg : args) commandLine += arg + " ";

    char cmd[MAX_PATH];
    strncpy(cmd, commandLine.c_str(), MAX_PATH);

    STARTUPINFO si = { sizeof(STARTUPINFO) };
    PROCESS_INFORMATION pi;

    BOOL success = CreateProcessA(
        NULL, cmd, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi
    );

    if (!success) {
        std::cerr << "Error: Failed to execute command.\n";
        return;
    }

    if (!runInBackground)
        WaitForSingleObject(pi.hProcess, INFINITE);

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
}

std::string readInputWithHistory() {
    std::string input;
    char ch;
    std::string tempInput;
    historyIndex = history.size();

    while (true) {
        ch = _getch();

        if (ch == 13) { // Enter
            std::cout << "\n";
            break;
        } else if (ch == 8) { // Backspace
            if (!input.empty()) {
                input.pop_back();
                std::cout << "\b \b";
            }
        } else if (ch == 0 || ch == 224) { // Arrow keys
            ch = _getch();
            if (ch == 72 && historyIndex > 0) { // Up arrow
                --historyIndex;
                tempInput = history[historyIndex];
                std::cout << "\r\33[2KLucky@minishell$ " << tempInput;
                input = tempInput;
            } else if (ch == 80 && historyIndex < history.size() - 1) { // Down arrow
                ++historyIndex;
                tempInput = history[historyIndex];
                std::cout << "\r\33[2KLucky@minishell$ " << tempInput;
                input = tempInput;
            }
        } else {
            input += ch;
            std::cout << ch;
        }
    }

    if (!input.empty()) history.push_back(input);
    return input;
}

void executeLine(std::string line) {
    std::vector<std::string> chained = tokenize(line, '&');
    for (size_t i = 0; i < chained.size(); ++i) {
        std::string trimmed = chained[i];
        bool background = false;

        if (!trimmed.empty() && trimmed.back() == ' ') trimmed.pop_back();
        if (!trimmed.empty() && trimmed.front() == ' ') trimmed.erase(0, 1);

        std::vector<std::string> args = tokenize(trimmed);

        if (args.empty()) continue;
        std::string command = args[0];

        // Aliases
        if (command == "ls") command = "dir";

        // Built-in commands
        if (command == "exit") {
            std::cout << "Goodbye! 👋\n";
            exit(0);
        } else if (command == "pwd") handlePwd();
        else if (command == "cd") handleCd(args);
        else if (command == "mkdir") handleMkdir(args);
        else if (command == "rmdir") handleRmdir(args);
        else if (command == "clear") handleClear();
        else if (command == "echo") handleEcho(args);
        else if (command == "cat") handleCat(args);
        else if (command == "touch") handleTouch(args);
        else if (command == "date") handleDate();
        else if (command == "whoami") handleWhoami();
        else if (command == "history") handleHistory();
        else if (command == "help") handleHelp();
        else handleExternalCommand(args, false);
    }
}

int main() {
    std::string input;
    SetConsoleOutputCP(CP_UTF8); // UTF-8 support

    while (true) {
        std::cout << ANSI_BLUE "Lucky@minishell$ " ANSI_RESET;
        input = readInputWithHistory();

        if (input.find("&&") != std::string::npos) {
            std::vector<std::string> commands = tokenize(input, '&');
            for (auto& cmd : commands) executeLine(cmd);
        } else {
            executeLine(input);
        }
    }

    return 0;
}
