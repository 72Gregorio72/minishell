/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpicchio <gpicchio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 10:53:50 by vcastald          #+#    #+#             */
/*   Updated: 2025/04/02 14:33:57 by gpicchio         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <signal.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <dirent.h>
# include <string.h>
# include <errno.h>
# include <termios.h>
# include <curses.h>
# include <term.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "../libft/libft.h"
# include "pipex_bonus.h"
# include <sys/stat.h>

# define CTRL_C 1
# define CTRL_BACK 2

extern int	g_sig_received;

typedef struct s_tree
{
	struct s_tree	*left;
	struct s_tree	*right;
	struct s_tree	*parent;
	t_lexing		*data;
}				t_tree;

typedef struct s_gen
{
	char		**my_env;
	char		**export_env;
	char		**av;
	int			exit_status;
	t_tree		*root;
	t_lexing	*lexed_data;
	t_lexing	*cleaned_data;
	int			*fds;
}				t_gen;

// built in
int			ft_echo(t_lexing *node, t_gen *gen, int fd);
int			ft_env(char **env, int export);
int			ft_pwd(char **env, int fd);
void		ft_exit(t_gen *gen);
int			ft_cd(char *new_path);
void		ft_export(char ***envp, const char *var, char ***export_env);
void		ft_unset(char ***envp, const char *var);
void		ft_unset_export(char ***envp, const char *var);

// parsing
int			parsing(t_gen *gen);
t_lexing	*lexer(char **matrix, t_gen *gen);
void		add_token(t_lexing **lexed, char *content,
				char *type, int strength);
void		other_checks(int *i, t_lexing **lexed, char *word);
void		other_checks_1(int *i, t_lexing **lexed, char *word);
void		check_pipe(int *i, t_lexing **lexed, char *word);
int			find_files(t_lexing *lexed, t_gen *gen);
void		find_env_var(t_lexing *lexed);
void		find_args(t_lexing *lexed);

// quotes
void		check_quotes(int *i, t_lexing **lexed, char *word);
int			quote_checker(char *line, int i);
int			unclosed_quotes(char *word);
int			quote_handler(t_gen *gen);
void		clean_quotes(t_lexing **node);
void		single_quotes(t_lexing **node, t_gen *gen);

// env vars
int			len_var(char *str, int dollar_pos);
char		*expand_env_var(char **env, char *var);
void		handle_env_variable(t_lexing **node, t_gen *gen, int clean);

// utils
void		free_matrix(char **av);
void		safe_free(t_gen *gen);
char		**copy_matrix(char **src);
int			layerize(t_gen *gen);
void		error_exit(t_gen *gen, char *str, int exit_status);
char		**ft_split_quote(char const *s, char c);
int			find_char_pos(char *s, char *chars, int start);
int			ft_swap(char **s1, char **s2);
void		sort_export(t_gen *gen);

// ctrl
void		ctrl_c(int new_line);
void		ctrl_d(t_gen *gen);
void		ctrl_backslash(void);

// signals
int			set_signals(struct sigaction sa);
int			checks(char *line, t_gen *gen);
void		handler(int sign, siginfo_t *info, void *context);

// binary tree
t_tree		*new_node(t_lexing *lexed_input, t_tree *left,
				t_tree *right, t_tree *parent);
t_tree		*least_important_leaf(t_tree *tree);
t_tree		*most_important_leaf(t_tree *tree);
void		delone_node(t_tree *node);
void		ft_treeclear(t_tree *tree);
t_lexing	*find_max_strength(t_lexing *lexed, t_lexing *max, t_lexing *last);
t_tree		*fill_tree(t_lexing *lexed, t_lexing *end, t_tree *tree);
void		print_binary_tree(t_tree *node, int depth);

// checks
int			check_all_upper(char *word);
int			check_spaces(char *line);
int			check_not_command(t_lexing	*succ);

// exec
void		exec_command(t_gen *gen);
int			find_cmd_num(t_lexing *node);
void		exec_single_command(t_gen *gen, t_lexing *node);
int			exec_builtin(t_gen *gen, t_lexing *node);

// pokemon :)
typedef struct s_attack
{
	char	*name;
	int		damage;
	char	*type;
}	t_attack;

typedef struct s_pokemon
{
	char		*name;
	int			health;
	t_attack	attack1;
	t_attack	attack2;
	t_attack	attack3;
	t_attack	attack4;
	int			defense;
	int			speed;
	char		*type;
}	t_pokemon;

t_pokemon	*ft_create_pokemon(char *name, int health, int defense, int speed);
t_attack	create_attack(char *name, int damage, char *type);
void		ft_pokemon(void);

#endif

/*
Lexing: prendere la linea di input e salvare i vari elementi in una lista
i cui nodi saranno le parole a cui viene assegnato un identificatore
https://gist.github.com/VideoCarp/d7cec2195a7de370d850aead62fa09cd
*/

/*
EXIT STATUS
130 per ctrl-c
131 per ctrl-\
0 riuscito
1 fallito 
127 comando non trovato
2 syntax error
tenere quello dell'ultimo comando*/