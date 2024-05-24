/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   queue_families.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 17:11:40 by rgramati          #+#    #+#             */
/*   Updated: 2024/05/22 17:43:34 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <app.h>

QueueFamilyIndices	findQueueFamilies(App *app, VkPhysicalDevice device)
{
	QueueFamilyIndices		indices = {0};
	VkQueueFamilyProperties	*queueFamilies;
	u32						queueFamilyCount;
	VkBool32				presentSupport;

	queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, NULL);
	queueFamilies = malloc(queueFamilyCount * sizeof(VkQueueFamilyProperties));
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies);
	for (u32 i = 0; i < queueFamilyCount; i++)
	{
		if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
		{
			indices.graphicsFamily = i;
			indices.gHasValue = true;
		}
		presentSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(device, i, app->surface, &presentSupport);
		if (presentSupport)
		{
			indices.presentFamily = i;
			indices.pHasValue = true;
		}
		if (indices.pHasValue && indices.gHasValue)
			break ;
	}
	free(queueFamilies);
	return (indices);
}
