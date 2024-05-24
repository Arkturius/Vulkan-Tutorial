/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   err.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 21:16:13 by rgramati          #+#    #+#             */
/*   Updated: 2024/05/23 17:39:52 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <app.h>

int	htErrorHandle(Error err)
{
	if (err.type != ERR_OK)
	{
		printf(DBG_ERROR, err.data);
		return (EXIT_FAILURE);
	}
	return(EXIT_SUCCESS);
}

Error	htError(ErrorType type, char *data)
{
	return (Error){.type = type, .data = data};
}