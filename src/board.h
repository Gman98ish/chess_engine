#ifndef BOARD_H
#define BOARD_H

#define U64 unsigned long long

#define A_FILE 0x0101010101010101
#define H_FILE 0x8080808080808080
#define RANK_1 0x00000000000000FF
#define RANK_8 0xFF00000000000000
#define DIAG_A1H8 0x8040201008040201
#define DIAG_H1A8 0x0102040810204080
#define LIGHT_SQUARES 0x55AA55AA55AA55AA
#define DARK_SQUARES 0xAA55AA55AA55AA55

#define WHITE 0
#define BLACK 1

/**
 * Little Endian Rank File (LERF) bit board.
 * 
 * Each piece set is stored in an array, 0 index is white, 1 index is black
 */
struct Board {
    U64 rooks[2];
    U64 knights[2];
    U64 bishops[2];
    U64 queens[2];
    U64 kings[2];
    U64 pawns[2];
};

struct Board start_position();
struct Board from_fen(const char *fen);
void Board_print(struct Board *board);

U64 Board_all_pieces(struct Board *board);

#endif