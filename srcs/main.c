/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmittie <lmittie@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/19 20:52:05 by lmittie           #+#    #+#             */
/*   Updated: 2020/12/03 18:47:24 by lmittie          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "corewar.h"

void 	print_usage()
{
	ft_printf("Usage:  ./corewar "
		   "[-dump nbr_cycles] [[-n number] champion1.cor] ...\n"
	 		"-dump nbr_cycles : Dumps memory after N cycles then exits\n"
			"-n number        : sets the number of the next player\n");
}

void 	init(t_data *data)
{
	ft_bzero(data, sizeof(t_data));
	data->cycles_to_die = CYCLE_TO_DIE;
	data->carriage_list = NULL;

	// testing
	init_carriage(&data->carriage_list, 1, 0);

	data->players_num = 1;
	data->champs[0].header.prog_name[0] = 'a';
	data->champs[0].uid = 1;

	data->carriage_list->registers[0] = -6;
	data->carriage_list->registers[1] = 6;
	data->carriage_list->registers[3] = 0;
	data->carriage_list->args[0] = T_REG;
	data->carriage_list->args[1] = T_DIR;
	data->carriage_list->args[2] = T_DIR;
	data->carriage_list->curr_pos = 0;
}

int 	main(int ac, char **av)
{
	t_data	data;

	init(&data);

	data.arena[1] = 0x00;
	data.arena[2] = 0x04;

	fork_(&data, &data.carriage_list, 1);

//	print_arena_state(&data.arena);

	//	if (ac == 1)
//		print_usage();
//	else
//	{
//		init(&data);
//		parse_arguments(ac, (const char**)av, &data);
//		game(&data);
//	}
	return (0);
}