#include <bits/stdc++.h>
#include "State.h"

using namespace std;

class Game {

    class PQ_Node {

    public:
        int level;
        int heuristic_dist;
        State state;

        PQ_Node() {

        }

        PQ_Node(int level, int heuristic_dist, State state) {
            this->level = level;
            this->heuristic_dist = heuristic_dist;
            this->state = state;
        }

        bool operator>(const PQ_Node &other) const {
            return make_pair(level+heuristic_dist, state) < make_pair(other.level+other.heuristic_dist, other.state);
        }

        bool operator<(const PQ_Node &other) const {
            return make_pair(level+heuristic_dist, state) > make_pair(other.level+other.heuristic_dist, other.state);
        }
    };

    int n;
    State initial_state;
    vector<char> inv_dir;

    int get_distance_from_state(int param, const State &state) {
        if (param == 0) return state.get_Hamming_distance();
        return state.get_Manhattan_distance();
    }

    void init_inv_dir() {
        inv_dir = vector<char> (130);
        inv_dir['L'] = 'R';
        inv_dir['R'] = 'L';
        inv_dir['D'] = 'U';
        inv_dir['U'] = 'D';
    }

    void init(int n) {
        this->n = n;
        initial_state.init(n);
        init_inv_dir();
    }

    void backtrack(const string &s) const {
        initial_state.write();
        State state = initial_state;
        for (int i = 1; i < s.length(); i++) {
            assert(state.get_next_state_in_dir(s[i], state));
            state.write();
        }
    }

public:

    Game(int n) {
        init(n);
        initial_state.read();
    }

    void play(int param) {
        priority_queue<pair<PQ_Node, string>> pq;
        PQ_Node src_pq_node(0, get_distance_from_state(param, initial_state), initial_state);
        pq.push(make_pair(src_pq_node, "I"));

        while (!pq.empty()) {
            auto [top_pq_node, dir] = pq.top();
            pq.pop();

            if (top_pq_node.state.check_target_board()) {
                // target state reached
                // backtrack
                cout << "Minimum number of moves = " << dir.length()-1 << "\n\n";
                backtrack(dir);
                break;
            }

            vector<pair<char, State>> next_states = top_pq_node.state.get_next_states();
            for (auto [state_dir, state] : next_states) {
                if (state_dir != inv_dir[dir.back()]) {
                    // not returning to parent state
                    PQ_Node pq_node(top_pq_node.level+1, get_distance_from_state(param, state), state);
                    pq.push(make_pair(pq_node, dir+state_dir));
                }
            }
        }
    }

};
