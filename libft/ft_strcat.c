/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcat.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohavryle <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/23 19:55:48 by ohavryle          #+#    #+#             */
/*   Updated: 2018/11/01 16:49:38 by ohavryle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

char	*ft_strcat(char *s1, char *s2)
{
	int i;

	i = 0;
	while (*s1)
	{
		s1++;
		i++;
	}
	while (*s2)
	{
		*s1 = *s2;
		s1++;
		s2++;
		i++;
	}
	*s1 = '\0';
	s1 -= i;
	return (s1);
}
