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
	t_pokemon *pokemon;

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

void	player_attack(t_pokemon *player_pokemon, t_pokemon *cpu_pokemon, int *player_turn)
{
	char *line;
	printf("\nWhat will %s do?\n", player_pokemon->name);
	printf("1. %s\n", player_pokemon->attack1.name);
	printf("2. %s\n", player_pokemon->attack2.name);
	printf("3. %s\n", player_pokemon->attack3.name);
	printf("4. %s\n", player_pokemon->attack4.name);

	printf("\nYour Pokemon: %d\n", player_pokemon->id);
	printf("Opponent Pokemon: %d\n", cpu_pokemon->id);

	line = get_next_line(0);

	if (ft_strncmp(line, "1", 1) == 0)
	{
		printf("\n%s used %s!\n", player_pokemon->name, player_pokemon->attack1.name);
		cpu_pokemon->health -= player_pokemon->attack1.damage - cpu_pokemon->defense / 2;
	}
	else if (ft_strncmp(line, "2", 1) == 0)
	{
		printf("\n%s used %s!\n", player_pokemon->name, player_pokemon->attack2.name);
		cpu_pokemon->health -= player_pokemon->attack2.damage - cpu_pokemon->defense / 2;
	}
	else if (ft_strncmp(line, "3", 1) == 0)
	{
		printf("\n%s used %s!\n", player_pokemon->name, player_pokemon->attack3.name);
		cpu_pokemon->health -= player_pokemon->attack3.damage - cpu_pokemon->defense / 2;
	}
	else if (ft_strncmp(line, "4", 1) == 0)
	{
		printf("\n%s used %s!\n", player_pokemon->name, player_pokemon->attack4.name);
		cpu_pokemon->health -= player_pokemon->attack4.damage - cpu_pokemon->defense / 2;
	}
	else
	{
		printf("\nInvalid attack!\n");
		*player_turn = 1;
		free(line);
		return;
	}
	if (cpu_pokemon->health <= 0)
		cpu_pokemon->health = 0;
	*player_turn = 0;
	free(line);
}

