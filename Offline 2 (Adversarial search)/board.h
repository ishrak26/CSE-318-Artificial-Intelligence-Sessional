#include <bits.stdc++.h>

using namespace std;

#define BINS 6 // no. of bins in mancala
#define STONES 4 // initial no. of stones in each bin
#define P0 1 // player 0
#define P1 0 // player 1

class Board {
    int board[2][BINS+1];
    // row 1 is player 0 (bottom row)
    // row 2 is player 1 (top row)
    pair<int, int> next_bin[2][BINS+1];

    void init_next_bin() {
        // initialize for bottom row
        for (int j = 1; j < BINS; j++) {
            next_bin[1][j] = {1, j+1};
        }
        next_bin[1][BINS] = {1, 0};
        next_bin[1][0] = {0, BINS};

        // initialize for top row
        for (int j = BINS; j > 0; j--) {
            next_bin[0][j] = {0, j-1};
        }
        next_bin[0][0] = {1, 1};
    }

public:
    Board() {
        // initialize board
        for (int i = 0; i < 2; i++) {
            for (int j = 1; j <= BINS; j++) {
                board[i][j] = STONES;
            }
        }
        init_next_bin();
    }

    Board(const Board &board) {
        // copy board
        for (int i = 0; i < 2; i++) {
            for (int j = 1; j <= BINS; j++) {
                this->board[i][j] = board[i][j];
            }
        }
        init_next_bin();
    }

    bool check_valid_bin(int player, int bin) {
        if (player == P0) {
            // bin count starts from left to right
            return board[P0][bin];
        }
        // else P1
        // bin count starts from right to left
        return board[P1][BINS-bin+1];
    }
};
