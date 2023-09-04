
CC=gcc
CFLAGS=-Wall -Wextra -Werror
# if there are readline errors execute 'brew install readline'
CPPFLAGS= -I${HOME}/.brew/opt/readline/include
LDFLAGS= -lreadline -L${HOME}/.brew/opt/readline/lib
NAME=minishell
#/sgoinfre/goinfre/Perso/fgomez-d/homebrew/Cellar/readline/8.2.1/ alternative path
SRC_DIR = sources
MAIN = $(SRC_DIR)/minishell.c
SRC = 	$(SRC_DIR)/executor.c $(SRC_DIR)/redirections.c $(SRC_DIR)/errors.c \
		$(SRC_DIR)/executor_utils.c $(SRC_DIR)/execve_utils.c $(SRC_DIR)/redirections_utils.c \
		$(SRC_DIR)/exits.c $(SRC_DIR)/builtins.c $(SRC_DIR)/builtins_utils.c \
		$(SRC_DIR)/long_long_parse.c $(SRC_DIR)/executor_utils2.c $(SRC_DIR)/mem_manage.c \
		$(SRC_DIR)/builtins2.c $(SRC_DIR)/builtins_utils2.c $(SRC_DIR)/heredoc_utils.c \
		$(SRC_DIR)/signals.c $(SRC_DIR)/heredoc.c $(SRC_DIR)/builtins_utils3.c \
		$(SRC_DIR)/split_parser_main.c $(SRC_DIR)/split_parser_word_count.c \
		$(SRC_DIR)/split_parser_inicialize.c $(SRC_DIR)/split_parser_fill_words.c \
		$(SRC_DIR)/split_parser_exp_and_join.c $(SRC_DIR)/minishell_utils.c \
		$(SRC_DIR)/inicialize_parsed_list.c $(SRC_DIR)/inicialize_structs.c \
		$(SRC_DIR)/expand_1pass_env_variables.c $(SRC_DIR)/expand_2pass_env_variables.c \
		$(SRC_DIR)/expand_env_variables_extra.c $(SRC_DIR)/free_minishell.c \
		$(SRC_DIR)/env_variables_split.c \

TEST_DIR = testing
TEST_MAIN = $(TEST_DIR)/executor_test.c
TEST_SRC = 

OBJ=$(SRC:.c=.o)
OBJ_TEST=$(TEST_SRC:.c=.o)

LIBFT_DIR=Libft
LIBFT=$(LIBFT_DIR)/libft.a


all: $(NAME)

$(NAME): $(LIBFT) $(OBJ) $(MAIN)
	@$(CC) $(CFLAGS) $(CPPFLAGS) $(OBJ) $(MAIN) $(LIBFT) -o $(NAME) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@ 

test: build_test
	@./test

build_test: $(LIBFT) $(OBJ) $(TEST_MAIN)
	@$(CC) $(OBJ) $(TEST_MAIN) $(LIBFT) -o test $(LDFLAGS)

$(LIBFT):
	@$(MAKE) -C Libft

clean:	
		@rm -f ${OBJ}
		@${MAKE} -C Libft clean

fclean:	clean
#		@rm -f ${NAME} ${MINISHELL} minisshell borrado
		@rm -f ${NAME}
		@rm -rf $(OBJ_DIR)
		@${MAKE} -C Libft fclean

re: fclean all

.PHONY: all clean fclean re lib main