/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   app.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 21:06:05 by rgramati          #+#    #+#             */
/*   Updated: 2024/05/24 18:47:50 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "colors.h"
#include <app.h>

#ifdef NDEBUG
    const bool	enableValidationLayers = false;
#else
    const bool	enableValidationLayers = true;
#endif

const char	*validationLayers[2] = {"VK_LAYER_KHRONOS_validation", NULL};
const u32	layerCount = 1;

const char	*deviceExtensions[2] = {"VK_KHR_swapchain", NULL};
const u32	deviceExtensionCount = 1;

void	checkArgs(App *app, int argc, char **argv)
{
	if (argc == 1)
		return ;
	while (--argc)
	{
		argv++;
		if (!strcmp(*argv, "-v"))
			app->flags |= APP_VERBOSE;
	}
}

void	initWindow(App *app, Error *err)
{
	if (!glfwInit())
		*err = htError(ERR_GLFW, "GLFW: failed to init glfw.");
	if (err->type == ERR_OK)
	{
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
		app->window = glfwCreateWindow(APP_WIDTH, APP_HEIGHT, APP_TITLE, NULL, NULL);
	}
}

void	initVulkan(App *app, Error *err)
{
	createInstance(app, err);
	if (err->type == ERR_OK)
		createSurface(app, err);
	if (err->type == ERR_OK)
		pickPhysicalDevice(app, err);
	if (err->type == ERR_OK)
		createLogicalDevice(app, err);
	if (err->type == ERR_OK)
		createSwapChain(app, err);
	if (err->type == ERR_OK)
		createImageViews(app, err);
	if (err->type == ERR_OK)
		createRenderPass(app, err);
	if (err->type == ERR_OK)
		createGraphicsPipeline(app, err);
	if (err->type == ERR_OK)
	    createFramebuffers(app, err);
	if (err->type == ERR_OK)
    	createCommandPool(app, err);
	if (err->type == ERR_OK)
		createCommandBuffer(app, err);
	if (err->type == ERR_OK)
    	createSyncObjects(app, err);
}

static void	drawFrame(App *app, Error *err)
{
	u32						imageIndex;
	VkSubmitInfo			submitInfo = {0};
	VkSemaphore				*waitSemaphores = &app->imageAvailableSemaphore;
	VkSemaphore				*signalSemaphores = &app->renderFinishedSemaphore;
	VkPipelineStageFlags	waitStages[1] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
	VkPresentInfoKHR		presentInfo = {0};
	VkSwapchainKHR			*swapChains = &app->swapChain;

	vkWaitForFences(app->device, 1, &app->inFlightFence, VK_TRUE, UINT64_MAX);
	vkResetFences(app->device, 1, &app->inFlightFence);
    vkAcquireNextImageKHR(app->device, app->swapChain, UINT64_MAX, app->imageAvailableSemaphore, VK_NULL_HANDLE, &imageIndex);
	vkResetCommandBuffer(app->commandBuffer, 0);
	recordCommandBuffer(app, err, app->commandBuffer, imageIndex);
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = waitSemaphores;
	submitInfo.pWaitDstStageMask = waitStages;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &app->commandBuffer;
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = signalSemaphores;
	if (vkQueueSubmit(app->graphicsQueue, 1, &submitInfo, app->inFlightFence))
	{
		*err = htError(ERR_VULKAN, "Vulkan: Failed to submit buffer");
		return ;
	}
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = signalSemaphores;
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = swapChains;
	presentInfo.pImageIndices = &imageIndex;
	presentInfo.pResults = NULL;
	vkQueuePresentKHR(app->presentQueue, &presentInfo);
}

void	mainLoop(App *app, Error *err)
{
	while (!glfwWindowShouldClose(app->window) && err->type == ERR_OK)
	{
		glfwPollEvents();
		drawFrame(app, err);
	}
    vkDeviceWaitIdle(app->device);
}

void	cleanup(App *app, __attribute__((unused)) Error *err)
{
	if (app->vk)
	{
		vkDestroySemaphore(app->device, app->imageAvailableSemaphore, NULL);
		vkDestroySemaphore(app->device, app->renderFinishedSemaphore, NULL);
		vkDestroyFence(app->device, app->inFlightFence, NULL);
		vkDestroyCommandPool(app->device, app->commandPool, NULL);
		for (u32 i = 0; i < app->swapChainImageCount; i++)
			vkDestroyFramebuffer(app->device, app->swapChainFramebuffers[i], NULL);
	    vkDestroyPipeline(app->device, app->graphicsPipeline, NULL);
		vkDestroyPipelineLayout(app->device, app->pipelineLayout, NULL);
    	vkDestroyRenderPass(app->device, app->renderPass, NULL);
		for (u32 i = 0; i < app->swapChainImageCount; i++)
			vkDestroyImageView(app->device, app->swapChainImageViews[i], NULL);
		free(app->swapChainImageViews);
		free(app->swapChainImages);
		vkDestroySwapchainKHR(app->device, app->swapChain, NULL);
		vkDestroySurfaceKHR(app->vk, app->surface, NULL);
		vkDestroyDevice(app->device, NULL);
		vkDestroyInstance(app->vk, NULL);
		printf(DBG_DONE, "Vulkan cleanup");
	}
	glfwDestroyWindow(app->window);
	glfwTerminate();
	printf(DBG_DONE, "GLFW cleanup");
}

int main(int argc, char **argv)
{
	App		app = {0};
	Error	err = {.type = ERR_OK, .data = NULL};

	checkArgs(&app, argc, argv);
	initWindow(&app, &err);
	if (err.type == ERR_OK)
	{
		printf(DBG_DONE, "Window init");
		initVulkan(&app, &err);
	}
	if (err.type == ERR_OK)
	{
		printf(DBG_DONE, "Vulkan init");
		mainLoop(&app, &err);
	}
	cleanup(&app, &err);
	return (htErrorHandle(err));
}
