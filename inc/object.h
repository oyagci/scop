/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   object.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyagci <oyagci@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/21 14:55:33 by oyagci            #+#    #+#             */
/*   Updated: 2019/11/21 15:13:15 by oyagci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef OBJECT_H
# define OBJECT_H

# include "glad/glad.h"
# include <GLFW/glfw3.h>
# include "shader.h"
# include "glm.h"

struct					s_object {
	t_vec3				pos;
	t_vec3				rot;
	GLfloat				scale;

	GLuint				vbo;
	GLuint				vao;
	GLuint				ebo;

	GLfloat				*vertices;
	GLsizei				nvert;

	GLuint				*indices;
	GLsizei				nind;

	struct s_program	*shader;
};

void					object_init(struct s_object *o,
							GLfloat const *vdata,
							GLsizei ndata,
							GLsizei stride,
							struct s_program *shader);
void					object_prepare_draw(struct s_object *o);
void					object_draw(struct s_object *o);
void					object_delete(struct s_object *o);
void					object_update(struct s_object *o);

void					object_set_pos(struct s_object *o, t_vec3 newpos);
void					object_rotx(struct s_object *o, float angle);
void					object_roty(struct s_object *o, float angle);
void					object_rotz(struct s_object *o, float angle);
void					object_set_scale(struct s_object *o, float angle);

#endif
