/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   swapchain.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 21:26:25 by rgramati          #+#    #+#             */
/*   Updated: 2024/05/22 23:03:32 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <app.h>
#include <stdio.h>
#include <stdlib.h>
#include <vulkan/vulkan_core.h>

static VkSurfaceFormatKHR	chooseSwapSurfaceFormat(SwapChainSupportDetails *details)
{
	VkSurfaceFormatKHR	*aFormats;

	aFormats = details->formats;
	for (u32 i = 0; i < details->formatCount; i++)
	{
		if (aFormats[i].format == VK_FORMAT_B8G8R8A8_SRGB && aFormats[i].colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
			return aFormats[i];
	}
	return aFormats[0];
}

static VkPresentModeKHR	chooseSwapPresentMode(SwapChainSupportDetails *details)
{
	VkPresentModeKHR	*aPresentModes;

	aPresentModes = details->presentModes;
	for (u32 i = 0; i < details->presentModesCount; i++)
	{
		if (aPresentModes[i] == VK_PRESENT_MODE_MAILBOX_KHR)
			return aPresentModes[i];
	}
	return (VK_PRESENT_MODE_FIFO_KHR);
}

static VkExtent2D	chooseSwapExtent(SwapChainSupportDetails *details)
{
	VkExtent2D actualExtent = {.height = APP_HEIGHT, .width = APP_WIDTH};

	if (details->capabilities.currentExtent.width != UINT32_MAX)
		return (details->capabilities.currentExtent);
	return (actualExtent);
}

SwapChainSupportDetails	querySwapChainSupport(App *app, VkPhysicalDevice device)
{
	SwapChainSupportDetails	details;

	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, app->surface, &details.capabilities);
	details.formatCount = 0;
	vkGetPhysicalDeviceSurfaceFormatsKHR(device, app->surface, &details.formatCount, NULL);
	if (details.formatCount != 0)
	{
		details.formats = malloc(details.formatCount * sizeof(VkSurfaceFormatKHR));
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, app->surface, &details.formatCount, details.formats);
	}
	details.presentModesCount = 0;
	vkGetPhysicalDeviceSurfacePresentModesKHR(device, app->surface, &details.presentModesCount, NULL);
	if (details.presentModesCount != 0)
	{
		details.presentModes = malloc(details.presentModesCount * sizeof(VkPresentModeKHR));
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, app->surface, &details.presentModesCount, details.presentModes);
	}
	return (details);
}

static void	retrieveSwapChainImages(App *app, Error *err, VkFormat format, VkExtent2D extent)
{
	u32	imageCount;

	imageCount = 0;
	vkGetSwapchainImagesKHR(app->device, app->swapChain, &imageCount, NULL);
	if (!imageCount)
	{
		*err = htError(ERR_VULKAN, "Vulkan: 0 images in SwapChain");
		return ;
	}
	app->swapChainImageCount = imageCount;
	app->swapChainImages = malloc(imageCount * sizeof(VkImage));
	vkGetSwapchainImagesKHR(app->device, app->swapChain, &imageCount, app->swapChainImages);
	app->swapChainFormat = format;
	app->swapChainExtent = extent;
}

void	createSwapChain(App *app, Error *err)
{
	SwapChainSupportDetails		swapChain;
	VkSwapchainCreateInfoKHR	createInfo = {0};
	VkSurfaceFormatKHR			surfaceFormat;
	VkPresentModeKHR			presentMode;
	VkExtent2D					extent;
	u32							imageCount;
	u32							indices[2];

	swapChain = querySwapChainSupport(app, app->physicalDevice);
	surfaceFormat = chooseSwapSurfaceFormat(&swapChain);
	presentMode = chooseSwapPresentMode(&swapChain);
	extent = chooseSwapExtent(&swapChain);
	if (app->flags & APP_VERBOSE)
	{
		printf(DBG_VERBOSE, "SwapChain extent ");
		printf(BOLD "WIDTH  - " FG_CYAN "%d"  RESET_ALL BOLD "\nHEIGTH - " FG_CYAN "%d\n" RESET_ALL BOLD, extent.width, extent.height);
	}
	imageCount = swapChain.capabilities.minImageCount + 1;
	if (swapChain.capabilities.maxImageCount > 0 && imageCount > swapChain.capabilities.maxImageCount)
		imageCount = swapChain.capabilities.maxImageCount;
	createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	createInfo.surface = app->surface;
	createInfo.minImageCount = imageCount;
	createInfo.imageFormat = surfaceFormat.format;
	createInfo.imageColorSpace = surfaceFormat.colorSpace;
	createInfo.imageExtent = extent;
	createInfo.imageArrayLayers = 1;
	createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
	if (app->queueFamilies.graphicsFamily != app->queueFamilies.presentFamily)
	{
		createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		createInfo.queueFamilyIndexCount = 2;
		indices[0] = app->queueFamilies.graphicsFamily;
		indices[1] = app->queueFamilies.presentFamily;
		createInfo.pQueueFamilyIndices = indices;
	}
	else
		createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
	createInfo.preTransform = swapChain.capabilities.currentTransform;
	createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	createInfo.presentMode = presentMode;
	createInfo.clipped = VK_TRUE;
	createInfo.oldSwapchain = VK_NULL_HANDLE;
	if (vkCreateSwapchainKHR(app->device, &createInfo, NULL, &app->swapChain))
		*err = htError(ERR_VULKAN, "Vulkan: Failed to create swapchain.");
	else
		printf(DBG_DONE, "SwapChain creation");
	retrieveSwapChainImages(app, err, surfaceFormat.format, extent);
	free(swapChain.formats);
	free(swapChain.presentModes);
}
