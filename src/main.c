#include <stdio.h>
#include <board.h>

int main(int argc, char **argv)
{
    struct Board board = from_fen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    Board_print(&board);
    return 0;
}