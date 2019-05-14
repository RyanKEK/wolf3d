/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohavryle <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/24 13:37:07 by ohavryle          #+#    #+#             */
/*   Updated: 2018/11/02 20:27:55 by ohavryle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void		*ft_memset(void *b, int c, size_t len)
{
	int		i;
	char	*str;

	str = (char*)b;
	i = -1;
	while (len-- > 0)
		*str++ = c;
	return (b);
}