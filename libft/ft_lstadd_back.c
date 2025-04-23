/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_back.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcastald <vcastald@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 09:12:43 by vcastald          #+#    #+#             */
/*   Updated: 2025/03/14 11:04:26 by vcastald         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>
#include <stdlib.h>

void	ft_lstadd_back(t_lexing **lst, t_lexing *new)
{
	t_lexing	*last;

	if (lst)
	{
		if (*lst)
		{
			last = ft_lstlast(*lst);
			last->next = new;
		}
		else
			*lst = new;
	}
}

/*typedef struct s_list
{
	int content;
	struct s_list *next;
} t_list;

void stampa(t_list *ptr)
{
	while (ptr != NULL)
	{
		printf("%p\n", ptr->content);
		ptr = ptr->next;
	}
}

int main(void)
{
	t_list *head;
	head = NULL;
	ft_lstadd_back(&head, ft_lstnew((void *)1));
	ft_lstadd_back(&head, ft_lstnew((void *)2));
	ft_lstadd_back(&head, ft_lstnew((void *)3));
	printf("%p\n", head->content);
	return (0);
}*/
