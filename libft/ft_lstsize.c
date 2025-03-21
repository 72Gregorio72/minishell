/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstsize.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcastald <vcastald@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 09:11:07 by vcastald          #+#    #+#             */
/*   Updated: 2025/03/14 11:12:15 by vcastald         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include "libft.h"

int	ft_lstsize(t_lexing *lst)
{
	int	i;

	i = 0;
	while (lst != NULL)
	{
		lst = lst->next;
		i++;
	}
	return (i);
}

/*int main(void)
{
	t_list *head = NULL;
	t_list *new = NULL;

	for (int i = 0; i < 10; i++)
	{
		new = malloc(sizeof(t_list));
		new->content = i;
		new->next = head;
		head = new;
	}
	int n = ft_lstsize(head);
	printf("%d\n", n);
	return (0);
}*/
