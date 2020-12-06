/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cycles_to_die_check.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmittie <lmittie@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/26 18:48:46 by lmittie           #+#    #+#             */
/*   Updated: 2020/12/05 21:11:51 by lmittie          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "corewar.h"

void	del_carriage(t_carriage **to_del, t_carriage **prev, t_carriage **head)
{
	if (*prev == NULL)
	{
		*head = (*to_del)->next;
		free(*to_del);
		*to_del = *head;
	}
	else
	{
		(*prev)->next = (*to_del)->next;
		free(*to_del);
		*to_del = (*prev)->next;
	}
}

void	ctd_check(t_data *data)
{
	t_carriage *it;
	t_carriage *prev_it;

	it = data->carriage_list;
	prev_it = NULL;
	while (it)
	{
		if (data->cycles - it->last_live_cycle >= data->cycles_to_die)
		{
			del_carriage(&it, &prev_it, &data->carriage_list);
			continue ;
		}
		prev_it = it;
		it = it->next;
	}
	data->checks_counter++;
	if (data->live_op_counter >= NBR_LIVE || data->checks_counter >= MAX_CHECKS)
	{
		data->cycles_to_die -= CYCLE_DELTA;
#ifdef TEST
		ft_printf("Cycle to die is now %d\n", data->cycles_to_die);

#endif
		data->checks_counter = 0;
	}
	data->live_op_counter = 0;
}