/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operations.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmittie <lmittie@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/27 18:05:33 by lmittie           #+#    #+#             */
/*   Updated: 2020/11/27 21:04:23 by lmittie          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "corewar.h"

extern t_op op_tab[17];

uint32_t	get_arg(size_t size, const uint8_t (*arena)[MEM_SIZE], size_t pos)
{
	size_t i;
	uint32_t arg;

	i = 0;
	arg = 0;
	while (i < size)
	{
		arg <<= (uint32_t)8;
		arg |= (*arena)[(pos + i) % MEM_SIZE];
		i++;
	}
//	ft_printf("\n");
	return (arg);
}

void		live(t_data *data, t_carriage **carriage, size_t pos)
{
	uint32_t	r;

	r = get_arg(DIR_SIZE, &data->arena, pos);
	ft_printf("r = %d, pos = %d\n", r, pos);
	if (r <= data->players_num && r > 0)
	{
		data->winner_id = r;
		ft_printf("A process shows that %u (%s) is alive\n",
				  r,
				  data->champs[r - 1].header.prog_name);
	}
	(*carriage)->last_live_cycle = data->cycles;
	data->live_op_counter++;
}

void	ld(t_data *data, t_carriage **carriage, size_t pos)
{
	uint32_t	arg;
	uint32_t	shift;
	uint16_t	r_inx;

	if ((*carriage)->args[0] == T_DIR)
	{
		arg = get_arg(DIR_SIZE, &data->arena, pos);
		r_inx = data->arena[(pos + DIR_SIZE) % MEM_SIZE];
	}
	else if ((*carriage)->args[0] == T_IND)
	{
		arg = get_arg(IND_SIZE, &data->arena, pos);
		shift = (*carriage)->curr_pos + arg % IDX_MOD;
		arg = get_arg(DIR_SIZE, &data->arena, shift);
		r_inx = data->arena[(pos + IND_SIZE) % MEM_SIZE];
	}
	(*carriage)->registers[r_inx - 1] = arg;
	(*carriage)->carry = (arg == 0) ? 1 : 0;
}

void	st(t_data *data, t_carriage **carriage, size_t pos)
{
	uint16_t r_inx1;
	uint32_t r_inx2;
	uint32_t shift;

	r_inx1 = data->arena[pos % MEM_SIZE];
	if ((*carriage)->args[1] == T_REG)
	{
		r_inx2 = data->arena[(pos + 1) % MEM_SIZE];
		(*carriage)->registers[r_inx2 - 1] = (*carriage)->registers[r_inx1 - 1];
	}
	else if ((*carriage)->args[1] == T_IND)
	{
		r_inx2 = get_arg(IND_SIZE, &data->arena, pos + 1);
		shift = (*carriage)->curr_pos + r_inx2 % IDX_MOD;
		data->arena[shift % MEM_SIZE] = (*carriage)->registers[r_inx1 - 1];
	}
}

void	add(t_data *data, t_carriage **carriage, size_t pos)
{
	uint16_t r_inx1;
	uint16_t r_inx2;
	uint16_t r_inx3;

	r_inx1 = data->arena[pos++ % MEM_SIZE];
	r_inx2 = data->arena[pos++ % MEM_SIZE];
	r_inx3 = data->arena[pos % MEM_SIZE];
	(*carriage)->registers[r_inx3 - 1] = (*carriage)->registers[r_inx1 - 1]
									+ (*carriage)->registers[r_inx2 - 1];
	(*carriage)->carry = ((*carriage)->registers[r_inx3 - 1]) ? 0 : 1;
}

void	sub(t_data *data, t_carriage **carriage, size_t pos)
{
	uint16_t r_inx1;
	uint16_t r_inx2;
	uint16_t r_inx3;

	r_inx1 = data->arena[pos++ % MEM_SIZE];
	r_inx2 = data->arena[pos++ % MEM_SIZE];
	r_inx3 = data->arena[pos % MEM_SIZE];
	(*carriage)->registers[r_inx3 - 1] = (*carriage)->registers[r_inx1 - 1]
									 - (*carriage)->registers[r_inx2 - 1];
	(*carriage)->carry = ((*carriage)->registers[r_inx3 - 1]) ? 0 : 1;
}


// TODO rewrite
void	and(t_data *data, t_carriage **carriage, size_t pos)
{
	uint32_t	arg1;
	uint32_t	arg2;
	uint16_t	r_inx3;
	size_t		shift;

	if ((*carriage)->args[0] == T_REG && (shift = T_REG))
		arg1 = (*carriage)->registers[data->arena[pos % MEM_SIZE] - 1];
	else if ((*carriage)->args[0] == T_DIR && (shift = DIR_SIZE))
		arg1 = get_arg(DIR_SIZE, &data->arena, pos);
	else if ((*carriage)->args[0] == T_IND && (shift = IND_SIZE))
		arg1 = get_arg(DIR_SIZE, &data->arena,
					   (*carriage)->curr_pos + get_arg(IND_SIZE, &data->arena, pos));
	if ((*carriage)->args[1] == T_REG && (shift += T_REG))
		arg2 = (*carriage)->registers[data->arena[(pos + shift - T_REG) % MEM_SIZE] - 1];
	else if ((*carriage)->args[1] == T_DIR && (shift += DIR_SIZE))
		arg2 = get_arg(DIR_SIZE, &data->arena, pos + shift - DIR_SIZE);
	else if ((*carriage)->args[1] == T_IND && (shift += IND_SIZE))
		arg2 = get_arg(DIR_SIZE, &data->arena,
				 (*carriage)->curr_pos + get_arg(IND_SIZE, &data->arena, pos + shift - IND_SIZE));
	r_inx3 = data->arena[(pos + shift) % MEM_SIZE];
	(*carriage)->registers[r_inx3 - 1] = arg1 & arg2;
	(*carriage)->carry = ((*carriage)->registers[r_inx3 - 1]) ? 0 : 1;
}

void	or(t_data *data, t_carriage **carriage, size_t pos)
{
	uint32_t	arg1;
	uint32_t	arg2;
	uint16_t	r_inx3;
	size_t		shift;

	if ((*carriage)->args[0] == T_REG && (shift = T_REG))
		arg1 = (*carriage)->registers[data->arena[pos % MEM_SIZE] - 1];
	else if ((*carriage)->args[0] == T_DIR && (shift = DIR_SIZE))
		arg1 = get_arg(DIR_SIZE, &data->arena, pos);
	else if ((*carriage)->args[0] == T_IND && (shift = IND_SIZE))
		arg1 = get_arg(DIR_SIZE, &data->arena,
					   (*carriage)->curr_pos + get_arg(IND_SIZE, &data->arena, pos));
	if ((*carriage)->args[1] == T_REG && (shift += T_REG))
		arg2 = (*carriage)->registers[data->arena[(pos + shift - T_REG) % MEM_SIZE] - 1];
	else if ((*carriage)->args[1] == T_DIR && (shift += DIR_SIZE))
		arg2 = get_arg(DIR_SIZE, &data->arena, pos + shift - DIR_SIZE);
	else if ((*carriage)->args[1] == T_IND && (shift += IND_SIZE))
		arg2 = get_arg(DIR_SIZE, &data->arena,
					   (*carriage)->curr_pos + get_arg(IND_SIZE, &data->arena, pos + shift - IND_SIZE));
	r_inx3 = data->arena[(pos + shift) % MEM_SIZE];
	(*carriage)->registers[r_inx3 - 1] = arg1 | arg2;
	(*carriage)->carry = ((*carriage)->registers[r_inx3 - 1]) ? 0 : 1;
}

void	xor(t_data *data, t_carriage **carriage, size_t pos)
{
	uint32_t	arg1;
	uint32_t	arg2;
	uint16_t	r_inx3;
	size_t		shift;

	if ((*carriage)->args[0] == T_REG && (shift = T_REG))
		arg1 = (*carriage)->registers[data->arena[pos % MEM_SIZE] - 1];
	else if ((*carriage)->args[0] == T_DIR && (shift = DIR_SIZE))
		arg1 = get_arg(DIR_SIZE, &data->arena, pos);
	else if ((*carriage)->args[0] == T_IND && (shift = IND_SIZE))
		arg1 = get_arg(DIR_SIZE, &data->arena,
					   (*carriage)->curr_pos + get_arg(IND_SIZE, &data->arena, pos));
	if ((*carriage)->args[1] == T_REG && (shift += T_REG))
		arg2 = (*carriage)->registers[data->arena[(pos + shift - T_REG) % MEM_SIZE] - 1];
	else if ((*carriage)->args[1] == T_DIR && (shift += DIR_SIZE))
		arg2 = get_arg(DIR_SIZE, &data->arena, pos + shift - DIR_SIZE);
	else if ((*carriage)->args[1] == T_IND && (shift += IND_SIZE))
		arg2 = get_arg(DIR_SIZE, &data->arena,
					   (*carriage)->curr_pos + get_arg(IND_SIZE, &data->arena, pos + shift - IND_SIZE));
	r_inx3 = data->arena[(pos + shift) % MEM_SIZE];
	(*carriage)->registers[r_inx3 - 1] = arg1 ^ arg2;
	(*carriage)->carry = ((*carriage)->registers[r_inx3 - 1]) ? 0 : 1;
}

void	zjmp(t_data *data, t_carriage **carriage, size_t pos)
{
	uint32_t arg;

	arg = get_arg(IND_SIZE, &data->arena, pos);
	if ((*carriage)->carry)
		(*carriage)->curr_pos += arg % IDX_MOD;
}

void	ldi(t_data *data, t_carriage **carriage, size_t pos)
{
	uint32_t	arg1;
	uint32_t	arg2;
	uint16_t	r_inx3;
	size_t		shift;

	if ((*carriage)->args[0] == T_REG && (shift = T_REG))
		arg1 = (*carriage)->registers[data->arena[pos % MEM_SIZE] - 1];
	else if ((*carriage)->args[0] == T_DIR && (shift = DIR_SIZE))
		arg1 = get_arg(DIR_SIZE, &data->arena, pos);
	else if ((*carriage)->args[0] == T_IND && (shift = IND_SIZE))
		arg1 = get_arg(DIR_SIZE, &data->arena,
					   (*carriage)->curr_pos + get_arg(IND_SIZE, &data->arena, pos));
	if ((*carriage)->args[1] == T_REG && (shift += T_REG))
		arg2 = (*carriage)->registers[data->arena[(pos + shift - T_REG) % MEM_SIZE] - 1];
	else if ((*carriage)->args[1] == T_DIR && (shift += DIR_SIZE))
		arg2 = get_arg(DIR_SIZE, &data->arena, pos + shift - DIR_SIZE);
	r_inx3 = data->arena[(pos + shift) % MEM_SIZE];
	(*carriage)->registers[r_inx3 - 1] = get_arg(
			DIR_SIZE,
			&data->arena,
			(*carriage)->curr_pos + (arg1 + arg2) % IDX_MOD);
}

void	sti(t_data *data, t_carriage **carriage, size_t pos)
{
	uint32_t	arg1;
	uint32_t	arg2;
	uint16_t	arg3;
	size_t		shift;

	arg1 = (*carriage)->registers[data->arena[pos % MEM_SIZE] - 1];
	shift = 1;
	if ((*carriage)->args[1] == T_REG && (shift += T_REG))
		arg2 = (*carriage)->registers[data->arena[pos % MEM_SIZE] - 1];
	else if ((*carriage)->args[1] == T_DIR && (shift += IND_SIZE))
		arg2 = get_arg(IND_SIZE, &data->arena, pos);
	else if ((*carriage)->args[1] == T_IND && (shift += IND_SIZE))
		arg2 = get_arg(IND_SIZE, &data->arena,
					   (*carriage)->curr_pos + get_arg(IND_SIZE, &data->arena, pos));
	if ((*carriage)->args[2] == T_REG )
		arg3 = (*carriage)->registers[data->arena[(pos + shift) % MEM_SIZE] - 1];
	else if ((*carriage)->args[2] == T_DIR)
		arg3 = get_arg(IND_SIZE, &data->arena, pos + shift);
	pos = (*carriage)->curr_pos + (arg2 + arg3) % IDX_MOD;
	int i = 4;
	while (i--)
	{
		data->arena[(pos + i) % MEM_SIZE] = arg1 & (uint32_t)3;
		arg1 >>= (uint32_t)8;
	}
}

void	fork_(t_data *data, t_carriage **carriage, size_t pos)
{
	uint32_t arg;

	arg = get_arg(IND_SIZE, &data->arena, pos);
	init_carriage(
			&data->carriage_list,
			data->carriage_list->uid + 1,
			(*carriage)->curr_pos + arg % IDX_MOD);
}

void	lld(t_data *data, t_carriage **carriage, size_t pos)
{
	uint32_t	arg;
	uint32_t	shift;
	uint16_t	r_inx;

	if ((*carriage)->args[0] == T_DIR)
	{
		arg = get_arg(DIR_SIZE, &data->arena, pos);
		r_inx = data->arena[(pos + DIR_SIZE) % MEM_SIZE];
	}
	else if ((*carriage)->args[0] == T_IND)
	{
		arg = get_arg(IND_SIZE, &data->arena, pos);
		shift = (*carriage)->curr_pos + arg;
		arg = get_arg(DIR_SIZE, &data->arena, shift);
		r_inx = data->arena[(pos + IND_SIZE) % MEM_SIZE];
	}
	(*carriage)->registers[r_inx - 1] = arg;
	(*carriage)->carry = (arg == 0) ? 1 : 0;
}

void	lldi(t_data *data, t_carriage **carriage, size_t pos)
{
	uint32_t	arg1;
	uint32_t	arg2;
	uint16_t	r_inx3;
	size_t		shift;

	if ((*carriage)->args[0] == T_REG && (shift = T_REG))
		arg1 = (*carriage)->registers[data->arena[pos % MEM_SIZE] - 1];
	else if ((*carriage)->args[0] == T_DIR && (shift = DIR_SIZE))
		arg1 = get_arg(DIR_SIZE, &data->arena, pos);
	else if ((*carriage)->args[0] == T_IND && (shift = IND_SIZE))
		arg1 = get_arg(DIR_SIZE, &data->arena,
					   (*carriage)->curr_pos + get_arg(IND_SIZE, &data->arena, pos));
	if ((*carriage)->args[1] == T_REG && (shift += T_REG))
		arg2 = (*carriage)->registers[data->arena[(pos + shift - T_REG) % MEM_SIZE] - 1];
	else if ((*carriage)->args[1] == T_DIR && (shift += DIR_SIZE))
		arg2 = get_arg(DIR_SIZE, &data->arena, pos + shift - DIR_SIZE);
	r_inx3 = data->arena[(pos + shift) % MEM_SIZE];
	(*carriage)->registers[r_inx3 - 1] = get_arg(
			DIR_SIZE,
			&data->arena,
			(*carriage)->curr_pos + (arg1 + arg2));
}

void	lfork(t_data *data, t_carriage **carriage, size_t pos)
{
	uint32_t arg;

	arg = get_arg(IND_SIZE, &data->arena, pos);
	init_carriage(
			&data->carriage_list,
			data->carriage_list->uid + 1,
			(*carriage)->curr_pos + arg);
}

void	aff(t_data *data, t_carriage **carriage, size_t pos)
{
	uint16_t r;

	r = data->arena[pos];
	if (data->a_flag)
		ft_printf("%c\n", (char)((*carriage)->registers[r - 1] % 256));
}
