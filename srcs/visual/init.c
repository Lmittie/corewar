/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acarlett <acarlett@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/23 23:30:54 by acarlett          #+#    #+#             */
/*   Updated: 2020/12/05 15:39:29 by acarlett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/corewar_visual.h"
#include <panel.h>
#include <sys/ioctl.h>
#include <stdio.h>

void		init_ncurses(t_data *data, t_visual *visual, struct winsize *win_size)
{
	initscr();
	noecho();
	curs_set(false);
	start_color();
    ioctl(0, TIOCGWINSZ, win_size);
	visual->wins.arena_win = newwin(win_size->ws_row, 238, 0, 0);
	visual->wins.info_win = newwin(win_size->ws_row, 55, 0, 238);

}

void		make_arena(t_data *data, WINDOW *arena_win, t_visual *visual)
{
	int i;
	int code_size;
	int	count_champs;

	i = 0;
	code_size = 0;
	count_champs = 0;
	wprintw(arena_win, "\n");
	while (i != MEM_SIZE - 1)
	{
		if (i % 79 == 0)
			wprintw(arena_win, " ");


		if (i == (MEM_SIZE / data->players_num) * count_champs || code_size)
		{
			code_size++;
			wattron(arena_win, COLOR_PAIR(count_champs + 1));
		}

		// if () //TODO условие на координаты каретки
			// wattron(arena_win, COLOR_PAIR(count_champs + 1) * 10);
		wprintw(arena_win, "%02x", data->arena[i]);
		if (code_size == 1)
			wattroff(arena_win, COLOR_PAIR(count_champs + 1) * 10);
		wprintw(arena_win, " ");
		

		if (code_size == data->champs[count_champs].header.prog_size)
		{
			wattroff(arena_win, COLOR_PAIR(count_champs + 1));
			code_size = 0;
			count_champs++;
		}
		i++;
	}
		box(arena_win, 0, 0);
}


void		make_color_pair()
{
	init_pair(1, COLOR_BLUE, COLOR_BLACK);
	init_pair(10, COLOR_WHITE, COLOR_BLUE);

	init_pair(2, COLOR_GREEN, COLOR_BLACK);
	init_pair(20, COLOR_WHITE, COLOR_GREEN);
	
	init_pair(3, COLOR_MAGENTA, COLOR_BLACK);
	init_pair(30, COLOR_WHITE, COLOR_MAGENTA);
	
	init_pair(4, COLOR_CYAN, COLOR_BLACK);
	init_pair(40, COLOR_WHITE, COLOR_CYAN);
	
	init_pair(5, COLOR_YELLOW, COLOR_BLACK);
	init_pair(6, COLOR_WHITE, COLOR_BLACK);
}

void		make_info_table(t_data *data, WINDOW *info_win)
{
	int i;
	int j;

	i = -1;
	wmove(info_win, 3, 2);

	wattron(info_win, COLOR_PAIR(6));
	wprintw(info_win, "CYCLE: ");
	wattron(info_win, COLOR_PAIR(5));
	write (1, "RT1\n", 4);
	wprintw(info_win, "%d\n\n", data->cycles);

	wattron(info_win, COLOR_PAIR(6));
	wprintw(info_win, "  CYCLES TO DIE: ");
	wattron(info_win, COLOR_PAIR(5));
	wprintw(info_win, "%d\n\n", data->cycles_to_die);
		
	wattron(info_win, COLOR_PAIR(6));
	wprintw(info_win, "  LIVE COUNTER:");
	wattron(info_win, COLOR_PAIR(5));
	wprintw(info_win, "  %d\n\n\n", data->live_op_counter);	

	while (++i != data->players_num)
	{
		wattron(info_win, COLOR_PAIR(6));
		wprintw(info_win, "  PLAYER #%d: ", i + 1);
		wattron(info_win, COLOR_PAIR(i + 1));

		j = 0;
		while(data->champs->header.prog_name[j] != '\0')
			wprintw(info_win, "%c", data->champs[i].header.prog_name[j++]);
		wattroff(info_win, COLOR_PAIR(i + 1));

		wprintw(info_win, "\n\n");
		wattron(info_win, COLOR_PAIR(6));
		wprintw(info_win, "\tLives in current period:      ");
		wattron(info_win, COLOR_PAIR(5));
		wprintw(info_win, "133");

		wprintw(info_win, "\n\n");
		wattron(info_win, COLOR_PAIR(6));
		wprintw(info_win, "\tLast live:                    ");
		wattron(info_win, COLOR_PAIR(5));
		wprintw(info_win, "1333");
		
		wprintw(info_win, "\n\n\n");
	}

	wattroff(info_win, COLOR_PAIR(5));
	wattroff(info_win, COLOR_PAIR(6));
	box(info_win, 0, 0);

}

void		visual(t_data *data)
{
	t_visual visual;
	int x;
	int y;
	int button;

	button = 1;
	init_ncurses(data, &visual, &(visual.win_size));
//	while (button != 113)
//	{
		make_color_pair();
		make_arena(data, visual.wins.arena_win, &visual);
		make_info_table(data, visual.wins.info_win);



		refresh();
		wrefresh(visual.wins.arena_win);
		wrefresh(visual.wins.info_win);
//		button = wgetch(visual.wins.arena_win);
//	}
	delwin(visual.wins.arena_win);
	delwin(visual.wins.info_win);
//	getch();
	curs_set(true);
}





	// while (1 || key != 27)
	// {
	// 	// key = getch();
	// 	mvaddstr((set_y / 2 + 0), (set_x / 2 + 0), "Just a String\n");
	// 	if (j < 10 && !j_f)
	// 		j++;
	// 	if (j == 10)
	// 		j_f = 1;
	// 	if (j <= 10 && j_f)
	// 		j--;
	// 	if (j == 0)
	// 		j_f = 0;
		
		
	// 	if (i < 10 && !i_f)
	// 		i++;
	// 	if (i == 10)
	// 		i_f = 1;
	// 	if (i <= 10 && i_f)
	// 		i--;
	// 	if (i == 0)
	// 		i_f = 0;
	// 	if (k == 1)
	// 		k = 2;
	// 	else if (k == 2)
	// 		k = 1;
	// 	refresh();
	// 	delay_output(100);
	// 	clear();
	// }