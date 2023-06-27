
NAME = minishell
MINISHELL = minishell.a
LIBFT = ./Libft/libft.a

#INCLUDES
#INCL_DIR = minishell/includes
#INCL = $(wildcard $(INCL_DIR)/*.h)

#SOURCES
#SRC_DIR = minishell/sources
#SRC = $(wildcard $(SRC_DIR)/*.c)
SRC = minishell.c

# OBJECTS
#OBJ_DIR = minishell/objects
#OBJ = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC))

CC = gcc
FLAGS = -Wall -Wextra -Werror
OBJ = ${SRC:.c=.o}

all:	${NAME}
		 
${NAME}:	${MINISHELL} ${OBJ} ${LIBFT} #$(OBJ_DIR) 
		${CC} ${FLAGS} ${OBJ} ${LIBFT} ${MINISHELL} -lreadline -o ${NAME}

${LIBFT}:
		@${MAKE} -C libft all

${MINISHELL}:
		${CC} ${FLAGS} -c ${SRC}
		ar rc ${MINISHELL} ${OBJ}

#$(OBJ_DIR):
#		@mkdir -p $(OBJ_DIR)
	
#$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(INCL)
#	$(CC) $(CFLAGS) -I$(INCL_DIR) -c $< -o $@

clean:	
		@rm -f ${OBJ}
		@${MAKE} -C libft clean

fclean:	clean
		@rm -f ${NAME} ${MINISHELL}
		@rm -rf $(OBJ_DIR)
		@${MAKE} -C libft fclean

re: fclean all

.PHONY: all clean fclean re