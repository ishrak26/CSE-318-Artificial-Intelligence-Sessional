#include <bits/stdc++.h>

using namespace std;

class Board {
    int n;
    vector<vector<int>> board;
    vector<pair<int, int>> pos;

    void init_board(int n) {
        this->n = n;
        board = vector<vector<int>> (n+1, vector<int>(n+1));
        pos = vector<pair<int, int>> (n*n);
    }

    void init_pos() {
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= n; j++) {
                pos[board[i][j]] = make_pair(i, j);
            }
        }
    }
 
    int calc_Manhattan_dist(int num) const {
        assert(num > 0 && num < n*n);
        int target_row = 1 + ((num-1) / n);
        int target_col = 1 + ((num-1) % n);
        return abs(target_row - pos[num].first) + abs(target_col - pos[num].second);
    }

public:
    Board() {

    }

    Board(int n) {
        init(n);
    }

    void init(int n) {
        init_board(n);
    }

    void init(int n, vector<vector<int>> &b) {
        init_board(n);
        board = b;
        init_pos();
    }

    void read() {
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= n; j++) {
                cin >> board[i][j];
            }
        }
        init_pos();
    }

    void write() const {
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= n; j++) {
                cout << board[i][j];
            }
        }
    }

    pair<int, int> get_blank_pos() const {
        return get_position(0);
    }

    // returns a vector of pairs where each pair gives direction of blank movement and new board
    vector<pair<char, Board>> get_next_boards() const {
        vector<pair<char, Board>> next_boards;
        auto [row, col] = get_blank_pos();
        if (row > 1) {
            // up
            Board upboard = *this;
            swap(upboard.board[row-1][col], upboard.board[row][col]);
            swap(upboard.pos[upboard.board[row][col]], upboard.pos[0]);
            next_boards.push_back(make_pair('U', upboard));
        }
        if (row < n) {
            // down
            Board downboard = *this;
            swap(downboard.board[row+1][col], downboard.board[row][col]);
            swap(downboard.pos[downboard.board[row][col]], downboard.pos[0]);
            next_boards.push_back(make_pair('D', downboard));
        }
        if (col > 1) {
            // left
            Board leftboard = *this;
            swap(leftboard.board[row][col-1], leftboard.board[row][col]);
            swap(leftboard.pos[leftboard.board[row][col]], leftboard.pos[0]);
            next_boards.push_back(make_pair('L', leftboard));
        }
        if (col < n) {
            // right
            Board rightboard = *this;
            swap(rightboard.board[row][col+1], rightboard.board[row][col]);
            swap(rightboard.pos[rightboard.board[row][col]], rightboard.pos[0]);
            next_boards.push_back(make_pair('R', rightboard));
        }
        return next_boards;
    }

    // returns the (row, col) value of num in this board
    pair<int, int> get_position(int num) const {
        assert(num < n*n);
        return pos[num];
    }

    // calculates the Hamming distance of the current board
    int get_Hamming_distance() const {
        int hamming_dist = 0;
        for (int i = 1; i < n*n; i++) {
            hamming_dist += (calc_Manhattan_dist(i) ? 1 : 0 );
        }
    }

    // calculates the Manhattan distance of the current board
    int get_Manhattan_distance() const {
        int manhattan_dist = 0;
        for (int i = 1; i < n*n; i++) {
            manhattan_dist += calc_Manhattan_dist(i);
        }
    }    
};
