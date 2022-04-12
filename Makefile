chess_engine: src/main.c src/board.c src/board.h
	gcc -Wall -g -o chess_engine -Isrc src/main.c src/board.c