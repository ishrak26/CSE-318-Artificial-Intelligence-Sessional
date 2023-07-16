#include <bits/stdc++.h>

using namespace std;

#define BINS 6 // no. of bins in mancala
#define STONES 4 // initial no. of stones in each bin
#define P0 1 // player 0
#define P1 0 // player 1

class Board {
    vector<vector<int>> board;
    // row 1 is player 0 (bottom row)
    // row 2 is player 1 (top row)

    vector<vector<pair<int, int>>> next_bin;

    vector<int> stone_count; // no. of stones remaining for a player

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
        board = vector<vector<int>>(2, vector<int>(BINS+1));
        next_bin = vector<vector<pair<int, int>>> (2, vector<pair<int, int>> (BINS+1));
        stone_count = vector<int>(2);
        for (int i = 0; i < 2; i++) {
            for (int j = 1; j <= BINS; j++) {
                board[i][j] = STONES;
            }
        }
        stone_count[P0] = stone_count[P1] = BINS * STONES;
        init_next_bin();
    }

    void copy_board(Board &other) {
        // copy board
        board = other.board;
        stone_count = other.stone_count;
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

    void print() {
        for (int i = 0; i < 2; i++) {
            for (int j = 0; j <= BINS; j++) {
                cout << board[i][j] << ' ';
            }
            cout << '\n';
        }
    }

    int get_storage(int player) {
        return board[player][0];
    }

    int get_rem_stones(int player) {
        return stone_count[player];
    }

    void play_move(int player, int bin, int &move_earned, int &stones_captured) {
        move_earned = stones_captured = 0;
        pair<int, int> curr;
        if (player == P0) {
            curr = make_pair(player, bin);
        }
        else {
            curr = make_pair(player, BINS-bin+1);
        }
        auto [x,y] = curr;
        int rem = board[x][y];
        board[x][y] = 0;
        while (rem) {
            curr = next_bin[curr.first][curr.second];
            auto [xx,yy] = curr;
            board[xx][yy]++;
            rem--;
        }
        if (curr == make_pair(player, 0)) {
            move_earned = 1;
        }
        else {
            auto [xx,yy] = curr;
            if (board[xx][yy] == 1 && xx == player) {
                board[player][0] += board[xx][yy] + board[xx^1][yy];
                stones_captured += board[xx][yy] + board[xx^1][yy];
                board[xx][yy] = 0;
                board[xx^1][yy] = 0;
            }
        }
        for (int i = 0; i < 2; i++) {
            stone_count[i] = 0;
            for (int j = 1; j <= BINS; j++) {
                stone_count[i] += board[i][j];
            }
        }
    }

    int get_score(int player) {
        return get_storage(player) + get_rem_stones(player);
    }
};
