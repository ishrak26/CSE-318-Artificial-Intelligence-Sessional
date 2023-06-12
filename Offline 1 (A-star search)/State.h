#include <bits/stdc++.h>

using namespace std;

class State {
    int n;
    vector<vector<int>> board;
    vector<pair<int, int>> pos;
    char parent_board_dir;

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

    State get_upstate() const {
        auto [row, col] = get_blank_pos();
        assert(row > 1);
        State upstate = *this;
        swap(upstate.board[row-1][col], upstate.board[row][col]);
        swap(upstate.pos[upstate.board[row][col]], upstate.pos[0]);
        return upstate;
    }

    State get_downstate() const {
        auto [row, col] = get_blank_pos();
        assert(row < n);
        State downstate = *this;
        swap(downstate.board[row+1][col], downstate.board[row][col]);
        swap(downstate.pos[downstate.board[row][col]], downstate.pos[0]);
        return downstate;
    }

    State get_leftstate() const {
        auto [row, col] = get_blank_pos();
        assert(col > 1);
        State leftstate = *this;
        swap(leftstate.board[row][col-1], leftstate.board[row][col]);
        swap(leftstate.pos[leftstate.board[row][col]], leftstate.pos[0]);
        return leftstate;
    }

    State get_rightstate() const {
        auto [row, col] = get_blank_pos();
        assert(col < n);
        State rightstate = *this;
        swap(rightstate.board[row][col+1], rightstate.board[row][col]);
        swap(rightstate.pos[rightstate.board[row][col]], rightstate.pos[0]);
        return rightstate;
    }
 
    int calc_Manhattan_dist(int num) const {
        assert(num > 0 && num < n*n);
        int target_row = 1 + ((num-1) / n);
        int target_col = 1 + ((num-1) % n);
        return abs(target_row - pos[num].first) + abs(target_col - pos[num].second);
    }

    bool operator<(const State &other) const {
        return get_Manhattan_distance() < other.get_Manhattan_distance();
    }

public:
    State() {

    }

    State(int n) {
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

    bool get_next_board_in_dir(char dir, State &board) const {
        auto [row, col] = get_blank_pos();
        if (dir == 'U') {
            if (row > 1) {
                board = get_upstate();
            }
            else {
                return false;
            }
        }
        else if (dir == 'D') {
            ;
        }
        return true;
    }

    // returns a vector of pairs where each pair gives direction of blank movement and new board
    vector<pair<char, State>> get_next_states() const {
        vector<pair<char, State>> next_states;
        auto [row, col] = get_blank_pos();
        if (row > 1) {
            // up
            State state;
            get_next_board_in_dir('U', state);
            next_states.push_back(make_pair('U', state));
        }
        if (row < n) {
            // down
            State state;
            get_next_board_in_dir('D', state);
            next_states.push_back(make_pair('D', state));
        }
        if (col > 1) {
            // left
            State state;
            get_next_board_in_dir('L', state);
            next_states.push_back(make_pair('L', state));
        }
        if (col < n) {
            // right
            State state;
            get_next_board_in_dir('R', state);
            next_states.push_back(make_pair('R', state));
        }
        return next_states;
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
        return hamming_dist;
    }

    // calculates the Manhattan distance of the current board
    int get_Manhattan_distance() const {
        int manhattan_dist = 0;
        for (int i = 1; i < n*n; i++) {
            manhattan_dist += calc_Manhattan_dist(i);
        }
        return manhattan_dist;
    }  

    bool check_target_board() const {
        return (get_Hamming_distance() == 0);
    }  
};