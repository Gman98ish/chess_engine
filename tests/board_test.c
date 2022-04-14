#include <board.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

void test_rook_moves()
{
    struct Board board;
    memset(&board, 0, sizeof(board));

    Board_generate_lookup_tables(&board);

    assert(board.rook_moves[10][NORTH] == 0x404040404040400);
    assert(board.rook_moves[20][NORTH] == 0x1010101010100000);
    assert(board.rook_moves[30][NORTH] == 0x4040404040000000);
    assert(board.rook_moves[50][NORTH] == 0x404000000000000);
    
    // TODO: Other directions
}

void test_bishop_moves()
{
    struct Board board;
    memset(&board, 0, sizeof(board));

    Board_generate_lookup_tables(&board);

    // assert(board.bishop_moves[10][NORTH_WEST] == 0x8040201008040200);
    assert(board.bishop_moves[10][NORTH_WEST] == 0x80402010080000);
    assert(board.bishop_moves[20][NORTH_WEST] == 0x804020000000);
    assert(board.bishop_moves[30][NORTH_WEST] == 0x8000000000);
    assert(board.bishop_moves[40][NORTH_WEST] == 0x402000000000000);
    
    // TODO: Other directions
}
