NAME = minishell

CC = gcc

LIBFT_NAME = libft/libft.a

HEADER_FILES =	inc/minishell.h\
				libft/libft.h

SRC_DIR =	src/

LIBFT_DIR =	libft/

OBJ_DIR =	obj/

SRC =	src/minishell.c\
		src/error.c\
		src/utils.c\
		src/free.c\
		src/check_path.c\
		src/redirect.c\
		src/lexer/lexer.c\
		src/lexer/lexer_utils.c\
		src/parser/parsing.c\
		src/parser/parse_utils.c\
		src/parser/parse_utils2.c\
		src/parser/parse_utils3.c\
		src/parser/parse_quotes.c\
		src/expansion/expand.c\
		src/expansion/exp_utils.c\
		src/expansion/exp_utils2.c\
		src/expansion/exp_utils3.c\
		src/expansion/exp_heredoc.c\
		src/environment/manage_env.c\
		src/environment/manage_env_utils.c\
		src/execute/exec.c\
		src/execute/cd.c\
		src/execute/exec_utils.c\

OBJ = $(SRC:$(SRC_DIR)%.c=$(OBJ_DIR)%.o)

CFLAGS = -Wall -Werror -Wextra #-fsanitize=address

LDFLAGS = -L$(HOME)/.brew/opt/readline/lib
CPPFLAGS= -I$(HOME)/.brew/opt/readline/include

all: $(LIBFT_NAME) $(OBJ_DIR) $(NAME)

$(NAME): $(OBJ) $(LIBFT_NAME)
	$(CC) $(OBJ) $(CFLAGS) -Llibft -lft -lreadline $(LDFLAGS) $(CPPFLAGS) -o $(NAME)

$(OBJ_DIR):
	mkdir $@ obj/expansion obj/lexer obj/parser obj/execute obj/environment

$(OBJ): $(OBJ_DIR)%.o: $(SRC_DIR)%.c $(HEADER_FILES)
	$(CC) -c $(CFLAGS) $(CPPFLAGS) -o $@ $<

$(LIBFT_NAME): $(HEADER_FILES) $(LIBFT_DIR)*.c $(LIBFT_DIR)
	make -C libft

clean:
	rm -f obj/*.o
	rm -f obj/*/*.o
	rm -f libft/*.o

fclean: clean
	rm -f $(NAME)
	rm -f $(LIBFT_NAME)

re: fclean all

.PHONY: all clean fclean re
