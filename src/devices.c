/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   devices.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 16:06:19 by rgramati          #+#    #+#             */
/*   Updated: 2024/05/24 18:49:06 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <app.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <vulkan/vulkan_core.h>

// static bool	checkDeviceExtensionSupport(VkPhysicalDevice device)
// {
// 	VkExtensionProperties	*availableExtensions;
// 	u32				extensionCount;
// 	u32				matchingExtensions;

// 	vkEnumerateDeviceExtensionProperties(device, NULL, &extensionCount, NULL);
// 	availableExtensions = malloc(extensionCount * sizeof(VkExtensionProperties));
// 	vkEnumerateDeviceExtensionProperties(device, NULL, &extensionCount, availableExtensions);
// 	matchingExtensions = 0;
// 	for (u32 i = 0; i < deviceExtensionCount; i++)
// 	{
// 		for (u32 j = 0; j < extensionCount; j++)
// 			matchingExtensions += (!strcmp(availableExtensions[j].extensionName, deviceExtensions[i]));
// 	}
// 	free(availableExtensions);
// 	return (matchingExtensions == deviceExtensionCount);
// }

// static bool	isDeviceSuitable(App *app, VkPhysicalDevice device)
// {
// 	QueueFamilyIndices		indices = {0};
// 	SwapChainSupportDetails	details = {0};
// 	bool					extensionsSupported = false;
// 	bool					validSwapChain = false;

// 	indices = findQueueFamilies(app, device);
// 	extensionsSupported = checkDeviceExtensionSupport(device);
// 	if (extensionsSupported)
// 	{
// 		details = querySwapChainSupport(app, device);
// 		validSwapChain = (details.formatCount && details.presentModesCount);
// 	}
// 	free(details.formats);
// 	free(details.presentModes);
// 	return (indices.gHasValue && indices.pHasValue && extensionsSupported && validSwapChain);
// }

void	pickPhysicalDevice(App *app, Error *err)
{
	VkPhysicalDevice	*devices;
	u32					deviceCount;
	// u32					i;

	deviceCount = 0;
	app->physicalDevice = VK_NULL_HANDLE;
	vkEnumeratePhysicalDevices(app->vk, &deviceCount, NULL);
	if (!deviceCount)
	{
		*err = htError(ERR_VULKAN, "Vulkan: No physical device found.");
		return ;
	}
	devices = malloc(deviceCount * sizeof(VkPhysicalDevice));
	vkEnumeratePhysicalDevices(app->vk, &deviceCount, devices);
	if (app->flags & APP_VERBOSE)
	{
		printf(DBG_VERBOSE, "Physical devices");
		for (u32 i = 0; i < deviceCount; i++)
			printf("   %3d - " FG_CYAN "%-30p\n" RESET_ALL BOLD, i, (devices[i]));
	}
	// i = 0;
	// while (!isDeviceSuitable(app, devices[i]))
	// 	i++;
	// if (i < deviceCount)
	// 	app->physicalDevice = devices[i];
	// i = 0;
	// while (i < deviceCount)
	// {
	// 	VkPhysicalDeviceProperties props;
	// 	vkGetPhysicalDeviceProperties(devices[i], &props);
	// 	printf("Vulkan : physical device, %s\n", props.deviceName);
	// 	i++;
	// }
	app->physicalDevice = devices[1];	// HARD PICKED THE BEST GPU, because i'm lazy: TODO: write a physicalDeviceScore()
	free(devices);
	if (app->physicalDevice == VK_NULL_HANDLE)
		*err = htError(ERR_VULKAN, "Vulkan: Failed to find a suitable GPU.");
	else
		printf(DBG_DONE, "GPU research");
}

static void	getFamilyDeviceQueues(VkDeviceQueueCreateInfo *queues, QueueFamilyIndices indices)
{
	float					queuePriority = 1.0f;
	VkDeviceQueueCreateInfo	graphicsQueue = {0};
	VkDeviceQueueCreateInfo	presentQueue = {0};

	graphicsQueue.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	graphicsQueue.queueFamilyIndex = indices.graphicsFamily;
	graphicsQueue.queueCount = 1;
	graphicsQueue.pQueuePriorities = &queuePriority;
	queues[0] = graphicsQueue;
	presentQueue = graphicsQueue;
	presentQueue.queueFamilyIndex = indices.graphicsFamily;
	queues[1] = presentQueue;
}

void	createLogicalDevice(App *app, Error *err)
{
	QueueFamilyIndices			indices;
	VkDeviceQueueCreateInfo		queueCreateInfo[2];
	VkPhysicalDeviceFeatures	deviceFeatures = {0};
	VkDeviceCreateInfo			createInfo = {0};

	indices = findQueueFamilies(app, app->physicalDevice);
	getFamilyDeviceQueues(queueCreateInfo, indices);
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	createInfo.pQueueCreateInfos = queueCreateInfo;
	createInfo.queueCreateInfoCount = (indices.graphicsFamily == indices.presentFamily ? 1 : 2);
	vkGetPhysicalDeviceFeatures(app->physicalDevice, &deviceFeatures);
	createInfo.pEnabledFeatures = &deviceFeatures;
	createInfo.enabledExtensionCount = deviceExtensionCount;
	createInfo.ppEnabledExtensionNames = deviceExtensions;
	createInfo.enabledLayerCount = 0;
	if (enableValidationLayers)
	{
		createInfo.ppEnabledLayerNames = (const char **) validationLayers;
		createInfo.enabledLayerCount = layerCount;
	}
	if (vkCreateDevice(app->physicalDevice, &createInfo, NULL, &app->device))
	{
		*err = htError(ERR_VULKAN, "Vulkan: failed to create logical device.");
		return ;
	}
	vkGetDeviceQueue(app->device, indices.graphicsFamily, 0, &app->graphicsQueue);
	vkGetDeviceQueue(app->device, indices.presentFamily, 0, &app->presentQueue);
	app->queueFamilies = indices;
}
