#include <bits/stdc++.h>
#include "mancala.h"

using namespace std;

class Game {
    const int INF = 10000;

    Board board;
    int additional_moves[2];
    int stones_captured[2];

    void helper_play_ai_ai(int h1, int h2, int curr_player) {
        cout << "Player " << (curr_player == P0 ? "1\n" : "2\n");
        board.print();
        cout << '\n';

        int alpha = -INF, beta = INF;
        int next_bin = -1;
        for (int i = 1; i <= BINS; i++) {
            if (board.check_valid_bin(curr_player, i)) {
                // try to play this move
                Mancala mancala(additional_moves[curr_player], stones_captured[curr_player], curr_player, 0, alpha, beta, board, (curr_player==P0 ? h1 : h2));
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
                additional_moves[curr_player]++;
                helper_play_ai_ai(h1, h2, curr_player);
            }
            else {
                stones_captured[curr_player] += sc;
                helper_play_ai_ai(h1, h2, curr_player^1);
            }
        }
        else {
            // game ended
            int s0 = board.get_score(P0);
            int s1 = board.get_score(P1);
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
    }


public:
    Game() {
        memset(additional_moves, 0, sizeof(additional_moves));
        memset(stones_captured, 0, sizeof(stones_captured));
    }

    // play AI vs AI
    void play_ai_ai(int h1, int h2) {
        helper_play_ai_ai(h1, h2, P0);
    }
};
