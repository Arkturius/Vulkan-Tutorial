/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   app.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 21:06:07 by rgramati          #+#    #+#             */
/*   Updated: 2024/05/23 20:23:11 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __APP_H
# define __APP_H

# include <unistd.h>
# include <sys/types.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <stdint.h>
# include <stdbool.h>
# include <fcntl.h>

# include <colors.h>
#include <vulkan/vulkan_core.h>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define DBG_DONE BOLD "🔄 [" FG_GREEN "%s" RESET_ALL BOLD "]\n"
#define DBG_VERBOSE BOLD "📑 [" FG_BLUE "%s" RESET_ALL BOLD "]\n"
#define DBG_ERROR BOLD "❌ [" FG_RED "Error" RESET_ALL BOLD "] %s\n"

# define APP_WIDTH	1366
# define APP_HEIGHT	768
# define APP_TITLE	"Hello Triangle"

typedef uint8_t		u8;
typedef uint16_t	u16;
typedef uint32_t	u32;

typedef int8_t		i8;
typedef int16_t		i16;
typedef int32_t 	i32;

extern const bool	enableValidationLayers;

extern const char	*validationLayers[2];
extern const u32	layerCount;

extern const char	*deviceExtensions[2];
extern const u32	deviceExtensionCount;

typedef enum e_err
{
	ERR_OK,
	ERR_ALLOC,
	ERR_FILE,
	ERR_GLFW,
	ERR_VULKAN,
}	ErrorType;

typedef enum e_appflags
{
	APP_VERBOSE = 0x0001,
}	AppFlags;

typedef struct s_err
{
	ErrorType	type;
	char		*data;
}	Error;

typedef struct s_qfamily
{
	bool	gHasValue;
	u32		graphicsFamily;
	bool	pHasValue;
	u32		presentFamily;
}	QueueFamilyIndices;

typedef struct s_scdetails
{
	VkSurfaceCapabilitiesKHR	capabilities;
	VkSurfaceFormatKHR			*formats;
	VkPresentModeKHR			*presentModes;
	u32							formatCount;
	u32							presentModesCount;
}	SwapChainSupportDetails;

typedef struct s_app
{
	GLFWwindow			*window;
	VkInstance			vk;

	VkPhysicalDevice	physicalDevice;
	VkDevice			device;

	VkQueue				graphicsQueue;
	VkQueue				presentQueue;

	VkSurfaceKHR		surface;
	VkSwapchainKHR		swapChain;
	VkImage				*swapChainImages;
	u32					swapChainImageCount;
	VkFormat			swapChainFormat;
	VkExtent2D			swapChainExtent;
	VkImageView			*swapChainImageViews;
	QueueFamilyIndices	queueFamilies;
	VkRenderPass		renderPass;
	VkPipelineLayout	pipelineLayout;
	VkPipeline			graphicsPipeline;
	VkFramebuffer		*swapChainFramebuffers;

	VkCommandPool		commandPool;
	VkCommandBuffer		commandBuffer;

	VkSemaphore			imageAvailableSemaphore;
	VkSemaphore			renderFinishedSemaphore;
	VkFence				inFlightFence;

	AppFlags			flags;
}	App;

typedef struct s_sfile
{
	char	*content;
	u32		size;
}	ShaderFile;

Error					htError(ErrorType type, char *data);

int						htErrorHandle(Error err);


void					initWindow(App *app, Error *err);

void					initVulkan(App *app, Error *err);

void					mainLoop(App *app, Error *err);

void					cleanup(App *app, Error *err);


void					createInstance(App *app, Error *err);

void					checkValidationLayers(App *app, Error *err);

void					checkExtensions(App *app, Error *err, const char **gExt, u32 gExtCount);

void					pickPhysicalDevice(App *app, Error *err);

QueueFamilyIndices		findQueueFamilies(App *app, VkPhysicalDevice device);

void					createLogicalDevice(App *app, Error *err);

void					createSurface(App *app, Error *err);


SwapChainSupportDetails	querySwapChainSupport(App *app, VkPhysicalDevice device);

void					createSwapChain(App *app, Error *err);

void					createImageViews(App *app, Error *err);

void					createRenderPass(App *app, Error *err);

void					createGraphicsPipeline(App *app, Error *err);

void					createFramebuffers(App *app, Error *err);


void					createCommandPool(App *app, Error *err);

void					createCommandBuffer(App *app, Error *err);

void					recordCommandBuffer(App *app, Error *err, VkCommandBuffer commandBuffer, uint32_t imageIndex);

void					createSyncObjects(App *app, Error *err);

#endif // __APP_H
