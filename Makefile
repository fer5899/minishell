
NAME = minishell
MINISHELL = minishell.a
LIBFT = ./Libft/libft.a

#INCLUDES
#INCL_DIR = minishell/includes
#INCL = $(wildcard $(INCL_DIR)/*.h)

#SOURCES
SRC_DIR = ./sources
SRC = $(wildcard $(SRC_DIR)/*.c)
#SRC = minishell.c \
#	ft_split_env.c \
#	env_variables_minishell.c \
#	free_minishell.c \
#	ft_split_parser_clean.c \
#	expand_env_variables.c

# OBJECTS
OBJ_DIR = ./objects
OBJ = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC))

CC = gcc -g
FLAGS = -Wall -Wextra -Werror
#OBJ = ${SRC:.c=.o}

all:	${NAME}
		 
${NAME}:	$(OBJ_DIR) ${OBJ} ${LIBFT} 
		${CC} ${FLAGS} ${OBJ} ${LIBFT} -lreadline -o ${NAME}

${LIBFT}:
		@${MAKE} -C Libft all
#		@${MAKE} -C Libft bonus

${MINISHELL}:
		${CC} ${FLAGS} -c ${SRC}
		ar rc ${MINISHELL} ${OBJ}

$(OBJ_DIR):
		@mkdir -p $(OBJ_DIR)
	
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:	
		@rm -f ${OBJ}
		@${MAKE} -C Libft clean

fclean:	clean
		@rm -f ${NAME} ${MINISHELL}
		@rm -rf $(OBJ_DIR)
		@${MAKE} -C Libft fclean

re: fclean all

.PHONY: all clean fclean re