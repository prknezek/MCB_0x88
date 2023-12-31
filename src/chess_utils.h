#ifndef CHESSUTILS_H
#define CHESSUTILS_H

#include "defs.h"
#include "tt.h"

// turn a move into an integer
/*
    Move formatting
    
    0000 0000 0000 0000 0111 1111       start square            0x7f
    0000 0000 0011 1111 1000 0000       target square           0x3f80
    0000 0011 1100 0000 0000 0000       promoted piece
                                        flags:
    0000 0100 0000 0000 0000 0000           capture
    0000 1000 0000 0000 0000 0000           double pawn move
    0001 0000 0000 0000 0000 0000           enpassant
    0010 0000 0000 0000 0000 0000           castling
*/
#define encode_move(source, target, piece, capture_f, pawn_f, enpassant_f, castling_f) \
(                           \
  (source) |              \
  (target << 7) |         \
  (piece << 14) |         \
  (capture_f << 18) |     \
  (pawn_f << 19) |        \
  (enpassant_f << 20) |   \
  (castling_f << 21)      \
)

// decode move's start square
#define get_move_start(move) (move & 0x7f)

// decode move's target square
#define get_move_target(move) ((move >> 7) & 0x7f)

// decode move's promoted piece
#define get_promoted_piece(move) ((move >> 14) & 0xf)

// decode move's capture flag
#define get_move_capture(move) ((move >> 18) & 0x1)

// decode move's double pawn push flag
#define get_move_pawn(move) ((move >> 19) & 0x1)

// decode move's enpassant flag
#define get_move_enpassant(move) ((move >> 20) & 0x1)

// decode move's castling flag
#define get_move_castling(move) ((move >> 21) & 0x1)

#define copy_board()                                                   \
    int board_copy[128], king_square_copy[2];                          \
    int side_copy, enpassant_copy, castle_copy;                        \
    memcpy(board_copy, board, 512);                                    \
    side_copy = side, enpassant_copy = enpassant, castle_copy = castle;\
    memcpy(king_square_copy, king_square, 8);                          \
    uint64_t hash_key_copy = hash_key;                                 \

#define restore_board()                                                \
    memcpy(board, board_copy, 512);                                    \
    side = side_copy, enpassant = enpassant_copy, castle = castle_copy;\
    memcpy(king_square, king_square_copy, 8);                          \
    hash_key = hash_key_copy;                                          \

// move list structure
typedef struct {
    // move list
    int moves[256];
    // move count
    int count = 0;
} moves;

// HELPER FUNCTION DEFINITIONS
extern bool on_board(int square);
extern bool is_empty_square(int square);

extern int is_square_attacked(int square, int side);
extern int is_square_attacked_pawn(int square, int side);
extern int in_check(int side);

extern void print_move_list(moves *move_list);
extern void print_move(int move);
extern void print_attacked_squares(int side);
extern void print_piece_counts();
extern void print_piece_squares();
extern void print_board();

extern void reset_board();
extern void parse_fen(char *fen);

extern bool on_board(int square);
extern bool rank_7(int square);
extern bool rank_2(int square);
extern bool is_empty_square(int square);
extern bool is_black_piece_square(int square);
extern bool is_white_piece_square(int square);

extern bool is_white_piece(int piece);
extern bool is_black_piece(int piece);
extern void add_piece_count(int piece);
extern void subtract_piece_count(int piece);
extern void add_piece_square(int square);
extern void update_piece_square(int piece, int old_square, int new_square);
extern void fill_piece_squares();

extern void add_move(moves *move_list, int move);
extern string get_move_string(int move);

extern int get_time_ms();

#endif