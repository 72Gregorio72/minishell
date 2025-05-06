NAME		= minishell

SRCS		=	main.c \
				ft_pokemon.c \
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
				parsing/utils_for_lexing2.c \
				parsing/check_lexing.c \
				parsing/quote_handler.c \
				parsing/dollar.c \
				parsing/utils_layer.c \
				parsing/utils_for_quotes.c \
				parsing/utils_for_quotes2.c \
				parsing/utils_wild.c \
				parsing/parsing_builtin.c \
				parsing/lexing_quotes.c \
				parsing/redirections.c \
				parsing/wildcards.c \
				parsing/utils_reds.c \
				parsing/expand.c \
				binary_tree/tree_functions.c \
				binary_tree/tree.c \
				builtin/ft_echo.c \
				builtin/ft_env.c \
				builtin/ft_pwd.c \
				builtin/ft_exit.c \
				builtin/ft_cd.c \
				builtin/ft_export.c \
				builtin/ft_unset.c \
				exec_commands/execute.c \
				pipex/bonus/pipex_bonus.c \
				pipex/bonus/pipex_utils_bonus.c \
				pipex/bonus/get_path_bonus.c \
				pipex/bonus/processes_bonus.c \
				pipex/bonus/processes_utils_bonus.c \
				pipex/bonus/read_bonus.c 
		
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

pokemon:
	@if [ ! -d "pokemonData" ]; then \
		echo "Cloning pokemonData repository..."; \
		git clone https://github.com/72Gregorio72/pokemonData.git; \
	else \
		echo "Updating pokemonData repository..."; \
		cd pokemonData && git pull; \
	fi


re: fclean all

.SILENT:	all clean fclean re bonus
.PHONY:		all clean fclean re bonus