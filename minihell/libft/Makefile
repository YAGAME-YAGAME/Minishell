CC = cc
CFLAGS = -Wall -Wextra -Werror
SRCS = ft_isalpha.c ft_isdigit.c ft_isalnum.c \
		ft_strlen.c ft_memset.c \
		ft_strdup.c \
		ft_strchr.c ft_strncmp.c \
		ft_atoi.c ft_substr.c \
		ft_strjoin.c ft_strtrim.c ft_split.c ft_itoa.c \
		ft_putchar_fd.c \
		ft_putstr_fd.c ft_putendl_fd.c ft_putnbr_fd.c \
		ft_lstnew_bonus.c ft_lstsize_bonus.c \
		ft_lstadd_back_bonus.c \
		ft_lstclear_bonus.c

RM = rm -rf
NAME = libft.a
OBJS = $(SRCS:.c=.o)
HEADER = ./libft.h

all: $(NAME)

$(NAME): $(OBJS)
	@ar rcs $(NAME) $^

%.o: %.c $(HEADER)
	@${CC} ${CFLAGS} -c $< -o $@

clean:
	@$(RM) $(OBJS) $(OBJBNS)

fclean: clean
	@$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
