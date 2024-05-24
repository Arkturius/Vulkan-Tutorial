/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vulkan.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 00:03:32 by rgramati          #+#    #+#             */
/*   Updated: 2024/05/22 00:16:40 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <app.h>

void	createInstance(App *app, Error *err)
{
	VkApplicationInfo		appInfo = {0};
	VkInstanceCreateInfo	createInfo = {0};
	const char				**glfwExtensions = NULL;
	u32				glfwExtensionCount;

	checkValidationLayers(app, err);
	if (err->type != ERR_OK)
		return ;
	glfwExtensionCount = 0;
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = APP_TITLE;
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;
	createInfo.pApplicationInfo = &appInfo;
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.enabledExtensionCount = glfwExtensionCount;
	createInfo.ppEnabledExtensionNames = glfwExtensions;
	createInfo.enabledLayerCount = 0;
	if (enableValidationLayers)
	{
		createInfo.ppEnabledLayerNames = (const char **) validationLayers;
		createInfo.enabledLayerCount = layerCount;
	}
	if (app->flags & APP_VERBOSE)
	{
		printf(DBG_VERBOSE, "GLFW extensions");
		for (u32 i = 0; i < glfwExtensionCount; i++)
			printf("   %3d - " FG_CYAN "%-30s\n" RESET_ALL BOLD, i, glfwExtensions[i]);
	}
	if (vkCreateInstance(&createInfo, NULL, &app->vk))
	{
		*err = htError(ERR_VULKAN, "Vulkan: failed to create instance.");
		return ;
	}
	checkExtensions(app, err, glfwExtensions, glfwExtensionCount);
}
