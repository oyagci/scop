NAME	:= scop
CC		:= gcc
CFLAGS	:= -Wall -Wextra -g3 -I glad/include -I inc/ -I lib/libobjparser/inc -I lib/libft/includes
LIBS	:= -lglfw3 -lGL -lX11 -lpthread -lXrandr -lXi -ldl -lm -lcglm -L./lib/libobjparser -lobjparser -L lib/libft -lft

SRCS	:= \
	glad/src/glad.c \
	src/main.c \
	src/object.c \
	src/shader.c \
	src/stb_image.c \

OBJS	:= $(SRCS:.c=.o)

# -- Rules --

$(NAME): all

all: $(OBJS)
	make -C lib/libobjparser
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME) $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all
