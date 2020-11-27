/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   corewar.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmittie <lmittie@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/19 20:53:44 by lmittie           #+#    #+#             */
/*   Updated: 2020/10/28 19:58:35 by lmittie          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COREWAR_H

# define COREWAR_H

# include "libft.h"
# include "ft_printf.h"
# include "op.h"
# include <libc.h>

typedef struct		s_champ
{
	uint8_t		uid;
	t_header	header;
}					t_champ;

typedef struct		s_carriage
{
	uint16_t			uid;
	uint8_t				carry;
	uint8_t				op_code;
	uint32_t			last_live_cycle;
	uint16_t			cycles_before;
	uint32_t			curr_pos;
	uint32_t			bytes_step;
	int32_t				registers[REG_NUMBER];
	uint8_t				args[3];
	struct s_carriage	*next;
}					t_carriage;

typedef struct		s_args
{
	int32_t				arg_it;
	int8_t				n_flag;
}					t_args;

typedef struct		s_data
{
	t_champ				champs[MAX_PLAYERS];
	uint16_t			players_num;
	uint8_t				arena[MEM_SIZE];
	t_carriage			*carriage_list;
	uint8_t				winner_id;
	uint32_t			cycles;
	uint32_t			live_op_counter;
	uint32_t			cycles_to_die;
	uint32_t			checks_counter;
	uint32_t			dump_cycles;
	uint8_t				a_flag;
}					t_data;

typedef struct	s_op
{
	const char	*op_name;
	uint8_t		args_num;
	t_arg_type	args_type[4];
	uint8_t		op_code;
	uint16_t	cycles;
	const char	*op_description;
	uint8_t		arg_type_code;
	uint8_t		dir_size;
	void		(*func)(t_data *, t_carriage **, size_t);
}				t_op;

void	parse_arguments(int ac, const char **av, t_data *data);
void	parse_champions(t_args (*champs)[MAX_PLAYERS], t_data *data, const char **av);
void	game(t_data *data);
void	ctd_check(t_data *data);
void	init_carriage(t_carriage **clist, uint8_t uid, size_t pos);

void		live(t_data *data, t_carriage **carriage, size_t pos);
void		ld(t_data *data, t_carriage **carriage, size_t pos);
void		st(t_data *data, t_carriage **carriage, size_t pos);
void		add(t_data *data, t_carriage **carriage, size_t pos);
void		sub(t_data *data, t_carriage **carriage, size_t pos);
void		and(t_data *data, t_carriage **carriage, size_t pos);
void		or(t_data *data, t_carriage **carriage, size_t pos);
void		xor(t_data *data, t_carriage **carriage, size_t pos);
void		zjmp(t_data *data, t_carriage **carriage, size_t pos);
void		ldi(t_data *data, t_carriage **carriage, size_t pos);
void		sti(t_data *data, t_carriage **carriage, size_t pos);
void		fork_(t_data *data, t_carriage **carriage, size_t pos);
void		lld(t_data *data, t_carriage **carriage, size_t pos);
void		lldi(t_data *data, t_carriage **carriage, size_t pos);
void		lfork(t_data *data, t_carriage **carriage, size_t pos);
void		aff(t_data *data, t_carriage **carriage, size_t pos);

#endif