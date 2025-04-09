/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pokemon.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpicchio <gpicchio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 14:44:12 by gpicchio          #+#    #+#             */
/*   Updated: 2025/04/08 16:29:17 by gpicchio         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_pokemon	*ft_create_pokemon(char *name, int health, int defense, int speed)
{
	t_pokemon	*pokemon;

	pokemon = (t_pokemon *)malloc(sizeof(t_pokemon));
	if (!pokemon)
		return (NULL);
	pokemon->name = name;
	pokemon->health = health;
	pokemon->defense = defense;
	pokemon->speed = speed;
	return (pokemon);
}

t_attack	create_attack(char *name, int damage, char *type)
{
	t_attack	attack;

	attack.name = name;
	attack.damage = damage;
	attack.type = type;
	return (attack);
}

void	add_attacks(t_pokemon *pokemon, t_attack *attacks)
{
	pokemon->attack1 = attacks[0];
	pokemon->attack2 = attacks[1];
	pokemon->attack3 = attacks[2];
	pokemon->attack4 = attacks[3];
}

void	print_healthbar(char *name, int health, int align_top)
{
	int	i;
	int	bar_length;
	
	bar_length = 20;
	if (align_top)
		printf("%-12s\n", name);
	else
		printf("\n%-12s\n", name);
	printf("HP: [");

	i = 0;
	while (i < (health * bar_length) / 100)
	{
		printf("█");
		i++;
	}
	while (i < bar_length)
	{
		printf(".");
		i++;
	}
	printf("] %3d%%\n", health);
}

void	clear_window(void)
{
	printf("\033[2J");
	printf("\033[H");
}

int pseudo_random()
{
    int fd = open("/dev/urandom", O_RDONLY);
    if (fd == -1)
        return 1;
    unsigned char num;
    read(fd, &num, 1);
    close(fd);
    return (num % 4) + 1;
}

void	player_attack(t_pokemon *pikachu, t_pokemon *bulbasaur, int *player_turn)
{
	char *line;
	printf("\nWhat will %s do?\n", pikachu->name);
	printf("1. %s\n", pikachu->attack1.name);
	printf("2. %s\n", pikachu->attack2.name);
	printf("3. %s\n", pikachu->attack3.name);
	printf("4. %s\n", pikachu->attack4.name);

	line = get_next_line(0);

	if (ft_strncmp(line, "1", 1) == 0)
	{
		printf("\n%s used %s!\n", pikachu->name, pikachu->attack1.name);
		bulbasaur->health -= pikachu->attack1.damage - bulbasaur->defense / 2;
	}
	else if (ft_strncmp(line, "2", 1) == 0)
	{
		printf("\n%s used %s!\n", pikachu->name, pikachu->attack2.name);
		bulbasaur->health -= pikachu->attack2.damage - bulbasaur->defense / 2;
	}
	else if (ft_strncmp(line, "3", 1) == 0)
	{
		printf("\n%s used %s!\n", pikachu->name, pikachu->attack3.name);
		bulbasaur->health -= pikachu->attack3.damage - bulbasaur->defense / 2;
	}
	else if (ft_strncmp(line, "4", 1) == 0)
	{
		printf("\n%s used %s!\n", pikachu->name, pikachu->attack4.name);
		bulbasaur->health -= pikachu->attack4.damage - bulbasaur->defense / 2;
	}
	else
	{
		printf("\nInvalid attack!\n");
		*player_turn = 1;
		free(line);
		return ;
	}
	if (bulbasaur->health <= 0)
		bulbasaur->health = 0;
	*player_turn = 0;
	free(line);
}

void	cpu_attack(t_pokemon *pikachu, t_pokemon *bulbasaur, int *player_turn)
{
	int attack_choice = pseudo_random();
	t_attack attack;

	if (attack_choice == 1)
		attack = bulbasaur->attack1;
	else if (attack_choice == 2)
		attack = bulbasaur->attack2;
	else if (attack_choice == 3)
		attack = bulbasaur->attack3;
	else
		attack = bulbasaur->attack4;
	printf("\n%s used %s!\n", bulbasaur->name, attack.name);
	pikachu->health -= attack.damage - pikachu->defense / 2;
	if (pikachu->health <= 0)
		pikachu->health = 0;
	*player_turn = 1;
}

void	initialize_pokemon(t_pokemon **pikachu, t_pokemon **bulbasaur, t_attack **attacks)
{
	*pikachu = ft_create_pokemon("Pikachu", 100, 50, 90);
	*attacks = (t_attack *)malloc(sizeof(t_attack) * 4);
	if (!*attacks)
		return ;
	(*attacks)[0] = create_attack("Thunderbolt", 90, "Electric");
	(*attacks)[1] = create_attack("Quick Attack", 40, "Normal");
	(*attacks)[2] = create_attack("Iron Tail", 100, "Steel");
	(*attacks)[3] = create_attack("Volt Tackle", 120, "Electric");
	add_attacks(*pikachu, *attacks);

	*bulbasaur = ft_create_pokemon("Bulbasaur", 100, 50, 45);
	(*attacks)[0] = create_attack("Vine Whip", 45, "Grass");
	(*attacks)[1] = create_attack("Tackle", 40, "Normal");
	(*attacks)[2] = create_attack("Razor Leaf", 55, "Grass");
	(*attacks)[3] = create_attack("Seed Bomb", 80, "Grass");
	add_attacks(*bulbasaur, *attacks);
}

void	ft_wait(long seconds)
{
	while (seconds > 0)
		seconds--;
}

void	print_ascii_art(const char *filepath)
{
	int		fd = open(filepath, O_RDONLY);
	if (fd == -1)
	{
		perror("Impossibile aprire il file dell'art ASCII");
		return;
	}

	char	buffer[1024];
	int		line_num = 0;
	ssize_t	bytes_read;
	size_t	i = 0;
	char	c;

	// Lettura carattere per carattere per contare le righe e stampare solo quelle desiderate
	while ((bytes_read = read(fd, &c, 1)) > 0)
	{
		if (c == '\n')
		{
			buffer[i] = '\0';

			// Stampa solo se NON è la prima riga (0) o l'ultima (gestita più sotto)
			if (line_num > 0)
				printf("%s\n", buffer);

			i = 0;
			line_num++;
		}
		else if (i < sizeof(buffer) - 1)
		{
			buffer[i++] = c;
		}
	}

	// Se il file non termina con '\n', gestisce l’ultima riga
	if (i > 0 && line_num > 0)
	{
		buffer[i] = '\0';
		// Non stampare l’ultima riga (tipicamente il "*/")
		// Quindi qui NON facciamo printf
	}

	if (bytes_read == -1)
		perror("Errore durante la lettura del file");

	close(fd);
}

