/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpicchio <gpicchio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 10:53:50 by vcastald          #+#    #+#             */
/*   Updated: 2025/04/08 16:18:29 by gpicchio         ###   ########.fr       */
/*   Updated: 2025/04/15 10:48:20 by vcastald         ###   ########.fr       */
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
# include <stddef.h>
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
	int					fd_stdin;
	char				**my_env;
	char				**export_env;
	char				**av;
	int					exit_status;
	t_tree				*root;
	t_lexing			*lexed_data;
	t_lexing			*cleaned_data;
	int					*fds;
	struct sigaction	sa;
}				t_gen;

// built in
int			ft_echo(t_lexing *node, t_gen *gen, int fd);
int			ft_env(char **env, int export, t_lexing *node);
int			ft_pwd(char **env, int fd);
int			ft_exit(t_gen *gen, t_lexing *node);
int			ft_cd(char *new_path, char **export_env, t_gen *gen);
int			ft_export(const char *var, t_gen *gen,
				t_lexing *node, char ***export_env);
void		ft_unset(char ***envp, const char *var);
void		ft_unset_export(char ***envp, const char *var);
int			call_unset(char **command, t_gen *gen);
int			call_export(t_gen *gen, t_lexing *node);
int			find_equal(char *var, char **export_env);
void		convert_plus_equal_to_equal(char *str);
void		add_in_env(char ***env, const char *var, int append);
void		add_in_export_env(char ***env, const char *var,
				int flag, int append);

// parsing
int			parsing(t_gen *gen);
t_lexing	*lexer(char **matrix, t_gen *gen);
void		add_token(t_lexing **lexed, char *content,
				char *type, int strength);
int			other_checks(int *i, t_lexing **lexed, char *word, t_gen *gen);
void		other_checks_1(int *i, t_lexing **lexed, char *word);
void		check_pipe(int *i, t_lexing **lexed, char *word);
int			find_files(t_lexing *lexed, t_gen *gen);
void		find_env_var_and_wild(t_lexing *lexed);
void		find_args(t_lexing *lexed);
int			layerize(t_gen *gen, t_lexing *lexed);
int			loop_expand(t_gen *gen);

// quotes
void		check_quotes(int *i, t_lexing **lexed, char *word);
int			quote_checker(char *line, int i);
int			unclosed_quotes(char *word);
int			quote_handler(t_gen *gen, t_lexing *lexed);
void		clean_quotes(t_lexing **node, t_gen *gen);
int			double_quotes(int *i, t_lexing **node, t_gen *gen);

// env vars
int			len_var(char *str, int dollar_pos);
char		*expand_env_var(char **env, char *var);
void		handle_env_variable(t_lexing **n, t_gen *gen, int *p);
char		*construct_env_var(char *before, char *after, char *tmp);
char		*build_tmp(t_gen *gen, int *e, char **value, int p);

// redirections and wildcards
int			find_red(t_lexing *node, t_gen *gen);
int			calc_mat_len(t_lexing *node, int *i);
int			expand_wildcard(t_lexing **node, t_gen *gen);
t_lexing	*find_prev_command(t_lexing *start, t_lexing *end);
t_lexing	*find_next_node(t_lexing *start, char *to_find);
void		sort_value(t_lexing **node, t_gen *gen);
void		position(t_lexing *prev, t_lexing *tmp_list, t_gen *gen);
int			loop_expand_wilds(t_gen *gen);

// utils
void		free_matrix(char **av);
void		safe_free(t_gen *gen);
char		**copy_matrix(char **src);
void		error_exit(t_gen *gen, char *str, int exit_status);
char		**ft_split_quote(char const *s, char c);
int			find_char_pos(char *s, char *chars, int start);
int			ft_swap(char **s1, char **s2);
void		sort_export(t_gen *gen);
void		util_free_env_var(char *before, char *tmp, char *after);
int			util_infile(char *filename, t_gen *gen, t_lexing *node);
int			util_outfile(char *filename, t_gen *gen, t_lexing *node, int flag);
void		util_exit(t_gen *gen);
void		change_shlvl(char ***env);
void		norm_add_exp_env(char ***env, int i, const char *var, int append);
void		ft_join(char ***env, int i, const char *var, int pos);
void		do_export(t_gen *gen, int equal_pos, int append, const char *var);
t_lexing	*find_prev_node(t_lexing *end, t_lexing *start);

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
int			check_files(t_gen *gen, t_lexing *lexed);
int			check_here_doc(t_gen *gen, t_lexing *lexed);
int			check_operators(t_gen *gen, t_lexing *lexed);
int			check_wildcards(t_gen *gen, t_lexing *lexed);
int			checks_layer(t_lexing *tmp, t_lexing *succ,
				t_gen *gen, t_lexing *lst);
int			check_not_opened(t_lexing *end, t_lexing *head);
int			check_close(t_lexing *node, t_lexing *succ);
int			check_redirect(t_lexing *node);
int			check_after_close(char *word, int *i);
int			checks_unset_export(const char *str, int export);
int			check_end(t_lexing *tmp);
int			check_after(t_lexing *node);
int			check_reds_in_parenth(t_lexing *node);
int			check_pipes_in_parenth(t_lexing *node);
t_lexing	*check_continue(t_lexing *lexed, int flag);

// exec
void		exec_command(t_gen *gen);
int			find_cmd_num(t_lexing *node);
void		exec_single_command(t_gen *gen, t_lexing *node);
int			exec_builtin(t_gen *gen, t_lexing *node);

// qui doc
void		here_doccer(t_lexing *node, t_lexing *cleaned_data, t_gen *gen);

// pokemon :)
typedef struct s_attack
{
	char	*name;
	int		damage;
	char	*type;
	int		id;
}	t_attack;

typedef struct s_pokemon
{
	char		*name;
	int			id;
	int			health;
	int			max_health;
	t_attack	attack1;
	t_attack	attack2;
	t_attack	attack3;
	t_attack	attack4;
	int			defense;
	int			attack;
	int			speed;
	char		*type;
	char		*ascii_path;
}	t_pokemon;

t_pokemon	*ft_create_pokemon(char *name, int health, int defense, int speed);
t_attack	create_attack(char *name, int damage, char *type);
void		ft_pokemon(t_gen *gen);

#endif
