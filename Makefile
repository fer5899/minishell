
CC=gcc
CFLAGS=-Wall -Wextra -Werror
CPPFLAGS= -ILibft -I.
NAME=minishell

SRC_DIR = sources
MAIN = $(SRC_DIR)/minishell.c
SRC = 	$(SRC_DIR)/executor.c $(SRC_DIR)/redirections.c $(SRC_DIR)/errors.c \
		$(SRC_DIR)/executor_utils.c $(SRC_DIR)/execve_utils.c

TEST_DIR = testing
TEST_MAIN = $(TEST_DIR)/executor_test.c
TEST_SRC =

OBJ=$(SRC:.c=.o)
OBJ_TEST=$(TEST_SRC:.c=.o)

LIBFT_DIR=Libft
LIBFT=$(LIBFT_DIR)/libft.a


all: $(NAME)

$(NAME): $(LIBFT) $(OBJ) $(MAIN)
	$(CC) $(CPPFLAGS) $(OBJ) $(MAIN) $(LIBFT) -lreadline -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@ 

test: build_test
	@./test

build_test: $(LIBFT) $(OBJ) $(TEST_MAIN)
	@$(CC) $(CPPFLAGS) $(OBJ) $(TEST_MAIN) $(LIBFT) -lreadline -o test

$(LIBFT):
	@$(MAKE) -C Libft

clean:
	rm -f $(OBJ)
	$(MAKE) -C Libft fclean
	
fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re lib main