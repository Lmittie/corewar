/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmittie <lmittie@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/02 17:06:41 by lmittie           #+#    #+#             */
/*   Updated: 2020/12/06 19:52:22 by lmittie          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "corewar.h"

void	fork_(t_data *data, t_carriage **carriage, int32_t pos)
{
	int32_t		arg;
	t_carriage	*carriage_copy;

	arg = get_value(IND_SIZE, &data->arena, pos);
	carriage_copy = *carriage;
	init_carriage(
			&data->carriage_list,
			data->carriage_list->uid + 1,
			(*carriage)->curr_pos + arg % IDX_MOD);
	ft_memcpy((void*)data->carriage_list->registers,
			  (void*)carriage_copy->registers, sizeof(int32_t) * REG_NUMBER);
	data->carriage_list->carry = carriage_copy->carry;
	data->carriage_list->last_live_cycle = carriage_copy->last_live_cycle;
#ifdef TEST
	ft_printf("%d (%d)\n", arg, data->carriage_list->curr_pos);
#endif
}

void	lfork(t_data *data, t_carriage **carriage, int32_t pos)
{
	int32_t		arg;
	t_carriage	*carriage_copy;

	arg = get_value(IND_SIZE, &data->arena, pos);
	carriage_copy = *carriage;
	init_carriage(
			&data->carriage_list,
			data->carriage_list->uid + 1,
			(*carriage)->curr_pos + arg);
	ft_memcpy((void*)data->carriage_list->registers,
			  (void*)carriage_copy->registers, sizeof(int32_t) * REG_NUMBER);
	data->carriage_list->carry = carriage_copy->carry;
	data->carriage_list->last_live_cycle = carriage_copy->last_live_cycle;

#ifdef TEST
	ft_printf("%d (%d)\n", arg, data->carriage_list->curr_pos);
#endif
}
