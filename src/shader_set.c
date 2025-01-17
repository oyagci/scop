/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shader_set.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyagci <oyagci@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/21 14:55:32 by oyagci            #+#    #+#             */
/*   Updated: 2019/11/21 14:55:39 by oyagci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shader.h"

void	shader_set_t_vec3(struct s_program *p, char const *name, t_vec3 v)
{
	program_use(p);
	glUniform3fv(glGetUniformLocation(p->index, name), 1, v);
}

void	shader_set_t_mat4(struct s_program *p, char const *name, t_mat4 m)
{
	program_use(p);
	glUniformMatrix4fv(glGetUniformLocation(p->index, name), 1,
		GL_FALSE, *m);
}

void	shader_set_float(struct s_program *p, char const *name, float f)
{
	program_use(p);
	glUniform1f(glGetUniformLocation(p->index, name), f);
}
