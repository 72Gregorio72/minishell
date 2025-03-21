/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstlast.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcastald <vcastald@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 09:11:52 by vcastald          #+#    #+#             */
/*   Updated: 2025/03/14 11:07:52 by vcastald         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>
#include <stdlib.h>

t_lexing	*ft_lstlast(t_lexing *lst)
{
	while (lst)
	{
		if (!lst->next)
			return (lst);
		lst = lst->next;
	}
	return (lst);
}

/*typedef struct s_list
{
	int content;
	struct s_list *next;
} t_list;

void stampa(t_list *list)
{
	while (list != NULL)
	{
		printf("%d\n", list->content);
		list = list->next;
	}
}

int main(void)
{
	t_list *head = NULL;
	t_list *new = NULL;

	new = malloc(sizeof(t_list));
	new->content = 1;
	new->next = head;
	head = new;
	t_list *test = ft_lstlast(head);
	stampa(head);
	printf("%d\n", test->content);
	return (0);
}*/
