/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validation_layers.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 14:50:51 by rgramati          #+#    #+#             */
/*   Updated: 2024/05/22 00:16:40 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <app.h>
#include <stdint.h>
#include <vulkan/vulkan_core.h>

static bool	validationLayerSupport(VkLayerProperties *aLayers, u32 aCount)
{
	u32			matchingLayers;
	u32			matchingCache;

	matchingLayers = 0;
	for (u32 i = 0; i < layerCount; i++)
	{
		matchingCache = matchingLayers;
		for (u32 j = 0; j < aCount; j++)
			matchingLayers += (!strcmp(aLayers[j].layerName, validationLayers[i]));
		if (matchingCache == matchingLayers)
			printf(DBG_ERROR, "Missing Layer");
	}
	return (matchingLayers == layerCount);
}

void	checkValidationLayers(App *app, Error *err)
{
	VkLayerProperties	*availableLayers = NULL;
	u32			availableLayersCount;

	if (!enableValidationLayers)
		return ;
	availableLayersCount = 0;
	vkEnumerateInstanceLayerProperties(&availableLayersCount, NULL);
	if (!availableLayersCount)
	{
		*err = htError(ERR_VULKAN, "Vulkan: No validations layers found.");
		return ;
	}
	availableLayers = malloc(availableLayersCount * sizeof(VkLayerProperties));
	vkEnumerateInstanceLayerProperties(&availableLayersCount, availableLayers);

	if (app->flags & APP_VERBOSE)
	{
		printf(DBG_VERBOSE, "Validation layers");
		for (u32 i = 0; i < layerCount; i++)
			printf("   %3d - " FG_CYAN "%-30s\n" RESET_ALL BOLD, i, validationLayers[i]);
	}
	if (!validationLayerSupport(availableLayers, availableLayersCount))
		*err = htError(ERR_VULKAN, "Missing layer support.");
	else
		printf(DBG_DONE, "Layers check");
	free(availableLayers);
}