/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_move.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyagci <oyagci@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/21 14:55:32 by oyagci            #+#    #+#             */
/*   Updated: 2019/11/21 14:55:39 by oyagci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

void	handle_forward(float speed)
{
	glm_t_vec3_add(g_engine.obj.pos, (t_vec3){ 0.0f, 0.0f, speed },
		g_engine.obj.pos);
}

void	handle_up(float speed)
{
	glm_t_vec3_add(g_engine.obj.pos, (t_vec3){ 0.0f, speed, 0.0f },
		g_engine.obj.pos);
}

void	handle_left(float speed)
{
	glm_t_vec3_add(g_engine.obj.pos, (t_vec3){ speed, 0.0f, 0.0f },
		g_engine.obj.pos);
}
