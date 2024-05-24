/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 23:12:54 by rgramati          #+#    #+#             */
/*   Updated: 2024/05/24 17:24:08 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <app.h>

static ShaderFile readFile(const char *filename, Error *err)
{
	ShaderFile	shader = {0};
	i8			fd;

	fd = open(filename, O_RDONLY);
	if (fd == -1)
		*err = htError(ERR_FILE, "Vulkan: failed to open shader file");
	else
	{
		shader.size = lseek(fd, 0, SEEK_END) - lseek(fd, 0, SEEK_SET);
		shader.content = malloc(shader.size * sizeof(char));
		if (shader.content)
			read(fd, shader.content, shader.size);
	}
	close(fd);
	return (shader);
}

static VkShaderModule	createShaderModule(App *app, Error *err, ShaderFile shader)
{
	VkShaderModuleCreateInfo	createInfo = {0};
	VkShaderModule				shaderModule = {0};

	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.codeSize = shader.size;
	createInfo.pCode = (u32 *)shader.content;
	if (vkCreateShaderModule(app->device, &createInfo, NULL, &shaderModule))
		*err = htError(ERR_VULKAN, "Vulkan: Failed to create shader module");
	else
		printf(DBG_DONE, "Shader Module");
	return (shaderModule);
}

void	createRenderPass(App *app, Error *err)
{
	VkAttachmentDescription	colorAttachment = {0};
	VkAttachmentReference	colorAttachmentRef = {0};
	VkSubpassDescription	subpass = {0};
	VkRenderPassCreateInfo	renderPassInfo = {0};
	VkSubpassDependency		dependency = {0};

    colorAttachment.format = app->swapChainFormat;
    colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
	colorAttachmentRef.attachment = 0;
	colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass.colorAttachmentCount = 1;
	subpass.pColorAttachments = &colorAttachmentRef;
	dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
	dependency.dstSubpass = 0;
	dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderPassInfo.attachmentCount = 1;
	renderPassInfo.pAttachments = &colorAttachment;
	renderPassInfo.subpassCount = 1;
	renderPassInfo.pSubpasses = &subpass;
	renderPassInfo.dependencyCount = 1;
	renderPassInfo.pDependencies = &dependency;
	if (vkCreateRenderPass(app->device, &renderPassInfo, NULL, &app->renderPass))
		*err = htError(ERR_VULKAN, "Vulkan: Failed to create Render Pass");
	else
		printf(DBG_DONE, "Render Pass");
}

void	createGraphicsPipeline(App *app, Error *err)
{
	ShaderFile								vertShader;
	ShaderFile								fragShader;
	VkShaderModule							vShaderModule = {0};
	VkShaderModule							fShaderModule = {0};
	VkPipelineShaderStageCreateInfo			shaderStage[2] = {0};
	VkDynamicState							dynamicStates[2] = {VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR};
	VkPipelineDynamicStateCreateInfo		dynamicState = {0};
	VkPipelineVertexInputStateCreateInfo	vertexInputInfo = {0};
	VkPipelineInputAssemblyStateCreateInfo	inputAssembly = {0};
	VkViewport 								viewport = {0};
	VkRect2D 								scissor = {0};
	VkPipelineViewportStateCreateInfo		viewportState = {0};
	VkPipelineRasterizationStateCreateInfo	rasterizer = {0};
	VkPipelineMultisampleStateCreateInfo	multisampling = {0};
	VkPipelineColorBlendAttachmentState 	colorBlendAttachment = {0};
	VkPipelineColorBlendStateCreateInfo 	colorBlending = {0};
	VkPipelineLayoutCreateInfo				pipelineLayoutInfo = {0};
	VkGraphicsPipelineCreateInfo			pipelineInfo = {0};

	vertShader = readFile("src/shaders/vert.spv", err);
	fragShader = readFile("src/shaders/frag.spv", err);
	vShaderModule = createShaderModule(app, err, vertShader);
	fShaderModule = createShaderModule(app, err, fragShader);
	free(vertShader.content);
	free(fragShader.content);
	if (err->type != ERR_OK)
		return ;
	shaderStage[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	shaderStage[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
	shaderStage[0].module = vShaderModule;
	shaderStage[0].pName = "main";
	shaderStage[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	shaderStage[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	shaderStage[1].module = fShaderModule;
	shaderStage[1].pName = "main";
	vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	inputAssembly.primitiveRestartEnable = VK_FALSE;
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = (float) app->swapChainExtent.width;
	viewport.height = (float) app->swapChainExtent.height;
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;
	scissor.offset = (VkOffset2D){0, 0};
	scissor.extent = app->swapChainExtent;
	viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportState.viewportCount = 1;
	viewportState.scissorCount = 1;
	rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterizer.depthClampEnable = VK_FALSE;
	rasterizer.rasterizerDiscardEnable = VK_FALSE;
	rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
	rasterizer.lineWidth = 1.0f;
	rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
	rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
	rasterizer.depthBiasEnable = VK_FALSE;
	multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisampling.sampleShadingEnable = VK_FALSE;
	multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
	colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	colorBlendAttachment.blendEnable = VK_FALSE;
	colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	colorBlending.logicOpEnable = VK_FALSE;
	colorBlending.attachmentCount = 1;
	colorBlending.pAttachments = &colorBlendAttachment;
	dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	dynamicState.dynamicStateCount = 2;
	dynamicState.pDynamicStates = dynamicStates;
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount = 0;
	pipelineLayoutInfo.pushConstantRangeCount = 0;
	if (vkCreatePipelineLayout(app->device, &pipelineLayoutInfo, NULL, &app->pipelineLayout))
		*err = htError(ERR_VULKAN, "Vulkan: Failed to create Pipeline Layout");
	else
		printf(DBG_DONE, "Pipeline Layout");
	pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipelineInfo.stageCount = 2;
	pipelineInfo.pStages = shaderStage;
	pipelineInfo.pVertexInputState = &vertexInputInfo;
	pipelineInfo.pInputAssemblyState = &inputAssembly;
	pipelineInfo.pViewportState = &viewportState;
	pipelineInfo.pRasterizationState = &rasterizer;
	pipelineInfo.pMultisampleState = &multisampling;
	pipelineInfo.pColorBlendState = &colorBlending;
	pipelineInfo.pDynamicState = &dynamicState;
	pipelineInfo.layout = app->pipelineLayout;
	pipelineInfo.renderPass = app->renderPass;
	pipelineInfo.subpass = 0;
	if (vkCreateGraphicsPipelines(app->device, VK_NULL_HANDLE, 1, &pipelineInfo, NULL, &app->graphicsPipeline))
		*err = htError(ERR_VULKAN, "Vulkan: Failed to create Graphics Pipeline");
	else
		printf(DBG_DONE, "Graphics Pipeline");
	vkDestroyShaderModule(app->device, vShaderModule, NULL);
	vkDestroyShaderModule(app->device, fShaderModule, NULL);
}

void	createFramebuffers(App *app, Error *err)
{
	VkFramebufferCreateInfo	framebufferInfo = {0};

	app->swapChainFramebuffers = malloc(app->swapChainImageCount * sizeof(VkFramebuffer));
	if (app->flags & APP_VERBOSE)
		printf(DBG_VERBOSE, "Framebuffers");
	for (u32 i = 0; i < app->swapChainImageCount; i++)
	{
		framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.renderPass = app->renderPass;
		framebufferInfo.attachmentCount = 1;
		framebufferInfo.pAttachments = &app->swapChainImageViews[i];
		framebufferInfo.width = app->swapChainExtent.width;
		framebufferInfo.height = app->swapChainExtent.height;
		framebufferInfo.layers = 1;
		if (vkCreateFramebuffer(app->device, &framebufferInfo, NULL, &app->swapChainFramebuffers[i]))
			*err = htError(ERR_VULKAN, "Vulkan: Failed to create Framebuffer");
		else if (app->flags & APP_VERBOSE)
			printf("   %3d - " FG_CYAN "%p\n" RESET_ALL BOLD, i, app->swapChainFramebuffers[i]);
	}
}
