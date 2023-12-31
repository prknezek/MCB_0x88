#include "defs.h"
#include "movegen.h"
#include "tt.h"

// move generator
void generate_moves(moves *move_list) {
    // reset move count
    move_list->count = 0;
    // loop over all board squares
    for (int square = 0; square < 128; ++square) {
        if (on_board(square)) {
            // white pawn and castling moves
            if (side == white) {
                // white pawn moves
                if (board[square] == P) {
                    int target_square = square - 16;
                    // quiet pawn moves (moves that aren't capturing)
                    // check if target square is on board
                    if (on_board(target_square) && is_empty_square(target_square)) {
                        // pawn promotions (make sure pawns are on 7th rank)
                        if (rank_7(square)) {
                            add_move(move_list, encode_move(square, target_square, Q, 0, 0, 0, 0));
                            add_move(move_list, encode_move(square, target_square, R, 0, 0, 0, 0));
                            add_move(move_list, encode_move(square, target_square, B, 0, 0, 0, 0));
                            add_move(move_list, encode_move(square, target_square, N, 0, 0, 0, 0));
                        } else {
                            // one square ahead pawn move
                            add_move(move_list, encode_move(square, target_square, 0, 0, 0, 0, 0));
                            // two squares ahead pawn move (make sure pawns are on 2nd rank)
                            if (rank_2(square) && is_empty_square(square - 32)) {
                                add_move(move_list, encode_move(square, square - 32, 0, 0, 1, 0, 0));
                            }
                        }
                    }
                    // white pawn captures (using bishop offsets b/c they're the same as pawns)
                    for (int i = 0; i < 4; ++i) {
                        // init pawn offset
                        int pawn_offset = bishop_offsets[i];
                        
                        // white pawn offsets
                        if (pawn_offset < 0) {
                            int target_square = square + pawn_offset;
                            // check if target square is on board
                            if (on_board(target_square)) {
                                // capture pawn promotion
                                if (rank_7(square) && is_black_piece_square(target_square)) {
                                    add_move(move_list, encode_move(square, target_square, Q, 1, 0, 0, 0));
                                    add_move(move_list, encode_move(square, target_square, R, 1, 0, 0, 0));
                                    add_move(move_list, encode_move(square, target_square, B, 1, 0, 0, 0));
                                    add_move(move_list, encode_move(square, target_square, N, 1, 0, 0, 0));
                                } else {
                                    // normal capture
                                    if (is_black_piece_square(target_square)) {
                                        add_move(move_list, encode_move(square, target_square, 0, 1, 0, 0, 0));
                                    }
                                    // enpassant capture
                                    if (target_square == enpassant) {
                                        add_move(move_list, encode_move(square, target_square, 0, 1, 0, 1, 0));
                                    }
                                }
                            }
                        }
                    }
                }
                // white king castling
                if (board[square] == K) {
                    // if king side castling is available
                    if (castle & KC) {
                        // make sure there are empty squares between king & rook
                        if (board[f1] == e && board[g1] == e) {
                            // make sure king & next squares are not under attack
                            if (!is_square_attacked(e1, black) && !is_square_attacked(f1, black)) {
                                add_move(move_list, encode_move(e1, g1, 0, 0, 0, 0, 1));
                            }
                        }
                    }
                    // if queen side castling is available
                    if (castle & QC) {
                        // make sure there are empty squares between king & rook
                        if (board[b1] == e && board[c1] == e && board[d1] == e) {
                            // make sure king & next squares are not under attack
                            if (!is_square_attacked(e1, black) && !is_square_attacked(d1, black)) {
                                add_move(move_list, encode_move(e1, c1, 0, 0, 0, 0, 1));
                            }
                        }
                    }
                }
            } 
            // black pawn and castling moves
            else {
                // black pawn moves (moves that aren't capturing)
                if (board[square] == p) {
                    int target_square = square + 16;
                    // check if target square is on board
                    if (on_board(target_square) && is_empty_square(target_square)) {
                        // pawn promotions (make sure pawns are on 2nd rank)
                        if (rank_2(square)) {
                            add_move(move_list, encode_move(square, target_square, q, 0, 0, 0, 0));
                            add_move(move_list, encode_move(square, target_square, r, 0, 0, 0, 0));
                            add_move(move_list, encode_move(square, target_square, b, 0, 0, 0, 0));
                            add_move(move_list, encode_move(square, target_square, n, 0, 0, 0, 0));
                        } else {
                            // one square ahead pawn move
                            add_move(move_list, encode_move(square, target_square, 0, 0, 0, 0, 0));
                            // two squares ahead pawn move (make sure pawns are on 7th rank)
                            if (rank_7(square) && is_empty_square(square + 32)) {
                                add_move(move_list, encode_move(square, square + 32, 0, 0, 1, 0, 0));
                            }
                        }
                    }
                    // black pawn captures (using bishop offsets b/c they're the same as pawns)
                    for (int i = 0; i < 4; ++i) {
                        // init pawn offset
                        int pawn_offset = bishop_offsets[i];
                        
                        // black pawn offsets
                        if (pawn_offset > 0) {
                            int target_square = square + pawn_offset;
                            // check if target square is on board
                            if (on_board(target_square)) {
                                // capture pawn promotion
                                if (rank_2(square) && is_white_piece_square(target_square)) {
                                    add_move(move_list, encode_move(square, target_square, q, 1, 0, 0, 0));
                                    add_move(move_list, encode_move(square, target_square, r, 1, 0, 0, 0));
                                    add_move(move_list, encode_move(square, target_square, b, 1, 0, 0, 0));
                                    add_move(move_list, encode_move(square, target_square, n, 1, 0, 0, 0));
                                } else {
                                    // normal capture
                                    if (is_white_piece_square(target_square)) {
                                        add_move(move_list, encode_move(square, target_square, 0, 1, 0, 0, 0));
                                    }
                                    // enpassant capture
                                    if (target_square == enpassant) {
                                        add_move(move_list, encode_move(square, target_square, 0, 1, 0, 1, 0));
                                    }
                                }
                            }
                        }
                    }
                }
                // black king castling
                if (board[square] == k) {
                    // if king side castling is available
                    if (castle & kc) {
                        // make sure there are empty squares between king & rook
                        if (board[f8] == e && board[g8] == e) {
                            // make sure king & next squares are not under attack
                            if (!is_square_attacked(e8, white) && !is_square_attacked(f8, white)) {
                                add_move(move_list, encode_move(e8, g8, 0, 0, 0, 0, 1));
                            }
                        }
                    }
                    // if queen side castling is available
                    if (castle & qc) {
                        // make sure there are empty squares between king & rook
                        if (board[b8] == e && board[c8] == e && board[d8] == e) {
                            // make sure king & next squares are not under attack
                            if (!is_square_attacked(e8, white) && !is_square_attacked(d8, white)) {
                                add_move(move_list, encode_move(e8, c8, 0, 0, 0, 0, 1));
                            }
                        }
                    }
                }
            }
            // knight moves
            if (side == white ? board[square] == N : board[square] == n) {
                // loop over knight offsets
                for (int i = 0; i < 8; ++i) {
                    int target_square = square + knight_offsets[i];
                    int target_piece = board[target_square];
                    
                    if (on_board(target_square)) {
                        if (side == white ?
                           (is_empty_square(target_square) || is_black_piece(target_piece)) :
                           (is_empty_square(target_square) || is_white_piece(target_piece))) {
                            // test if we are capturing
                            if (target_piece != e) {
                                add_move(move_list, encode_move(square, target_square, 0, 1, 0, 0, 0));
                            }
                            // moving to empty square
                            else {
                                add_move(move_list, encode_move(square, target_square, 0, 0, 0, 0, 0));
                            }
                        }
                    }
                }
            }
            // king moves
            if (side == white ? board[square] == K : board[square] == k) {
                // loop over knight offsets
                for (int i = 0; i < 8; ++i) {
                    int target_square = square + king_offsets[i];
                    int target_piece = board[target_square];
                    
                    if (on_board(target_square)) {
                        if (side == white ?
                           (is_empty_square(target_square) || is_black_piece(target_piece)) :
                           (is_empty_square(target_square) || is_white_piece(target_piece))) {
                            // test if we are capturing
                            if (target_piece != e) {
                                add_move(move_list, encode_move(square, target_square, 0, 1, 0, 0, 0));
                            }
                            // moving to empty square
                            else {
                                add_move(move_list, encode_move(square, target_square, 0, 0, 0, 0, 0));
                            }
                        }
                    }
                }
            }
            // bishop & queen moves
            if (side == white ?
                ((board[square] == B) || (board[square] == Q)) :
                ((board[square] == b) || (board[square] == q))) {
                // loop over bishop & queen offsets
                for (int i = 0; i < 4; ++i) {
                    int target_square = square + bishop_offsets[i];
                    // loop over attack ray
                    while (on_board(target_square)) {
                        int target_piece = board[target_square];
                        // if hits own piece
                        if (side == white ? is_white_piece(target_piece) : is_black_piece(target_piece)) {
                            break;
                        }
                        // if hits enemy piece
                        if (side == white ? is_black_piece(target_piece) : is_white_piece(target_piece)) {
                            add_move(move_list, encode_move(square, target_square, 0, 1, 0, 0, 0));
                            break;
                        }
                        // if empty square
                        if (is_empty_square(target_square)) {
                            add_move(move_list, encode_move(square, target_square, 0, 0, 0, 0, 0));
                        }
                        // increment target square
                        target_square += bishop_offsets[i];
                    }
                }
            }
            // rook & queen moves
            if (side == white ?
                ((board[square] == R) || (board[square] == Q)) :
                ((board[square] == r) || (board[square] == q))) {
                // loop over rook & queen offsets
                for (int i = 0; i < 4; ++i) {
                    int target_square = square + rook_offsets[i];
                    // loop over attack ray
                    while (on_board(target_square)) {
                        int target_piece = board[target_square];
                        // if hits own piece
                        if (side == white ? is_white_piece(target_piece) : is_black_piece(target_piece)) {
                            break;
                        }
                        // if hits enemy piece
                        if (side == white ? is_black_piece(target_piece) : is_white_piece(target_piece)) {
                            add_move(move_list, encode_move(square, target_square, 0, 1, 0, 0, 0));
                            break;
                        }
                        // if empty square
                        if (is_empty_square(target_square)) {
                            add_move(move_list, encode_move(square, target_square, 0, 0, 0, 0, 0));
                        }
                        // increment target square
                        target_square += rook_offsets[i];
                    }
                }
            }
        }
    }
}