void	cpu_attack(t_pokemon *player_pokemon, t_pokemon *cpu_pokemon, int *player_turn)
{
	int attack_choice = pseudo_random();
	t_attack attack;

	if (attack_choice == 1)
		attack = cpu_pokemon->attack1;
	else if (attack_choice == 2)
		attack = cpu_pokemon->attack2;
	else if (attack_choice == 3)
		attack = cpu_pokemon->attack3;
	else
		attack = cpu_pokemon->attack4;
	printf("\n%s used %s!\n", cpu_pokemon->name, attack.name);
	player_pokemon->health -= attack.damage - player_pokemon->defense / 2;
	if (player_pokemon->health <= 0)
		player_pokemon->health = 0;
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
	ssize_t	bytes_read;
	while ((bytes_read = read(fd, buffer, sizeof(buffer) - 1)) > 0)
	{
		buffer[bytes_read] = '\0';
		printf("%s", buffer);
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
	print_ascii_art(enemy->ascii_path);
	printf("   %s (nemico)\n\n", enemy->name);
	printf("\n\n");
	print_ascii_art(player->ascii_path);
	printf("   %s (tuo)\n", player->name);
	print_healthbar(player->name, player->health, 0);
}

char *choose_pokemon(char *prompt, t_pokemon *pokemon)
{
	char *input = NULL;
	int	file;
	char base_path[] = "pokemonData/pokemonImages/";
	char filepath[256];

	while (1)
	{
		ft_putendl_fd(prompt, 1);
		input = get_next_line(0);

		if (!input)
		{
			ft_putendl_fd("Error reading input.", 1);
			continue;
		}
		size_t len = strlen(input);
		if (len > 0 && input[len - 1] == '\n')
			input[len - 1] = '\0';
		ft_strlcpy(filepath, base_path, sizeof(filepath));
		ft_strlcat(filepath, input, sizeof(filepath));

		file = open(filepath, O_RDONLY);
		if (file != -1)
		{
			close(file);
			break;
		}
		else
		{
			ft_putendl_fd("Pokemon not found. Please try again.", 1);
			free(input);
		}
	}
	pokemon->ascii_path = ft_strdup(filepath);
	pokemon->name = ft_strdup(input);
	free(input);
	return ft_strdup(filepath);
}

char **split_pokemon(char *str, char sep)
{
    char **res;
    int i = 0;
    int start = 0;
    int j = 0;
    int count = 1;

    // Conta i separatori
    while (str[i])
        if (str[i++] == sep)
            count++;
    
    res = malloc(sizeof(char *) * (count + 1));
    if (!res)
        return NULL;

    i = 0;
    while (str[i])
    {
        if (str[i] == sep || str[i+1] == '\0')
        {
            int len = i - start + (str[i+1] == '\0' ? 1 : 0);
            res[j] = malloc(len + 1);
            strncpy(res[j], &str[start], len);
            res[j][len] = '\0';
            j++;
            start = i + 1;
        }
        i++;
    }
    res[j] = NULL;
    return res;
}

void get_pokemon_data(t_pokemon *pokemon)
{
    int fd = open("pokemonData/pokemonData/pokemon.csv", O_RDONLY);
    if (fd == -1)
    {
        perror("Impossibile aprire il file");
        return;
    }
    char *line;
    while ((line = get_next_line(fd)))
    {
        char **fields = split_pokemon(line, ',');
        if (fields && fields[1] && !ft_strncmp(fields[1], pokemon->name, ft_strlen(pokemon->name)))
        {
            pokemon->id = ft_atoi(fields[0]);
            free_matrix(fields);
            free(line);
            close(fd);
            return;
        }
        free_matrix(fields);
        free(line);
    }
    printf("Pokemon con nome %s non trovato\n", pokemon->name);
    close(fd);
}

void	ft_pokemon()
{
	t_pokemon	*player_pokemon;
	t_pokemon	*cpu_pokemon;
	t_attack	*attacks;
	int			player_turn;

	char player_choice[50];
	char enemy_choice[50];
	
	

	player_turn = 1;
	attacks = (t_attack *)malloc(sizeof(t_attack) * 4);
	attacks[0] = create_attack("Thunderbolt", 90, "Electric");
	attacks[1] = create_attack("Quick Attack", 40, "Normal");
	attacks[2] = create_attack("Iron Tail", 100, "Steel");
	attacks[3] = create_attack("Volt Tackle", 120, "Electric");
	player_pokemon = ft_create_pokemon(player_choice, 100, 50, 90);
	add_attacks(player_pokemon, attacks);

	attacks[0] = create_attack("Vine Whip", 45, "Grass");
	attacks[1] = create_attack("Tackle", 40, "Normal");
	attacks[2] = create_attack("Razor Leaf", 55, "Grass");
	attacks[3] = create_attack("Seed Bomb", 80, "Grass");
	cpu_pokemon = ft_create_pokemon(enemy_choice, 100, 50, 45);
	add_attacks(cpu_pokemon, attacks);
	char *player_path = choose_pokemon("Choose your Pokemon: ", player_pokemon);
	char *enemy_path = choose_pokemon("Choose opponent's Pokemon: ", cpu_pokemon);

	get_pokemon_data(player_pokemon);
	get_pokemon_data(cpu_pokemon);

	printf("\nA wild %s appears!\n\n", cpu_pokemon->name);
	display_battle(player_pokemon, cpu_pokemon);

	while (player_pokemon->health > 0 && cpu_pokemon->health > 0)
	{
		display_battle(player_pokemon, cpu_pokemon);
		if (player_turn)
			player_attack(player_pokemon, cpu_pokemon, &player_turn);
		else
			cpu_attack(player_pokemon, cpu_pokemon, &player_turn);
		ft_wait(800000000);
	}
	if (player_pokemon->health <= 0)
		printf("\n%s fainted!\n", player_pokemon->name);
	if (cpu_pokemon->health <= 0)
		printf("\n%s fainted!\n", cpu_pokemon->name);
	free(player_pokemon->ascii_path);
	free(cpu_pokemon->ascii_path);
	free(player_pokemon->name);
	free(cpu_pokemon->name);
	free(player_pokemon);
	free(cpu_pokemon);
	free(attacks);
	free(player_path);
    free(enemy_path);
	get_next_line(-42);
}
