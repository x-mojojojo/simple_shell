#include "shell.h"

/**
  * print_ps1 - prints the primary shell prompt
  * @num: number that determines which errno value should be set
  */
void print_ps1(int num)
{
	char ps1[] = "ALX$ ";
	size_t len_ps1;
	int org_error;

	org_error = errno;
	len_ps1 = _strlen(ps1);
	if (isatty(0))
		write(STDOUT_FILENO, ps1, len_ps1);
	if (num == 0)
		errno = 0;
	else
		errno = org_error;
}
/**
  * find_pathname - finds the pathname attached to the associated command
  * @path: pointer to 2d array of tokenized directories in PATH
  * @input: input to find
  * Return: Path to the input file.
  */
char* find_pathname(char **path, char *input)
{
	unsigned int i;
	DIR *directory;
	struct dirent *filename;
	int str_cmp, match_found;
	char *result;

	filename = NULL;
	match_found = 0;
	for (i = 0; path[i] != NULL; i++)
	{
		directory = opendir(path[i]);
		if (directory == NULL)
		{
			errno = EBADF;
			return (NULL);
		}
		while ((filename = readdir(directory)) != NULL)
		{
			/* TODO make strcmp function */
			str_cmp = _strcmp(filename->d_name, input);
			if (str_cmp == 0)
			{
				match_found = 1;
				break;
			}
		}
		if (match_found == 1)
			break;
		closedir(directory);
	}
	if (match_found == 1)
	{
		result = make_pathname(path[i], input);
		if (access(result, R_OK) != -1)
			errno = EACCES;
		closedir(directory);
		return (result);
	}
	errno = EBADF;
	return (NULL);
}
/**
  * free_shell_t - frees elements in shell_t struct
  * @shell_ptrs: structure of malloced elements
  */
void free_shell_t(shell_t *shell_ptrs)
{
	free(shell_ptrs->path_values);
	free(shell_ptrs->input);
	free(shell_ptrs->input_token);
	free(shell_ptrs->modify_path);
}
/**
  * p_commanderr - prints error for failed command
  * @command: command that was submitted by user
  * @filename: name of the file being run
  */
void p_commanderr(char *command, char *filename)
{
	char *error_message;
	char *error = ": command not found\n";
	size_t num_char, i;
	(void)filename;

	num_char = _strlen(error) + _strlen(command);
	error_message = malloc(sizeof(char) * (++num_char));

	i = 0;
	while (*command != '\0')
		error_message[i++] = *command++;
	while (*error != '\0')
		error_message[i++] = *error++;
	error_message[i] = '\0';
	write(STDERR_FILENO, error_message, num_char);
	free(error_message);
}
/**
  * _strtok - tokenizes a string
  * @str: string to tokenize
  * @delim: string that contains the delimiters
  * Return: pointer to position in string of a null terminated word.
  */
char* _strtok(char *str, const char *delim)
{
	static char *begin, *end_str, *end;
	unsigned int i, j;
	int word_found = 0;

	if (str == NULL)
	{
		if (begin == NULL || end == NULL)
			return (NULL);
		str = end + 1;
	}
	else
		end_str = str + _strlen(str);
	for (i = 0; str + i < end_str; i++)
	{
		for (j = 0; delim != NULL && delim[j] != '\0'; j++)
		{
			if (str[i] == delim[j])
			{
				if (word_found == 1)
				{
					str[i] = '\0';
					end = str + i;
					return (begin);
				}
				break;
			}
		}
		if (delim[j] == '\0' && word_found == 0)
		{
			word_found = 1;
			begin = str + i;
		}
	}
	end = NULL;
	if (word_found == 1)
		return (begin);
	return (NULL);
}
