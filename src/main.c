#include <stdio.h>
#include <board.h>

int main(int argc, char **argv)
{
    struct Board board = start_position();
    Board_print(&board);
    return 0;
}