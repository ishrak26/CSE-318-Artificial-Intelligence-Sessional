#include <bits/stdc++.h>
#include "mancala.h"

using namespace std;

class Game {
    const int INF = 10000;

    Board board;
    int additional_moves;
    int stones_captured;

    int s0, s1;

    bool print;

    void print_game_results() {
        s0 = board.get_score(P0);
        s1 = board.get_score(P1);
        cout << "Game ended\n";
        cout << "Player 1: " << s0 << "\nPlayer 2: " << s1 << "\n";
        if (s0 > s1) {
            cout << "Player 1 wins";
        }
        else if (s0 < s1) {
            cout << "Player 2 wins";
        }
        else {
            cout << "Tie";
        }
        cout << "!\n";
    }

    void helper_play_ai_ai(int h1, int h2, int curr_player) {

        if (print) {
            board.print();
            cout << "Player " << (curr_player == P0 ? "1" : "2") << "'s turn\n";
            cout << '\n';
        }

        int alpha = -INF, beta = INF;
        int next_bin = -1;
        for (int i = 1; i <= BINS; i++) {
            if (board.check_valid_bin(curr_player, i)) {
                // try to play this move
                Mancala mancala(additional_moves, stones_captured, curr_player, 0, alpha, beta, board, (curr_player==P0 ? h1 : h2));
                mancala.play_fixed_move(i);
                int val = mancala.simulate_next_moves();
//                cerr << i << " " << curr_player << " " << val << '\n';
                if (curr_player == MAX_PLAY) {
                    if (val > alpha) {
                        alpha = val;
                        next_bin = i;
                    }
                }
                else {
                    if (val < beta) {
                        beta = val;
                        next_bin = i;
                    }
                }
            }
        }
        if (next_bin != -1) {
            // play the move
            int me, sc; // moves earned, stones captured
            board.play_move(curr_player, next_bin, me, sc);
            if (me) {
                if (curr_player == P0) additional_moves++;
                helper_play_ai_ai(h1, h2, curr_player);
            }
            else {
                if (curr_player == P1) stones_captured += sc;
                helper_play_ai_ai(h1, h2, curr_player^1);
            }
        }
        else {
            // game ended
            print_game_results();
        }
    }

    void helper_play_human_ai(int h1, int h2, int curr_player) {
        board.print();
        cout << "Player: " << (curr_player == P0 ? "You\n" : "Computer\n");
        cout << '\n';

        if (curr_player == MAX_PLAY) {
           // human
           if (board.check_game_end(curr_player)) {
                // game ended
                print_game_results();
                return;
           }
           int flag = 1;
           while (flag) {
                cout << "Enter the bin number: ";
                int bin;
                cin >> bin;
                if (bin < 1 || bin > BINS || !board.check_valid_bin(curr_player, bin)) {
                    cout << "Invalid bin selected! Try again.\n";
                }
                else {
                    // play the move
                    int me, sc; // moves earned, stones captured
                    board.play_move(curr_player, bin, me, sc);
                    if (me) {
                        if (curr_player == P0) additional_moves++;
                        helper_play_human_ai(h1, h2, curr_player);
                    }
                    else {
                        if (curr_player == P1) stones_captured += sc;
                        helper_play_human_ai(h1, h2, curr_player^1);
                    }
                    flag = 0;
                }
           }
        }
        else {
            // ai
            int alpha = -INF, beta = INF;
            int next_bin = -1;
            for (int i = 1; i <= BINS; i++) {
                if (board.check_valid_bin(curr_player, i)) {
                    // try to play this move
                    Mancala mancala(additional_moves, stones_captured, curr_player, 0, alpha, beta, board, (curr_player==P0 ? h1 : h2));
                    mancala.play_fixed_move(i);
                    int val = mancala.simulate_next_moves();
    //                cerr << i << " " << curr_player << " " << val << '\n';
                    if (curr_player == MAX_PLAY) {
                        if (val > alpha) {
                            alpha = val;
                            next_bin = i;
                        }
                    }
                    else {
                        if (val < beta) {
                            beta = val;
                            next_bin = i;
                        }
                    }
                }
            }
            if (next_bin != -1) {
                // play the move
                int me, sc; // moves earned, stones captured
                board.play_move(curr_player, next_bin, me, sc);
                if (me) {
                    if (curr_player == P0) additional_moves++;
                    helper_play_human_ai(h1, h2, curr_player);
                }
                else {
                    if (curr_player == P1) stones_captured += sc;
                    helper_play_human_ai(h1, h2, curr_player^1);
                }
            }
            else {
                // game ended
                print_game_results();
            }
        }


    }

public:
    Game() {
        additional_moves = stones_captured = 0;
    }

    // play AI vs AI
    pair<int, int> play_ai_ai(int h1, int h2, bool print=false) {
        this->print = print;
        cout << "Game starting...\n";
        helper_play_ai_ai(h1, h2, P0);
        return {s0, s1};
    }

    void play_human_vs_ai(int h1, int h2) {
        cout << "Game starting...You are playing as player 1\n";
        helper_play_human_ai(h1, h2, P0);
    }
};
