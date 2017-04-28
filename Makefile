FILES			= color draw fdf mapdata matrix

DOTCFILES		= $(addprefix srcs/fdf/, $(addsuffix .c, $(FILES)))
OBJECT_FILES	= $(addprefix build/, $(addsuffix .o, $(FILES)))

NAME			= fdf

CC				= gcc -O3

CFLAGS			= -Wall -Werror -Wextra

APPLEKIT		= -framework OpenGL -framework AppKit

INCLUDES		= -I includes -I ./srcs/libft/includes -I ./srcs/mlx/includes

LIBS			= ./srcs/libft/libft.a ./srcs/mlx/libmlx.a

.PHONY: all clean fclean re

all: $(NAME)

clean:
		make -C ./srcs/libft/ clean
		rm -rf build/

fclean: clean
		make -C ./srcs/libft/ fclean
		rm -rf $(NAME)

re: fclean all
		make -C ./srcs/libft/ re

build:
		mkdir build/

build/%.o: srcs/*/%.c | build
		$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(NAME): $(OBJECT_FILES)
		make -C ./srcs/libft/
		make -C ./srcs/mlx/
		$(CC) $(CFLAGS) $(APPLEKIT) $(OBJECT_FILES) $(LIBS) -o $(NAME)