#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>

#include "systemcalls.h"

/**
 * @param cmd the command to execute with system()
 * @return true if the command in @param cmd was executed
 *   successfully using the system() call, false if an error occurred,
 *   either in invocation of the system() call, or if a non-zero return
 *   value was returned by the command issued in @param cmd.
*/
bool do_system(const char *cmd)
{

/*
 * TODO  add your code here
 *  Call the system() function with the command set in the cmd
 *   and return a boolean true if the system() call completed with success
 *   or false() if it returned a failure
*/

    int status = system(cmd);
    return (status == 0) ? true : false;
}

/**
* @param count -The numbers of variables passed to the function. The variables are command to execute.
*   followed by arguments to pass to the command
*   Since exec() does not perform path expansion, the command to execute needs
*   to be an absolute path.
* @param ... - A list of 1 or more arguments after the @param count argument.
*   The first is always the full path to the command to execute with execv()
*   The remaining arguments are a list of arguments to pass to the command in execv()
* @return true if the command @param ... with arguments @param arguments were executed successfully
*   using the execv() call, false if an error occurred, either in invocation of the
*   fork, waitpid, or execv() command, or if a non-zero return value was returned
*   by the command issued in @param arguments with the specified arguments.
*/

bool do_exec(int count, ...)
{
    va_list args;
    va_start(args, count);
    char * command[count+1];
    int i;
    for(i=0; i<count; i++)
    {
        command[i] = va_arg(args, char *);
    }
    command[count] = NULL;
    va_end(args);


/*
 * TODO:
 *   Execute a system command by calling fork, execv(),
 *   and wait instead of system (see LSP page 161).
 *   Use the command[0] as the full path to the command to execute
 *   (first argument to execv), and use the remaining arguments
 *   as second argument to the execv() command.
 *
*/
    pid_t pid = fork();

    if (pid == -1) {
        // Fork failed
        perror("Fork failed");
        return false;
    } else if (pid == 0) {
        // Child process
        if (execv(command[0], command) == -1) {
            // execv failed
            perror("Execv failed");
            exit(EXIT_FAILURE);
        }
    } else {
        // Parent process
        int status;
        if (waitpid(pid, &status, 0) == -1) {
            // waitpid failed
            perror("Waitpid failed");
            return false;
        } else {
            // Check if child process terminated normally
            if (WIFEXITED(status)) {
                // Check the exit status of the child process
                if (WEXITSTATUS(status) == 0) {
                    // Child process exited successfully
                    return true;
                } else {
                    // Child process exited with non-zero status
                    return false;
                }
            } else {
                // Child process did not terminate normally
                return false;
            }
        }
    }

    // This point should not be reached under normal circumstances
    return false;
}

/**
* @param outputfile - The full path to the file to write with command output.
*   This file will be closed at completion of the function call.
* All other parameters, see do_exec above
*/
bool do_exec_redirect(const char *outputfile, int count, ...)
{
    va_list args;
    va_start(args, count);
    char * command[count+1];
    int i;
    for(i=0; i<count; i++)
    {
        command[i] = va_arg(args, char *);
    }
    command[count] = NULL;

    va_end(args);


/*
 * TODO
 *   Call execv, but first using https://stackoverflow.com/a/13784315/1446624 as a refernce,
 *   redirect standard out to a file specified by outputfile.
 *   The rest of the behaviour is same as do_exec()
 *
*/

    // Open the output file for writing, creating it if it does not exist, and truncating it if it does
    int output_fd = open(outputfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (output_fd == -1) {
        perror("Failed to open output file");
        return false;
    }

    pid_t pid = fork();

    if (pid == -1) {
        // Fork failed
        perror("Fork failed");
        close(output_fd); // Close the output file descriptor before returning
        return false;
    } else if (pid == 0) {
        // Child process
        // Redirect standard output to the output file
        if (dup2(output_fd, STDOUT_FILENO) == -1) {
            perror("Failed to redirect standard output");
            close(output_fd); // Close the output file descriptor before exiting
            exit(EXIT_FAILURE);
        }

        // Close the duplicated output file descriptor since it's no longer needed
        close(output_fd);

        // Execute the command
        if (execv(command[0], command) == -1) {
            // execv failed
            perror("Execv failed");
            exit(EXIT_FAILURE);
        }
    } else {
        // Parent process
        int status;
        if (waitpid(pid, &status, 0) == -1) {
            // waitpid failed
            perror("Waitpid failed");
            close(output_fd); // Close the output file descriptor before returning
            return false;
        } else {
            // Check if child process terminated normally
            if (WIFEXITED(status)) {
                // Check the exit status of the child process
                if (WEXITSTATUS(status) == 0) {
                    // Child process exited successfully
                    close(output_fd); // Close the output file descriptor before returning
                    return true;
                } else {
                    // Child process exited with non-zero status
                    close(output_fd); // Close the output file descriptor before returning
                    return false;
                }
            } else {
                // Child process did not terminate normally
                close(output_fd); // Close the output file descriptor before returning
                return false;
            }
        }
    }

    // This point should not be reached under normal circumstances
    close(output_fd); // Close the output file descriptor before returning
    return false;
}
