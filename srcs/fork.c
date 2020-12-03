/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmittie <lmittie@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/02 17:06:41 by lmittie           #+#    #+#             */
/*   Updated: 2020/12/03 19:34:51 by lmittie          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "corewar.h"

void	fork_(t_data *data, t_carriage **carriage, int32_t pos)
{
	int32_t arg;

	arg = get_value(IND_SIZE, &data->arena, pos);
	init_carriage(
			&data->carriage_list,
			data->carriage_list->uid + 1,
			(*carriage)->curr_pos + arg % IDX_MOD);
	ft_memcpy((void*)data->carriage_list->registers,
			  (void*)(*carriage)->registers, sizeof(int32_t) * REG_NUMBER);
	data->carriage_list->carry = (*carriage)->carry;
	data->carriage_list->last_live_cycle = (*carriage)->last_live_cycle;

	(*carriage)->registers[0] = 5;
	for (int i = 0; i < REG_NUMBER; ++i) {
		ft_printf("%d = %d ", i + 1, data->carriage_list->registers[i]);
	}
	ft_printf("\n");
	for (int i = 0; i < REG_NUMBER; ++i) {
		ft_printf("%d = %d ", i + 1, (*carriage)->registers[i]);
	}
	ft_printf("\ncurr_pos = %d\n", data->carriage_list->curr_pos);
}

void	lfork(t_data *data, t_carriage **carriage, int32_t pos)
{
	int32_t arg;

	arg = get_value(IND_SIZE, &data->arena, pos);
	init_carriage(
			&data->carriage_list,
			data->carriage_list->uid + 1,
			(*carriage)->curr_pos + arg);
	ft_memcpy((void*)data->carriage_list->registers,
			  (void*)(*carriage)->registers, sizeof(int32_t) * REG_NUMBER);
	data->carriage_list->carry = (*carriage)->carry;
	data->carriage_list->last_live_cycle = (*carriage)->last_live_cycle;

	for (int i = 0; i < REG_NUMBER; ++i) {
		ft_printf("%d = %d ", i + 1, data->carriage_list->registers[i]);
	}
	ft_printf("\n");
	for (int i = 0; i < REG_NUMBER; ++i) {
		ft_printf("%d = %d ", i + 1, (*carriage)->registers[i]);
	}
	ft_printf("\ncurr_pos = %d\n", data->carriage_list->curr_pos);
}