void	display_battle(t_pokemon *player, t_pokemon *enemy)
{
	clear_window();

	printf("        Opponent\n");
	print_healthbar(enemy->name, enemy->health, 1);
	printf("\n");
	print_ascii_art("bulbasaur.c");
	printf("   %s (nemico)\n\n", enemy->name);
	printf("\n\n");
	print_ascii_art("pikachu.c");
	printf("   %s (tuo)\n", player->name);
	print_healthbar(player->name, player->health, 0);
}

void	ft_pokemon()
{
	t_pokemon	*pikachu;
	t_pokemon	*bulbasaur;
	t_attack	*attacks;
	int			player_turn;

	player_turn = 1;
	initialize_pokemon(&pikachu, &bulbasaur, &attacks);
	printf("\nA wild %s appears!\n\n", bulbasaur->name);
	display_battle(pikachu, bulbasaur);

	while (pikachu->health > 0 && bulbasaur->health > 0)
	{
		display_battle(pikachu, bulbasaur);
		if (player_turn)
			player_attack(pikachu, bulbasaur, &player_turn);
		else
			cpu_attack(pikachu, bulbasaur, &player_turn);
		ft_wait(800000000);
	}
	if (pikachu->health <= 0)
		printf("\n%s fainted!\n", pikachu->name);
	if (bulbasaur->health <= 0)
		printf("\n%s fainted!\n", bulbasaur->name);
	free(pikachu);
	free(bulbasaur);
	free(attacks);
	get_next_line(-42);
}
