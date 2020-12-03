/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   st.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmittie <lmittie@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/02 17:06:08 by lmittie           #+#    #+#             */
/*   Updated: 2020/12/03 18:14:20 by lmittie          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "corewar.h"

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

	if ((*carriage)->args[1] == T_REG)
		ft_printf("r inx1  = %d (%x), r inx2 = %d (%x), r[r2] = %d (%x)\n",
			r_inx1, r_inx1, r_inx2, r_inx2, (*carriage)->registers[r_inx2 - 1], (*carriage)->registers[r_inx2 - 1]);
	else if ((*carriage)->args[1] == T_IND) {
		ft_printf("r inx1  = %d (%x), r inx2 = %d (%x), ", r_inx1, r_inx1, r_inx2, r_inx2);
		for (int i = (*carriage)->curr_pos + r_inx2 % IDX_MOD;
		i < (*carriage)->curr_pos + r_inx2 % IDX_MOD + REG_SIZE; i++)
			ft_printf("%x", data->arena[i % MEM_SIZE]);
	}
	ft_printf("\n");
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
		arg2 = get_value(DIR_SIZE, &data->arena,
						 (*carriage)->curr_pos + arg2);
	type = (*carriage)->args[2] == T_DIR ? T_IND : (*carriage)->args[2];
	arg3 = get_arg(type, &pos, &data->arena);
	if ((*carriage)->args[2] == T_REG)
		arg3 = (*carriage)->registers[arg3 - 1];
	place_value(arg1,
				(*carriage)->curr_pos + (arg2 + arg3) % IDX_MOD,
				REG_SIZE,
				&data->arena);

	ft_printf("arg1 = %d (%x), arg2 = %d (%x), arg3 = %d (%x), ",
		   arg1, arg1, arg2, arg2, arg3, arg3);
	for (int i = (*carriage)->curr_pos + (arg2 + arg3) % IDX_MOD;
		 i < (*carriage)->curr_pos + (arg2 + arg3) % IDX_MOD + REG_SIZE; ++i) {
		ft_printf("%x", data->arena[i % MEM_SIZE]);
	}
	ft_printf("\n");
}
