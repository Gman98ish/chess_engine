#include <stdio.h>
#include <board.h>
#include <string.h>
#include <ctype.h>

const int _index64[64] = {
    0,  1, 48,  2, 57, 49, 28,  3,
   61, 58, 50, 42, 38, 29, 17,  4,
   62, 55, 59, 36, 53, 51, 43, 22,
   45, 39, 33, 30, 24, 18, 12,  5,
   63, 47, 56, 27, 60, 41, 37, 16,
   54, 35, 52, 21, 44, 32, 23, 11,
   46, 26, 40, 15, 34, 20, 31, 10,
   25, 14, 19,  9, 13,  8,  7,  6
};

struct Board start_position()
{
    return from_fen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
}

struct Board from_fen(const char *fen)
{
    struct Board board;
    memset(&board, 0, sizeof(struct Board));

    const char *c = fen;
    int idx = 63;
    while (*c != ' ') {
        if (*c >= '0' && *c <= '9') {
            idx -= *c - '0';
            c++;
            continue;
        }

        if (*c == '/') {
            c++;
            continue;
        }

        int side = WHITE;
        if (*c >= 'a' && *c <= 'z') {
            side = BLACK;
        }

        switch (toupper(*c)) {
            case 'K':
                board.kings[side] |= (1ULL << idx);
                break;
            case 'Q':
                board.queens[side] |= (1ULL << idx);
                break;
            case 'R':
                board.rooks[side] |= (1ULL << idx);
                break;
            case 'B':
                board.bishops[side] |= (1ULL << idx);
                break;
            case 'N':
                board.knights[side] |= (1ULL << idx);
                break;
            case 'P':
                board.pawns[side] |= (1ULL << idx);
                break;
        }
        c++;
        idx--;
    }
    return board;
}

U64 Board_all_pieces(struct Board *board)
{
    U64 all_pieces = 0;
    all_pieces |= board->rooks[WHITE];
    all_pieces |= board->knights[WHITE];
    all_pieces |= board->bishops[WHITE];
    all_pieces |= board->queens[WHITE];
    all_pieces |= board->kings[WHITE];
    all_pieces |= board->pawns[WHITE];
    all_pieces |= board->rooks[BLACK];
    all_pieces |= board->knights[BLACK];
    all_pieces |= board->bishops[BLACK];
    all_pieces |= board->queens[BLACK];
    all_pieces |= board->kings[BLACK];
    all_pieces |= board->pawns[BLACK];
    return all_pieces;
}

void Board_print(struct Board *board)
{
    U64 all_pieces = Board_all_pieces(board);

    for (int rank = 7; rank >= 0; rank--) {
        for (int file = 0; file < 8; file++) {
            U64 square = (U64)1 << (8 * rank + file);
            printf(" %c ", (square & all_pieces) == 0 ? '0' : '1');
        }
        printf("\n");
    }
}

void _generate_rook_moves(struct Board *board, int rank, int file)
{
    // SOUTH
    U64 move = 0;
    for (int i = 0; i < rank; i++) {
        move |= (1ULL << (8 * i + file));
    }
    board->rook_moves[rank * 8 + file][SOUTH] = move;

    // NORTH
    move = 0;
    for (int i = rank; i < 8; i++) {
        move |= (1ULL << (8 * i + file));
    }
    board->rook_moves[rank * 8 + file][NORTH] = move;

    // EAST
    move = 0;
    for (int i = file + 1; i < 8; i++) {
        move |= (1ULL << (8 * rank + i));
    }
    board->rook_moves[rank * 8 + file][EAST] = move;

    // WEST
    move = 0;
    for (int i = file - 1; i >= 0; i--) {
        move |= (1ULL << (8 * rank + i));
    }
    board->rook_moves[rank * 8 + file][WEST] = move;
}


