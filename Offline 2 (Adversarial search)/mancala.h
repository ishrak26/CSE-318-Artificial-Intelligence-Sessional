#include<bits/stdc++.h>
#include "board.h"

using namespace std;

#define HEU_CNT 4
#define DEPTH 5

#define W1 3
#define W2 4
#define W3 3
#define W4 4

#define MAX_PLAY P0
#define MIN_PLAY P1

class Mancala {

    int additional_move_earned;
    int stones_captured;
    int curr_player;
    int depth;
    int alpha;
    int beta;
    int h_num; // heuristic function number

    int heuristic_func1() {
        return board.get_storage(P0) - board.get_storage(P1);
    }

    int heuristic_func2() {
        return W1 * heuristic_func1() + W2 * (board.get_rem_stones(P0) - board.get_rem_stones(P1));
    }

    int heuristic_func3() {
        return heuristic_func2() + W3 * additional_move_earned;
    }

    int heuristic_func4() {
        return heuristic_func3() + W4 * stones_captured;
    }

//    int (*heuristics[])() = {heuristic_func1, heuristic_func2, heuristic_func3, heuristic_func4};

public:
    Board board;

    Mancala(int add_move, int capt, int player, int d, int a, int b, Board &board, int h_num) {
        additional_move_earned = add_move;
        stones_captured = capt;
        curr_player = player;
        depth = d;
        alpha = a;
        beta = b;
        this->board.copy_board(board);
        this->h_num = h_num;
    }

    void copy_mancala(Mancala &other) {
        board.copy_board(other.board);
        additional_move_earned = other.additional_move_earned;
        stones_captured = other.stones_captured;
    }

    int get_heuristic_val(int num) {
        if (num == 1) {
            return heuristic_func1();
        }
        else if (num == 2) {
            return heuristic_func2();
        }
        else if (num == 3) {
            return heuristic_func3();
        }
        else {
            return heuristic_func4();
        }
    }

    // returns score for the current config exploring all children
    int simulate_next_moves() {
        if (depth == DEPTH) {
            // handle separately



            return get_heuristic_val(h_num);
        }
        if (curr_player == MAX_PLAY) {
            int flag = 0;
            for (int i = 1; i <= BINS; i++) {
                if (board.check_valid_bin(curr_player, i)) {
                    // try to play this move
                    if (!flag) {
                        flag = 1;
                    }
                    Board new_board;
                    new_board.copy_board(board);
                    int m, c;
                    new_board.play_move(curr_player, i, m, c);
                    if (m) {
                        // move earned
                        Mancala new_mancala(additional_move_earned+1, stones_captured, curr_player, depth+1, alpha, beta, new_board, h_num);
                        int val = new_mancala.simulate_next_moves();
                        if (val > alpha) {
                            alpha = val;
                            if (val > beta) {
                                // cannot impact parent
                                // don't need to explore other valid moves
                                return val;
                            }
                        }
                    }
                    else {
                        if (c) {
                            Mancala new_mancala(additional_move_earned, stones_captured+c, curr_player^1, depth+1, alpha, beta, new_board, h_num);
                            int val = new_mancala.simulate_next_moves();
                            if (val > alpha) {
                                alpha = val;
                                if (val > beta) {
                                    // cannot impact parent
                                    // don't need to explore other valid moves
                                    return val;
                                }
                            }
                        }
                        else {
                            Mancala new_mancala(additional_move_earned, stones_captured, curr_player^1, depth+1, alpha, beta, new_board, h_num);
                            int val = new_mancala.simulate_next_moves();
                            if (val > alpha) {
                                alpha = val;
                                if (val > beta) {
                                    // cannot impact parent
                                    // don't need to explore other valid moves
                                    return val;
                                }
                            }
                        }
                    }
                }
            }
            if (!flag) {
                // no valid move for this player
                // game ended
                return board.get_score(curr_player);
            }
            else {
                return alpha;
            }
        }
        else {
            int flag = 0;
            for (int i = 1; i <= BINS; i++) {
                if (board.check_valid_bin(curr_player, i)) {
                    // try to play this move
                    if (!flag) {
                        flag = 1;
                    }
                    Board new_board;
                    new_board.copy_board(board);
                    int m, c;
                    new_board.play_move(curr_player, i, m, c);
                    if (m) {
                        // move earned
                        Mancala new_mancala(additional_move_earned+1, stones_captured, curr_player, depth+1, alpha, beta, new_board, h_num);
                        int val = new_mancala.simulate_next_moves();
                        if (val < beta) {
                            beta = val;
                            if (val < alpha) {
                                // cannot impact parent
                                return val;
                            }
                        }
                    }
                    else {
                        if (c) {
                            Mancala new_mancala(additional_move_earned, stones_captured+c, curr_player^1, depth+1, alpha, beta, new_board, h_num);
                            int val = new_mancala.simulate_next_moves();
                            if (val < beta) {
                                beta = val;
                                if (val < alpha) {
                                    // cannot impact parent
                                    return val;
                                }
                            }
                        }
                        else {
                            Mancala new_mancala(additional_move_earned, stones_captured, curr_player^1, depth+1, alpha, beta, new_board, h_num);
                            int val = new_mancala.simulate_next_moves();
                            if (val < beta) {
                                beta = val;
                                if (val < alpha) {
                                    // cannot impact parent
                                    return val;
                                }
                            }
                        }
                    }
                }
            }
            if (!flag) {
                // no valid move for this player
                // game ended
                return board.get_score(curr_player);
            }
            else {
                return beta;
            }
        }
    }

};
