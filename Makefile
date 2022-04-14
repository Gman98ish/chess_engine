tests: tests/main.c src/board.c src/board.h tests/board_test.c
	gcc -Wall -g -o runtests -Isrc tests/main.c src/board.c tests/board_test.c

chess_engine: src/main.c src/board.c src/board.h
	gcc -Wall -g -o chess_engine -Isrc src/main.c src/board.c
