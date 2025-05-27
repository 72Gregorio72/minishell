/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structures_bonus.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcastald <vcastald@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 11:50:57 by gpicchio          #+#    #+#             */
/*   Updated: 2025/05/27 09:19:19 by vcastald         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTURES_BONUS_H
# define STRUCTURES_BONUS_H

typedef struct s_data_bonus
{
	int		fd1;
	int		fd2;
	char	***cmds;
	char	**envp;
	char	**av;
	int		*fd;
	int		ac;
	int		cmd_num;
}	t_data_bonus;

#endif