/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   images.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 17:18:09 by rgramati          #+#    #+#             */
/*   Updated: 2024/05/22 23:04:13 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <app.h>

void	createImageViews(App *app, Error *err)
{
	VkImageViewCreateInfo	createInfo = {0};

	app->swapChainImageViews = malloc(app->swapChainImageCount * sizeof(VkImageView));
	if (app->flags & APP_VERBOSE)
		printf(DBG_VERBOSE, "Image Views");
	for (u32 i = 0; i < app->swapChainImageCount; i++)
	{
		createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		createInfo.image = app->swapChainImages[i];
		createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		createInfo.format = app->swapChainFormat;
		createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		createInfo.subresourceRange.baseMipLevel = 0;
		createInfo.subresourceRange.levelCount = 1;
		createInfo.subresourceRange.baseArrayLayer = 0;
		createInfo.subresourceRange.layerCount = 1;
		if (vkCreateImageView(app->device, &createInfo, NULL, &app->swapChainImageViews[i]))
		{
			*err = htError(ERR_VULKAN, "Vulkan: Failed to create image view.");
			break ;
		}
		else if (app->flags & APP_VERBOSE)
			printf("   %3d - " FG_CYAN "%p\n" RESET_ALL BOLD, i, app->swapChainImageViews[i]);
	}
}
