#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#define BUFF_SIZE 256
#define blue() printf("\033[0;34m")
#define reset() printf("\033[0m")
#define green() printf("\033[0;32m")
#define bold() printf("\e[1m")
#define boldOff() printf("\e[m")
void get_dir();
/**
 * Reads a line of input from the user.
 *
 * This function reads characters from the standard input (stdin) until a newline
 * character ('\n') is encountered, indicating the end of the input line.
 * It dynamically allocates memory to store the input string. The caller is
 * responsible for freeing the allocated memory when it's no longer needed.
 *
 * @return A dynamically allocated string (char*) containing the input line read from the user.
 *         The string is null-terminated. Returns NULL if memory allocation fails.
 */
char *getInputFromUser();
/**
 * Splits a string into individual tokens based on whitespace.
 *
 * This function takes a string as input and tokenizes it based on whitespace
 * characters (space, tab, newline). It dynamically allocates memory for an
 * array of strings to store the tokens. The caller is responsible for freeing
 * the memory allocated for the array and its elements when it's no longer needed.
 *
 * @param input A pointer to the string to be split.
 *
 * @return A dynamically allocated array of strings (char**) containing the tokens.
 *         The last element of the array is NULL. Returns NULL if input is NULL
 *         or if memory allocation fails.
 *
 * @note The input string will be modified (tokenized) by this function.
 *       Make sure to use a copy of the original string if preservation
 *       of the original string is required.
 *
 * @warning The caller is responsible for freeing the memory allocated for
 *          the returned array and its elements using free().
 */
char **splitArgument(char *);
/**
 * Tokenizes a string based on a specified delimiter.
 *
 * This function takes a string and a delimiter as input and tokenizes the string
 * based on occurrences of the delimiter character(s). It returns a pointer to the
 * next token in the string on each successive call, or NULL when no more tokens
 * are found.
 *
 * @param str A pointer to the string to be tokenized.
 *            If NULL, the function continues tokenization from the previous position.
 * @param delim A pointer to the string containing the delimiter character(s).
 *
 * @return A pointer to the next token in the string, or NULL if no more tokens are found.
 *
 * @note The input string will be modified (tokenized) by this function.
 *       Make sure to use a copy of the original string if preservation
 *       of the original string is required.
 *
 * @warning The function maintains internal state to track the next token position.
 *          It is not thread-safe when used concurrently with multiple input strings.
 */
char *strwok(char *str, const char *delim);
/**
 * Retrieves the current location (working directory) and system hostname.
 *
 * This function retrieves the current working directory and system hostname,
 * and prints them in the format "<username>@<hostname>:<location>$ ".
 * The username is retrieved from the environment variable "USER".
 * The function uses ANSI escape codes to colorize and format the output.
 *
 * @note The function uses the global constants BUFF_SIZE for buffer size.
 *       Ensure that it is defined and has an appropriate value.
 *       Make sure to include the necessary header files.
 *
 * @warning This function may fail if the current working directory or system
 *          hostname cannot be retrieved. Error messages are printed to stderr
 *          if such errors occur. Additionally, if the username cannot be retrieved,
 *          an error message is printed to stderr.
 */
void getLocation();
/**
 * Logs out the current user and terminates the program.
 *
 * This function frees the memory allocated for the input string,
 * prints a logout message to stdout, and terminates the program
 * with a success exit code.
 *
 * @param input A pointer to the input string that was tokenized.
 *              The memory allocated for this string is freed.
 *
 * @note This function is typically called after the user logs out
 *       or terminates a session. It performs cleanup tasks and
 *       gracefully exits the program.
 *
 * @warning This function does not perform any validation on the
 *          input parameters. Ensure that the input parameter is
 *          a valid pointer to a dynamically allocated string.
 *          Calling this function with invalid input may result
 *          in undefined behavior.
 */
void logout(char *);
/**
 * Prints tokens of a string array to stdout.
 *
 * This function takes an array of strings as input and prints each
 * token to stdout, separated by a space character. It continues printing
 * tokens until reaching a NULL pointer in the array.
 *
 * @param arg A pointer to the array of strings (char**) containing the tokens.
 *            The array must be terminated with a NULL pointer.
 *
 * @note This function assumes that the input array is properly terminated
 *       with a NULL pointer to indicate the end of tokens.
 *
 * @warning This function does not perform any validation on the input parameters.
 *          Ensure that the input array is properly formatted and terminated
 *          with a NULL pointer. Calling this function with an improperly
 *          terminated array may result in undefined behavior.
 */
