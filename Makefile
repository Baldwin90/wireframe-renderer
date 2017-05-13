FILES			= color draw draw_v2 fdf mapdata mapdata_v2 matrix main

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
		make -C ./srcs/mlx/ clean
		rm -rf build/

fclean: clean
		make -C ./srcs/libft/ fclean
		make -C ./srcs/mlx/ clean
		rm -rf $(NAME)

re: fclean all

build:
		mkdir build/

build/%.o: srcs/*/%.c | build
		$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(NAME): $(OBJECT_FILES)
		make -C ./srcs/libft/
		make -C ./srcs/mlx/
		$(CC) $(CFLAGS) $(APPLEKIT) $(OBJECT_FILES) $(LIBS) -o $(NAME)
