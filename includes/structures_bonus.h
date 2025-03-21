/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structures_bonus.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpicchio <gpicchio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 11:50:57 by gpicchio          #+#    #+#             */
/*   Updated: 2025/02/27 11:38:17 by gpicchio         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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