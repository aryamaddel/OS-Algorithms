#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <vector>
#include <string>
#include <sstream>

// Function to split a string into a vector of strings
std::vector<std::string> split(const std::string& s) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, ' ')) {
        tokens.push_back(token);
    }
    return tokens;
}

int main() {
    std::string command_line;

    std::cout << "Enter a Linux command (e.g., 'cp source.txt dest.txt' or 'grep hello file.txt'). Type 'exit' to quit." << std::endl;

    while (true) {
        std::cout << "> ";
        std::getline(std::cin, command_line);

        if (command_line == "exit") {
            std::cout << "Exiting shell simulator. PID: " << getpid() << std::endl;
            exit(0); // Exit the parent process
        }

        std::vector<std::string> args_str = split(command_line);
        if (args_str.empty()) {
            continue; // Handle empty command
        }
        
        // Convert vector of strings to char* array for execvp
        std::vector<char*> args;
        for (const auto& s : args_str) {
            args.push_back(const_cast<char*>(s.c_str()));
        }
        args.push_back(nullptr); // execvp expects a null-terminated array

        // Fork a new process
        pid_t pid = fork();

        if (pid < 0) {
            // Error handling for fork failure
            perror("fork failed");
            exit(1);
        } else if (pid == 0) {
            // --- This is the child process ---
            std::cout << "Child Process Started. PID: " << getpid() << ", Parent PID: " << getppid() << std::endl;
            
            // Execute the command
            execvp(args[0], args.data());
            
            // execvp only returns if an error occurred
            perror("execvp failed");
            exit(1); // Exit child with an error status
        } else {
            // --- This is the parent process ---
            std::cout << "Parent Process. PID: " << getpid() << ", Waiting for child PID: " << pid << std::endl;
            
            int status;
            // wait() system call: Parent waits for the child process to terminate
            wait(&status);
            
            if (WIFEXITED(status)) {
                std::cout << "Child process " << pid << " exited with status " << WEXITSTATUS(status) << std::endl;
            } else {
                std::cout << "Child process " << pid << " terminated abnormally." << std::endl;
            }
        }
    }
}
