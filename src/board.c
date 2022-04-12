#include <stdio.h>
#include <board.h>
#include <string.h>
#include <ctype.h>

struct Board start_position()
{
    struct Board board;
    memset(&board, 0, sizeof(struct Board));

    board.rooks[WHITE] |= (1ULL << 7);
    board.rooks[WHITE] |= (1ULL << 0);
    return board;
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
            // idx -= 8;
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