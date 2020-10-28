/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmittie <lmittie@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/25 19:45:53 by lmittie           #+#    #+#             */
/*   Updated: 2020/10/27 18:08:48 by lmittie          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "corewar.h"

extern t_op    op_tab[17];

void	print_arena_state(uint8_t (*arena)[MEM_SIZE])
{

}

void	set_op_code(t_carriage **carriage, const uint8_t (*arena)[MEM_SIZE])
{
	// valid op -> set op && cycles_before set, else read && cycles_before = 0
	(*carriage)->op_code = (*arena)[(*carriage)->curr_pos % MEM_SIZE];
	if ((*carriage)->op_code > 0 && (*carriage)->op_code < 17)
		(*carriage)->cycles_before = op_tab[(*carriage)->op_code - 1].cycles;
}

int		validate_op(t_carriage **carriage, const uint8_t (*arena)[MEM_SIZE])
{
	if ((*carriage)->op_code > 16)
	{
		(*carriage)->curr_pos++;
		return (0);
	}
	return (1);
}

uint8_t	one_argument_type(uint8_t arg_type, uint8_t shift)
{
	arg_type >>= shift;
	arg_type &= (uint8_t)3;
	if (!(arg_type ^ (uint8_t)1))
		return (T_REG);
	if (!(arg_type ^ (uint8_t)2))
		return (T_DIR);
	if (!(arg_type ^ (uint8_t)3))
		return (T_IND);
	return (0);
}

int		validate_args(t_carriage **carriage, const uint8_t (*arena)[MEM_SIZE])
{
	int			i;
	uint8_t		arg_type;
	uint8_t		wrong_args;

	(*carriage)->bytes_step = 1; // escape code
	wrong_args = 0;
	if (op_tab[(*carriage)->op_code - 1].arg_type_code)
	{
		i = 0;
		(*carriage)->bytes_step++; // escape arg type code
		arg_type = (*arena)[((*carriage)->curr_pos + 1) % MEM_SIZE];
		while (i < op_tab[(*carriage)->op_code - 1].args_num) // go forward args
		{
			(*carriage)->args[i] = one_argument_type(arg_type, 6 - i * 2);
			if (!((*carriage)->args[i] & op_tab[(*carriage)->op_code - 1].args_type[i]))
				wrong_args = 1;
			if ((*carriage)->args[i] == T_REG
			&& ((*arena)[(*carriage)->curr_pos + (*carriage)->bytes_step] == 0
			|| (*arena)[(*carriage)->curr_pos + (*carriage)->bytes_step] > REG_NUMBER))
				wrong_args = 1;
			if ((*carriage)->args[i] == T_REG)
				(*carriage)->bytes_step += T_REG;
			else if ((*carriage)->args[i] == T_DIR)
				(*carriage)->bytes_step += DIR_SIZE; // or 2
			else if ((*carriage)->args[i] == T_IND)
				(*carriage)->bytes_step += IND_SIZE;
		}
	}
	if (wrong_args)
	{
		(*carriage)->curr_pos += (*carriage)->bytes_step;
		return (0);
	}
	return (1);
}

void	exec_op(t_carriage **carriage, const uint8_t (*arena)[MEM_SIZE])
{
	size_t pos;

	pos = (*carriage)->curr_pos;
	pos += (op_tab[(*carriage)->op_code - 1].arg_type_code) ? 2 : 1;
	// execute
}

void	validate_and_exec(t_carriage **carriage, const uint8_t (*arena)[MEM_SIZE])
{
	if (!validate_op(carriage, arena))
		return ;
	if (!validate_args(carriage, arena))
		return ;
	exec_op(carriage, arena);
}

void	carriage_check(t_carriage **clist, const uint8_t (*arena)[MEM_SIZE])
{
	t_carriage *it;

	it = *clist;
	while (it)
	{
		if (it->cycles_before == 0)
			set_op_code(&it, arena);
		if (it->cycles_before > 0)
			it->cycles_before--;
		if (it->cycles_before == 0)
			validate_and_exec(&it, arena);
		it = it->next;
	}
}

void	game(t_data *data)
{
	data->winner_id = data->champs[data->players_num - 1].uid;
	while (1)
	{
		data->cycles++;
		if (data->cycles == data->dump_cycles)
		{
			print_arena_state(&data->arena);
			return ;
		}
		carriage_check(&data->carriage_list, &data->arena);
		if (data->cycles % data->cycles_to_die == 0)
		{
			ctd_check(data);
			data->live_op_counter = 0;
		}
	}
}
