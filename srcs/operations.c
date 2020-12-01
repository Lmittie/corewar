/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operations.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmittie <lmittie@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/27 18:05:33 by lmittie           #+#    #+#             */
/*   Updated: 2020/12/01 19:36:34 by lmittie          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "corewar.h"

extern t_op op_tab[17];

int32_t	get_pos(int32_t pos)
{
	if (pos < 0)
		return (pos % MEM_SIZE + MEM_SIZE);
	return (pos % MEM_SIZE);
}

int32_t	get_value(size_t size, const uint8_t (*arena)[MEM_SIZE], int32_t pos)
{
	size_t	i;
	int32_t	arg;

	i = 0;
	arg = 0;
	while (i < size)
	{
		arg <<= (int32_t)8;
		arg |= (*arena)[get_pos(pos + i)];
		i++;
	}
	if (size == 2 && arg & 0x8000)
		arg |= 0xFFFF0000;
	return ((int)arg);
}

int32_t		get_arg(uint8_t arg_type,  int32_t *pos, const uint8_t (*arena)[MEM_SIZE])
{
	int32_t arg;

	arg = 0;
	if (arg_type == T_DIR)
	{
		arg = get_value(DIR_SIZE, arena, *pos);
		*pos += DIR_SIZE;
	}
	else if (arg_type == T_IND)
	{
		arg = get_value(IND_SIZE, arena, *pos);
		*pos += IND_SIZE;
	}
	else if (arg_type == T_REG)
	{
		arg = (*arena)[get_pos(*pos)];
		(*pos)++;
	}
	return (arg);
}

void		place_value(int32_t arg, int32_t pos, size_t size, uint8_t (*arena)[MEM_SIZE])
{
	while (size--)
	{
		(*arena)[get_pos(pos + size)] = arg & 0xFF;
		arg >>= (uint32_t)8;
	}
}

void		live(t_data *data, t_carriage **carriage, int32_t pos)
{
	int32_t	r;

	r = get_value(DIR_SIZE, &data->arena, pos);
	if (r >= -data->players_num && r < 0)
	{
		data->winner_id = r * -1;
		printf("A process shows that %d (%s) is alive\n",
				  r * -1,
				  data->champs[-r - 1].header.prog_name);
	}
	(*carriage)->last_live_cycle = data->cycles;
	data->live_op_counter++;
}

void	ld(t_data *data, t_carriage **carriage, int32_t pos)
{
	int32_t	arg;
	int16_t	r_inx;

	arg = get_arg((*carriage)->args[0], &pos, &data->arena);
	r_inx = data->arena[get_pos(pos)];
	if ((*carriage)->args[0] == T_IND)
		arg = get_value(DIR_SIZE, &data->arena,
				  (*carriage)->curr_pos + arg % IDX_MOD);
	(*carriage)->registers[r_inx - 1] = arg;
	(*carriage)->carry = (arg == 0) ? 1 : 0;
}

void	st(t_data *data, t_carriage **carriage, int32_t pos)
{
	int16_t	r_inx1;
	int32_t	r_inx2;

	r_inx1 = data->arena[get_pos(pos++)];
	r_inx2 = get_arg((*carriage)->args[1], &pos, &data->arena);
	if ((*carriage)->args[1] == T_REG)
		(*carriage)->registers[r_inx2 - 1] = (*carriage)->registers[r_inx1 - 1];
	else if ((*carriage)->args[1] == T_IND)
		place_value((*carriage)->registers[r_inx1 - 1],
					(*carriage)->curr_pos + r_inx2 % IDX_MOD,
					REG_SIZE,
					&data->arena);
}

void	add(t_data *data, t_carriage **carriage, int32_t pos)
{
	uint16_t r_inx1;
	uint16_t r_inx2;
	uint16_t r_inx3;

	r_inx1 = data->arena[get_pos(pos++)];
	r_inx2 = data->arena[get_pos(pos++)];
	r_inx3 = data->arena[get_pos(pos)];
	(*carriage)->registers[r_inx3 - 1] = (*carriage)->registers[r_inx1 - 1]
									+ (*carriage)->registers[r_inx2 - 1];
	(*carriage)->carry = ((*carriage)->registers[r_inx3 - 1]) ? 0 : 1;
}

void	sub(t_data *data, t_carriage **carriage, int32_t pos)
{
	uint16_t r_inx1;
	uint16_t r_inx2;
	uint16_t r_inx3;

	r_inx1 = data->arena[get_pos(pos++)];
	r_inx2 = data->arena[get_pos(pos++)];
	r_inx3 = data->arena[get_pos(pos)];
	(*carriage)->registers[r_inx3 - 1] = (*carriage)->registers[r_inx1 - 1]
									 - (*carriage)->registers[r_inx2 - 1];
	(*carriage)->carry = ((*carriage)->registers[r_inx3 - 1]) ? 0 : 1;
}


void	and(t_data *data, t_carriage **carriage, int32_t pos)
{
	int32_t	arg1;
	int32_t	arg2;
	int16_t	r_inx3;

	arg1 = get_arg((*carriage)->args[0], &pos, &data->arena);
	if ((*carriage)->args[0] == T_REG)
		arg1 = (*carriage)->registers[arg1 - 1];
	else if ((*carriage)->args[0] == T_IND)
		arg1 = get_value(DIR_SIZE, &data->arena,
						 (*carriage)->curr_pos + arg1);
	arg2 = get_arg((*carriage)->args[1], &pos, &data->arena);
	if ((*carriage)->args[1] == T_REG)
		arg2 = (*carriage)->registers[arg2 - 1];
	else if ((*carriage)->args[1] == T_IND)
		arg2 = get_value(DIR_SIZE, &data->arena,
						 (*carriage)->curr_pos + arg2);
	r_inx3 = data->arena[get_pos(pos)];
	(*carriage)->registers[r_inx3 - 1] = arg1 & arg2;
	(*carriage)->carry = ((*carriage)->registers[r_inx3 - 1]) ? 0 : 1;
}

void	or(t_data *data, t_carriage **carriage, int32_t pos)
{
	int32_t	arg1;
	int32_t	arg2;
	int16_t	r_inx3;

	arg1 = get_arg((*carriage)->args[0], &pos, &data->arena);
	if ((*carriage)->args[0] == T_REG)
		arg1 = (*carriage)->registers[arg1 - 1];
	else if ((*carriage)->args[0] == T_IND)
		arg1 = get_value(DIR_SIZE, &data->arena,
						 (*carriage)->curr_pos + arg1);
	arg2 = get_arg((*carriage)->args[1], &pos, &data->arena);
	if ((*carriage)->args[1] == T_REG)
		arg2 = (*carriage)->registers[arg2 - 1];
	else if ((*carriage)->args[1] == T_IND)
		arg2 = get_value(DIR_SIZE, &data->arena,
						 (*carriage)->curr_pos + arg2);
	r_inx3 = data->arena[get_pos(pos)];
	(*carriage)->registers[r_inx3 - 1] = arg1 | arg2;
	(*carriage)->carry = ((*carriage)->registers[r_inx3 - 1]) ? 0 : 1;
}

void	xor(t_data *data, t_carriage **carriage, int32_t pos)
{
	int32_t	arg1;
	int32_t	arg2;
	int16_t	r_inx3;

	arg1 = get_arg((*carriage)->args[0], &pos, &data->arena);
	if ((*carriage)->args[0] == T_REG)
		arg1 = (*carriage)->registers[arg1 - 1];
	else if ((*carriage)->args[0] == T_IND)
		arg1 = get_value(DIR_SIZE, &data->arena,
						 (*carriage)->curr_pos + arg1);
	arg2 = get_arg((*carriage)->args[1], &pos, &data->arena);
	if ((*carriage)->args[1] == T_REG)
		arg2 = (*carriage)->registers[arg2 - 1];
	else if ((*carriage)->args[1] == T_IND)
		arg2 = get_value(DIR_SIZE, &data->arena,
						 (*carriage)->curr_pos + arg2);
	r_inx3 = data->arena[get_pos(pos)];
	(*carriage)->registers[r_inx3 - 1] = arg1 ^ arg2;
	(*carriage)->carry = ((*carriage)->registers[r_inx3 - 1]) ? 0 : 1;
}

void	zjmp(t_data *data, t_carriage **carriage, int32_t pos)
{
	int32_t arg;

	arg = get_value(IND_SIZE, &data->arena, pos);
	ft_printf(" |jump = %d, carry = %d| \n", arg, (*carriage)->carry);
	if ((*carriage)->carry)
		(*carriage)->curr_pos += arg % IDX_MOD;
}

void	ldi(t_data *data, t_carriage **carriage, int32_t pos)
{
	int32_t	arg1;
	int32_t	arg2;
	int16_t	r_inx3;
	uint8_t type;

	type = (*carriage)->args[0] == T_DIR ? T_IND : (*carriage)->args[0];
	arg1 = get_arg(type, &pos, &data->arena);
	if ((*carriage)->args[0] == T_REG)
		arg1 = (*carriage)->registers[arg1 - 1];
	else if ((*carriage)->args[0] == T_IND)
		arg1 = get_value(DIR_SIZE, &data->arena,
						 (*carriage)->curr_pos + arg1);
	type = (*carriage)->args[1] == T_DIR ? T_IND : (*carriage)->args[1];
	arg2 = get_arg(type, &pos, &data->arena);
	if ((*carriage)->args[1] == T_REG)
		arg2 = (*carriage)->registers[arg2 - 1];
	else if ((*carriage)->args[1] == T_IND)
		arg2 = get_value(DIR_SIZE, &data->arena,
						 (*carriage)->curr_pos + arg2);
	r_inx3 = data->arena[get_pos(pos)];
	(*carriage)->registers[r_inx3 - 1] = get_value(
			DIR_SIZE,
			&data->arena,
			(*carriage)->curr_pos + (arg1 + arg2) % IDX_MOD);
}

void	sti(t_data *data, t_carriage **carriage, int32_t pos)
{
	int32_t	arg1;
	int32_t	arg2;
	int16_t	arg3;
	uint8_t type;

	arg1 = (*carriage)->registers[data->arena[get_pos(pos++)] - 1];
	type = (*carriage)->args[1] == T_DIR ? T_IND : (*carriage)->args[1];
	arg2 = get_arg(type, &pos, &data->arena);
	if ((*carriage)->args[1] == T_REG)
		arg2 = (*carriage)->registers[arg2 - 1];
	else if ((*carriage)->args[1] == T_IND)
		arg2 = get_value(IND_SIZE, &data->arena,
						 (*carriage)->curr_pos + arg2);
	type = (*carriage)->args[2] == T_DIR ? T_IND : (*carriage)->args[2];
	arg3 = get_arg(type, &pos, &data->arena);
	if ((*carriage)->args[2] == T_REG)
		arg3 = (*carriage)->registers[arg3 - 1];
	place_value(arg1,
				(*carriage)->curr_pos + (arg2 + arg3) % IDX_MOD,
				REG_SIZE,
				&data->arena);
}

void	fork_(t_data *data, t_carriage **carriage, int32_t pos)
{
	int32_t arg;

	arg = get_value(IND_SIZE, &data->arena, pos);
	init_carriage(
			&data->carriage_list,
			data->carriage_list->uid + 1,
			(*carriage)->curr_pos + arg % IDX_MOD);
}

void	lld(t_data *data, t_carriage **carriage, int32_t pos)
{
	int32_t	arg;
	int16_t	r_inx;

	arg = get_arg((*carriage)->args[0], &pos, &data->arena);
	r_inx = data->arena[get_pos(pos)];
	if ((*carriage)->args[0] == T_IND)
		arg = get_value(DIR_SIZE, &data->arena,
						(*carriage)->curr_pos + arg);
	(*carriage)->registers[r_inx - 1] = arg;
	(*carriage)->carry = (arg == 0) ? 1 : 0;
}

void	lldi(t_data *data, t_carriage **carriage, int32_t pos)
{
	int32_t	arg1;
	int32_t	arg2;
	int16_t	r_inx3;
	uint8_t type;

	type = (*carriage)->args[0] == T_DIR ? T_IND : (*carriage)->args[0];
	arg1 = get_arg(type, &pos, &data->arena);
	if ((*carriage)->args[0] == T_REG)
		arg1 = (*carriage)->registers[arg1 - 1];
	else if ((*carriage)->args[0] == T_IND)
		arg1 = get_value(DIR_SIZE, &data->arena,
						 (*carriage)->curr_pos + arg1);
	type = (*carriage)->args[1] == T_DIR ? T_IND : (*carriage)->args[1];
	arg2 = get_arg(type, &pos, &data->arena);
	if ((*carriage)->args[1] == T_REG)
		arg2 = (*carriage)->registers[arg2 - 1];
	else if ((*carriage)->args[1] == T_IND)
		arg2 = get_value(DIR_SIZE, &data->arena,
						 (*carriage)->curr_pos + arg2);
	r_inx3 = data->arena[get_pos(pos)];
	(*carriage)->registers[r_inx3 - 1] = get_value(
			DIR_SIZE,
			&data->arena,
			(*carriage)->curr_pos + (arg1 + arg2));
}

void	lfork(t_data *data, t_carriage **carriage, int32_t pos)
{
	int32_t arg;

	arg = get_value(IND_SIZE, &data->arena, pos);
	init_carriage(
			&data->carriage_list,
			data->carriage_list->uid + 1,
			(*carriage)->curr_pos + arg);
}

void	aff(t_data *data, t_carriage **carriage, int32_t pos)
{
	int16_t r;

	r = data->arena[get_pos(pos)];
	if (data->a_flag)
		ft_printf("%c\n",
			(char)((*carriage)->registers[r - 1] % 256));
}
