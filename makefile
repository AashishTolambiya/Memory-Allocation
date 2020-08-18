a.out : 1_main.o 1_impl.o
	gcc 1_main.o 1_impl.o -lncurses -lm

1_main.o: 1_main.c 1_header.h
	gcc -c 1_main.c -lncurses -lm

1_impl.o : 1_impl.c 1_header.h
	gcc -c 1_impl.c -lncurses -lm

