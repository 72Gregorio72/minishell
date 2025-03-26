NAME		= minishell

SRCS		=	main.c \
				srcs/free.c \
				srcs/util.c \
				srcs/ctrl.c \
				srcs/signals.c \
				srcs/check.c \
				srcs/layerize.c \
				parsing/split_quote.c \
				parsing/lexing.c \
				parsing/parsing.c \
				parsing/utils_for_lexing.c \
				parsing/check_lexing.c \
				parsing/quote_handler.c \
				parsing/dollar.c \
				parsing/utils_for_quotes.c \
				binary_tree/tree_functions.c \
				binary_tree/tree.c \
				builtin/ft_echo.c \
				builtin/ft_env.c \
				builtin/ft_pwd.c \
				builtin/ft_exit.c \
				builtin/ft_cd.c \
				builtin/ft_export.c \
				builtin/ft_unset.c \
				pipex/bonus/pipex_bonus.c \
				pipex/bonus/pipex_utils_bonus.c \
				pipex/bonus/get_path_bonus.c \
				pipex/bonus/processes_bonus.c \
				pipex/bonus/processes_utils_bonus.c \
				pipex/bonus/read_bonus.c \
				ft_pokemon.c \
		
OBJS		= $(SRCS:.c=.o)

CC = cc

CFLAGS	= -Wall -Werror -Wextra -g

RM = rm -rf

INCLUDES = -Iincludes

all: $(NAME)

$(NAME): $(OBJS)
	@echo Compiling libft...
	@[ -f ./libft/libft.a ] || $(MAKE) -C ./libft --quiet
	@echo Compiling $(NAME)...
	@$(CC) $(CFLAGS) $(OBJS) ./libft/libft.a -lreadline -o $(NAME)

%.o: %.c
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

vall: all clean
	valgrind --leak-check=full --show-leak-kinds=all --suppressions=readLine.supp ./$(NAME)

vall_plus : all clean
	valgrind --leak-check=full --show-leak-kinds=all --suppressions=readLine.supp --track-fds=yes --trace-children=yes ./$(NAME)

clean:
	@$(MAKE) -C ./libft fclean --quiet
	@echo Cleaning libft...
	@$(RM) $(OBJS)
	@echo Cleaning objects...

fclean: clean
	@$(RM) $(NAME)
	@echo Cleaning $(NAME)...
	@$(RM) $(NAME_BONUS)


re: fclean all

.SILENT:	all clean fclean re bonus
.PHONY:		all clean fclean re bonus