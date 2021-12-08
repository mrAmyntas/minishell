NAME = minishell

LIBFT_NAME = libft/libft.a

HEADER_FILES =	inc/minishell.h\
				libft/libft.h

SRC_DIR =	src/

LIBFT_DIR =	libft/

OBJ_DIR =	obj/

SRC =	src/minishell.c src/exec.c

OBJ = $(SRC:$(SRC_DIR)%.c=$(OBJ_DIR)%.o)

CFLAGS = #-Wall -Werror -Wextra

all: $(LIBFT_NAME) $(OBJ_DIR) $(NAME)

$(NAME): $(OBJ) $(LIBFT_NAME)
	$(CC) $(OBJ) -ledit -Llibft -lft -o $(NAME)

$(OBJ_DIR):
	mkdir $@

$(OBJ): $(OBJ_DIR)%.o: $(SRC_DIR)%.c $(HEADER_FILES)
	$(CC) -c $(CFLAGS) -o $@ $<

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
