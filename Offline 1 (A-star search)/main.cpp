#include <bits/stdc++.h>
#include "Game.h"

using namespace std;

int main() {
    freopen("input.txt", "r", stdin);
//    freopen("output.txt", "w", stdout);
//    freopen("error.txt", "w", stderr);
    int n;
    cin >> n;
    Game game(n);
    game.play(0);
    return 0;
}
