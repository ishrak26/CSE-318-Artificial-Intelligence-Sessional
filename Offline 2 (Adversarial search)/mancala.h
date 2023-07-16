#include<bits/stdc++.h>
#include "board.h"

using namespace std;

#define HEU_CNT 4
#define DEPTH 13

#define W1 3
#define W2 3
#define W3 3
#define W4 3

#define MAX_PLAY P0
#define MIN_PLAY P1

class Mancala {
    Board board;
    int additional_move_earned;
    int stones_captured;
    int curr_player;
    int depth;
    int alpha;
    int beta;

    int heuristic_func1() {
        return board.get_storage(curr_player) - board.get_storage(curr_player ^ 1);
    }

    int heuristic_func2() {
        return W1 * heuristic_func1() - W2 * (board.get_rem_stones(curr_player) - board.get_rem_stones(curr_player ^ 1));
    }

    int heuristic_func3() {
        return heuristic_func2() + W3 * additional_move_earned;
    }

    int heuristic_func4() {
        return heuristic_func3() + W4 * stones_captured;
    }

//    int (*heuristics[])() = {heuristic_func1, heuristic_func2, heuristic_func3, heuristic_func4};

public:
    Mancala(int add_move, int capt, int player, int d, int a, int b, Board &board) {
        additional_move_earned = add_move;
        stones_captured = capt;
        curr_player = player;
        depth = d;
        alpha = a;
        beta = b;
        this->board.copy_board(board);
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

    int play_next_move() {
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
                        Mancala new_mancala(additional_move_earned+1, stones_captured, curr_player, depth+1, alpha, beta, new_board);
                        int val = new_mancala.play_next_move();
                        alpha = max(alpha, val);
                    }
                }
            }
        }

    }

};
