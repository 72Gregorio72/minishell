/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcastald <vcastald@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 09:09:10 by vcastald          #+#    #+#             */
/*   Updated: 2025/04/08 13:42:22 by vcastald         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include "libft.h"

t_lexing	*ft_lstnew_cleaned(char *value, char *type,
		int strength, char **command)
{
	t_lexing	*d;

	d = (t_lexing *)malloc(sizeof(t_lexing));
	if (!d)
		return (NULL);
	d->value = value;
	d->type = type;
	d->pos = 0;
	d->layer = 0;
	d->strength = strength;
	d->env_variable = 0;
	d->wildcard = 0;
	d->infile = 0;
	d->outfile = 1;
	d->expanded = 1;
	d->next = NULL;
	d->command = command;
	d->piped = 0;
	return (d);
}

t_lexing	*ft_lstnew(char *value, char *type, int strength)
{
	t_lexing	*d;

	d = (t_lexing *)malloc(sizeof(t_lexing));
	if (!d)
		return (NULL);
	d->value = value;
	d->type = type;
	d->pos = 0;
	d->layer = 0;
	d->strength = strength;
	d->env_variable = 0;
	d->wildcard = 0;
	d->expanded = 1;
	d->infile = 0;
	d->outfile = 1;
	d->next = NULL;
	d->command = NULL;
	d->piped = 0;
	return (d);
}

/*typedef struct s_list
{
	void *content;
	struct s_list *next;
}		t_list;


void stampa(t_list *list)
{
	while (list != NULL)
	{
		printf("%d", list->content);
		list = list->next; 
	}
}

int main(void)
{
	int i = 0;
	t_list *d;

	while (i < 10)
	{
		d = ft_lstnew((void *)i);
		i++;
	}
	stampa(d);
	return (0);
}*/
