<<<<<<< HEAD
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcastald <vcastald@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 09:14:28 by vcastald          #+#    #+#             */
/*   Updated: 2025/04/09 09:29:45 by vcastald         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>
#include <stdlib.h>

void	ft_lstclear(t_lexing *lex_head, int clean)
{
	t_lexing	*temp;

	temp = NULL;
	while (lex_head)
	{
		temp = (lex_head)->next;
		if (clean)
			ft_lstdelone_clean(lex_head);
		else
			ft_lstdelone(lex_head);
		lex_head = temp;
	}
}

/*int main(void)
{
	t_list *head = NULL;

	// Creazione di una lista con 3 nodi
	for (int i = 0; i < 3; i++)
	{
		ft_lstadd_front(&head, ft_lstnew("Hello world"));
	}

	printf("Lista creata, ora la cancelliamo...\n");

	// Cancella tutti i nodi della lista
	ft_lstclear(&head, del);

	printf("Lista cancellata con successo.\n");

	return (0);
}*/
=======
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcastald <vcastald@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 09:14:28 by vcastald          #+#    #+#             */
/*   Updated: 2025/04/09 09:29:45 by vcastald         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>
#include <stdlib.h>

void	ft_lstclear(t_lexing *lex_head, int clean)
{
	t_lexing	*temp;

	temp = NULL;
	while (lex_head)
	{
		temp = (lex_head)->next;
		if (clean)
			ft_lstdelone_clean(lex_head);
		else
			ft_lstdelone(lex_head);
		lex_head = temp;
	}
}

/*int main(void)
{
	t_list *head = NULL;

	// Creazione di una lista con 3 nodi
	for (int i = 0; i < 3; i++)
	{
		ft_lstadd_front(&head, ft_lstnew("Hello world"));
	}

	printf("Lista creata, ora la cancelliamo...\n");

	// Cancella tutti i nodi della lista
	ft_lstclear(&head, del);

	printf("Lista cancellata con successo.\n");

	return (0);
}*/
>>>>>>> origin/vcastald