void new_generate_moves(moves *move_list) {
    // reset move count
    move_list->count = 0;

    // loop over all pieces in piece_squares array
    for (int piece = 0; piece < 6; ++piece) {
        // get piece indices
        int *indices = piece_squares[side][piece];
        
        for (int i = 0; i < 10; ++i) {
            // get from_square
            int square = indices[i];
            
            // if there are no more valid squares, break loop
            if (square == no_sq) {
                break;
            }

            // white pawn and castling moves
            if (side == white) {
                // white pawn moves
                if (piece == 0) {
                    int target_square = square - 16;
                    // quiet pawn moves (moves that aren't capturing)
                    // check if target square is on board
                    if (on_board(target_square) && is_empty_square(target_square)) {
                        // pawn promotions (make sure pawns are on 7th rank)
                        if (rank_7(square)) {
                            add_move(move_list, encode_move(square, target_square, Q, 0, 0, 0, 0));
                            add_move(move_list, encode_move(square, target_square, R, 0, 0, 0, 0));
                            add_move(move_list, encode_move(square, target_square, B, 0, 0, 0, 0));
                            add_move(move_list, encode_move(square, target_square, N, 0, 0, 0, 0));
                        } else {
                            // one square ahead pawn move
                            add_move(move_list, encode_move(square, target_square, 0, 0, 0, 0, 0));
                            // two squares ahead pawn move (make sure pawns are on 2nd rank)
                            if (rank_2(square) && is_empty_square(square - 32)) {
                                add_move(move_list, encode_move(square, square - 32, 0, 0, 1, 0, 0));
                            }
                        }
                    }
                    // white pawn captures (using bishop offsets b/c they're the same as pawns)
                    for (int i = 0; i < 4; ++i) {
                        // init pawn offset
                        int pawn_offset = bishop_offsets[i];
                        
                        // white pawn offsets
                        if (pawn_offset < 0) {
                            int target_square = square + pawn_offset;
                            // check if target square is on board
                            if (on_board(target_square)) {
                                // capture pawn promotion
                                if (rank_7(square) && is_black_piece_square(target_square)) {
                                    add_move(move_list, encode_move(square, target_square, Q, 1, 0, 0, 0));
                                    add_move(move_list, encode_move(square, target_square, R, 1, 0, 0, 0));
                                    add_move(move_list, encode_move(square, target_square, B, 1, 0, 0, 0));
                                    add_move(move_list, encode_move(square, target_square, N, 1, 0, 0, 0));
                                } else {
                                    // normal capture
                                    if (is_black_piece_square(target_square)) {
                                        add_move(move_list, encode_move(square, target_square, 0, 1, 0, 0, 0));
                                    }
                                    // enpassant capture
                                    if (target_square == enpassant) {
                                        add_move(move_list, encode_move(square, target_square, 0, 1, 0, 1, 0));
                                    }
                                }
                            }
                        }
                    }
                }
                // white king castling
                if (piece == 5) {
                    // if king side castling is available
                    if (castle & KC) {
                        // make sure there are empty squares between king & rook
                        if (board[f1] == e && board[g1] == e) {
                            // make sure king & next squares are not under attack
                            if (!is_square_attacked(e1, black) && !is_square_attacked(f1, black)) {
                                add_move(move_list, encode_move(e1, g1, 0, 0, 0, 0, 1));
                            }
                        }
                    }
                    // if queen side castling is available
                    if (castle & QC) {
                        // make sure there are empty squares between king & rook
                        if (board[b1] == e && board[c1] == e && board[d1] == e) {
                            // make sure king & next squares are not under attack
                            if (!is_square_attacked(e1, black) && !is_square_attacked(d1, black)) {
                                add_move(move_list, encode_move(e1, c1, 0, 0, 0, 0, 1));
                            }
                        }
                    }
                }
            }
            // black pawn and castling moves
            else {
                // black pawn moves (moves that aren't capturing)
                if (piece == 0) {
                    int target_square = square + 16;
                    // check if target square is on board
                    if (on_board(target_square) && is_empty_square(target_square)) {
                        // pawn promotions (make sure pawns are on 2nd rank)
                        if (rank_2(square)) {
                            add_move(move_list, encode_move(square, target_square, q, 0, 0, 0, 0));
                            add_move(move_list, encode_move(square, target_square, r, 0, 0, 0, 0));
                            add_move(move_list, encode_move(square, target_square, b, 0, 0, 0, 0));
                            add_move(move_list, encode_move(square, target_square, n, 0, 0, 0, 0));
                        } else {
                            // one square ahead pawn move
                            add_move(move_list, encode_move(square, target_square, 0, 0, 0, 0, 0));
                            // two squares ahead pawn move (make sure pawns are on 7th rank)
                            if (rank_7(square) && is_empty_square(square + 32)) {
                                add_move(move_list, encode_move(square, square + 32, 0, 0, 1, 0, 0));
                            }
                        }
                    }
                    // black pawn captures (using bishop offsets b/c they're the same as pawns)
                    for (int i = 0; i < 4; ++i) {
                        // init pawn offset
                        int pawn_offset = bishop_offsets[i];
                        
                        // black pawn offsets
                        if (pawn_offset > 0) {
                            int target_square = square + pawn_offset;
                            // check if target square is on board
                            if (on_board(target_square)) {
                                // capture pawn promotion
                                if (rank_2(square) && is_white_piece_square(target_square)) {
                                    add_move(move_list, encode_move(square, target_square, q, 1, 0, 0, 0));
                                    add_move(move_list, encode_move(square, target_square, r, 1, 0, 0, 0));
                                    add_move(move_list, encode_move(square, target_square, b, 1, 0, 0, 0));
                                    add_move(move_list, encode_move(square, target_square, n, 1, 0, 0, 0));
                                } else {
                                    // normal capture
                                    if (is_white_piece_square(target_square)) {
                                        add_move(move_list, encode_move(square, target_square, 0, 1, 0, 0, 0));
                                    }
                                    // enpassant capture
                                    if (target_square == enpassant) {
                                        add_move(move_list, encode_move(square, target_square, 0, 1, 0, 1, 0));
                                    }
                                }
                            }
                        }
                    }
                }
                // black king castling
                if (piece == 5) {
                    // if king side castling is available
                    if (castle & kc) {
                        // make sure there are empty squares between king & rook
                        if (board[f8] == e && board[g8] == e) {
                            // make sure king & next squares are not under attack
                            if (!is_square_attacked(e8, white) && !is_square_attacked(f8, white)) {
                                add_move(move_list, encode_move(e8, g8, 0, 0, 0, 0, 1));
                            }
                        }
                    }
                    // if queen side castling is available
                    if (castle & qc) {
                        // make sure there are empty squares between king & rook
                        if (board[b8] == e && board[c8] == e && board[d8] == e) {
                            // make sure king & next squares are not under attack
                            if (!is_square_attacked(e8, white) && !is_square_attacked(d8, white)) {
                                add_move(move_list, encode_move(e8, c8, 0, 0, 0, 0, 1));
                            }
                        }
                    }
                }
            }
            // knight moves
            if (piece == 1) {
                // loop over knight offsets
                for (int i = 0; i < 8; ++i) {
                    int target_square = square + knight_offsets[i];
                    int target_piece = board[target_square];
                    
                    if (on_board(target_square)) {
                        if (side == white ?
                           (is_empty_square(target_square) || is_black_piece(target_piece)) :
                           (is_empty_square(target_square) || is_white_piece(target_piece))) {
                            // test if we are capturing
                            if (target_piece != e) {
                                add_move(move_list, encode_move(square, target_square, 0, 1, 0, 0, 0));
                            }
                            // moving to empty square
                            else {
                                add_move(move_list, encode_move(square, target_square, 0, 0, 0, 0, 0));
                            }
                        }
                    }
                }
            }
            // king moves
            if (piece == 5) {
                // loop over king offsets
                for (int i = 0; i < 8; ++i) {
                    int target_square = square + king_offsets[i];
                    int target_piece = board[target_square];
                    
                    if (on_board(target_square)) {
                        if (side == white ?
                           (is_empty_square(target_square) || is_black_piece(target_piece)) :
                           (is_empty_square(target_square) || is_white_piece(target_piece))) {
                            // test if we are capturing
                            if (target_piece != e) {
                                add_move(move_list, encode_move(square, target_square, 0, 1, 0, 0, 0));
                            }
                            // moving to empty square
                            else {
                                add_move(move_list, encode_move(square, target_square, 0, 0, 0, 0, 0));
                            }
                        }
                    }
                }
            }
            // bishop & queen moves
            if (piece == 2 || piece == 4) {
                // loop over bishop & queen offsets
                for (int i = 0; i < 4; ++i) {
                    int target_square = square + bishop_offsets[i];
                    // loop over attack ray
                    while (on_board(target_square)) {
                        int target_piece = board[target_square];
                        // if hits own piece
                        if (side == white ? is_white_piece(target_piece) : is_black_piece(target_piece)) {
                            break;
                        }
                        // if hits enemy piece
                        if (side == white ? is_black_piece(target_piece) : is_white_piece(target_piece)) {
                            add_move(move_list, encode_move(square, target_square, 0, 1, 0, 0, 0));
                            break;
                        }
                        // if empty square
                        if (is_empty_square(target_square)) {
                            add_move(move_list, encode_move(square, target_square, 0, 0, 0, 0, 0));
                        }
                        // increment target square
                        target_square += bishop_offsets[i];
                    }
                }
            }
            // rook & queen moves
            if (piece == 3 || piece == 4) {
                // loop over rook & queen offsets
                for (int i = 0; i < 4; ++i) {
                    int target_square = square + rook_offsets[i];
                    // loop over attack ray
                    while (on_board(target_square)) {
                        int target_piece = board[target_square];
                        // if hits own piece
                        if (side == white ? is_white_piece(target_piece) : is_black_piece(target_piece)) {
                            break;
                        }
                        // if hits enemy piece
                        if (side == white ? is_black_piece(target_piece) : is_white_piece(target_piece)) {
                            add_move(move_list, encode_move(square, target_square, 0, 1, 0, 0, 0));
                            break;
                        }
                        // if empty square
                        if (is_empty_square(target_square)) {
                            add_move(move_list, encode_move(square, target_square, 0, 0, 0, 0, 0));
                        }
                        // increment target square
                        target_square += rook_offsets[i];
                    }
                }
            }
        }
    }
}

