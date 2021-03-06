/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putendl_fd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohavryle <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/24 14:59:18 by ohavryle          #+#    #+#             */
/*   Updated: 2018/11/01 18:53:53 by ohavryle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putendl_fd(const char *s, int fd)
{
	char nl;

	nl = '\n';
	if (!s)
		return ;
	while (*s)
		write(fd, &*s++, 1);
	write(fd, &nl, 1);
}
