#include <stdio.h>
#include <board.h>


void print_bitboard(U64 bitboard)
{
    for (int rank = 7; rank >= 0; rank--) {
        for (int file = 0; file < 8; file++) {
            U64 square = (U64)1 << (8 * rank + file);
            printf(" %c ", (square & bitboard) == 0 ? '0' : '1');
        }
        printf("\n");
    }
}

void print_bin(uint8_t n)
{
    for (int i = 7; i >= 0; i--) {
        printf("%d", (n >> i) & 1);
    }
    printf("\n");
}

int main(int argc, char **argv)
{
    struct Board board = start_position();
    Board_generate_lookup_tables(&board);

    print_bitboard(board.knight_moves[63]);
    printf("\n");
    print_bitboard(board.knight_moves[62]);
    printf("\n");
    print_bitboard(board.knight_moves[61]);
    printf("\n");
    print_bitboard(board.knight_moves[20]);
    printf("\n");

    return 0;
}
