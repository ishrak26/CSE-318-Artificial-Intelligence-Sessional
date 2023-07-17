#include <bits/stdc++.h>
#include "game.h"

using namespace std;

int main() {
//    Game game;
//    game.play_human_vs_ai(2, 2);
//    game.play_ai_ai(3, 3);

    for (int i = 1; i <= HEU_CNT; i++) {
        for (int j = 1; j <= HEU_CNT; j++) {
            cout << "Player 1 playing with heuristic " << i << '\n';
            cout << "Player 2 playing with heuristic " << j << '\n';
            Game game;
            auto [s0, s1] = game.play_ai_ai(i, j);
            cout << '\n';
        }
    }
    return 0;
}
