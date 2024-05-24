/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   window_surface.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 18:09:09 by rgramati          #+#    #+#             */
/*   Updated: 2024/05/21 18:25:49 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <app.h>

void	createSurface(App *app, Error *err)
{
	if (glfwCreateWindowSurface(app->vk, app->window, NULL, &app->surface))
		*err = htError(ERR_VULKAN, "GLFW: Failed to create surface.");
}