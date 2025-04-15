/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pokemon.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpicchio <gpicchio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 14:44:12 by gpicchio          #+#    #+#             */
/*   Updated: 2025/03/26 16:40:02 by gpicchio         ###   ########.fr       */
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

void	print_healthbar(int health, char *name)
{
	int i;

	printf("\r%s: [", name);
	i = 0;
	while (i < health / 10)
	{
		printf("█");
		i++;
	}
	while (i < 10)
	{
		printf(".");
		i++;
	}
	printf("] %d%%  \n", health);
	fflush(stdout);
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
	fflush(stdout);

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

void	ft_pokemon()
{
	t_pokemon	*pikachu;
	t_pokemon	*bulbasaur;
	t_attack	*attacks;
	int			player_turn;

	player_turn = 1;
	initialize_pokemon(&pikachu, &bulbasaur, &attacks);
	printf("\nA wild %s appears!\n\n", bulbasaur->name);
	print_healthbar(bulbasaur->health, bulbasaur->name);
	print_healthbar(pikachu->health, pikachu->name);

	while (pikachu->health > 0 && bulbasaur->health > 0)
	{
		if (player_turn)
			player_attack(pikachu, bulbasaur, &player_turn);
		else
			cpu_attack(pikachu, bulbasaur, &player_turn);
		clear_window();
		print_healthbar(bulbasaur->health, bulbasaur->name);
		print_healthbar(pikachu->health, pikachu->name);
		ft_wait(500000000);
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
