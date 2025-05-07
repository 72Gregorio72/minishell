#include "minishell.h"

#define INITIAL_BUF 64

#include <termios.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define INITIAL_BUF 64

static void restore_terminal(struct termios *old)
{
	tcsetattr(STDIN_FILENO, TCSANOW, old);
}

static void move_cursor_left(int times)
{
	while (times-- > 0)
		write(STDOUT_FILENO, "\x1b[D", 3);
}

static void move_cursor_right(int times)
{
	while (times-- > 0)
		write(STDOUT_FILENO, "\x1b[C", 3);
}

static void clear_line(int len)
{
	while (len--)
		write(STDOUT_FILENO, "\b \b", 3);
}

static char *ft_realloc_string(char *str, int new_size)
{
	char *new_str = malloc(new_size);
	if (!new_str)
		return (NULL);
	ft_strlcpy(new_str, str, new_size);
	free(str);
	return (new_str);
}

char *read_line1(const char *prompt, t_gen *gen)
{
	struct termios	oldt, newt;
	char			*buf = malloc(INITIAL_BUF);
	int				size = INITIAL_BUF, len = 0, cursor = 0;
	char			c;
	t_history		*current = NULL;

	if (!buf)
		return (NULL);

	write(STDOUT_FILENO, prompt, ft_strlen(prompt));
	tcgetattr(STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);

	while (read(STDIN_FILENO, &c, 1) == 1)
	{
		if (c == 27)
		{
			char seq[2];
			if (read(STDIN_FILENO, &seq, 2) == 2 && seq[0] == '[')
			{
				if (seq[1] == 'A') // ↑
				{
					if (!current)
					{
						current = gen->g_history;
						while (current && current->next)
							current = current->next;
					}
					else if (current->prev)
						current = current->prev;

					if (current)
					{
						move_cursor_left(cursor);
						clear_line(len);
						ft_strlcpy(buf, current->cmd, size);
						len = ft_strlen(buf);
						cursor = len;
						write(STDOUT_FILENO, buf, len);
					}
				}
				else if (seq[1] == 'B') // ↓
				{
					if (current && current->next)
						current = current->next;
					else
						current = NULL;

					move_cursor_left(cursor);
					clear_line(len);
					if (current)
						ft_strlcpy(buf, current->cmd, size);
					else
						buf[0] = '\0';

					len = ft_strlen(buf);
					cursor = len;
					write(STDOUT_FILENO, buf, len);
				}
				else if (seq[1] == 'D') // ← sinistra
				{
					if (cursor > 0)
					{
						move_cursor_left(1);
						cursor--;
					}
				}
				else if (seq[1] == 'C') // → destra
				{
					if (cursor < len)
					{
						move_cursor_right(1);
						cursor++;
					}
				}
			}
		}
		else if (c == '\n' || c == '\r')
			break;
		else if ((c == 127 || c == 8) && cursor > 0) // Backspace
		{
			ft_memmove(&buf[cursor - 1], &buf[cursor], len - cursor);
			len--;
			cursor--;
			move_cursor_left(1);
			write(STDOUT_FILENO, &buf[cursor], len - cursor);
			write(STDOUT_FILENO, " ", 1);
			move_cursor_left(len - cursor + 1);
		}
		else if (c == 4 && len == 0) // Ctrl+D
		{
			free(buf);
			buf = NULL;
			break ;
		}
		else if (c >= 32 && c <= 126) // Printabile
		{
			if (len + 1 >= size)
			{
				char *newbuf = ft_realloc_string(buf, size * 2);
				if (!newbuf)
					break ;
				buf = newbuf;
				size *= 2;
			}
			ft_memmove(&buf[cursor + 1], &buf[cursor], len - cursor);
			buf[cursor] = c;
			write(STDOUT_FILENO, &buf[cursor], len - cursor + 1);
			move_cursor_left(len - cursor);
			cursor++;
			len++;
		}
	}

	if (buf)
		buf[len] = '\0';
	write(STDOUT_FILENO, "\n", 1);
	restore_terminal(&oldt);
	return buf;
}

t_history *create_node(const char *line)
{
	t_history *new = malloc(sizeof(t_history));
	if (!new)
		return (NULL);
	new->cmd = ft_strdup(line);
	if (!new->cmd)
		return (free(new), NULL);
	new->next = NULL;
	new->prev = NULL;
	return new;
}

void	add_history1(const char *line, t_gen *gen)
{
	t_history *last;
	t_history *new;

	if (!line || line[0] == '\0')
		return;

	last = gen->g_history;
	while (last && last->next)
		last = last->next;
	if (last && strcmp(last->cmd, line) == 0)
		return;

	new = create_node(line);
	if (!new)
		return;
	if (!gen->g_history)
		gen->g_history = new;
	else
	{
		last->next = new;
		new->prev = last;
	}
}

void	free_history(t_history *head)
{
	t_history *tmp;

	while (head)
	{
		tmp = head;
		head = head->next;
		free(tmp->cmd);
		free(tmp);
	}
}