void _generate_bishop_moves(struct Board *board, int rank, int file)
{
    int right_dist = 7 - file;
    int left_dist = file;
    int top_dist = 7 - rank;
    int bot_dist = rank;

    U64 move = 0;
 
    // SOUTH EAST
    for (int i = 1; i <= (right_dist > bot_dist ? bot_dist : right_dist); i++) {
        move |= (1ULL << (8 * (rank - i) + (file + i)));
    }
    board->bishop_moves[rank * 8 + file][SOUTH_EAST] = move;

    // NORTH EAST
    move = 0;
    for (int i = 1; i <= (right_dist > top_dist ? top_dist : right_dist); i++) {
        move |= (1ULL << (8 * (rank + i) + (file + i)));
    }
    board->bishop_moves[rank * 8 + file][NORTH_EAST] = move;

    // SOUTH WEST
    move = 0;
    for (int i = 1; i <= (left_dist > bot_dist ? bot_dist : left_dist); i++) {
        move |= (1ULL << (8 * (rank - i) + (file - i)));
    }
    board->bishop_moves[rank * 8 + file][SOUTH_WEST] = move;
    
    // NORTH WEST
    move = 0;
    for (int i = 1; i <= (top_dist > right_dist ? right_dist : top_dist); i++) {
        move |= (1ULL << (8 * (rank + i) + (file + i)));
    }
    board->bishop_moves[rank * 8 + file][NORTH_WEST] = move;
}

void _generate_knight_moves(struct Board *board, int rank, int file)
{
    U64 move = 0;
    // two up one left
    if (rank - 2 >= 0 && file - 1 >= 0) {
        move |= (1ULL << (8 * (rank - 2) + (file - 1)));
    }

    // two up one right
    if (rank - 2 >= 0 && file + 1 < 8) {
        move |= (1ULL << (8 * (rank - 2) + (file + 1)));
    }

    // two right one up
    if (rank - 1 >= 0 && file + 2 < 8) {
        move |= (1ULL << (8 * (rank - 1) + (file + 2)));
    }

    // two right one down
    if (rank + 1 < 8 && file + 2 < 8) {
        move |= (1ULL << (8 * (rank + 1) + (file + 2)));
    }

    // two down one left
    if (rank + 2 < 8 && file - 1 >= 0) {
        move |= (1ULL << (8 * (rank + 2) + (file - 1)));
    }

    // two down one right
    if (rank + 2 < 8 && file + 1 < 8) {
        move |= (1ULL << (8 * (rank + 2) + (file + 1)));
    }

    // two left one up
    if (rank - 1 >= 0 && file - 2 >= 0) {
        move |= (1ULL << (8 * (rank - 1) + (file - 2)));
    }
    
    // two left one down
    if (rank + 1 < 8 && file - 2 >= 0) {
        move |= (1ULL << (8 * (rank + 1) + (file - 2)));
    }

    board->knight_moves[rank * 8 + file] = move;
}

void _generate_white_pawn_moves(struct Board *board, int rank, int file)
{
    U64 move = 0;

    if (rank == 1) {
        move |= (1ULL << (8 * (rank + 2) + file));
    }
    if (rank < 7) {
        move |= (1ULL << (8 * (rank + 1) + file));
    }
    
    board->pawn_moves[rank * 8 + file][WHITE] = move;
}

void _generate_black_pawn_moves(struct Board *board, int rank, int file)
{
    U64 move = 0;

    if (rank == 6) {
        move |= (1ULL << (8 * (rank - 2) + file));
    }
    if (rank > 0) {
        move |= (1ULL << (8 * (rank - 1) + file));
    }
    
    board->pawn_moves[rank * 8 + file][BLACK] = move;
}

void _generate_king_moves(struct Board *board, int rank, int file) {}

/**
 * @brief Generate the lookup tables for the sliding piece moves.
 * 
 * The values this makes should be the same each time, so would be
 * cool if I could do this at compile time or something
 * 
 * @param board 
 */
void Board_generate_lookup_tables(struct Board *board)
{
    for (int rank = 7; rank >= 0; rank--) {
        for (int file = 0; file < 8; file++) {
            _generate_rook_moves(board, rank, file);
            _generate_bishop_moves(board, rank, file);
            _generate_knight_moves(board, rank, file);
            _generate_white_pawn_moves(board, rank, file);
            _generate_black_pawn_moves(board, rank, file);
            _generate_king_moves(board, rank, file);
        }
    }
}

int _bitScanForwardIndex(U64 bb)
{
    // debruijn64
    const U64 funny_number = (U64) 0x03f79d71b4cb0a89;
    return ((bb & -bb) * funny_number) >> 58;
}

int _bitscanForward(U64 bb)
{
    return _index64[_bitScanForwardIndex(bb)];
}

