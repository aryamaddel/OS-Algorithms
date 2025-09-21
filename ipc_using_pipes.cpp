#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <string>

#define MSG_SIZE 256

int main() {
    char message_to_child[] = "Hello Child Process!";
    char read_buffer[MSG_SIZE];
    int pipefd[2]; // pipefd[0] is for reading, pipefd[1] is for writing

    // Create a pipe
    if (pipe(pipefd) == -1) {
        perror("pipe failed");
        return 1;
    }

    // Fork a new process
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork failed");
        return 1;
    }

    if (pid == 0) {
        // --- Child Process (Reader) ---
        // Close the write end of the pipe, as it's not used by the child
        close(pipefd[1]);
        
        std::cout << "[Child] Reading from pipe..." << std::endl;
        
        // Read the message from the pipe
        read(pipefd[0], read_buffer, MSG_SIZE);
        
        std::cout << "[Child] Message received: '" << read_buffer << "'" << std::endl;
        
        // Close the read end of the pipe
        close(pipefd[0]);
        
        // --- Example of I/O Redirection ---
        // Redirecting child's stdout to a file named "child_output.txt"
        // To see this work, you would need to add file operations here.
        // For simplicity, we just print a message about it.
        std::cout << "[Child] This output could be redirected. Now exiting." << std::endl;
        exit(0);

    } else {
        // --- Parent Process (Writer) ---
        // Close the read end of the pipe, as it's not used by the parent
        close(pipefd[0]);
        
        std::cout << "[Parent] Writing message to pipe: '" << message_to_child << "'" << std::endl;
        
        // Write the message to the pipe
        write(pipefd[1], message_to_child, sizeof(message_to_child));
        
        // Close the write end of the pipe
        close(pipefd[1]);

        // Wait for the child process to finish
        wait(NULL);
        std::cout << "[Parent] Child process has finished. Exiting." << std::endl;
    }

    return 0;
}
