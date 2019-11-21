NAME	:= scop
CC		:= gcc
CFLAGS	:= -Wall -Wextra -g3 -I glad/include -I inc/ -I libft/includes
LIBS	:= -lglfw3 -lGL -lX11 -lpthread -lXrandr -lXi -ldl -lm -L libft -lft

SRCS	:= \
	glad/src/glad.c \
	src/main.c \
	src/object.c \
	src/shader.c \
	src/objparser.c \
	src/read_file.c \
	src/engine_init.c \
	src/print_fps.c \
	src/bmp/bmp.c \
	src/process_input.c \
	src/glm/vec3.c \
	src/glm/vec4.c \
	src/glm/mat4.c \
	src/glm/cam.c \
	src/glm/util.c \
	src/malloc_abort.c \
	src/triangle_container.c \
	src/face_container.c \
	src/vertex_container.c \
	src/handle_move.c \
	src/handle_rot.c \
	src/scop.c \
	src/texture.c \
	src/update.c \
	src/scop_init.c \
	src/mouse.c \
	src/shader_set.c \
	src/shader2.c

OBJS	:= $(SRCS:.c=.o)

# -- Rules --

$(NAME): all

all: $(OBJS)
	make -j$(nproc) -C libft
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME) $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all
