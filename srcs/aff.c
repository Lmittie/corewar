/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aff.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmittie <lmittie@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/02 17:07:01 by lmittie           #+#    #+#             */
/*   Updated: 2020/12/02 17:11:20 by lmittie          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "corewar.h"

void	aff(t_data *data, t_carriage **carriage, int32_t pos)
{
	int16_t r;

	r = data->arena[get_pos(pos)];
	if (data->a_flag)
		ft_printf("%c\n",
				  (char)((*carriage)->registers[r - 1] % 256));
}