// updates the board position by making the given move
int make_move(int move, int capture_flag) {
    // make quiet move
    if (capture_flag == all_moves) {
        // copy board state
        copy_board();

        // decode move
        int from_square = get_move_start(move);
        int target_square = get_move_target(move);
        int promoted_piece = get_promoted_piece(move);
        int enpassant_flag = get_move_enpassant(move);
        int double_pawn_move_flag = get_move_pawn(move);
        int castle_flag = get_move_castling(move);

        // get the piece that is making the move
        int piece = board[from_square];
        int target_piece = board[target_square];

        // move piece
        board[target_square] = board[from_square];
        board[from_square] = e;

        // update hash for moved piece (xor out old square and xor in new square)
        hash_key ^= piece_keys[piece - 1][from_square];
        hash_key ^= piece_keys[piece - 1][target_square];
        // update hash for captured piece (if there is one)
        if (target_piece != e) {
            hash_key ^= piece_keys[target_piece - 1][target_square];
        }

        // pawn promotion
        if (promoted_piece != e) {
            // place promoted piece on the board
            board[target_square] = promoted_piece;
            // update hash for promoted piece (xor out pawn from target square)
            hash_key ^= piece_keys[piece - 1][target_square];
            // update hash for promoted piece (add promoted piece to target square)
            hash_key ^= piece_keys[promoted_piece - 1][target_square];
        }

        // enpassant capture
        if (enpassant_flag) {
            if (side == white) {
                board[target_square + 16] = e;
                // update hash for captured pawn (xor out pawn from enpassant square)
                hash_key ^= piece_keys[p - 1][target_square + 16];
            } else {
                board[target_square - 16] = e;
                // update hash for captured pawn (xor out pawn from enpassant square)
                hash_key ^= piece_keys[P - 1][target_square - 16];
            }
        }

        // update hash to remove enpassant square if there is one
        if (enpassant != no_sq) {
            hash_key ^= enpassant_keys[enpassant];
        }
        // reset enpassant square
        enpassant = no_sq;

        // double pawn push
        if (double_pawn_move_flag) {
            if (side == white) {
                enpassant = target_square + 16;
            } else {
                enpassant = target_square - 16;
            }
            // update hash to add enpassant square
            hash_key ^= enpassant_keys[enpassant];
        }

        // castle move
        if (castle_flag) {
            // switch target square
            switch (target_square) {
                // white king side castling
                case g1:
                    // update hash for rook (xor out rook from h1)
                    hash_key ^= piece_keys[R - 1][h1];
                    // update hash for rook (xor in rook to f1)
                    hash_key ^= piece_keys[R - 1][f1];
                    board[f1] = board[h1];
                    board[h1] = e;
                    break;
                // white queen side castling
                case c1:
                    // update hash for rook (xor out rook from a1)
                    hash_key ^= piece_keys[R - 1][a1];
                    // update hash for rook (xor in rook to d1)
                    hash_key ^= piece_keys[R - 1][d1];
                    board[d1] = board[a1];
                    board[a1] = e;
                    break;
                // black king side castling
                case g8:
                    // update hash for rook (xor out rook from h8)
                    hash_key ^= piece_keys[r - 1][h8];
                    // update hash for rook (xor in rook to f8)
                    hash_key ^= piece_keys[r - 1][f8];
                    board[f8] = board[h8];
                    board[h8] = e;
                    break;
                // black queen side castling
                case c8:
                    // update hash for rook (xor out rook from a8)
                    hash_key ^= piece_keys[r - 1][a8];
                    // update hash for rook (xor in rook to d8)
                    hash_key ^= piece_keys[r - 1][d8];
                    board[d8] = board[a8];
                    board[a8] = e;
                    break; 
            }
        }

        // update king square array
        if (board[target_square] == K || board[target_square] == k) {
            king_square[side] = target_square;
        }

        // update hash for castling rights (xor out old castling rights)
        hash_key ^= castling_keys[castle];

        // update castling rights
        castle &= castling_rights[from_square];
        castle &= castling_rights[target_square];

        // update hash for castling rights (xor in new castling rights)
        hash_key ^= castling_keys[castle];

        // change side
        side ^= 1;
        // update hash for side to move
        hash_key ^= side_key;

        // take move back if king is in check
        if (in_check(side)) {
            // restore board position
            restore_board();
            // illegal move
            return 0;
        }
        // legal move
        return 1;
    } else {
        // capture move
        
        // if move is a capture
        if (get_move_capture(move)) {
            // make capture move
            return make_move(move, all_moves);
        } else {
            // move is not a capture
            return 0;
        }
    }
    return 0;
}