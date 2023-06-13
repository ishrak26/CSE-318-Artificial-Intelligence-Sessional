#include <bits/stdc++.h>
#include "Game.h"

using namespace std;

int main() {
//    freopen("input.txt", "r", stdin);
//    freopen("output.txt", "w", stdout);
//    freopen("error.txt", "w", stderr);
    int n;
    cin >> n;
    Game game(n);
    string s;
    int explored, expanded;
    int manhattan = game.play(1, s, explored, expanded);
    if (manhattan == -1) {
        cout << "Unsolvable puzzle\n";
    }
    else {
        cout << "Using Manhattan distance as heuristic function,\n";
        cout << "Minimum number of moves = " << manhattan << "\n";
        cout << "No. of explored nodes = " << explored << "\n";
        cout << "No. of expanded nodes = " << expanded << "\n\n";
        game.backtrack(s);

        s.clear();
        int hamming = game.play(0, s, explored, expanded);
        cout << "Using Hamming distance as heuristic function,\n";
        cout << "Minimum number of moves = " << hamming << "\n\n";
        cout << "No. of explored nodes = " << explored << "\n";
        cout << "No. of expanded nodes = " << expanded << "\n\n";
        game.backtrack(s);
    }
    return 0;
}