void echo(char **);
/**
 * Appends text to a file specified by the user.
 *
 * This function takes an array of strings as input, where the first part of the
 * array contains the text to be appended, and the second part contains the path
 * to the file to which the text should be appended. The function searches for
 * the '>>' token to determine the split between the text and the file path.
 * If the '>>' token is not found or if the command format is incorrect, an
 * error message is displayed, and no action is taken.
 *
 * @param args A pointer to the array of strings (char**) containing the command
 *             and its arguments. The array must be properly formatted, with the
 *             text to be appended preceding the '>>' token, followed by the file
 *             path to which the text should be appended.
 *
 * @note This function assumes that the input array is properly formatted, with
 *       the '>>' token separating the text and the file path. If the command
 *       format is incorrect or the '>>' token is not found, an error message
 *       is displayed, and no action is taken.
 *
 * @warning This function does not perform any validation on the input parameters.
 *          Ensure that the input array is properly formatted before calling
 *          this function. Calling this function with an improperly formatted
 *          array may result in undefined behavior.
 */
void echoppend(char **);
/**
 * Writes text to a file specified by the user, replacing existing content.
 *
 * This function takes an array of strings as input, where the text to be written
 * precedes the '>' operator, followed by the file path to which the text should
 * be written. It searches for the '>' operator to determine the split between the
 * text and the file path. If the '>' operator is not found or if the command format
 * is incorrect, an error message is displayed, and no action is taken.
 *
 * @param args A pointer to the array of strings (char**) containing the command
 *             and its arguments. The array must be properly formatted, with the
 *             text to be written preceding the '>' operator, followed by the file
 *             path to which the text should be written.
 *
 * @note This function assumes that the input array is properly formatted, with
 *       the '>' operator separating the text and the file path. If the command
 *       format is incorrect or the '>' operator is not found, an error message
 *       is displayed, and no action is taken.
 *
 * @warning This function does not perform any validation on the input parameters.
 *          Ensure that the input array is properly formatted before calling
 *          this function. Calling this function with an improperly formatted
 *          array may result in undefined behavior.
 */
void echorite(char **);
/**
 * Copies the contents of one file to another.
 *
 * This function takes an array of strings as input, where the first argument
 * is the path to the source file and the second argument is the path to the
 * destination file. It opens the source file for reading and the destination
 * file for writing. It then reads the contents of the source file character
 * by character and writes them to the destination file until the end of the
 * source file is reached. If any errors occur during file operations, an error
 * message is displayed, and the function returns without completing the copy
 * operation.
 *
 * @param arguments A pointer to the array of strings (char**) containing the
 *                  source and destination file paths.
 *
 * @note This function assumes that the input array is properly formatted, with
 *       the source file path as the first argument and the destination file
 *       path as the second argument. If the command format is incorrect or if
 *       any errors occur during file operations, the function displays an error
 *       message and returns without completing the copy operation.
 *
 * @warning This function does not perform any validation on the input parameters
 *          or file paths. Ensure that the input array is properly formatted and
 *          that the file paths are valid before calling this function. Calling
 *          this function with invalid file paths or improper command format may
 *          result in undefined behavior.
 */
void cd(char **);
/**
 * Copies the contents of one file to another.
 *
 * This function takes an array of strings as input, where the first two arguments
 * are the source file path and the destination file path, respectively. It attempts
 * to open the source file for reading and the destination file for writing. If either
 * file cannot be opened, an appropriate error message is displayed, and the function
 * returns without performing any file copying. If both files are successfully opened,
 * the function reads each character from the source file and writes it to the destination
 * file until the end of file (EOF) is reached in the source file. Finally, both files
 * are closed, and the function terminates.
 *
 * @param arguments A pointer to the array of strings (char**) containing the source
 *                  and destination file paths.
 *
 * @note This function assumes that the input array is properly formatted, with the
 *       source and destination file paths as the first and second arguments, respectively.
 *       If the source or destination file path is not provided, or if either file cannot
 *       be opened, the function displays an appropriate error message and returns without
 *       performing any file copying.
 *
 * @warning This function does not perform any validation on the input parameters or file
 *          paths. Ensure that the input array is properly formatted and that the source
 *          and destination file paths are valid before calling this function. Calling this
 *          function with invalid file paths or improper command format may result in undefined
 *          behavior or unintentional file operations.
 */
