
CC=gcc
CFLAGS=-Wall -Wextra -Werror
CPPFLAGS= -ILibft -L /System/Volumes/Data/opt/vagrant/embedded/lib\
			-I. -I /System/Volumes/Data/opt/vagrant/embedded/include
NAME=minishell
#-L /System/Volumes/Data/opt/vagrant/embedded/lib
SRC_DIR = sources
MAIN = $(SRC_DIR)/minishell.c
SRC = 	$(SRC_DIR)/executor.c $(SRC_DIR)/redirections.c $(SRC_DIR)/errors.c \
		$(SRC_DIR)/executor_utils.c $(SRC_DIR)/execve_utils.c $(SRC_DIR)/redirections_utils.c \
		$(SRC_DIR)/exits.c $(SRC_DIR)/builtins.c $(SRC_DIR)/builtins_utils.c \
		$(SRC_DIR)/long_long_parse.c $(SRC_DIR)/executor_utils2.c $(SRC_DIR)/mem_manage.c \
		$(SRC_DIR)/builtins2.c $(SRC_DIR)/builtins_utils2.c $(SRC_DIR)/expand_env_variables.c \
		$(SRC_DIR)/free_minishell.c $(SRC_DIR)/ft_split_env.c $(SRC_DIR)/ft_split_parser_clean.c \
		$(SRC_DIR)/env_variables_minishell.c

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
		@rm -f ${OBJ}
		@${MAKE} -C Libft clean

fclean:	clean
		@rm -f ${NAME} ${MINISHELL}
		@rm -rf $(OBJ_DIR)
		@${MAKE} -C Libft fclean

re: fclean all

.PHONY: all clean fclean re lib main