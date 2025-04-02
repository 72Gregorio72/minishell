/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstdelone.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcastald <vcastald@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 09:13:42 by vcastald          #+#    #+#             */
/*   Updated: 2025/03/26 14:37:42 by vcastald         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>
#include <stdlib.h>

/*typedef struct s_list
{
	void *content;
	struct s_list *next;
} t_list;

void del(void *p)
{
	free(p);
}*/

/* static int	util(char **final)
{
	int	i;

	i = 0;
	while (final[i])
	{
		free(final[i]);
		i++;
	}
	free(final);
	return (1);
} */

void	ft_lstdelone(t_lexing *lst)
{
	if (!lst)
		return ;
	free(lst->value);
	if (ft_strncmp(lst->type, "argument", 9) == 0
		|| ft_strncmp(lst->type, "option", 7) == 0
		|| !ft_strncmp(lst->type, "outfile", 8)
		|| !ft_strncmp(lst->type, "infile", 7))
		free(lst->type);
	if (lst->command)
		free_matrix(lst->command);
	free(lst);
}

/*void stampa(t_list *list)
{
	while (list != NULL)
	{
		printf("%p\n", list->content);
		list = list->next;
	}
}

int main(void)
{
	t_list *head = NULL;
	t_list *new = NULL;

	new = malloc(sizeof(t_list));
	new->content = NULL;
	new->next = head;
	head = new;
	printf("Stampa prima di del: ");
	stampa(head);
	ft_lstdelone(new, del);
	printf("Stampa dopo del: ");
	stampa(head);
	return (0);
}*/
