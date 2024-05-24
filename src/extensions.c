/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extensions.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 15:37:43 by rgramati          #+#    #+#             */
/*   Updated: 2024/05/22 00:17:09 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <app.h>

static bool extensionSupport(const char **gExt, u32 gExtCount, VkExtensionProperties *vkExt, u32 vkExtCount)
{
	u32	matchingExtensions;
	u32	matchingCache;

	matchingExtensions = 0;
	for (u32 i = 0; i < gExtCount; i++)
	{
		matchingCache = matchingExtensions;
		for (u32 j = 0; j < vkExtCount; j++)
			matchingExtensions += (!strcmp(vkExt[j].extensionName, gExt[i]));
		if (matchingCache == matchingExtensions)
			printf(DBG_ERROR,"Missing Extension");
	}
	return (matchingExtensions == gExtCount);
}

void checkExtensions(App *app, Error *err, const char **gExt, u32 gExtCount)
{
	VkExtensionProperties	*extensions = NULL;
	u32						extensionCount;

	extensionCount = 0;
	vkEnumerateInstanceExtensionProperties(NULL, &extensionCount, NULL);
	if (!extensionCount)
	{
		*err = htError(ERR_VULKAN, "Vulkan: No extensions found.");
		return ;
	}
	extensions = malloc(extensionCount * sizeof(VkExtensionProperties));
	vkEnumerateInstanceExtensionProperties(NULL, &extensionCount, extensions);
	if (app->flags & APP_VERBOSE)
	{
		printf(DBG_VERBOSE, "Vulkan extensions");
		for (u32 i = 0; i < extensionCount; i++)
			printf("   %3d - " FG_CYAN "%-30s\n" RESET_ALL BOLD, i, extensions[i].extensionName);
	}
	if (!extensionSupport(gExt, gExtCount, extensions, extensionCount))
		*err = htError(ERR_VULKAN, "Missing extension support.");
	else
		printf(DBG_DONE, "Extensions check");
	free(extensions);
}