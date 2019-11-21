/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scop.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyagci <oyagci@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/21 14:55:33 by oyagci            #+#    #+#             */
/*   Updated: 2019/11/21 16:12:38 by oyagci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SCOP_H
# define SCOP_H

# include "glad/glad.h"
# include <GLFW/glfw3.h>
# include <stdint.h>
# include "glm.h"
# include "shader.h"
# include "object.h"
# include "obj.h"

# define WIDTH		1280
# define HEIGHT		720

typedef struct	s_gl_camera {
	t_vec3	pos;
	t_vec3	front;
	t_vec3	up;
}				t_gl_camera;

typedef struct	s_render_win {
	int	width;
	int	height;
}				t_render_win;

typedef struct	s_mouse {
	double	last_x;
	double	last_y;
	float	pitch;
	float	yaw;
	float	first_mouse;
	float	sensitivity;
}				t_mouse;

typedef struct	s_obj_info {
	t_vec3		pos;
	t_vec3		rot;
	GLboolean	opacity;
	GLint		opacity_dir;
	float		opacity_value;
}				t_obj_info;

typedef struct	s_engine {
	t_gl_camera		camera;
	t_render_win	window;
	t_mouse			mouse;
	float			delta_time;
	t_obj_info		obj;
}				t_engine;

typedef struct	s_glfw_input {
	unsigned int	key;
	void			(*handler)(float);
	float			dir;
}				t_glfw_input;

void			engine_init(t_engine *engine);
void			mouse_init(t_mouse *mouse);
void			render_win_init(t_render_win *window);
void			gl_camera_init(t_gl_camera *camera);

void			process_input(GLFWwindow *win);
void			framebuffer_resize(GLFWwindow *win, int width,
					int height);
int				read_file(char const *const filename, char **buf);
void			mouse_move(GLFWwindow *win, double xpos, double ypos);
void			print_fps(void);
GLuint			texture_create(GLenum texture, char const *path);

void			update_delta_time(void);
void			update_opacity(struct s_object *o);
void			update_camera(size_t n, struct s_program *shaders);

extern t_engine	g_engine;

void			handle_pitch(float speed);
void			handle_yaw(float speed);
void			handle_roll(float speed);
void			handle_forward(float speed);
void			handle_up(float speed);
void			handle_left(float speed);

int				shader_from_files(char const *vpath, char const *fpath,
					struct s_program *p);
int				scop_obj_init(char const *objpath, t_obj *objbuf);
GLFWwindow		*scop_init_window(int width, int height);
int				scop_glfw_init(void);
int				scop(GLFWwindow *window, char const *filename);

#endif
