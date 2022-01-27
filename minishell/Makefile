NAME = minishell

CC = gcc

LIBFT_NAME = libft/libft.a

HEADER_FILES =	inc/minishell.h\
				libft/libft.h

SRC_DIR =	src/

LIBFT_DIR =	libft/

OBJ_DIR =	obj/

SRC =	src/minishell.c\
		src/exec.c\
		src/parsing.c\
		src/error.c\
		src/lexer.c\
		src/utils.c\
		src/cd.c\
		src/manage_env.c\
		src/parse_utils.c\
		src/parse_utils2.c\
		src/parse_quotetrim_dollar.c\
		src/expansion.c

OBJ = $(SRC:$(SRC_DIR)%.c=$(OBJ_DIR)%.o)

CFLAGS = #-fsanitize=address #-Wall -Werror -Wextra 

LDFLAGS = -L/Users/mgroen/.brew/opt/readline/lib
CPPFLAGS= -I/Users/mgroen/.brew/opt/readline/include

all: $(LIBFT_NAME) $(OBJ_DIR) $(NAME)

$(NAME): $(OBJ) $(LIBFT_NAME)
	$(CC) $(OBJ) $(CFLAGS) -ledit -Llibft -lft -lreadline $(LDFLAGS) $(CPPFLAGS) -o $(NAME)

$(OBJ_DIR):
	mkdir $@

$(OBJ): $(OBJ_DIR)%.o: $(SRC_DIR)%.c $(HEADER_FILES)
	$(CC) -c $(CFLAGS) $(CPPFLAGS) -o $@ $<

$(LIBFT_NAME): $(HEADER_FILES) $(LIBFT_DIR)*.c $(LIBFT_DIR)
	make -C libft

clean:
	rm -f obj/*.o
	rm -f libft/*.o

fclean: clean
	rm -f $(NAME)
	rm -f $(LIBFT_NAME)

re: fclean all

.PHONY: all clean fclean re