void cp(char **);
/**
 * Deletes a file specified by its path.
 *
 * This function takes an array of strings as input, where the first argument
 * is the path to the file to be deleted. It concatenates all elements of the
 * input array to construct the full file path. If the file path is NULL or not
 * provided, or if the file cannot be deleted, an appropriate error message is
 * displayed. If the file is successfully deleted, a success message is printed.
 *
 * @param path A pointer to the array of strings (char**) containing the file path.
 *
 * @note This function assumes that the input array is properly formatted, with
 *       the file path as the first argument. If the file path is NULL or not
 *       provided, the function displays an error message and returns without
 *       attempting to delete the file. If an error occurs during deletion, an
 *       error message is displayed, and the function returns without completing
 *       the deletion operation.
 *
 * @warning This function does not perform any validation on the input parameters
 *          or file path. Ensure that the input array is properly formatted and
 *          that the file path is valid before calling this function. Calling
 *          this function with an invalid file path or improper command format may
 *          result in undefined behavior or unintentional deletion of files.
 */
void delete(char **);
/**
 * Executes a system command with the specified arguments.
 *
 * This function takes an array of strings as input, where the first argument is
 * the command to be executed, and subsequent arguments are passed as command-line
 * arguments to the executed command. It creates a new process using fork(), and
 * the child process then replaces itself with the specified command using execvp().
 * If fork() or execvp() fails, appropriate error messages are displayed, and the
 * function returns.
 *
 * @param arg A pointer to the array of strings (char**) containing the command
 *            and its arguments.
 *
 * @note This function assumes that the input array is properly formatted, with
 *       the command name as the first argument and its arguments following it.
 *       If the command cannot be executed or if an error occurs during process
 *       creation, the function displays an appropriate error message and returns.
 *
 * @warning This function does not perform any validation on the input parameters
 *          or command name. Ensure that the command and its arguments are valid
 *          before calling this function. Calling this function with invalid or
 *          malicious input may result in undefined behavior or unintentional
 *          execution of commands on the system.
 */
void systemCall(char **);

void mypipe(char **, char **);
/**
 * Moves a file from one location to another.
 *
 * This function takes an array of strings as input, where the first argument is
 * the source path of the file to be moved, and the second argument is the destination
 * path where the file should be moved. It attempts to move the file using the rename()
 * function. If the move operation is successful, a success message is displayed. If
 * an error occurs during the move operation, an appropriate error message is displayed.
 *
 * @param args A pointer to the array of strings (char**) containing the source path
 *             and destination path of the file to be moved.
 *
 * @note This function assumes that the input array is properly formatted, with the
 *       source path as the first argument and the destination path as the second argument.
 *       If the move operation fails, an error message is displayed, and the function returns.
 *
 * @warning This function does not perform validation on the input parameters. Ensure that
 *          the source and destination paths are valid before calling this function. Calling
 *          this function with invalid paths may result in undefined behavior or unintentional
 *          file operations on the system.
 */
void move(char **args);
/**
 * Reads and displays the contents of a file.
 *
 * This function takes an array of strings as input, where the first argument is
 * expected to be the path to the file to be read. It attempts to open the specified
 * file in read mode and then reads its contents line by line, printing each line
 * to the standard output. If the file cannot be opened or an error occurs during
 * reading, appropriate error messages are displayed.
 *
 * @param args A pointer to the array of strings (char**) containing the path to
 *             the file to be read and any additional arguments.
 *
 * @note This function assumes that the input array is properly formatted, with
 *       the file path as the first argument. If the file path is not provided
 *       or if there are any issues opening or reading the file, error messages
 *       are displayed, and the function returns.
 *
 * @warning This function does not perform extensive error checking on the file
 *          path or the file itself. Ensure that the file path is valid and that
 *          the file can be read before calling this function to avoid unexpected
 *          behavior or errors.
 */
void readd(char **args);
/**
 * Counts the number of lines or words in a file.
 *
 * This function takes an array of strings as input, where the second argument
 * specifies the counting mode (-l for lines, -w for words), and the third argument
 * is expected to be the path to the file to be processed. The function then opens
 * the specified file in read mode and counts either the number of lines or words,
 * depending on the specified mode. The result is printed to the standard output.
 * If the file cannot be opened or if an invalid counting mode is provided, error
 * messages are displayed.
 *
 * @param args A pointer to the array of strings (char**) containing the counting
 *             mode and the path to the file to be processed.
 *
 * @note This function assumes that the input array is properly formatted, with
 *       the counting mode as the second argument (-l or -w) and the file path
 *       as the third argument. If any of these arguments are missing or if there
 *       are any issues opening or reading the file, appropriate error messages
 *       are displayed, and the function returns.
 *
 * @warning This function does not perform extensive error checking on the file
 *          path or the file itself. Ensure that the file path is valid and that
 *          the file can be read before calling this function to avoid unexpected
 *          behavior or errors.
 */
void wordCount(char **args);