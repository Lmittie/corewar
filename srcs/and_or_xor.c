/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   and_or_xor.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmittie <lmittie@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/02 17:06:50 by lmittie           #+#    #+#             */
/*   Updated: 2020/12/05 21:36:39 by lmittie          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "corewar.h"

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

//	ft_printf("arg1 = %d (%x), arg2 = %d (%x), r = %d (val %d / %x), carry %d\n",
//			  arg1, arg1, arg2, arg2, r_inx3,
//			  (*carriage)->registers[r_inx3 - 1],
//			  (*carriage)->registers[r_inx3 - 1],
//			  (*carriage)->carry);
	ft_printf("%d %d r%d\n", arg1, arg2, r_inx3);
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

//	ft_printf("arg1 = %d (%x), arg2 = %d (%x), r = %d (val %d / %x), carry %d\n",
//			  arg1, arg1, arg2, arg2, r_inx3,
//			  (*carriage)->registers[r_inx3 - 1],
//			  (*carriage)->registers[r_inx3 - 1],
//			  (*carriage)->carry);
	ft_printf("%d %d r%d\n", arg1, arg2, r_inx3);
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

//	ft_printf("arg1 = %d (%x), arg2 = %d (%x), r = %d (val %d / %x), carry %d\n",
//			  arg1, arg1, arg2, arg2, r_inx3,
//			  (*carriage)->registers[r_inx3 - 1],
//			  (*carriage)->registers[r_inx3 - 1],
//			  (*carriage)->carry);
	ft_printf("%d %d r%d\n", arg1, arg2, r_inx3);
}
