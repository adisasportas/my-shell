#include "myFunction.h"

char *strwok(char *str, const char *delim)
{
    static char *next_token = NULL;
    if (str)
        next_token = str;
    if (!next_token || *next_token == '\0')
        return NULL;

    char *token_start = next_token;
    char *current;
    for (current = next_token; *current != '\0'; current++)
    {
        const char *d;
        for (d = delim; *d != '\0'; d++)
        {
            if (*current == *d)
            {

                token_start++;
                break;
            }
        }
        if (*d == '\0')
            break;
    }
    if (*token_start == '\0')
    {
        next_token = token_start;
        return NULL;
    }
    next_token = token_start;
    for (current = token_start; *current != '\0'; current++)
    {
        const char *d;
        for (d = delim; *d != '\0'; d++)
        {
            if (*current == *d)
            {
                *current = '\0';
                next_token = current + 1;
                return token_start;
            }
        }
    }

    next_token = current;
    return token_start;
}
char *getInputFromUser()
{
    char ch;
    int size = 1;
    int index = 0;
    char *str = (char *)malloc(size * sizeof(char));
    while ((ch = getchar()) != '\n')
    {
        *(str + index) = ch;
        size++;
        index++;
        str = (char *)realloc(str, size * sizeof(char));
    }
    *(str + index) = '\0';

    return str;
}
char **splitArgument(char *str)
{

    char *subStr;
    int size = 2;
    int index = 0;
    subStr = strwok(str, " ");
    char **argumnts = (char **)malloc(size * sizeof(char *));
    *(argumnts + index) = subStr;
    while ((subStr = strwok(NULL, " ")) != NULL)
    {
        size++;
        index++;
        *(argumnts + index) = subStr;
        argumnts = (char **)realloc(argumnts, size * sizeof(char *));
    }
    *(argumnts + (index + 1)) = NULL;

    return argumnts;
}
void getLocation()
{
    char location[BUFF_SIZE];
    char hostname[BUFF_SIZE];
    char *username = getenv("USER");

    if (getcwd(location, BUFF_SIZE) == NULL)
    {
        perror("Error retrieving current directory");
        return;
    }

    if (gethostname(hostname, BUFF_SIZE) != 0)
    {
        perror("Error retrieving hostname");
        return;
    }

    if (username == NULL)
    {
        fprintf(stderr, "Error retrieving username\n");
        return;
    }

    green();
    bold();
    printf("%s@%s:", username, hostname);
    reset();
    blue();
    printf("%s", location);
    reset();
    printf("$ ");
}
void logout(char *input)
{
    free(input);
    puts("Logout successful.");
    exit(EXIT_SUCCESS);
}
void echo(char **arg)
{
    while (*(++arg))
    {
        printf("%s ", *arg);
    }
    puts("");
}
void echoppend(char **args)
{

    char textToAppend[2048] = {0};
    char filePath[2048] = {0};

    int redirectIndex = -1;
    for (int i = 0; args[i] != NULL; ++i)
    {
        if (strcmp(args[i], ">>") == 0)
        {
            redirectIndex = i;
            break;
        }
    }

    if (redirectIndex == -1)
    {
        fprintf(stderr, "Error: Command format incorrect. Expected '>>' for redirection.\n");
        return;
    }

    for (int i = 1; i < redirectIndex; ++i)
    {
        strcat(textToAppend, args[i]);
        if (i < redirectIndex - 1)
            strcat(textToAppend, " ");
    }

    for (int i = redirectIndex + 1; args[i] != NULL; ++i)
    {
        strcat(filePath, args[i]);
        if (args[i + 1] != NULL)
            strcat(filePath, " ");
    }

    FILE *file = fopen(filePath, "a");
    if (file == NULL)
    {
        perror("Error opening file");
        return;
    }

    fprintf(file, "%s\n", textToAppend);
    fclose(file);

    printf("Appended '%s' to '%s'.\n", textToAppend, filePath);
}
void echorite(char **args)
{
    char textToWrite[2048] = {0};
    char filePath[2048] = {0};

    int foundRedirection = 0;
    for (int i = 1; args[i] != NULL; i++)
    {
        if (strcmp(args[i], ">") == 0)
        {
            foundRedirection = 1;
            if (args[i + 1] != NULL)
            {
                for (int j = i + 1; args[j] != NULL; j++)
                {
                    if (j > i + 1)
                        strcat(filePath, " ");
                    strcat(filePath, args[j]);
                }
            }
            else
            {
                fprintf(stderr, "File path not provided.\n");
                return;
            }
            break;
        }
        else
        {

            if (i > 1)
                strcat(textToWrite, " ");
            strcat(textToWrite, args[i]);
        }
    }
    if (!foundRedirection)
    {
        fprintf(stderr, "Redirection operator '>' not found.\n");
        return;
    }
    FILE *file = fopen(filePath, "w");
    if (!file)
    {
        perror("Error opening file");
        return;
    }
    fprintf(file, "%s\n", textToWrite);
    fclose(file);

    printf("Content written to '%s'.\n", filePath);
}
void cd(char **arg)
{
    char path[1024] = {0};

    if (arg[1] == NULL)
    {
        printf("-myShell: cd: missing operand\n");
        return;
    }

    if (strcmp(arg[1], "..") == 0 && arg[2] == NULL)
    {
        if (chdir("..") != 0)
        {
            perror("-myShell: cd: error changing to parent directory");
        }
        return;
    }

    if (arg[1][0] == '\"')
    {

        strncpy(path, arg[1] + 1, sizeof(path) - 1);
        size_t path_len = strlen(path);
        if (path[path_len - 1] == '\"')
        {
            path[path_len - 1] = '\0';
        }
        else if (arg[2] != NULL)
        {
            for (int i = 2; arg[i] != NULL; i++)
            {
                strcat(path, " ");
                (path, arg[i], sizeof(path) - strlen(path) - 1);
                if (arg[i][strlen(arg[i]) - 1] == '\"')
                {
                    path[strlen(path) - 1] = '\0';
                    break;
                }
            }
        }
    }
    else
    {

        strncpy(path, arg[1], sizeof(path) - 1);
        for (int i = 2; arg[i] != NULL; i++)
        {
            if (strlen(path) + strlen(arg[i]) + 2 > sizeof(path))
            {
                printf("-myShell: cd: path is too long\n");
                return;
            }
            strcat(path, " ");
            strcat(path, arg[i]);
        }
    }

    if (chdir(path) != 0)
    {
        printf("-myShell: cd: %s: No such file or directory\n", path);
    }
}
void cp(char **arguments)
{
    char ch;
    FILE *src, *des;
    if ((src = fopen(arguments[1], "r")) == NULL)
    {
        puts("Erorr");
        return;
    }
    if ((des = fopen(arguments[2], "w")) == NULL)
    {
        puts("Erorr");
        fclose(src);
        return;
    }

    while ((ch = fgetc(src)) != EOF)
    {
        fputc(ch, des);
    }
    fclose(src);
    fclose(des);
}
void delete(char **path)
{
    if (path == NULL || path[1] == NULL)
    {
        fprintf(stderr, "Error: Path is NULL or not provided.\n");
        return;
    }
    char fullPath[1024] = {0};
    for (int i = 1; path[i] != NULL; ++i)
    {
        if (i > 1)
            strcat(fullPath, " ");
        strcat(fullPath, path[i]);
    }
    printf("Attempting to delete file: %s\n", fullPath);
    if (unlink(fullPath) == 0)
    {
        printf("File '%s' successfully deleted.\n", fullPath);
    }
    else
    {
        perror("Error deleting file");
    }
}
void systemCall(char **arg)
{

    pid_t pid = fork();
    if (pid == -1)
    {
        printf("fork err\n");
        return;
    }
    if (pid == 0 && execvp(arg[0], arg) == -1)
        exit(1);
}
char **splitInput(char *input, int *pipeIndex)
{
    char **arguments = malloc(strlen(input) * sizeof(char *));
    if (arguments == NULL)
    {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    int argCount = 0;
    int index = 0;
    *pipeIndex = -1;
    for (int i = 0; input[i] != '\0'; i++)
    {

        if (input[i] == '|')
        {
            *pipeIndex = i;
            break;
        }
        if (input[i] != ' ')
        {
            int start = i;

            while (input[i] != ' ' && input[i] != '|' && input[i] != '\0')
                i++;
            int end = i;

            arguments[argCount] = malloc((end - start + 1) * sizeof(char));
            if (arguments[argCount] == NULL)
            {
                fprintf(stderr, "Memory allocation failed\n");
                exit(EXIT_FAILURE);
            }

            strncpy(arguments[argCount], input + start, end - start);
            arguments[argCount][end - start] = '\0';

            argCount++;
        }
    }
    arguments[argCount] = NULL;

    return arguments;
}
char **splitAfterPipe(char *input, int pipeIndex)
{
    char **arguments = malloc(strlen(input) * sizeof(char *));
    if (arguments == NULL)
    {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    int argCount = 0;
    for (int i = pipeIndex + 1; input[i] != '\0'; i++)
    {

        while (input[i] == ' ')
            i++;

        if (input[i] != '\0')
        {
            int start = i;

            while (input[i] != ' ' && input[i] != '\0')
                i++;
            int end = i;
            arguments[argCount] = malloc((end - start + 1) * sizeof(char));
            if (arguments[argCount] == NULL)
            {
                fprintf(stderr, "Memory allocation failed\n");
                exit(EXIT_FAILURE);
            }

            strncpy(arguments[argCount], input + start, end - start);
            arguments[argCount][end - start] = '\0';
            argCount++;
        }
    }
    arguments[argCount] = NULL;

    return arguments;
}
void mypipe(char **argv1, char **argv2)
{
    int fildes[2];
    if (fork() == 0)
    {
        pipe(fildes);
        if (fork() == 0)
        {
            close(STDOUT_FILENO);
            dup(fildes[1]);
            close(fildes[1]);
            close(fildes[0]);

            execvp(argv1[0], argv1);
        }

        close(STDIN_FILENO);
        dup(fildes[0]);
        close(fildes[0]);
        close(fildes[1]);

        execvp(argv2[0], argv2);
    }
}
void move(char **args)
{
    char srcPath[2048] = {0};
    char destPath[2048] = {0};

    strcat(srcPath, args[1]);
    if (args[2] != NULL)
    {
        strcat(srcPath, " ");
        strcat(srcPath, args[2]);
    }
    if (args[3] != NULL)
    {
        strcat(destPath, args[3]);
        if (args[4] != NULL)
        {
            strcat(destPath, " ");
            strcat(destPath, args[4]);
        }
    }

    printf("Attempting to move from '%s' to '%s'\n", srcPath, destPath);
    if (rename(srcPath, destPath) == 0)
    {
        printf("File successfully moved.\n");
    }
    else
    {
        perror("Error movingfile");
    }
}
void readd(char **args)
{
    if (args[1] == NULL)
    {
        fprintf(stderr, "Usage: read <path>\n");
        return;
    }
    char filePath[2048] = {0};
    for (int i = 1; args[i] != NULL; ++i)
    {
        if (i > 1)
            strcat(filePath, " ");
        strcat(filePath, args[i]);
    }
    FILE *file = fopen(filePath, "r");
    if (!file)
    {
        perror("Error opening file");
        return;
    }
    char buffer[1024];
    while (fgets(buffer, sizeof(buffer), file) != NULL)
    {
        printf("%s", buffer);
    }
    fclose(file);
}
void wordCount(char **args)
{
    if (args[1] == NULL || args[2] == NULL)
    {
        fprintf(stderr, "Usage: wc <-l/-w> <file path>\n");
        return;
    }
    char *option = args[1];
    char file_path[2048] = {0};
    for (int i = 2; args[i] != NULL; ++i)
    {
        if (i > 2)
            strcat(file_path, " ");
        strcat(file_path, args[i]);
    }
    FILE *file = fopen(file_path, "r");
    if (file == NULL)
    {
        printf("File '%s' does not exist or cannot be opened.\n", file_path);
        return;
    }
    int count = 0;

    if (strcmp(option, "-l") == 0)
    {
        char buffer[1024];
        while (fgets(buffer, sizeof(buffer), file) != NULL)
        {
            count++;
        }
        printf("Number of lines in the file: %d\n", count);
    }
    else if (strcmp(option, "-w") == 0)
    {
        int prev_char = ' ';
        int current_char;
        while ((current_char = fgetc(file)) != EOF)
        {
            if (current_char == ' ' || current_char == '\n' || current_char == '\t')
            {
                if (prev_char != ' ' && prev_char != '\n' && prev_char != '\t')
                {
                    count++;
                }
            }
            prev_char = current_char;
        }
        if (prev_char != ' ' && prev_char != '\n' && prev_char != '\t')
        {
            count++;
        }
        printf("Number of words in the file: %d\n", count);
    }
    else
    {
        fprintf(stderr, "Invalid option. Usage: wc <-l/-w> <file path>\n");
    }
    fclose(file);
}