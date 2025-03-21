NAME		= libft.a

SRCS		= ft_memset.c \
			  ft_bzero.c \
			  ft_memcpy.c \
			  ft_memmove.c \
			  ft_memchr.c \
			  ft_memcmp.c \
			  ft_strlen.c \
			  ft_strlcpy.c \
			  ft_strlcat.c \
			  ft_strchr.c \
			  ft_strrchr.c \
			  ft_strnstr.c \
			  ft_strncmp.c \
			  ft_atoi_mod.c \
			  ft_isalpha.c \
			  ft_isdigit.c \
			  ft_isalnum.c \
			  ft_isascii.c \
			  ft_isprint.c \
			  ft_toupper.c \
			  ft_tolower.c \
			  ft_calloc.c \
			  ft_strdup.c \
			  ft_substr.c \
			  ft_strtrim.c \
			  ft_split.c \
			  ft_itoa.c \
			  ft_striteri.c \
			  ft_strmapi.c \
			  ft_putchar_fd.c \
			  ft_putstr_fd.c \
			  ft_putendl_fd.c \
			  ft_putnbr_fd.c \
			  ft_printf/ft_printf.c \
			  ft_printf/ft_putnbr_base.c \
			  ft_printf/ft_putstr_fd.c \
			  ft_printf/ft_putnbr.c \
			  ft_printf/ft_putchar_fd.c \
			  get_next_line/get_next_line.c \
			  get_next_line/get_next_line_utils.c \

BNS_SRCS	= ft_lstnew.c \
			  ft_lstadd_front.c \
			  ft_lstsize.c \
			  ft_lstlast.c \
			  ft_lstadd_back.c \
			  ft_lstdelone.c \
			  ft_lstclear.c \
				
OBJS		= $(SRCS:.c=.o)

BNS_OBJS	= $(BNS_SRCS:.c=.o)

CC = cc

CFLAGS	= -Wall -Werror -Wextra

all: $(NAME)

$(NAME): $(OBJS) $(BNS_OBJS)
	ar rc $(NAME) $(OBJS) $(BNS_OBJS)

%.o: %.c
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

bonus: $(OBJS) $(BNS_OBJS)
	ar rcs $(NAME) $(OBJS) $(BNS_OBJS)

clean:
	rm -rf $(OBJS) $(BNS_OBJS)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.SILENT:	all clean fclean re
.PHONY:		all clean fclean re