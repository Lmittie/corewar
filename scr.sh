#!/bin/bash
gcc srcs/*.c -I includes/ -I ft_printf/libft/ -I ft_printf -L ft_printf/ -lftprintf
./a.out champs/examples/fluttershy.cor | head -$1 > t1
./corewar -v 6 champs/examples/fluttershy.cor | head -$1 > t2
diff t1 t2
