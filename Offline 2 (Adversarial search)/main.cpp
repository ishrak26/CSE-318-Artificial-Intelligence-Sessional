#include <bits/stdc++.h>
#include "game.h"

using namespace std;

int main() {
//    Game game;
//    game.play_human_vs_ai(2, 2);
//    game.play_ai_ai(3, 3);

    int flag = 1;
    while (flag) {
        cout << "1. Play with computer\n";
        cout << "2. Simulate computer vs computer\n";

        int opt;
        cin >> opt;
        if (opt >= 1 && opt <= 2) {
            if (opt == 1) {
                // human vs ai
                int h1, h2;
                cout << "You will play as Player 1\n";
                cout << "Enter heuristic function for you (1-4): ";
                cin >> h1;
                cout << "Enter heuristic function for computer (1-4): ";
                cin >> h2;
                Game game;
                game.play_human_vs_ai(h1, h2);
            }
            else {
                // ai vs ai
                int h1, h2;
                cout << "Enter heuristic function for player 1 (1-4): ";
                cin >> h1;
                cout << "Enter heuristic function for player 2 (1-4): ";
                cin >> h2;
                Game game;
                game.play_ai_ai(h1, h2, true);
            }
            flag = 0;
        }
        else {
            cout << "Invalid input! Try again.\n";
        }
    }


//    for (int i = 1; i <= HEU_CNT; i++) {
//        for (int j = 1; j <= HEU_CNT; j++) {
//            cout << "Player 1 playing with heuristic " << i << '\n';
//            cout << "Player 2 playing with heuristic " << j << '\n';
//            Game game;
//            auto [s0, s1] = game.play_ai_ai(i, j);
//            cout << '\n';
//        }
//    }
    return 0;
}
