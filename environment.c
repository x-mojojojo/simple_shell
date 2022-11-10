#include "shell.h"
/**
  * print_env - prints out the current environment
  * @ptrs: structure containing all malloced memory
  */
void print_env(shell_t* ptrs)
{
	unsigned int i, k;
	char newline = '\n';

	(void)ptrs;
	if (environ == NULL)
		return;
	for (i = 0; environ[i] != NULL; i++)
	{
		k = _strlen(environ[i]);
		if (k != 0)
		{
			write(STDOUT_FILENO, environ[i], k);
			write(STDOUT_FILENO, &newline, 1);
		}
	}
	errno = 0;
}
/**
  * _getenv - gets the value of the environment variable
  * @name: variable to find
  * Return: pointer to the value of the environment variable.
  */
char* _getenv(const char* name)
{
	unsigned int i, j;

	for (i = 0; environ[i] != NULL; i++)
	{
		for (j = 0; environ[i][j] != '=' && environ[i][j] == name[j] && name[j]; j++)
			;
		if (environ[i][j] == '=' && name[j] == '\0')
			return (&environ[i][++j]);
	}
	return (NULL);
}
/**
 * get_path - get the env path and tokenizes it into a array.
 * @modify_path: pointer to store modified path string into
 *
 * Return: array / double pointer.
 */
char** get_path(char** modify_path)
{
	char** token_ptr;
	char* path, * delim;
	unsigned int i, j, num_char;

	delim = ":";
	path = _getenv("PATH");
	num_char = _strlen(path);
	*modify_path = malloc(sizeof(char) * (num_char + 2));
	if (num_char == 0)
	{
		(*modify_path)[0] = '.';
		(*modify_path)[1] = '\0';
	}
	else
	{
		for (i = 0, j = 0; path[i] != '\0'; i++)
		{
			if (i == 0 && path[i] == ':')
			{
				(*modify_path)[j++] = '.';
				(*modify_path)[j++] = path[i];
			}
			else if (i == num_char - 1 && path[i] == ':')
			{
				(*modify_path)[j++] = path[i];
				(*modify_path)[j++] = '.';
			}
			else if (path[i] == ':' && path[i + 1] == ':')
			{
				(*modify_path)[j++] = path[i];
				(*modify_path)[j++] = '.';
			}
			else
				(*modify_path)[j++] = path[i];
		}
		for (; j < num_char + 2; j++)
			(*modify_path)[j] = '\0';
	}
	token_ptr = tokenize_str(*modify_path, delim);
	return (token_ptr);
}
/**
  * make_pathname - takes result from path_name and creates a string.
  * string contains the full pathname.
  * @path: pathname
  * @file: file in the path
  * Return: pointer to an allocated string that contains the full pathname.
  */
char* make_pathname(char* path, char* file)
{
	size_t num_path, num_file;
	char* result;
	unsigned int j, k;

	num_path = _strlen(path);
	num_file = _strlen(file);
	result = malloc(sizeof(char) * (num_path + num_file + 2));
	if (result == NULL)
		return (NULL);
	for (j = 0; j < num_path; j++)
		result[j] = path[j];
	result[j++] = '/';
	for (k = 0; k < num_file; k++)
		result[j + k] = file[k];
	result[j + k] = '\0';
	return (result);
}
/**
 * my_exit - simple impl of exit.
 * @ptrs: structure containing all malloced memory
 */
void my_exit(shell_t* ptrs)
{
	unsigned int i;
	char* exit_str;

	exit_str = ptrs->input_token[1];
	if (exit_str != NULL || ptrs == NULL)
	{
		errno = 0;
		for (i = 0; exit_str[i] != '\0'; i++)
			errno = errno * 10 + (exit_str[i] - '0');
	}
	free_shell_t(ptrs);
	if (errno > 255)
		errno %= 256;
	exit(errno);
}
