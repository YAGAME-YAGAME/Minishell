TARGET = minishell
CC = clang
CFLAGS = -Wall -Wextra -Werror -Wunreachable-code
MINIFLAGS = -lreadline

READLINE = $(shell brew --prefix readline)
CFLAGS = -Wall -Wextra -Werror -I$(READLINE)/include
MINIFLAGS =  -lreadline -L$(READLINE)/lib -g -Wl,-no_pie

SRCS=	$(TARGET).c \
	minishell_utils.c \
	./parsing/cwd.c \
	./parsing/env_init.c \
	./parsing/env_parse.c \
	./builtins/builtin.c \
	./parsing/strtok.c \
	./parsing/ft_handles.c \
	./parsing/utils.c \
	./parsing/free.c \
	./parsing/tokens.c \
	./parsing/printers.c \
	./parsing/parser.c \
	./parsing/varexp.c \
	./parsing/nodes.c \
	./parsing/expanding_utils.c \
	./parsing/heredoc_exp.c \
	./parsing/redirection.c \
	./exec/parsing_split.c \
	./exec/utils.c \
	./exec/run_cmd.c \
	./exec/run_cmd_utils.c \
	./exec/execution.c \
	./exec/ft_error.c \
	./exec/heredoc/here_doc.c \
	./exec/heredoc/here_doc_utils.c \
	./exec/gnl/gnl.c \
	./exec/gnl/gnl_utils.c \
	./exec/signals.c \
	./exec/get_cmd_path.c \
	./exec/ambiguous.c \
	./exec/run_cmd_utils_2.c \
	./builtins/builtin.c \
	./builtins/cd.c \
	./builtins/echo.c \
	./builtins/env.c \
	./builtins/exit.c \
	./builtins/export.c \
	./builtins/export_utils.c \
	./builtins/pwd.c \
	./builtins/unset.c \
	./builtins/handle_builtin_redi.c \
	./builtins/handle_builtin_redi_utils.c \
	./builtins/utils.c \
	./builtins/cd_utils.c \
	./builtins/export_utils_2.c \

RM = rm -rf
NAME = $(TARGET).a
OBJS = $(SRCS:.c=.o)
HEADER = $(TARGET).h
LIBFT = ./libft/libft.a
GET_NEXT_LINE = ./exec/gnl/get_next_line.h

DEF_COLOR = \033[0;39m
GRAY = \033[0;90m
RED = \033[0;91m
GREEN = \033[0;92m
YELLOW = \033[0;93m
BLUE = \033[0;94m
MAGENTA = \033[0;95m
CYAN = \033[0;96m
WHITE = \033[0;97m

all: $(TARGET)

$(TARGET): $(NAME) $(HEADER) $(LIBFT)
	@echo "$(BLUE)Making $(TARGET) ...$(DEF_COLOR)"
	@${CC} ${CFLAGS} -o $(TARGET) $(TARGET).c $(NAME) $(LIBFT) ${MINIFLAGS}
	@echo "$(GREEN) ___  ____       _     _          _ _"
	@echo "$(GREEN) |  \/  |_|     |_|   | |        | | |"
	@echo "$(GREEN) | .  . |_ _ __  _ ___| |__   ___| | |"
	@echo "$(GREEN) | |\/| | | '_ \| / __| '_ \ / _ \ | |"
	@echo "$(GREEN) | |  | | | | | | \__ \ | | |  __/ | |"
	@echo "$(GREEN) \_|  |_/_|_| |_|_|___/_| |_|\___|_|_|"
	@echo "$(GREEN)\n==The Executable of [$(TARGET)] has been compiled!==\n$(DEF_COLOR)"
	@echo "$(YELLOW)\n=> Usage: ./$(TARGET)\n$(DEF_COLOR)"

$(NAME): $(OBJS)
	@echo "$(BLUE)Archiving the Library $(NAME) ...$(DEF_COLOR)"
	@ar rcs $(NAME) $^

%.o: %.c $(HEADER) $(GET_NEXT_LINE)
	@echo "$(BLUE)Compiling $< ...$(DEF_COLOR)"
	@${CC} ${CFLAGS} -c $< -o $@

$(LIBFT):
	@echo "$(BLUE)Compiling the Library LIBFT ...$(DEF_COLOR)"
	@make -C ./libft
	@sleep 1

clean:
	@echo "$(MAGENTA)Cleaning minishell objects ...$(DEF_COLOR)"
	@$(RM) $(OBJS)
	@make -C ./libft clean
	@echo "$(MAGENTA)Cleaning libft objects ...$(DEF_COLOR)"

fclean: clean
	@echo "$(RED)Cleaning $(LIBFT) ...$(DEF_COLOR)"
	@echo "$(RED)Cleaning $(NAME) and $(TARGET) ...$(DEF_COLOR)"
	@$(RM) $(NAME) $(TARGET)
	@make -C ./libft fclean
	@rm -rf minishell.dSYM/

re: fclean all

.PHONY: all clean fclean re
