/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/23 19:49:57 by rgramati          #+#    #+#             */
/*   Updated: 2024/05/24 18:48:08 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <app.h>
#include <stdio.h>
#include <time.h>
#include <vulkan/vulkan_core.h>

void	createCommandPool(App *app, Error *err)
{
	VkCommandPoolCreateInfo	poolInfo = {0};

	poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	poolInfo.queueFamilyIndex = app->queueFamilies.graphicsFamily;
	if (vkCreateCommandPool(app->device, &poolInfo, NULL, &app->commandPool))
		*err = htError(ERR_VULKAN, "Vulkan: Failed to create Command Pool");
	else
		printf(DBG_DONE, "Command Pool");
}

void	createCommandBuffer(App *app, Error *err)
{
	VkCommandBufferAllocateInfo	allocInfo = {0};

	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.commandPool = app->commandPool;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandBufferCount = 1;

	if (vkAllocateCommandBuffers(app->device, &allocInfo, &app->commandBuffer))
		*err = htError(ERR_VULKAN, "Vulkan: Failed to create Command Buffer");
	else
		printf(DBG_DONE, "Command Buffer");
}

void	recordCommandBuffer(App *app, Error *err, VkCommandBuffer commandBuffer, uint32_t imageIndex)
{
	VkCommandBufferBeginInfo	beginInfo = {0};
	VkRenderPassBeginInfo		renderPassInfo = {0};
	VkClearValue 				clearColor = {{{0.0f, 0.0f, 0.0f, 1.0f}}};
	VkViewport					viewport = {0};
	VkRect2D					scissor = {0};

	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	if (vkBeginCommandBuffer(commandBuffer, &beginInfo))
		*err = htError(ERR_VULKAN, "Vulkan: Failed to begin recording Command Buffer");
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassInfo.renderPass = app->renderPass;
	renderPassInfo.framebuffer = app->swapChainFramebuffers[imageIndex];
	renderPassInfo.renderArea.offset = (VkOffset2D) {0, 0};
	renderPassInfo.renderArea.extent = app->swapChainExtent;
	renderPassInfo.clearValueCount = 1;
	renderPassInfo.pClearValues = &clearColor;
	vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
	vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, app->graphicsPipeline);
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = (float) (app->swapChainExtent.width);
	viewport.height = (float) (app->swapChainExtent.height);
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;
	vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
	scissor.offset =(VkOffset2D) {0, 0};
	scissor.extent = app->swapChainExtent;
	vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
	vkCmdDraw(commandBuffer, 3, 1, 0, 0);
	vkCmdEndRenderPass(commandBuffer);
	if (vkEndCommandBuffer(commandBuffer))
		*err = htError(ERR_VULKAN, "Vulkan: Failed to record Command Buffer");
}

void	createSyncObjects(App *app, Error *err)
{
	VkSemaphoreCreateInfo	semaphoreInfo = {0};
	VkFenceCreateInfo		fenceInfo = {0};

    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
	fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
	if (vkCreateSemaphore(app->device, &semaphoreInfo, NULL, &app->imageAvailableSemaphore))
		*err = htError(ERR_VULKAN, "Vulkan: Failed to create semaphore");
	if (vkCreateSemaphore(app->device, &semaphoreInfo, NULL, &app->renderFinishedSemaphore))
		*err = htError(ERR_VULKAN, "Vulkan: Failed to create semaphore");
	if (vkCreateFence(app->device, &fenceInfo, NULL, &app->inFlightFence))
		*err = htError(ERR_VULKAN, "Vulkan: Failed to create fence");
	if (err->type != ERR_OK)
		return ;
	printf(DBG_DONE, "Semaphores");
}

